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
	if (fork()==0)
	{
		struct sockaddr_in addr;
		int  sock, cnt;
		struct ip_mreq mreq;
		char message[50];
		socklen_t addrlen;

		/* set up socket */
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) 
		{
			perror("socket");
			exit(1);
		}
		//bzero((char *)&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port = htons(9000);
		addrlen = sizeof(addr);
	   
		int one = 1;
      
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
		/* receive */
		if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) 
		{        
			perror("bind");
			exit(1);
		}  
		  
		mreq.imr_multiaddr.s_addr = inet_addr("228.0.0.1");         
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);         
		if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq)) < 0) 
		{
			perror("setsockopt mreq");
			exit(1);
		}         
		while (1) 
		{
			cnt = recvfrom(sock, message, sizeof(message), 0, (struct sockaddr *) &addr, &addrlen);
			if (cnt < 0) 
			{
				perror("recvfrom");
				exit(1);
			} 
			else if (cnt == 0) 
				break;
			printf("\n%s\a", message);
		}
	}
	else
	{
		char endServer[100], buffer[BUF_SIZE];
		char pontuacao[]=";";
		char user[BUF_SIZE], passw[BUF_SIZE], AS[]="AS", Val[]="0", tot[BUF_SIZE];
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
		
		//le o titulo (login,criar conta,sair)
		nread = read(fd, buffer, BUF_SIZE-1);		
		buffer[nread] = '\0';
		printf("%s", buffer);  
	  
		//escreve a opção
		scanf("%s", buffer);
		write(fd, buffer,1 + strlen(buffer));
	  
		//le o pedido da conta
		nread = read(fd, buffer, BUF_SIZE-1);		
		buffer[nread] = '\0';
		printf("%s", buffer); 
		
		if (strcmp(buffer, "Login: ")==0) 
		{
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
				
				if (strcmp(buffer, "Conta não validada!\n")==0)
				{
					close(fd);
					exit(-1);
				} 
				
				if (strcmp(buffer, "\n----------------MENU--------------\n\n1.Consultar\n2.Alterar conta\n3.Apagar\n4.Help\n5.Sair\nIndique a opção: ")==0)
				{
					//escreve a opçao
					scanf("%s", buffer); 
					write(fd, buffer,1 + strlen(buffer));
					
					//le confirmações
					nread = read(fd, buffer, BUF_SIZE-1);		
					buffer[nread] = '\0';
					printf("%s", buffer);
					
					if (strcmp(buffer, "\n1.Tudo\n2.Por data\n3.Por horas\n4.Por local\n5.Por tipo\n6.Por nome\nIndique a opção: ")==0) 
					{
						//manda a opção
						scanf("%s", buffer);
						write(fd, buffer,BUF_SIZE-1);
						printf("\n");
						
						//lê confirmação
						nread = read(fd, buffer, BUF_SIZE-1);		
						buffer[nread] = '\0';
						printf("%s", buffer);
						
						if (strcmp(buffer, "Indique a data (dd/mm/aaaa): ")==0) 
						{
							//manda data
							scanf("%s", buffer); 
							write(fd, buffer,BUF_SIZE-1);
							printf("\n");
							
							//le os crimes
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
						
						if (strcmp(buffer, "Indique a hora (hh:mm:ss): ")==0) 
						{
							//manda data
							scanf("%s", buffer); 
							write(fd, buffer,BUF_SIZE-1);
							printf("\n");
							
							//le os crimes
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
						
						if (strcmp(buffer, "Indique o local: ")==0) 
						{
							//manda data
							scanf("%s", buffer); 
							write(fd, buffer,BUF_SIZE-1);
							printf("\n");
							
							//le os crimes
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
						
						if (strcmp(buffer, "Indique o tipo: ")==0) 
						{
							//manda data
							scanf("%s", buffer); 
							write(fd, buffer,BUF_SIZE-1);
							printf("\n");
							
							//le os crimes
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
						
						if (strcmp(buffer, "Indique o nome: ")==0) 
						{
							//manda data
							scanf("%s", buffer); 
							write(fd, buffer,BUF_SIZE-1);
							printf("\n");
							
							//le os crimes
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
					}
					
					if (strcmp(buffer, "\n1.Conta\n2.Password\nIndique a opção: ")==0) 
					{
						//manda a opção
						scanf("%s", buffer); 
						write(fd, buffer,1 + strlen(buffer));
						
						//le o pedido
						nread = read(fd, buffer, BUF_SIZE-1);		
						buffer[nread] = '\0';
						printf("%s", buffer);
						
						if (strcmp(buffer, "Indique a nova conta: ")==0) 
						{
							//manda nova conta
							scanf("%s", buffer); 
							write(fd, buffer,1 + strlen(buffer));
							
							//lê alterado
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
						
						if (strcmp(buffer, "Indique a nova pass: ")==0) 
						{
							//manda nova pass
							scanf("%s", buffer); 
							write(fd, buffer,1 + strlen(buffer));
							
							//lê alterado
							nread = read(fd, buffer, BUF_SIZE-1);		
							buffer[nread] = '\0';
							printf("%s", buffer);
						}
					}

					if (strcmp(buffer, "4\n")==0)
					{
						nread = read(fd, buffer, BUF_SIZE-1);		
						buffer[nread] = '\0';
						printf("%s", buffer);
					}
				}
			}
		}
		
		if (strcmp(buffer, "User: ")==0) 
		{	
			memset(user,'\0',sizeof(user));
			memset(passw,'\0',sizeof(passw));
			memset(tot,'\0',sizeof(tot));
			
			scanf("%s", user);
			user[strcspn(user,"\n")]='\0';  
			strcat(user, pontuacao);
			strcat(tot, user);
			
			printf("Password: ");
			scanf("%s", passw);
			passw[strcspn(passw,"\n")]='\0';  
			strcat(passw, pontuacao);
			strcat(tot, passw);
			
			strcat(AS, pontuacao);
			strcat(tot, AS);
			strcat(Val, pontuacao);
			strcat(tot, Val);
			
			write(fd, tot, strlen(tot)+1);
			
			read(fd, buffer, BUF_SIZE-1);
			printf("%s", buffer);
		}
		if (strcmp(buffer, "3\n")==0)
		{
			nread = read(fd, buffer, BUF_SIZE-1);		
			buffer[nread] = '\0';
			printf("%s", buffer);
		}
		if (strcmp( buffer, "ByeBye!") == 0 )
		{
			printf("\n");
			exit(-1);
		}
		fflush(stdout);
		close(fd);
	}
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
