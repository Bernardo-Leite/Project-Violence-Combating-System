// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#include <fcntl.h>
  
#define PORT     8080
#define MAXLINE 1024
#define BUF_SIZE 1024
 

// Driver code
int main() 
{
    int sockfd, nread, nBytes;
    char buffer[MAXLINE];
    
    char pontuacao[]=";";
    char pontos[]=":";
    char barra[]="/";
    char local[BUF_SIZE];
    char crime[BUF_SIZE];
    char vitima[BUF_SIZE];
    char anonimo[]="Anónimo";
    char total[BUF_SIZE];
    char hours[BUF_SIZE], minutes[BUF_SIZE], seconds[BUF_SIZE], day[BUF_SIZE], month[BUF_SIZE], year[BUF_SIZE];
    char user[BUF_SIZE], passw[BUF_SIZE], PS[]="PS", Val[]="0", tot[BUF_SIZE];
    
    struct sockaddr_in     servaddr;
  
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));
    
    //copia o caracter '\0' (zero) para o tamanho (length) para o qual o ponteiro inicio esta a apontar
    memset(local,'\0',sizeof(local));
    memset(crime,'\0',sizeof(crime));
    memset(vitima,'\0',sizeof(vitima));
    memset(total,'\0',sizeof(total));
      
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8081);
    servaddr.sin_addr.s_addr = INADDR_ANY;
      
    int n;
    socklen_t len;
    
