#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h> 
#include <arpa/inet.h>

#define BUF_SIZE 1024

void erro(char *msg);

int main(int argc, char *argv[]) 
{
	char endServer[100], buffer[BUF_SIZE];
	int fd, nread;
	struct sockaddr_in addr;
	struct hostent *hostPtr;

	if (argc != 3) 
	{
		printf("cliente <host> <port>\n");
		exit(-1);
	}

	strcpy(endServer, argv[1]);
	if ((hostPtr = gethostbyname(endServer)) == 0) 
	{
		printf("Couldn t get host address.\n");
		exit(-1);
	}

	bzero((void *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
	addr.sin_port = htons((short) atoi(argv[2]));

	if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
		erro("socket");
	if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
		erro("Connect");
	
	//le o titulo	
	nread = read(fd, buffer, BUF_SIZE-1);		
	buffer[nread] = '\0';
	printf("%s", buffer);  
  
	//escreve a conta
	scanf("%s", buffer); 
	write(fd, buffer,1 + strlen(buffer));
  
	//le o pedido da password
	nread = read(fd, buffer, BUF_SIZE-1);		
	buffer[nread] = '\0';
	printf("%s", buffer); 
	
	if (strcmp(buffer, "Conta errada!\n")==0)
	{
		close(fd);
		exit(-1);
	}
	
	if (strcmp(buffer, "Password: ")==0)
	{
		//escreve a password
		scanf("%s", buffer); 
		write(fd, buffer,1 + strlen(buffer));

		//le o menu
		nread = read(fd, buffer, BUF_SIZE-1);		
		buffer[nread] = '\0';
		printf("%s", buffer);  
		
		if (strcmp(buffer, "Password errada!\n")==0)
		{
			close(fd);
			exit(-1);
		} 
			
		if (strcmp(buffer, "Profi. errada!\n")==0)
		{
			close(fd);
			exit(-1);
		} 
		
		if (strcmp(buffer, "\n----------------MENU--------------\n\n1.Validar\n2.Consultar\n3.Apagar\n4.Help\n5.Sair\nIndique a opção: ")==0)
		{
			//escreve a opçao
			scanf("%s", buffer); 
			write(fd, buffer,1 + strlen(buffer));
			
			//le confirmações
			nread = read(fd, buffer, BUF_SIZE-1);		
			buffer[nread] = '\0';
			printf("%s", buffer);
			
			if (strcmp(buffer, "Indique a conta a validar: ")==0)
			{
				//escreve a conta que quer apagar
				scanf("%s", buffer); 
				write(fd, buffer,1 + strlen(buffer));
				
				//recebe validado
				nread = read(fd, buffer, BUF_SIZE-1);		
				buffer[nread] = '\0';
				printf("%s\n", buffer); 
			}
			
			if (strcmp(buffer, "2")==0)
			{
				//le as contas
				printf("\n");
				nread = read(fd, buffer, BUF_SIZE-1);		
				buffer[nread] = '\0';
				printf("%s\n", buffer); 
			}
			
			if (strcmp(buffer, "Indique a conta a apagar: ")==0)
			{	
				//escreve a conta que quer apagar
				scanf("%s", buffer); 
				write(fd, buffer,1 + strlen(buffer));
				
				//recebe apagado
				nread = read(fd, buffer, BUF_SIZE-1);		
				buffer[nread] = '\0';
				printf("%s\n", buffer); 
			}
			
			if (strcmp(buffer, "4\n")==0)
			{
				nread = read(fd, buffer, BUF_SIZE-1);		
				buffer[nread] = '\0';
				printf("%s", buffer);
			}
			
			/*if (strcmp(buffer, "ByeBye!")==0)
			{
				printf("%s", buffer);
			}*/
		}
	}
  
	fflush(stdout);
	close(fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
