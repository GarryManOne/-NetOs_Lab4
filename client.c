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

#include "client.h"

struct sockaddr_in description_sockaddr;
int id_client_socket;

// Вывод карты в консоль
void PrintMap(){
    // char* buf;
    for (int i = 0; i < kMapSizeX; i++){
        for (int j = 0; j < kMapSizeY; j++){
            if (data_recv.map[i][j] == 19){
                printf("[*]");
            }
            else{
                
                printf("[%d]", data_recv.map[i][j]);
            }                
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char * argv[])
{
     if(argc != 4)
    {
        printf("Не достаточно аргументов!");
        return 0;
    }
    DataSend data_out;
     
    data_out.coord.x = atoi(argv[1]);   //  Расположение по X
    data_out.coord.y = atoi(argv[2]);   //  Расположение по Y
    data_out.type    = atoi(argv[3]);   //  Тип животного

    id_client_socket = socket(AF_INET,SOCK_STREAM,0);	                // create soket
	description_sockaddr.sin_family = AF_INET;		                    // режим "интернет"
	description_sockaddr.sin_addr.s_addr = inet_addr ("127.0.0.1");	    // указывем ip сервера 
	description_sockaddr.sin_port  = htons (5432);	

	connect(id_client_socket, (const struct sockaddr *)&description_sockaddr, sizeof (description_sockaddr));	

    send(id_client_socket, &data_out, sizeof(data_out), 0);

    while (1)
    {
        recv(id_client_socket, &data_recv, sizeof(DataRecv), 0);
        if (data_recv.dead == 1){
            printf("Умер\n");
            exit(0);
        }
        else{
            PrintMap();
        }
    }
    
    return 0;
}