menu1:    printf("\n--------------MENU-------------\n\n");
    printf("1.Login\n2.Criar conta\n3.Help\n4.Sair\nIndique a opção: ");
    
    //manda op
    fgets(buffer,1024,stdin);
	nread = strlen(buffer) + 1;
	sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));

	//recebe confirma
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
	buffer[n] = '\0';
    
    if (memcmp( buffer, "Login: ", strlen( "Login: ") ) == 0 )
    {
		printf("%s", buffer);
		//manda conta
		fgets(buffer,1024,stdin);
		nread = strlen(buffer) + 1;
		sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		//recebe a pedir pass   
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
		buffer[n] = '\0';
		printf("%s ", buffer);
		
		if (memcmp( buffer, "Conta errada!\n", strlen( "Conta errada!\n") ) == 0 )
			goto menu1;
		
		if (memcmp( buffer, "Password: ", strlen( "Password: ") ) == 0 )
		{
			//manda pass
			fgets(buffer,1024,stdin);
			nread = strlen(buffer) + 1;
			sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
			
menu2:		printf("\n");
			//le menu
			n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
			buffer[n] = '\0';
			printf("%s", buffer);
			
			if (memcmp( buffer, "Password errada!\n", strlen( "Password errada!\n") ) == 0 )
				goto menu1;
			
			if (memcmp( buffer, "Profi. errada!\n", strlen( "Profi. errada!\n") ) == 0 )
				goto menu1;
			
			if (memcmp( buffer, "Conta não validada!\n", strlen( "Conta não validada!\n") ) == 0 )
				goto menu1;
				
			if (memcmp( buffer,"\n-----------MENU-----------\n\n1.Registar crime\n2.Alterar conta\n3.Apagar conta\n4.Help\n5.Alarme\n6.Sair\nIndique a opção: ", strlen("\n-----------MENU-----------\n\n1.Registar crime\n2.Alterar conta\n3.Apagar conta\n4.Help\n5.Sair\nIndique a opção: ") ) == 0 )
			{
				//manda a opção
				fgets(buffer,1024,stdin);
				nread = strlen(buffer) + 1;
				sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
				
				//recebe confirma ou para indicar conta/pass
				n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
				buffer[n] = '\0';
				
				if (memcmp( buffer, "\n1.Registar com nome\n2.Registar em anonimato\n3.Voltar\nIndique a opção: ", strlen( "\n1.Registar com nome\n2.Registar em anonimato\n3.Voltar\nIndique a opção: ") ) == 0 )
				{	
					printf("%s", buffer);
					//manda a opção
					fgets(buffer,1024,stdin);
					nread = strlen(buffer) + 1;
					sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
					
					n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
					
					if (memcmp( buffer, "1\n", strlen( "1\n") ) == 0 )
					{
					
						time_t now;
						time(&now); 	
							
						struct tm *tempo = localtime(&now);
							
						sprintf(hours, "%d", tempo->tm_hour);       	// get hours since midnight (0-23)
						sprintf(minutes, "%d", tempo->tm_min);        	// get minutes passed after the hour (0-59)
						sprintf(seconds, "%d", tempo->tm_sec);        	// get seconds passed after minute (0-59) 	
								
						sprintf(day, "%d", tempo->tm_mday);            	// get day of month (1 to 31)
						sprintf(month, "%d", tempo->tm_mon + 1);       	// get month of year (0 to 11)
						sprintf(year, "%d", tempo->tm_year + 1900);    	// get year since 1900

						strcat(day,barra);  //concatena as duas strings
						strcat(total,day);
						strcat(month,barra);
						strcat(total,month);
						strcat(year,pontuacao);
						strcat(total,year);

						strcat(hours,pontos);
						strcat(total,hours);
						strcat(minutes,pontos);
						strcat(total,minutes);
						strcat(seconds,pontuacao);
						strcat(total,seconds);

						printf("Local do crime: ");
						fgets(local,BUF_SIZE,stdin);
						//strcspn devolve o numero de caracteres antes das duas strings haja coincidencia (\n) e coloca a 0 na posição desse valor
						local[strcspn(local,"\n")]='\0';  
						strcat(local, pontuacao);
						strcat(total, local);

						printf("Tipo de crime: ");
						fgets(crime,BUF_SIZE,stdin);
						crime[strcspn(crime,"\n")]='\0';
						strcat(crime, pontuacao);
						strcat(total,crime);

						printf("Nome da vitima: ");
						fgets(vitima,BUF_SIZE,stdin);
						vitima[strcspn(vitima,"\n")]='\0';
						strcat(vitima, pontuacao);
						strcat(total, vitima);
							
						nBytes = strlen(total) + 1;
						//manda info
						sendto(sockfd, total, nBytes,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
						goto menu2;
					}
					
					if (memcmp( buffer, "2\n", strlen( "2\n") ) == 0 )
					{
					
						time_t now;
						time(&now); 	
							
						struct tm *tempo = localtime(&now);
							
						sprintf(hours, "%d", tempo->tm_hour);       	// get hours since midnight (0-23)
						sprintf(minutes, "%d", tempo->tm_min);        	// get minutes passed after the hour (0-59)
						sprintf(seconds, "%d", tempo->tm_sec);        	// get seconds passed after minute (0-59) 	
								
						sprintf(day, "%d", tempo->tm_mday);            	// get day of month (1 to 31)
						sprintf(month, "%d", tempo->tm_mon + 1);       	// get month of year (0 to 11)
						sprintf(year, "%d", tempo->tm_year + 1900);    	// get year since 1900

						strcat(day,barra);  //concatena as duas strings
						strcat(total,day);
						strcat(month,barra);
						strcat(total,month);
						strcat(year,pontuacao);
						strcat(total,year);

						strcat(hours,pontos);
						strcat(total,hours);
						strcat(minutes,pontos);
						strcat(total,minutes);
						strcat(seconds,pontuacao);
						strcat(total,seconds);

						printf("Local do crime: ");
						fgets(local,BUF_SIZE,stdin);
						//strcspn devolve o numero de caracteres antes das duas strings haja coincidencia (\n) e coloca a 0 na posição desse valor
						local[strcspn(local,"\n")]='\0';  
						strcat(local, pontuacao);
						strcat(total, local);

						printf("Tipo de crime: ");
						fgets(crime,BUF_SIZE,stdin);
						crime[strcspn(crime,"\n")]='\0';
						strcat(crime, pontuacao);
						strcat(total,crime);

						strcat(anonimo, pontuacao);
						strcat(total, anonimo);
							
						nBytes = strlen(total) + 1;
						//manda info
						sendto(sockfd, total, nBytes,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
						goto menu2;
					}
					
					if (memcmp( buffer, "3\n", strlen( "3\n") ) == 0 )
						goto menu2;
				}
				
				if (memcmp( buffer, "\n1.Conta\n2.Password\n3.Voltar\nIndique a opção: ", strlen("\n1.Conta\n2.Password\n3.Voltar\nIndique a opção: ") ) == 0 )
				{
					printf("%s", buffer);
					//manda a opção
					fgets(buffer,1024,stdin);
					nread = strlen(buffer) + 1;
					sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
					
					//recebe a pedir nova conta ou pass
					n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
					buffer[n] = '\0';
					printf("%s", buffer);
					
					if (memcmp( buffer, "Indique a nova conta: ", strlen("Indique a nova conta: ") ) == 0 )
					{
						//manda nova conta
						fgets(buffer,1024,stdin);
						nread = strlen(buffer) + 1;
						sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
						
						n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
						buffer[n] = '\0';
						printf("%s", buffer);
						if (memcmp( buffer, "Alterado!\n", strlen("Alterado!\n") ) == 0 )
							goto menu1;
					}
					
					if (memcmp( buffer, "Indique a nova pass: ", strlen("Indique a nova pass: ") ) == 0 )
					{
						//manda nova pass
						fgets(buffer,1024,stdin);
						nread = strlen(buffer) + 1;
						sendto(sockfd, buffer, nread,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
						
						n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
						buffer[n] = '\0';
						printf("%s", buffer);
						if (memcmp( buffer, "Alterado!\n", strlen("Alterado!\n") ) == 0 )
							goto menu1;
					}
					
					if (memcmp( buffer, "3\n", strlen( "3\n") ) == 0 )
						goto menu2;	
				}
				
				if (memcmp( buffer, "Apagado!", strlen( "Apagado!") ) == 0 )
				{
					printf("%s", buffer);
					goto menu1;
				}
				
				if (memcmp( buffer, "4\n", strlen( "4\n") ) == 0 )
				{
					printf("\n");
					n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
					buffer[n] = '\0';
					printf("%s", buffer);
					goto menu2;
				}
				
				if (memcmp( buffer, "Enviado!\n", strlen( "Enviado!\n") ) == 0 )
				{
					printf("%s", buffer);
					goto menu2;
				}
					
				if (memcmp( buffer, "ByeBye!", strlen( "ByeBye!") ) == 0 )
				{
					printf("%s", buffer);
					printf("\n");
					exit(-1);
				}
			}
		}
	}

	if (memcmp( buffer, "User: ", strlen( "User: ") ) == 0 )
	{
		printf("%s", buffer);
		
		memset(user,'\0',sizeof(user));
		memset(passw,'\0',sizeof(passw));
		memset(tot,'\0',sizeof(tot));
		
		fgets(user,BUF_SIZE,stdin);
		user[strcspn(user,"\n")]='\0';  
		strcat(user, pontuacao);
		strcat(tot, user);
		
		printf("Password: ");
		fgets(passw,BUF_SIZE,stdin);
		passw[strcspn(passw,"\n")]='\0';  
		strcat(passw, pontuacao);
		strcat(tot, passw);
		
		strcat(PS, pontuacao);
		strcat(tot, PS);
		strcat(Val, pontuacao);
		strcat(tot, Val);
		
		nBytes = strlen(tot) + 1;
		sendto(sockfd, tot, nBytes,MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
		
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
		buffer[n] = '\0';
		printf("%s", buffer);
		goto menu1;
	}
	
	if (memcmp( buffer, "3\n", strlen( "3\n") ) == 0 )
	{
		printf("\n");
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
		buffer[n] = '\0';
		printf("%s", buffer);
		goto menu1;
	}
	
	if (memcmp( buffer, "ByeBye!", strlen( "ByeBye!") ) == 0 )
	{
		printf("%s", buffer);
		printf("\n");
		exit(-1);
	}
    return 0;
}
