#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>

int main(){
    int clientSocket;
    char buffer[10000];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    serverAddr.sin_family = AF_INET;;
    serverAddr.sin_port = htons(5566);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero,'\0',sizeof serverAddr.sin_zero);

    addr_size = sizeof serverAddr;
    connect(clientSocket,(struct sockaddr *) &serverAddr, addr_size);

    recv(clientSocket, buffer, 100, 0);
    printf("%s", buffer);
    char str[10000];
    printf("\nEnter your message: ");
    fgets(str,10000,stdin);

    int i=0,noofchars=0;
    for(;i<strlen(str);i++){
        noofchars++;
    }

    char noofC[10];
    noofC[0]=noofchars;
    //Sending the count of the words first
    send(clientSocket,noofC,10,0);

    /*************** Writing message to plaintext.txt**********************/

    FILE *fptr;
    fptr = fopen("plaintext.txt","w");
    if(fptr==NULL){
        printf("Error opening file! \n");
    }
    fprintf(fptr,"%s",str);
    fclose(fptr);

    /*************** Running rsa.py bash command ***************************/

    FILE *commandfp;
    char path[1035];
    commandfp = popen("python rsa.py encrypt keyFile plaintext.txt encryptedText.txt","r");
    if(commandfp==NULL){
        printf("Failed to run command\n");
    }
    while (fgets(path, sizeof(path)-1, commandfp) != NULL) {
        printf("%s", path);
    }
    pclose(commandfp);

    /************ Reading the encryptedText ************************/

    char buf[10000];
    fptr = fopen("encryptedText.txt","r");
    if(fptr==NULL){
        printf("Error reading file! \n");
    }
    while(fgets(buf,sizeof(buf),fptr)){
        send(clientSocket,buf,10000,0);
    }
    fclose(fptr);

    return 0;

}
