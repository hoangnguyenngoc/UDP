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

struct Staff{
	int ID;
	char name[50];
	int age;
	char sex[10];
	char country[20];
};


int get_data_line(char line[], int limit);

void send_file_data(char filename[],int sockfd, struct sockaddr_in addr)
{	
	
	int n;
	char buffer[SIZE];
	
	
	FILE *fp = fopen(filename, "r");
	if (fp == NULL){
    		perror("[ERROR] reading the file");
    		exit(1);
  	}

	while (fgets(buffer, SIZE, fp) != NULL){
		printf("[SENDING] Data: %s", buffer);

		n = sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));
		if (n == -1){
			perror("[ERROR] sending data to the server.");
			fclose(fp);
			exit(1);
		}
		bzero(buffer, SIZE);
	}

	strcpy(buffer, "END");
	sendto(sockfd, buffer, SIZE, 0, (struct sockaddr*)&addr, sizeof(addr));

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
struct Staff Push_Staff(){
	struct Staff Temp_Staff;
	
	printf("ID: ");
	scanf("%d",&Temp_Staff.ID);
	
	//fflush(stdin);
	printf("Name: ");
	scanf("%s",&Temp_Staff.name);
	//get_data_line(Temp_Staff.name,sizeof(Temp_Staff.name));
	//fgets(Temp_Staff.name,sizeof(Temp_Staff.name),stdin);
	
	printf("Age: ");
	scanf("%d", &Temp_Staff.age);
	
	printf("Sex: ");
	scanf("%s", &Temp_Staff.sex);
	//get_data_line(Temp_Staff.sex,sizeof(Temp_Staff.sex));
	//fgets(Temp_Staff.sex,sizeof(Temp_Staff.sex),stdin);
	
	printf("Country: ");
	scanf("%s", &Temp_Staff.country);
	//get_data_line(Temp_Staff.country,sizeof(Temp_Staff.country));
	//fgets(Temp_Staff.country,sizeof(Temp_Staff.country),stdin);
	
	return Temp_Staff;
}

void Data_to_File(char filename[],struct Staff *staff,int Number_Staff){
	FILE *fp = fopen(filename, "a");
	for(int i = 0; i < Number_Staff; i++) {
		struct Staff Temp_Staff = staff[i];
		fprintf(fp, "%-10d %-10s %-10d %-10s %-20s\n",	Temp_Staff.ID,Temp_Staff.name,Temp_Staff.age,Temp_Staff.sex,Temp_Staff.country);
	}
	fclose(fp);
}
int main() { 
	
	int sockfd;
	struct sockaddr_in  server_addr;
	
	char ip[SIZE_DATA] ;
	printf("IP address: ");
	fgets(ip,SIZE_DATA,stdin);
	char port[SIZE_DATA] ;
	printf("PORT : ");
	fgets(port,SIZE_DATA,stdin);
  
    	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        	perror("socket creation failed"); 
       		exit(EXIT_FAILURE); 
    	} 
    
    	memset(&server_addr, 0, sizeof(server_addr)); 
        
    	server_addr.sin_family = AF_INET; 
    	server_addr.sin_port = htons(atoi(port)); 
	server_addr.sin_addr.s_addr = inet_addr(ip); 
	
    	char filename[SIZE_DATA] ;
	printf("File name: ");
	fgets(filename,SIZE_DATA,stdin);
	
	struct Staff staff[100],no_staff;
	int Number_Staff = 0;
	int ct = 0;
	while(1){
		printf("Continue y/n(1/0) = ?");
		scanf("%d",&ct);
		if(ct == 0) break;
		no_staff = Push_Staff();
		staff[Number_Staff++] = no_staff;
	}
	
	Data_to_File(filename,staff,Number_Staff);
    	send_file_data(filename,sockfd, server_addr);

  	printf("[SUCCESS] Data transfer complete.\n");
  	printf("[CLOSING] Disconnecting from the server.\n");
    
    	return 0; 
}


