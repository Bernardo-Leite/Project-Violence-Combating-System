#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
  
#define MAXLINE 1024
#define BUF_SIZE 1024
 
void process_client(int fd);
void process_gestor(int fd);
void erro(char *msg);

FILE *ftxt;
  
int main() 
{
	if (fork()==0) //processo filho para udp_APS
	{
		int sockfd;
		char buffer[MAXLINE];
		char *hy="Login: ";
		char *hello = "Password: ";
		char *xau="ByeBye!";
		char *menu="\n-----------MENU-----------\n\n1.Registar crime\n2.Alterar conta\n3.Apagar conta\n4.Help\n5.Alarme\n6.Sair\nIndique a opção: ";
		size_t leng = 100;
		char *linha = malloc(leng);
		char *token;
		char id[1024], pass[1024], prof[1024], val[1024];
		int opcao = 0;
		FILE *fp;
		FILE *fptemp;
		FILE *fh;
		char *fcontent;
		struct sockaddr_in servaddr, cliaddr;
		struct sockaddr_storage serverStorage;
		socklen_t addr_size, len;
		
		addr_size = sizeof serverStorage;
		  
		// Creating socket file descriptor
		if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
		{
			perror("socket creation failed");
			exit(EXIT_FAILURE);
		}
		
		memset(&servaddr, 0, sizeof(servaddr));
		memset(&cliaddr, 0, sizeof(cliaddr));
		  
		// Filling server information
		servaddr.sin_family    = AF_INET; // IPv4
		servaddr.sin_addr.s_addr = INADDR_ANY;
		servaddr.sin_port = htons(8081);
		  
		// Bind the socket with the server address
		if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
		{
			perror("bind failed0");
			exit(EXIT_FAILURE);
		}
		  
		int n;
	  
		len = sizeof(cliaddr);  //len is value/resuslt
		//recebe op
inicio1:	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
			buffer[n] = '\0';
			printf("Client udp: %s", buffer);
			
			if (memcmp( buffer, "1", strlen( "1") ) == 0 )
			{
				//manda login
				sendto(sockfd, (const char *)hy, strlen(hy), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				//recebe conta
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
				buffer[n] = '\0';
				printf("Client udp: %s", buffer);
				
				int size = strlen(buffer);
				buffer[size-1] = '\0';	 //apaga \n do buffer
				
				if ((fp = fopen ("Login.txt","r")) != NULL) 
				{
					while (getline(&linha, &leng, fp) > 0)  //percorre linha a linha no ficheiro
					{
						token = strtok(linha,";");  //guarda a string até ; exclusivamente do ficheiro na variável token 
						
						memset(id, '\0', sizeof(id));
						memset(pass, '\0', sizeof(pass));
						memset(prof, '\0', sizeof(prof));
						memset(val, '\0', sizeof(val));
						
						opcao = 0;
						
						while(token!=NULL)
						{
							switch (opcao)
							{
								case 0:
									strcpy(id, token);  //copia a string que está em token para id
									break;
								case 1:
									strcpy(pass, token);
									break;
								case 2:
									strcpy(prof, token);
									break;
								case 3:
									strcpy(val, token);
									break;
							}
							opcao++;
							token = strtok(NULL, ";");
						}	
						if (strcmp(id, buffer) == 0) break;
					}
					fclose(fp);	
					
					if (strcmp(id, buffer) == 0)
					{
						//manda a pedir pass
						sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
						
						//recebe pass
						n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
						int size = strlen(buffer);
						buffer[size-1] = '\0';	
						printf("Client udp: %s\n", buffer); 

						if (strcmp(pass, buffer) == 0)   //se pass e buffer forem iguais
						{
							if(strcmp(prof, "PS") == 0)
							{
								if(strcmp(val, "1")==0)
								{
									//manda menu (registar, alterar, etc.)
inicio2:							sendto(sockfd, (const char *)menu, strlen(menu), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
									//recebe a opção
									n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
									int size = strlen(buffer);
									buffer[size-1] = '\0';	
									printf("Client udp: %s\n", buffer);
									
									if (memcmp( buffer, "1", strlen( "1") ) == 0 )
									{
										//manda 1
										printf("\n");
										sendto(sockfd, "\n1.Registar com nome\n2.Registar em anonimato\n3.Voltar\nIndique a opção: ", strlen("\n1.Registar com nome\n2.Registar em anonimato\n3.Voltar\nIndique a opção: "), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										//recebe info
										n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
										buffer[n] = '\0';
										printf("Client udp: %s\n", buffer);
										
										if (memcmp( buffer, "1", strlen( "1") ) == 0 )
										{
											sendto(sockfd, "1\n", strlen("1\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											
											n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
											buffer[n] = '\0';
											printf("Client udp: %s\n", buffer);
									
											ftxt=fopen("crimes.txt","a");
											fprintf(ftxt,"%s\n",buffer); //guarda no ficheiro
											fclose(ftxt);
											goto inicio2;
										}
										
										if (memcmp( buffer, "2", strlen( "2") ) == 0 )
										{
											sendto(sockfd, "2\n", strlen("2\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											
											n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
											buffer[n] = '\0';
											printf("Client udp: %s\n", buffer);
									
											ftxt=fopen("crimes.txt","a");
											fprintf(ftxt,"%s\n",buffer); //guarda no ficheiro
											fclose(ftxt);
											goto inicio2;
										}
										
										if (memcmp( buffer, "3", strlen( "3") ) == 0 )
										{
											sendto(sockfd, "3\n", strlen("3\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											goto inicio2;
										}
									}
									
									if(memcmp( buffer , "2", strlen( "2") ) == 0 )
									{
										char *alt="\n1.Conta\n2.Password\n3.Voltar\nIndique a opção: ";
										//manda alt
										sendto(sockfd, (const char *)alt, strlen(alt), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										//recebe a opção
										n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
										buffer[n] = '\0';
										printf("Client udp: %s\n", buffer);
										
										if(memcmp( buffer , "1", strlen( "1") ) == 0 )
										{
											sendto(sockfd, "Indique a nova conta: ", strlen("Indique a nova conta: "), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											//recebe nova conta
											n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
											buffer[n] = '\0';
											printf("Client udp: %s\n", buffer);
											
											fp = fopen( "Login.txt", "r");
											fptemp=fopen("temp.tmp", "w"); //ficheiro temporário
											
											while( getline(&linha,&leng,fp)>0)
											{
												char aux[1024];
												strcpy(aux,linha);   //copia linha para aux
												token = strtok(linha,";");
												
												if(strcmp(token,id)==0)
												{
													buffer[strcspn(buffer,"\n")]='\0'; //retira \n do buffer
													fputs(buffer,fptemp);
													fputs(";",fptemp);
													fputs(pass,fptemp);
													fputs(";",fptemp);
													fputs(prof,fptemp);
													fputs(";",fptemp);
													fputs(val,fptemp);
													fputs(";\n",fptemp);
												}
												if(strcmp(token,id)!=0)  //se as contas não corresponderem imprime a linha para o ficheiro
													fputs(aux,fptemp);
											}
											fclose(fp);
											fclose(fptemp);
											remove("Login.txt");  //elimina o ficheiro
											rename("temp.tmp", "Login.txt");  //renomeia o ficheiro temporário como Login.txt
											sendto(sockfd, "Alterado!\n", strlen("Alterado!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											//close(sockfd);
											goto inicio1;
										}
										
										if(memcmp( buffer , "2", strlen( "2") ) == 0 )
										{
											sendto(sockfd, "Indique a nova pass: ", strlen("Indique a nova pass: "), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											//recebe nova pass
											n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
											buffer[n] = '\0';
											printf("Client udp: %s\n", buffer);
											
											fp = fopen( "Login.txt", "r");
											fptemp=fopen("temp.tmp", "w");
											
											while( getline(&linha,&leng,fp)>0)
											{
												char aux[1024];
												strcpy(aux,linha);
												token = strtok(linha,";");
												
												if(strcmp(token,id)==0) 
												{
													fputs(linha,fptemp);
													fputs(";",fptemp);
													buffer[strcspn(buffer,"\n")]='\0';
													fputs(buffer,fptemp);
													fputs(";",fptemp);
													fputs(prof,fptemp);
													fputs(";",fptemp);
													fputs(val,fptemp);
													fputs(";\n",fptemp);
												}
												if(strcmp(token,id)!=0) 
													fputs(aux,fptemp);
											}
											fclose(fp);
											fclose(fptemp);
											remove("Login.txt");
											rename("temp.tmp", "Login.txt");
											sendto(sockfd, "Alterado!\n", strlen("Alterado!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											goto inicio1;
										}
										if(memcmp( buffer , "3", strlen( "3") ) == 0 )
										{
											sendto(sockfd, "3\n", strlen("3\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
											goto inicio2;
										}
									}
									
									if(memcmp( buffer , "3", strlen( "3") ) == 0 )
									{	
										fp = fopen( "Login.txt", "r");
										fptemp=fopen("temp.tmp", "w");
										
										while( getline(&linha,&leng,fp)>0)
										{
											char aux[1024];
											strcpy(aux,linha);
											token = strtok(linha,";");
										
											if(strcmp(token,id)!=0) 
											  fputs(aux,fptemp);
										}
										fclose(fp);
										fclose(fptemp);
										remove("Login.txt");
										rename("temp.tmp", "Login.txt");
										sendto(sockfd, "Apagado!", strlen("Apagado!"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										goto inicio1;
									}
									
									if (memcmp(buffer, "4", strlen("4"))==0)
									{
										fh=fopen("help1.txt","r");
										fseek(fh,0,SEEK_END);  //move para o fim do ficheiro
										long int fsize = ftell(fh); //devolve o valor até à posição marcada (fim do ficheiro)
										fseek(fh,0,SEEK_SET); //move para o início do ficheiro
										fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
										
										fread(fcontent,1,fsize,fh); 
										fclose(fh);
										
										sendto(sockfd, "4\n", strlen("4\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										sendto(sockfd, fcontent, strlen(fcontent), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										goto inicio2;
									}
									if (memcmp( buffer, "5", strlen( "5") ) == 0 )
									{
										struct sockaddr_in addr;
										int sock, cnt;
										socklen_t addrlen;
										char sos[BUF_SIZE];

										/* set up socket */
										sock = socket(AF_INET, SOCK_DGRAM, 0);
										if (sock < 0) 
										{
											erro("socket");
											exit(1);
										}

										addr.sin_family = AF_INET;
										addr.sin_addr.s_addr = htonl(INADDR_ANY);
										addr.sin_port = htons(9000);
										addrlen = sizeof(addr);
										
										addr.sin_addr.s_addr = inet_addr("228.0.0.1");
										
										memset(sos, 0, sizeof(sos));
										strcat(id, ": ");
										strcat(id, "SOS!!!\n");
										strcat(sos, id);
										
										for (int i=0; i<1; i++)
										{
											cnt=sendto(sock, sos, sizeof(sos), MSG_CONFIRM, (struct sockaddr *) &addr, addrlen);
											if (cnt < 0) 
											{
												perror("sendto");
												exit(1);
											}
											else if (cnt == 0) 
												break;
											
										}
										
										sendto(sockfd, "Enviado!\n", strlen("Enviado!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										goto inicio2;
									}
									
									if (memcmp( buffer, "6", strlen( "6") ) == 0 )
									{
										sendto(sockfd, (const char *)xau, strlen(xau), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
										goto inicio1;
									}
								}
								else
								{
									sendto(sockfd, "Conta não validada!\n", strlen("Conta não validada!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
									goto inicio1;
								}
							}
							else
							{
								sendto(sockfd, "Profi. Errada!\n", strlen("Profi. Errada!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
								goto inicio1;
							}
						}
						else
						{
							sendto(sockfd, "Password errada!\n", strlen("Password errada!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
							goto inicio1;
						}
					}
					else
					{
						sendto(sockfd, "Conta errada!\n", strlen("Conta errada!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len); 
						goto inicio1;
					}
				}
				else
				{
					sendto(sockfd, "Erro ao procurar!\n", strlen("Erro ao procurar!\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
					goto inicio1;
				}
			}
			
			if (memcmp( buffer, "2", strlen( "2") ) == 0 )
			{
				//manda pedir
				sendto(sockfd, "User: ", strlen("User: "), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				//recebe info
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &serverStorage,&addr_size);
				buffer[n] = '\0';
				printf("Client udp: %s\n", buffer);
								
				ftxt=fopen("Login.txt","a");
				fprintf(ftxt,"%s\n",buffer);
				fclose(ftxt);
				
				sendto(sockfd, "Criado!", strlen("Criado!"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				goto inicio1;
			}
			
			if (memcmp( buffer, "3", strlen( "3") ) == 0 )
			{
				fh=fopen("help.txt","r");
				fseek(fh,0,SEEK_END);  //move para o fim do ficheiro
				long int fsize = ftell(fh); //devolve o valor até à posição marcada (fim do ficheiro)
				fseek(fh,0,SEEK_SET); //move para o início do ficheiro
				fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
				
				fread(fcontent,1,fsize,fh); 
				fclose(fh);
				
				sendto(sockfd, "3\n", strlen("3\n"), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				sendto(sockfd, fcontent, strlen(fcontent), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				goto inicio1;
			}
			
			if (memcmp( buffer, "4", strlen( "4") ) == 0 )
			{
				sendto(sockfd, (const char *)xau, strlen(xau), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
				goto inicio1;
			}
		}
		else //processo pai
		{
			if (fork()==0)     //processo filho-aas_tcp
			{ 
				int fd, client;
				struct sockaddr_in addr, client_addr;
				socklen_t client_addr_size;

				//bzero((void *) &addr, sizeof(addr));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = htonl(INADDR_ANY);
				addr.sin_port        = htons(10001);

				if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
				{
					erro("na funcao socket1");
					exit(-1);
				}
				if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
				{
					erro("na funcao bind1");
					exit(-1);
				}
				if( listen(fd, 5) < 0) 
				{
					erro("na funcao listen1");
					exit(-1);
				}
		  
				int nclientes=0;
			
				while (1) 
				{
					client_addr_size = sizeof(client_addr);
					client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
					nclientes++;
					if (client > 0) 
					{
						if (fork() == 0) 
						{
							close(fd);
							process_client(client);
							exit(0);
						}
						close(client);
					}
				 }
				 close(fd);
			}
			else //processo pai-ags_tcp
			{
				int fd, client;
				struct sockaddr_in addr, client_addr;
				socklen_t client_addr_size;

				//bzero((void *) &addr, sizeof(addr));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = htonl(INADDR_ANY);
				addr.sin_port        = htons(20001);

				if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
				{
					erro("na funcao socket2");
					exit(-1);
				}
				if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
				{
					erro("na funcao bind2");
					exit(-1);
				}
				if( listen(fd, 5) < 0) 
				{
					erro("na funcao listen2");
					exit(-1);
				}
	  
				int nclientes=0;
		
				while (1) 
				{
					client_addr_size = sizeof(client_addr);
					client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
					nclientes++;
					if (client > 0) 
					{
						if (fork() == 0) 
						{
							close(fd);
							process_gestor(client);
							exit(0);
						}
						close(client);
					}
				}
				close(fd);
			}
		}   
		return 0;
}

void process_client(int client_fd)
{
    char buffer[BUF_SIZE];
    char titulo[BUF_SIZE]="Bem-vindo ao programa de registos de crimes!\n\n1.Login\n2.Criar conta\n3.Help\n4.Sair\nIndique a opção: ";
    char *fcontent;
    size_t leng = 100;
    char *linha = malloc(leng);
    char *token;
    char id[1024], pass[1024], prof[1024],val[1024];
	int opcao = 0;
	FILE *fp;
	FILE *fptemp;
	FILE *ft;
	FILE *fh;
    
    //escreve titulo
    write(client_fd, titulo,1 + strlen(titulo));
    //le a opção
    read(client_fd,buffer, BUF_SIZE-1);
    printf("Client A: %s\n", buffer);		
		
	if (strcmp(buffer, "1") == 0 )
	{
		//pede a conta
		write(client_fd, "Login: ",BUF_SIZE-1);
		//le a conta
		read(client_fd,buffer, BUF_SIZE-1);
		printf("Client A: %s\n", buffer);
		
		if ((fp = fopen ("Login.txt","r")) != NULL) 
		{
			while (getline(&linha, &leng, fp) > 0)
			{
				token = strtok(linha,";");
				//copia o caracter '\0' (zero) para o tamanho (length) para o qual o ponteiro id esta a apontar
				memset(id, '\0', sizeof(id));  
				memset(pass, '\0', sizeof(pass));
				memset(prof, '\0', sizeof(prof));
				memset(val, '\0', sizeof(val));
					
				opcao = 0;
					
				while(token!=NULL)
				{
					switch (opcao)
					{
						case 0:
							strcpy(id, token);
							break;
						case 1:
							strcpy(pass, token);
							break;
						case 2:
							strcpy(prof, token);
							break;
						case 3:
							strcpy(val, token);
							break;
					}
					opcao++;
					token = strtok(NULL, ";");
				}	
				if (strcmp(id, buffer) == 0) break;
			}
			fclose(fp);	
			if (strcmp(id, buffer) == 0)
			{
				//manda a pedir password
				write(client_fd, "Password: ",1+strlen("Password: "));
				//le a password recebida
				read(client_fd,buffer, BUF_SIZE-1);

				printf("Client A: %s\n", buffer);
				
				if (strcmp(pass, buffer) == 0)
				{
					if(strcmp(prof, "AS") == 0)
					{
						if (strcmp(val, "1")==0)
						{
							write(client_fd,"\n----------------MENU--------------\n\n1.Consultar\n2.Alterar conta\n3.Apagar\n4.Help\n5.Sair\nIndique a opção: ", BUF_SIZE-1);
							//le a opção
							read(client_fd,buffer, BUF_SIZE-1);
							
							if (strcmp(buffer, "1") == 0 )
							{
								write(client_fd,"\n1.Tudo\n2.Por data\n3.Por horas\n4.Por local\n5.Por tipo\n6.Por nome\nIndique a opção: ", BUF_SIZE-1);
								//le a opção
								read(client_fd, buffer, BUF_SIZE-1);
								printf("Client A: %s\n",buffer);
								
								if (strcmp(buffer, "1") == 0 )
								{
									ftxt=fopen("crimes.txt","r");
									if(ftxt)
									{
										fseek(ftxt,0,SEEK_END);  //move para o fim do ficheiro
										long int fsize = ftell(ftxt); //devolve o valor até à posição marcada (fim do ficheiro)
										fseek(ftxt,0,SEEK_SET); //move para o início do ficheiro
										fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
															  //le a informação dada pelo ficheiro ftxt, byte a byte, no array, apontado por fcontent
										fread(fcontent,1,fsize,ftxt); 
										fclose(ftxt);
									} 
									//manda a informação toda do obtida do ficheiro
									write(client_fd, fcontent, 1 + strlen(fcontent));
								}
								
								if (strcmp(buffer, "2") == 0 )
								{
									write(client_fd,"Indique a data (dd/mm/aaaa): ",BUF_SIZE-1);
									//recebe data
									read(client_fd,buffer, BUF_SIZE-1);
									
									char data[1024],hora[1024],local[1024],tipo[1024],nome[1024];
									int op, x=0;
									
									ftxt=fopen("crimes.txt","r");
									ft=fopen("tempo.tmp", "w");
									
									while( getline(&linha,&leng,ftxt)>0)
									{	
								
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										op=0;
										
										while(token!=NULL)
										{	
											switch(op)
											{
												case 0: strcpy(data,token);
														break;
												case 1: strcpy(hora,token);
														break;
												case 2: strcpy(local,token);
														break;
												case 3: strcpy(tipo,token);
														break;
												case 4: strcpy(nome,token);
														break;
											}
											op++;
											token = strtok(NULL, ";");
										}
										if (strcmp(data, buffer)==0)
										{
											//se houver correspondência x incrementa 1 valor
											x++;   
											fputs(data,ft);
											fputs(";",ft);
											fputs(hora,ft);
											fputs(";",ft);
											fputs(local,ft);
											fputs(";",ft);
											fputs(tipo,ft);
											fputs(";",ft);
											fputs(nome,ft);
											fputs(";",ft);
											fputs("\n",ft);
										}
									}
									if (x==0)
										fputs("Data não existe!\n",ft);
										
									fclose(ftxt);
									fclose(ft);
									
									ft=fopen("tempo.tmp","r");
									
									fseek(ft,0,SEEK_END); 
									long int fsize = ftell(ft); 
									fseek(ft,0,SEEK_SET); 
									fcontent = malloc (fsize); 
									fread(fcontent,1,fsize,ft);
									
									write(client_fd, fcontent, 1 + strlen(fcontent));
									
									fclose(ft);
									remove("tempo.tmp");
								}
								if (strcmp(buffer, "3") == 0 )
								{
									write(client_fd,"Indique a hora (hh:mm:ss): ",BUF_SIZE-1);
									//recebe hora
									read(client_fd,buffer, BUF_SIZE-1);
									
									char data[1024],hora[1024],local[1024],tipo[1024],nome[1024];
									int op, x=0;
									
									ftxt=fopen("crimes.txt","r");
									ft=fopen("tempo.tmp", "w");
									
									while( getline(&linha,&leng,ftxt)>0)
									{	
								
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										op=0;
										
										while(token!=NULL)
										{	
											switch(op)
											{
												case 0: strcpy(data,token);
														break;
												case 1: strcpy(hora,token);
														break;
												case 2: strcpy(local,token);
														break;
												case 3: strcpy(tipo,token);
														break;
												case 4: strcpy(nome,token);
														break;
											}
											op++;
											token = strtok(NULL, ";");
										}
										if (strcmp(hora, buffer)==0)
										{
											x++;
											fputs(data,ft);
											fputs(";",ft);
											fputs(hora,ft);
											fputs(";",ft);
											fputs(local,ft);
											fputs(";",ft);
											fputs(tipo,ft);
											fputs(";",ft);
											fputs(nome,ft);
											fputs(";",ft);
											fputs("\n",ft);
										}
									}
									if (x==0)
										fputs("Hora não existe!\n",ft);
										
									fclose(ftxt);
									fclose(ft);
									
									ft=fopen("tempo.tmp","r");
									
									fseek(ft,0,SEEK_END); 
									long int fsize = ftell(ft); 
									fseek(ft,0,SEEK_SET); 
									fcontent = malloc (fsize); 
									fread(fcontent,1,fsize,ft);
									
									write(client_fd, fcontent, 1 + strlen(fcontent));
									
									fclose(ft);
									remove("tempo.tmp");
								}
								if (strcmp(buffer, "4") == 0 )
								{
									write(client_fd,"Indique o local: ",BUF_SIZE-1);
									//recebe local
									read(client_fd,buffer, BUF_SIZE-1);
									char data[1024],hora[1024],local[1024],tipo[1024],nome[1024];
									int op, x=0;
									
									ftxt=fopen("crimes.txt","r");
									ft=fopen("tempo.tmp", "w");
									
									while( getline(&linha,&leng,ftxt)>0)
									{	
								
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										op=0;
										
										while(token!=NULL)
										{	
											switch(op)
											{
												case 0: strcpy(data,token);
														break;
												case 1: strcpy(hora,token);
														break;
												case 2: strcpy(local,token);
														break;
												case 3: strcpy(tipo,token);
														break;
												case 4: strcpy(nome,token);
														break;
											}
											op++;
											token = strtok(NULL, ";");
										}
										if (strcmp(local, buffer)==0)
										{
											x++;
											fputs(data,ft);
											fputs(";",ft);
											fputs(hora,ft);
											fputs(";",ft);
											fputs(local,ft);
											fputs(";",ft);
											fputs(tipo,ft);
											fputs(";",ft);
											fputs(nome,ft);
											fputs(";",ft);
											fputs("\n",ft);
										}
									}
									if (x==0)
										fputs("Local não existe!\n",ft);
										
									fclose(ftxt);
									fclose(ft);
									
									ft=fopen("tempo.tmp","r");
									
									fseek(ft,0,SEEK_END); 
									long int fsize = ftell(ft); 
									fseek(ft,0,SEEK_SET); 
									fcontent = malloc (fsize); 
									fread(fcontent,1,fsize,ft);
									
									write(client_fd, fcontent, 1 + strlen(fcontent));
									
									fclose(ft);
									remove("tempo.tmp");
								}
								if (strcmp(buffer, "5") == 0 )
								{
									write(client_fd,"Indique o tipo: ",BUF_SIZE-1);
									//recebe tipo
									read(client_fd,buffer, BUF_SIZE-1);
									char data[1024],hora[1024],local[1024],tipo[1024],nome[1024];
									int op, x=0;
									
									ftxt=fopen("crimes.txt","r");
									ft=fopen("tempo.tmp", "w");
									
									while( getline(&linha,&leng,ftxt)>0)
									{	
								
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										op=0;
										while(token!=NULL)
										{	
											switch(op)
											{
												case 0: strcpy(data,token);
														break;
												case 1: strcpy(hora,token);
														break;
												case 2: strcpy(local,token);
														break;
												case 3: strcpy(tipo,token);
														break;
												case 4: strcpy(nome,token);
														break;
											}
											op++;
											token = strtok(NULL, ";");
										}
										if (strcmp(tipo, buffer)==0)
										{
											x++;
											fputs(data,ft);
											fputs(";",ft);
											fputs(hora,ft);
											fputs(";",ft);
											fputs(local,ft);
											fputs(";",ft);
											fputs(tipo,ft);
											fputs(";",ft);
											fputs(nome,ft);
											fputs(";",ft);
											fputs("\n",ft);
										}
									}
									if (x==0)
										fputs("Tipo não existe!\n",ft);
										
									fclose(ftxt);
									fclose(ft);
									
									ft=fopen("tempo.tmp","r");
									
									fseek(ft,0,SEEK_END); 
									long int fsize = ftell(ft); 
									fseek(ft,0,SEEK_SET); 
									fcontent = malloc (fsize); 
									fread(fcontent,1,fsize,ft);
									
									write(client_fd, fcontent, 1 + strlen(fcontent));
									
									fclose(ft);
									remove("tempo.tmp");
								}
								if (strcmp(buffer, "6") == 0 )
								{
									write(client_fd,"Indique o nome: ",BUF_SIZE-1);
									//recebe nome
									read(client_fd,buffer, BUF_SIZE-1);
									
									char data[1024],hora[1024],local[1024],tipo[1024],nome[1024];
									int op, x=0;
									
									ftxt=fopen("crimes.txt","r");
									ft=fopen("tempo.tmp", "w");
									
									while( getline(&linha,&leng,ftxt)>0)
									{	
								
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										op=0;
										
										while(token!=NULL)
										{	
											switch(op)
											{
												case 0: strcpy(data,token);
														break;
												case 1: strcpy(hora,token);
														break;
												case 2: strcpy(local,token);
														break;
												case 3: strcpy(tipo,token);
														break;
												case 4: strcpy(nome,token);
														break;
											}
											op++;
											token = strtok(NULL, ";");
										}
										if (strcmp(nome, buffer)==0)
										{
											x++;
											fputs(data,ft);
											fputs(";",ft);
											fputs(hora,ft);
											fputs(";",ft);
											fputs(local,ft);
											fputs(";",ft);
											fputs(tipo,ft);
											fputs(";",ft);
											fputs(nome,ft);
											fputs(";",ft);
											fputs("\n",ft);
										}
									}
									if (x==0)
										fputs("Nome não existe!\n",ft);
										
									fclose(ftxt);
									fclose(ft);
									
									ft=fopen("tempo.tmp","r");
									
									fseek(ft,0,SEEK_END); 
									long int fsize = ftell(ft); 
									fseek(ft,0,SEEK_SET); 
									fcontent = malloc (fsize); 
									fread(fcontent,1,fsize,ft);
									
									write(client_fd, fcontent, 1 + strlen(fcontent));
									
									fclose(ft);
									remove("tempo.tmp");
								}
							}
							if (strcmp(buffer, "2") == 0 )
							{
								write(client_fd,"\n1.Conta\n2.Password\nIndique a opção: ", BUF_SIZE-1);
								//recebe a opção
								read(client_fd,buffer, BUF_SIZE-1);
								printf("Client A: %s\n", buffer);
								
								if (strcmp(buffer, "1") == 0 )
								{
									write(client_fd, "Indique a nova conta: ",BUF_SIZE-1);
									//recebe nova conta
									read(client_fd,buffer,BUF_SIZE-1);
									printf("Client A: %s\n", buffer);
									
									fp = fopen( "Login.txt", "r");
									fptemp=fopen("temp.tmp", "w");
									
									while( getline(&linha,&leng,fp)>0)
									{
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										
										if(strcmp(token,id)==0)
										{
											buffer[strcspn(buffer,"\n")]='\0';
											fputs(buffer,fptemp);
											fputs(";",fptemp);
											fputs(pass,fptemp);
											fputs(";",fptemp);
											fputs(prof,fptemp);
											fputs(";",fptemp);
											fputs(val,fptemp);
											fputs(";\n",fptemp);
										}
										if(strcmp(token,id)!=0) 
											fputs(aux,fptemp);
									}
									fclose(fp);
									fclose(fptemp);
									
									remove("Login.txt");
									rename("temp.tmp", "Login.txt");
									
									write(client_fd, "Alterado!\n", BUF_SIZE-1);
									close(client_fd);
								}
								if (strcmp(buffer, "2") == 0 )
								{
									write(client_fd, "Indique a nova pass: ", BUF_SIZE-1);
									//recebe nova pass
									read(client_fd,buffer,BUF_SIZE-1);
									printf("Client A: %s\n", buffer);
									
									fp = fopen( "Login.txt", "r");
									fptemp=fopen("temp.tmp", "w");
									
									while( getline(&linha,&leng,fp)>0)
									{
										char aux[1024];
										strcpy(aux,linha);
										token = strtok(linha,";");
										
										if(strcmp(token,id)==0) 
										{
											fputs(linha,fptemp);
											fputs(";",fptemp);
											buffer[strcspn(buffer,"\n")]='\0';
											fputs(buffer,fptemp);
											fputs(";",fptemp);
											fputs(prof,fptemp);
											fputs(";",fptemp);
											fputs(val,fptemp);
											fputs(";\n",fptemp);
										}
										if(strcmp(token,id)!=0) 
											fputs(aux,fptemp);
									}
									fclose(fp);
									fclose(fptemp);
									
									remove("Login.txt");
									rename("temp.tmp", "Login.txt");
									
									write(client_fd, "Alterado!\n", BUF_SIZE-1);
									close(client_fd);
								}
							}
							if (strcmp(buffer, "3") == 0 )
							{	
								fp = fopen( "Login.txt", "r");
								fptemp=fopen("temp.tmp", "w");
								
								while( getline(&linha,&leng,fp)>0)
								{
									char aux[1024];
									strcpy(aux,linha);
									token = strtok(linha,";");
								
									if(strcmp(token,id)!=0) 
									  fputs(aux,fptemp);
								}
								fclose(fp);
								fclose(fptemp);
								
								remove("Login.txt");
								rename("temp.tmp", "Login.txt");
								
								write(client_fd,"Apagado!\n", BUF_SIZE-1);
								close(client_fd);
							}
							if (strcmp(buffer, "4") == 0 )
							{
								fh=fopen("help2.txt","r");
								fseek(fh,0,SEEK_END);  //move para o fim do ficheiro
								long int fsize = ftell(fh); //devolve o valor até à posição marcada (fim do ficheiro)
								fseek(fh,0,SEEK_SET); //move para o início do ficheiro
								fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
										
								fread(fcontent,1,fsize,fh); 
								fclose(fh);
										
								write(client_fd,"4\n",BUF_SIZE-1);
								write(client_fd,fcontent,strlen(fcontent)+1);
							}
							if (strcmp(buffer, "5") == 0 )
							{
								write(client_fd,"ByeBye!", BUF_SIZE-1);
								close(client_fd);
							}
						}
						else
						{
							write(client_fd,"Conta não validada!\n", BUF_SIZE-1);
							close(client_fd);
						}
					}
					else
					{
						write(client_fd,"Profi. errada!\n", BUF_SIZE-1);
						close(client_fd);
					}
				}
				else
				{
				  write(client_fd, "Password errada!\n", BUF_SIZE-1);
				  close(client_fd);
				}
			}
			else
			{
				write(client_fd, "Conta errada!\n", BUF_SIZE-1);
				close(client_fd);
			}
		}
		else
		{
			write(client_fd, "Erro ao procurar!", BUF_SIZE-1);
			close(client_fd);
		}
	}
	if (strcmp(buffer, "2") == 0 )
	{
		//manda pedir
		write(client_fd, "User: ", BUF_SIZE-1);
		//recebe info
		read(client_fd,buffer,BUF_SIZE-1);
		printf("Client A: %s\n", buffer);
							
		ftxt=fopen("Login.txt","a");
		fprintf(ftxt,"%s\n",buffer);
		
		fclose(ftxt);
		write(client_fd,"Criado!\n",BUF_SIZE-1);
	}
	if (strcmp(buffer, "3")==0)
	{
		fh=fopen("help.txt","r");
		fseek(fh,0,SEEK_END);  //move para o fim do ficheiro
		long int fsize = ftell(fh); //devolve o valor até à posição marcada (fim do ficheiro)
		fseek(fh,0,SEEK_SET); //move para o início do ficheiro
		fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
				
		fread(fcontent,1,fsize,fh); 
		fclose(fh);
				
		write(client_fd,"3\n",BUF_SIZE-1);
		write(client_fd,fcontent,strlen(fcontent)+1);
	}
	if (strcmp(buffer, "4") == 0 )
		write(client_fd,"ByeBye!", BUF_SIZE-1);
	fflush(stdout);
	close(client_fd);
}

void process_gestor(int gestor_fd)
{
    char buffer[BUF_SIZE];
    char titulo[BUF_SIZE]="Bem-vindo ao programa central!\nLogin: " ;
    char *fcontent;
    size_t leng = 100;
	char *linha = malloc(leng);
	char *token;
    char id[1024], pass[1024], prof[1024], val[1024], valor[]="1";
	int opcao = 0;
	FILE *fp;
	FILE *fptemp;
	FILE *fh;
    
    //escreve titulo
    write(gestor_fd, titulo,1 + strlen(titulo));
    //le a conta
    read(gestor_fd,buffer, BUF_SIZE-1);
    printf("Client B: %s\n", buffer);		
	
	if ((fp = fopen ("Login.txt","r")) != NULL) 
	{
		while (getline(&linha, &leng, fp) > 0)
		{
			token = strtok(linha,";");
				
			memset(id, '\0', sizeof(id));
			memset(pass, '\0', sizeof(pass));
			memset(prof, '\0', sizeof(prof));
			memset(val, '\0', sizeof(val));
				
			opcao = 0;
				
			while(token!=NULL)
			{
				switch (opcao)
				{
					case 0:
						strcpy(id, token);
						break;
					case 1:
						strcpy(pass, token);
						break;
					case 2:
						strcpy(prof, token);
						break;
					case 3:
						strcpy(val, token);
						break;
				}
				opcao++;
				token = strtok(NULL, ";");
			}	
			if (strcmp(id, buffer) == 0) break;
		}
		fclose(fp);	
		
		if (strcmp(id, buffer) == 0)
		{
			write(gestor_fd, "Password: ",1+strlen("Password: "));
			read(gestor_fd,buffer, BUF_SIZE-1);
			
			printf("Client B: %s\n", buffer);
			
			if (strcmp(pass, buffer) == 0)
			{
				if(strcmp(prof, "GS") == 0)
				{
					write(gestor_fd,"\n----------------MENU--------------\n\n1.Validar\n2.Consultar\n3.Apagar\n4.Help\n5.Sair\nIndique a opção: ", BUF_SIZE-1); 
					//le a opção
					read(gestor_fd,buffer, BUF_SIZE-1);
					printf("Client B: %s\n", buffer);
					
					if (strcmp(buffer, "1") == 0 )
					{
						char a[1024],b[1024],c[1024];
						int op;
						
						write(gestor_fd, "Indique a conta a validar: ", BUF_SIZE-1); 
						//le a conta a apagar
						read(gestor_fd,buffer, BUF_SIZE-1);
						
						fp = fopen( "Login.txt", "r");
						fptemp=fopen("temp.tmp", "w");
						
						while( getline(&linha,&leng,fp)>0)
						{
							char aux[1024];
							strcpy(aux,linha);
							token = strtok(linha,";");
							op=0;
							
							while(token!=NULL)
							{
								switch(op)
								{
									case 0: strcpy(a,token);
											break;
									case 1: strcpy(b,token);
											break;
									case 2: strcpy(c,token);
											break;
								}
								op++;
								token = strtok(NULL, ";");
							}
							if(strcmp(a,buffer)==0) 
							{
								fputs(a,fptemp);
								fputs(";",fptemp);
								fputs(b,fptemp);
								fputs(";",fptemp);
								fputs(c,fptemp);
								fputs(";",fptemp);
								fputs(valor,fptemp);
								fputs(";\n",fptemp);
							}
							if(strcmp(a,buffer)!=0) 
								fputs(aux,fptemp);
						}
						fclose(fp);
						fclose(fptemp);
						
						remove("Login.txt");
						rename("temp.tmp", "Login.txt");
						
						write(gestor_fd,"Validado!", BUF_SIZE-1);
					}
					if (strcmp(buffer, "2") == 0 )
					{
						fp=fopen("Login.txt","r");
						
						write(gestor_fd, "2", BUF_SIZE-1);
						
						fseek(fp,0,SEEK_END); 
						long int fsize = ftell(fp); 
						fseek(fp,0,SEEK_SET); 
						fcontent = malloc (fsize); 
						fread(fcontent,1,fsize,fp);
						
						write(gestor_fd, fcontent, 1 + strlen(fcontent));
						
						fclose(fp);
					}
					if (strcmp(buffer, "3") == 0 )
					{
						write(gestor_fd, "Indique a conta a apagar: ", BUF_SIZE-1); 
						//le a conta a apagar
						read(gestor_fd,buffer, BUF_SIZE-1);
						
						fp = fopen( "Login.txt", "r");
						fptemp=fopen("temp.tmp", "w");
						
						while( getline(&linha,&leng,fp)>0)
						{
							char aux[1024];
							strcpy(aux,linha);
							token = strtok(linha,";");
								
							if(strcmp(token,buffer)!=0) 
								fputs(aux,fptemp);
						}
						fclose(fp);
						fclose(fptemp);
						
						remove("Login.txt");
						rename("temp.tmp", "Login.txt");
						
						write(gestor_fd,"Apagado!", BUF_SIZE-1);
					}
					if (strcmp(buffer, "4") == 0 )
					{
						fh=fopen("help3.txt","r");
						fseek(fh,0,SEEK_END);  //move para o fim do ficheiro
						long int fsize = ftell(fh); //devolve o valor até à posição marcada (fim do ficheiro)
						fseek(fh,0,SEEK_SET); //move para o início do ficheiro
						fcontent = malloc (fsize);  //malloc-retorna o ponteiro para a memŕia alocada
										
						fread(fcontent,1,fsize,fh); 
						fclose(fh);
										
						write(gestor_fd,"4\n",BUF_SIZE-1);
						write(gestor_fd,fcontent,strlen(fcontent)+1);
					}
					if (strcmp(buffer, "5") == 0 )
					{
						write(gestor_fd,"ByeBye!", BUF_SIZE-1);
						close(gestor_fd);
					}
				}
				else
				{
					write(gestor_fd,"Profi. errada!", BUF_SIZE-1);
					close(gestor_fd);
				}
			}
			else
			{
			  write(gestor_fd, "Password errada!\n", BUF_SIZE-1);
			  close(gestor_fd);
		  }
		}
		else
		{
			write(gestor_fd, "Conta errada!\n", BUF_SIZE-1);
			close(gestor_fd);
		}
	}
	else
	{
		write(gestor_fd, "Erro ao procurar!", BUF_SIZE-1);
		close(gestor_fd);
	}
    fflush(stdout);
    close(gestor_fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
