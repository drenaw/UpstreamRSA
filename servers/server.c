#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>

int main(){
	int welcomeSocket, newSocket;
	char buffer[10000];
	struct sockaddr_in serverAddr, serverStorage;
	int addr_size;

	welcomeSocket = socket(AF_INET,SOCK_STREAM,0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5566);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero,'\0',sizeof serverAddr.sin_zero);

	bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

	if(listen(welcomeSocket,5)==0)printf("Listening");

	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket,(struct sockaddr *)&serverStorage, &addr_size );

	char buf[10000];
	strcpy(buffer,"This is an end to end encryption based Message Transmission System");
	send(newSocket,buffer,100,0);
	
	char noofW[10];
	recv(newSocket,noofW,10,0);

	int noofWords = (int)noofW[0];

	FILE *fptr;
	fptr = fopen("encryptedText.txt","w");
	if(fptr==NULL){
       	printf("Error opening file! \n");
    }
	for(int i=0;i<noofWords+1;i++){
		recv(newSocket,buf,10000,0);
    	fprintf(fptr,"%s",buf);
	}
	fclose(fptr);

    FILE *commandfp;
    char path[1035];
    commandfp = popen("python rsa.py decrypt keyFile encryptedText.txt plaintext.txt","r");
    if(commandfp==NULL){
        printf("Failed to run command\n");
    }
    while (fgets(path, sizeof(path)-1, commandfp) != NULL) {
        printf("%s", path);
    }
    pclose(commandfp);	

	fptr = fopen("plaintext.txt","r");
	if(fptr==NULL){
		printf("Error reading fike! \n");
	}
	printf("Received Message: ");
	while(fgets(buf,sizeof(buf),fptr)){
		printf("%s\n",buf);
	}
	fclose(fptr);

}
