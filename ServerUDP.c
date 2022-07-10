#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
    
#define SIZE 1024
#define SIZE_DATA 128
void write_file(int sockfd, struct sockaddr_in addr)
{

	char* filename = "Rev.txt";
	int n;
	char buffer[SIZE];
	int addr_size = sizeof(addr);

	FILE *fp = fopen(filename, "a");
	
	while (1){

	n = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, &addr_size);

	if (strcmp(buffer, "END") == 0)
	{
		break;
	}

	printf("[RECEVING] Data: %s", buffer);
	fprintf(fp, "%s", buffer);
	//fprintf(fp, "\n");
	bzero(buffer, SIZE);
	}

	fclose(fp);
}
int get_data_line(char line[], int limit){
	int c;
	int n;
	for(n = 0; n < limit && ( c = getchar()) != '\n' && c != EOF ; ++n ){
		line[n] = c;
	}
	
	if(line[n] == '\n'){
		line[n] = '\0';
	}
	return n;
}

int main() { 
	int sockfd; 
	char* ip = "127.0.0.1";
	struct sockaddr_in server_addr, client_addr; 
         
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	char port[SIZE_DATA] ;
	printf("PORT : ");
	get_data_line(port,SIZE_DATA);

	memset(&server_addr, 0, sizeof(server_addr)); 
	memset(&client_addr, 0, sizeof(client_addr)); 
		
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	server_addr.sin_port = htons(atoi(port)); 
        
	if ( bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	while(1){
		printf("----------------------------------- \n");
		printf("[STARTING] UDP File Server started. \n");
		write_file(sockfd, client_addr);
		printf("[SUCCESS] Data transfer complete.\n");
	  	
	}
	printf("[CLOSING] Closing the server.\n");
	close(sockfd);
	return 0; 
}

