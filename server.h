#ifndef LAB1_H_INCLUDED
#define LAB1_H_INCLUDED

#include <stdio.h>
#include <pthread.h>
#include <pthread.h>


// ************************* Константы ************************************

// Продолжительность жизни (измеряется в количествах сделанных переходов)
const int kLifeTime = 15;    

// Продолжительность голодания (измеряется в количествах сделанных переходов)
const int kStarvationTime = 10;  

// Размеры карты 
const unsigned int kMapSizeX = 4;
const unsigned int kMapSizeY = 4;

// ************************************************************************


// **************************** Описание объектов *************************

// Виды животных
typedef enum {ANIMAL_1, ANIMAL_2, ANIMAL_3, NONE} TypeAnimal;

// Передвижение
typedef enum {RIGHT, LEFT, UP, DOWN} Direction;

// Координаты
typedef struct {
    int x;
    int y;
} Coordinate;

// Атрибуты животного
typedef struct {
    Coordinate coord;       // Координаты
    TypeAnimal type;        // Тип животного
    int life_time;          // Время жизни
    int startvation_time;   // Время голодания
    int socket;             // Сокет для отправки клиенту
} AnimalAttributes;

// Данные от клиента
typedef struct
{
    Coordinate coord;       // Координаты
    TypeAnimal type;        // Тип животного
} DataRecv;

// Данные для клиента
typedef struct
{
    int map[4][4];          // Карат
    int dead;               // Умер(да, нет)
} DataSend;

// ************************************************************************


// *************************** Глобальные перменные ***********************

// База данных о животных
AnimalAttributes db_animals[16];

// Поле по которым перемещаются животные
int map[4][4];

// Мьютекс
pthread_mutex_t mutex;

// ************************************************************************


// *************************** Прототипы функций **************************

// Функция работающая в отдельном потоке
void* Animal(void* atr);

// Генерация псевдослучайных чисел на определнном промежутке
int GetRandRangeInt(int min, int max);

// Создание потока
void CreateThread(int row, int column, TypeAnimal type);

// Создание карты для отправки клиенту
void CreateMap(DataSend* data);

// ************************************************************************


#endif /* LAB1_H_INCLUDED */