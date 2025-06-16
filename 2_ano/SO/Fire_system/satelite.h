/* IMPORTANTE: Este ficheiro não pode ser alterado! */

#ifndef __SATELITE_H
#define __SATELITE_H

/* Formato dos ficheiros binários */
struct pixel { unsigned int r, g, b, ir; };

/* Formato da resposta da função pesquisaLote */
struct coordenada { int latitude, longitude; };

/* Função para pesquisar situações de alarme num lote de dados */
int pesquisaLote(char* ficheiro, struct coordenada* alarmes, int n);

#endif