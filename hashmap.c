#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

//================================================
//2.- Implemente la función void insertMap(HashMap * map, char * key, void * value). Esta función inserta un nuevo dato (key,value) en el mapa y actualiza el índice current a esa posición. Recuerde que para insertar un par (clave,valor) debe:

//a - Aplicar la función hash a la clave para obtener la posición donde debería insertar el nuevo par

//b - Si la casilla se encuentra ocupada, avance hasta una casilla disponible (método de resolución de colisiones). Una casilla disponible es una casilla nula, pero también una que tenga un par inválido (key==NULL).

void insertMap(HashMap * map, char * key, void * value) {
    if(map==NULL || key==NULL) return;
    long pos = hash(key, map->capacity);
    long copia_i = pos;
    while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
      {
        //Si la posición es válida nunca se va a ejecutar el while xd
        pos = (pos + 1) % map->capacity;
        if (pos == copia_i)
        {
          return;
          //debería hacer enlarge!
        }
        
        //el % a capacity hace que nunca me salga de los valores del mapa
        //me llama la atencion que enlarge no esté antes de esto, debería utilizarse :) pq si no se podría hacer un while infinito??
      }
    map->buckets[pos] = createPair(key, value);
    map->size++; //porque agregamos
    map->current = pos; //pos actual
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}

//================================================
//1.- Implemente la función createMap en el archivo hashmap.c. Esta función crea una variable de tipo HashMap, inicializa el arreglo de buckets con casillas nulas, inicializa el resto de variables y retorna el mapa. Inicialice el índice current a -1."""

HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)calloc(capacity, sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    map->size = 0;
    map->capacity = capacity;
  map->current = -1;
  for(long i = 0; i < capacity ; i++)
    {
      map->buckets[i] = NULL;
    }
    return map;
}

//================================================
//4.- Implemente la función void eraseMap(HashMap * map, char * key). Está función elimina el dato correspondiente a la clave key. Para hacerlo debe buscar el dato y luego marcarlo para que no sea válido. No elimine el par, sólo invalídelo asignando NULL a la clave (pair->key=NULL). Recuerde actualizar la variable size.
void eraseMap(HashMap * map,  char * key) {    
  if(map==NULL || key==NULL) return;
  long pos = hash(key, map->capacity);
  long copia_i = pos;
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
    {
      if(is_equal(map->buckets[pos]->key, key))
      {
        map->buckets[pos]->key = NULL;
        map->size--;
      }
      pos = (pos) + 1 % map->capacity;
      if (pos == copia_i)
      {
        return;
      }
    }
}

//================================================
//3.- Implemente la función Pair * searchMap(HashMap * map, char * key), la cual retorna el Pair asociado a la clave ingresada. Recuerde que para buscar el par debe:

//a - Usar la función hash para obtener la posición donde puede encontrarse el par con la clave

//b - Si la clave no se encuentra avance hasta encontrarla (método de resolución de colisiones)

//c - Si llega a una casilla nula, retorne NULL inmediatamente (no siga avanzando, la clave no está)

//Recuerde actualizar el índice current a la posición encontrada. Recuerde que el arreglo es circular.
Pair * searchMap(HashMap * map,  char * key) {   
  long pos = hash(key, map->capacity);
  //si no es la que encontré con el hash entonces fue una coalición, busco lineal:
  long copia_i = pos;
  while(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
    {
      if (is_equal(key, map->buckets[pos]->key))
      {
        map->current = pos;
        return map->buckets[pos];
      }
      pos = (pos + 1) % map->capacity;
      if (copia_i == pos)
      {
        return NULL;
      }
    }
  return NULL;
}

//================================================
//Implemente las funciones para recorrer la estructura: Pair * firstMap(HashMap * map) retorna el primer Pair válido del arreglo buckets. Pair * nextMap(HashMap * map) retorna el siguiente Pair del arreglo buckets a partir índice current. Recuerde actualizar el índice.
Pair * firstMap(HashMap * map) {
  if(map==NULL) return NULL;
  long pos = 0;
  long copia_i = 0;
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL)
    {
      pos++;
      if (pos == copia_i)
      {
        return NULL;
      }
    }
    map->current = pos;
    return map->buckets[pos];
}

Pair * nextMap(HashMap * map) {
  long copia_i = map->current;
  long pos = ((map->current) + 1) % map->capacity; // controlar que no se salga del mapa
  if (copia_i == pos)
  {
    map->current = pos;
    return map->buckets[pos];
  }
  while(copia_i != pos)
    {
      if(map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
      {
        map->current = pos;
        return map->buckets[pos];
      }
      pos = (pos + 1) % map->capacity;
    }
  return NULL;
}
