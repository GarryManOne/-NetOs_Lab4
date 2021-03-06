#ifndef LAB1_H_INCLUDED
#define LAB1_H_INCLUDED

#include <stdio.h>
#include <pthread.h>
#include <pthread.h>

// ************************* Константы ************************************
// Размеры карты 
const unsigned int kMapSizeX = 4;
const unsigned int kMapSizeY = 4;

// ************************************************************************

// **************************** Описание объектов *************************

// Виды животных
typedef enum {ANIMAL_1, ANIMAL_2, ANIMAL_3, NONE} TypeAnimal;

// Координаты
typedef struct {
    int x;
    int y;
} Coordinate;

// Данные от сервера
typedef struct
{
    Coordinate coord;       // Координаты
    TypeAnimal type;        // Тип животного
} DataSend;

// Данные для сервера
typedef struct
{
    int map[4][4];          // Карат
    int dead;               // Умер(да, нет)
} DataRecv;

// ************************************************************************


// *************************** Глобальные перменные ***********************

DataRecv data_recv;

// ************************************************************************


// *************************** Прототипы функций **************************

// Вывод карты в консоль
void PrintMap();

// ************************************************************************

#endif /* LAB1_H_INCLUDED */