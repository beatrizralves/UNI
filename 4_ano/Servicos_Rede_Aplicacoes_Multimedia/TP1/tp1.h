#ifndef TP1_H
#define TP1_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <vector>
#include <filesystem>
#pragma execution_character_set("utf-8") // para imprimir acentos no terminal em windows

#if defined(_WIN32) || defined(_WIN64) || (defined(__CYGWIN__) && !defined(_WIN32)) // se for windows
#include <windows.h>                                                                //para imprimir acentos no terminal em windows
#endif

// alias para o tempo assim cada vez que se quiser usar tempo no código
using Tempo = std::chrono::time_point<std::chrono::steady_clock>;

// alias para strings c++
using String = std::string;

struct EntradaDicionario // Entrada do Dicionário
{
    String padrao;
    unsigned int n_vezes;
    unsigned int codigo;
    EntradaDicionario *next;
};

void LZWDR(std::ifstream &ficheiro_entrada, std::ofstream &ficheiro_saida);
void Comprimir(EntradaDicionario *dicionario, const String &buffer_entrada, String &buffer_saida);
unsigned int ObterHash(const String &padrao);
void InicializarDicionario(EntradaDicionario *dicionario);
void LimparDicionario(EntradaDicionario *dicionario);
EntradaDicionario *search(EntradaDicionario *dicionario, const String &padrao);
EntradaDicionario *insert(EntradaDicionario *dicionario, const String &padrao);
void output(EntradaDicionario *padrao, String &buffer_saida);
String concat(const String &Pa, const String &Pb);
String reverse(const String &padrao);
void statics();
unsigned int ObterFicheiroProcessadoBytes(String &blocos_processados);

/**
 * @brief Calcula a diferença entre dois tempos da biblioteca chrono, em segundos
 *
 * @param inicio o tempo inicial
 * @param fim o tempo final
 * @return A diferenca entre os dois tempos (em segundos)
 */
double ObterDiferencaTempoSegundos(const Tempo &inicio, const Tempo &fim);
void ImprimirLogotipo();
void printParameters(const std::string &inputFileName, const std::string &outputFileName);
void ImprimeArgErro();
void printTime();
void imprimeDicionario(const EntradaDicionario *dicionario);
void imprimePadrao(const EntradaDicionario &entrada_dicionario);
void printStatics(double insertMean, unsigned int numOfBlocks, const std::string &processedBlocks, unsigned int fileSize, double findMean);

#endif // TP1_H
