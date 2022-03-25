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

#include "server.h"

struct sockaddr_in description_sockaddr;
int id_server_socket;

void* Animal(void* atr){

    int* index = (int*) atr;
    int n = 0;

    while (1){
        // Смотрим продолжительность жизни 
        if (db_animals[*index].life_time == 0 || db_animals[*index].startvation_time == 0){
            map[db_animals[*index].coord.x][db_animals[*index].coord.y] = 17;
            pthread_exit(NULL);
        }

        int x = db_animals[*index].coord.x;
        int y = db_animals[*index].coord.y;

        // Случайное передвижение
        switch (GetRandRangeInt(0, 4)){
            case RIGHT:
                if ((x + 1) < kMapSizeX){
                    x += 1;
                }
                else continue;
                break;
            case LEFT: 
                if ((x - 1) > 0){
                    x -= 1;
                }
                else continue;
                break;
            case UP: 
                if ((y - 1) > 0){
                    y -= 1;
                }
                else continue;
                break;
            case DOWN: 
                if ((y + 1) < kMapSizeY){
                    y += 1;
                }
                else continue;
                break;
            default:
                break;
        }

        pthread_mutex_lock(&mutex);

        // Смотрим продолжительность жизни 
        if (db_animals[*index].life_time == 0){
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        // Проверка какое животное находится в этой ячейке
        if(map[x][y] != 17){
            
            // Спаривание :)
            if (db_animals[*index].type == db_animals[map[x][y]].type){
                // Смотрим продолжительность жизни 
                if (db_animals[*index].life_time == 0){
                    pthread_mutex_unlock(&mutex);
                    pthread_exit(NULL);
                }
                CreateThread(GetRandRangeInt(0, kMapSizeX), GetRandRangeInt(0, kMapSizeY), db_animals[*index].type);
                pthread_mutex_unlock(&mutex);
            }
            // Ест
            else if ((db_animals[*index].type + 1 ) % 3 == db_animals[map[x][y]].type){
                // Смотрим продолжительность жизни 
                if (db_animals[*index].life_time == 0){
                    pthread_mutex_unlock(&mutex);
                    pthread_exit(NULL);
                }

                db_animals[map[x][y]].life_time = 0;

                map[db_animals[*index].coord.x][db_animals[*index].coord.y] = 17;


                db_animals[*index].coord.x = x;
                db_animals[*index].coord.y = y;
                db_animals[*index].startvation_time = kStarvationTime;
                
                map[x][y] = *index;

                send(db_animals[*index].socket, &map, sizeof(map), 0);
                // PrintMap();

                pthread_mutex_unlock(&mutex);
            }
            // Его едят
            else{
                // Смотрим продолжительность жизни 
                if (db_animals[*index].life_time == 0){
                    pthread_mutex_unlock(&mutex);
                    pthread_exit(NULL);
                }

                map[db_animals[*index].coord.x][db_animals[*index].coord.y] = 17;

                send(db_animals[*index].socket, &map, sizeof(map), 0);
                // PrintMap();

                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }
        }
        else
        {
            // Смотрим продолжительность жизни 
            if (db_animals[*index].life_time == 0){
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            }

            // Обнуление прошлой ячейки
            map[db_animals[*index].coord.x][db_animals[*index].coord.y] = 17;
            
            // Переход
            db_animals[*index].coord.x = x;
            db_animals[*index].coord.y = y;
            db_animals[*index].life_time -= 1;
            db_animals[*index].startvation_time -= 1;   
            map[x][y] = *index;

            send(db_animals[*index].socket, &map, sizeof(map), 0);
            // PrintMap();
            pthread_mutex_unlock(&mutex);
        }

        // Смотрим продолжительность жизни 
        if (db_animals[*index].life_time == 0){
            pthread_exit(NULL);
        }
        usleep(50000);
    }
    
    return NULL;
}

// void* CreateAnimals(void* arg){
// 	while(1)
// 	{
// 		int new_clent_socket = accept(id_server_socket,0,0);	//получаем идентификатро для нового сокета

//         // Ожидаем данные от клиента 
//         AnimalSocket description_animal;
//         recv(new_clent_socket, &description_animal, siziof(description_animal), 0);

//         pthread_t* animal_id = (pthread_t*)(malloc(sizeof(pthread_t)));

//         // Блокировка
//         pthread_mutex_lock(&mutex);

//         // Расположение животного на карте
//         if (map[description_animal.coord.x][description_animal.coord.y] == 17){
//             for(int i = 0; i < kMapSizeX*kMapSizeY; i++){
//                 if (db_animals[i].type == NONE){
//                     db_animals[i].type = description_animal.type;       // Тип животного              
//                     db_animals[i].coord.x = description_animal.coord.x; // Координаты по X
//                     db_animals[i].coord.y = description_animal.coord.y; // Координаты по Y
//                     db_animals[i].life_time = kLifeTime;                // Продолжительность жизни
//                     db_animals[i].startvation_time = kStarvationTime;   // Продолжительность голодания
//                     db_animals[i].socket = new_clent_socket;

//                     map[description_animal.coord.x][description_animal.coord.y] = i;

//                     int* index = (int*)malloc(sizeof(int));
//                     *index = i;

//                     pthread_create(animal_id, NULL, &Animal, index);
//                     break;
//                 }
//             }
//         }
//         else{
//             // Разблокировка
//             pthread_mutex_unlock(&mutex);

//             // Отправить клиенту что он сдох
//         }

//         // Разблокировка
//         pthread_mutex_unlock(&mutex);
//     }
// }

int main()
{
    srand(time(NULL));

    // Обнуление карты 
    for (int i = 0; i < kMapSizeX; i++){
        for (int j = 0; j < kMapSizeY; j++){
            map[i][j] = 17;
        }
    }

    for(int i = 0; i < kMapSizeX*kMapSizeY; i++){
        db_animals[i].type = NONE;
    }

    // Соедение
    id_server_socket = socket(AF_INET,SOCK_STREAM,0);	//создаем сокет 
	description_sockaddr.sin_addr.s_addr = inet_addr ("127.0.0.1");
	description_sockaddr.sin_port        = htons(5432);
	description_sockaddr.sin_family      = AF_INET;

	bind(id_server_socket, (const struct sockaddr*)&description_sockaddr, sizeof(description_sockaddr)); 
	listen(id_server_socket,10);	//запуск сервера (10 max клентов ожидающих очереди)

    // Создание потоков
    while(1)
	{
		int new_clent_socket = accept(id_server_socket,0,0);	//получаем идентификатро для нового сокета

        // Ожидаем данные от клиента 
        AnimalSocket description_animal;
        recv(new_clent_socket, &description_animal, siziof(description_animal), 0);

        pthread_t* animal_id = (pthread_t*)(malloc(sizeof(pthread_t)));

        // Блокировка
        pthread_mutex_lock(&mutex);

        // Расположение животного на карте
        if (map[description_animal.coord.x][description_animal.coord.y] == 17){
            for(int i = 0; i < kMapSizeX*kMapSizeY; i++){
                if (db_animals[i].type == NONE){
                    db_animals[i].type = description_animal.type;       // Тип животного              
                    db_animals[i].coord.x = description_animal.coord.x; // Координаты по X
                    db_animals[i].coord.y = description_animal.coord.y; // Координаты по Y
                    db_animals[i].life_time = kLifeTime;                // Продолжительность жизни
                    db_animals[i].startvation_time = kStarvationTime;   // Продолжительность голодания
                    db_animals[i].socket = new_clent_socket;

                    map[description_animal.coord.x][description_animal.coord.y] = i;

                    int* index = (int*)malloc(sizeof(int));
                    *index = i;

                    pthread_create(animal_id, NULL, &Animal, index);
                    break;
                }
            }
        }
        else{
            // Разблокировка
            pthread_mutex_unlock(&mutex);

            // Отправить клиенту что он сдох
        }

        // Разблокировка
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}