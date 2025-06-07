#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "C:\Users\Bia\OneDrive\Ambiente de Trabalho\SGT-20-21\struct.h"


void menu();
void menudata();
void tipos(char *tipo_array[], size_t n2);
void criar(int i, TAREFA *tarefa,char *tipo_array[],size_t n2);
void editar( TAREFA *tarefa,char *tipo_array[],size_t n2);
void consultar(TAREFA *tarefa);
void eliminar(int i, TAREFA *tarefa);
void lista(char letra,TAREFA *tarefa);
void printList(colab* list);
void printcolab(colab* list);
int tamanho_lista(colab* list);