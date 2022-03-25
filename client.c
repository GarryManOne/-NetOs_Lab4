#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <malloc.h>
#include <fcntl.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct sockaddr_in description_sockaddr;
int id_client_socket;
// #include "lab2.h"




int main()
{
    id_client_socket = socket(AF_INET,SOCK_STREAM,0);	                // create soket
	description_sockaddr.sin_family = AF_INET;		                    // режим "интернет"
	description_sockaddr.sin_addr.s_addr = inet_addr ("127.0.0.1");	    // указывем ip сервера 
	description_sockaddr.sin_port  = htons (5432);	

	connect(id_client_socket, (const struct sockaddr *)&description_sockaddr, sizeof (description_sockaddr));	

    int Data = 1;	
    send(id_client_socket,&Data,sizeof(Data),0);
    
    return 0;
}