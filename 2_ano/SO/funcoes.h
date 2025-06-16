#ifndef AUX_H
#define AUX_H

//Os dois defines abaixo são necessarios pois os tipo ssize_t e size_t não são nativos da liguagem de programação c
#define ssize_t signed int
#define size_t unsigned int

int readchar(int fd);
ssize_t readln(int fd, char *line, size_t maxLength);
int myStrlen (char * str);
int numberOFdigits (int numero);
void storageARRAY (int numero, int *array);
int myATOI(char *str);
void myStrcpy(char *dest, const char *src);
void splitString(const char *str, int *latitude, int *longitude, char *caminho);

#endif