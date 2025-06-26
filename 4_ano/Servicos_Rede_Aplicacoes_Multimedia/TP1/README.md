
# LZWdR Compression

## Description

This project implements the LZWdR compression algorithm, a variation of the LZW algorithm that uses a dynamic dictionary for file compression. Its goal is to efficiently compress textual data by exploiting repeated patterns to reduce the final file size.

The implementation is in C++ and uses a linked list–based data structure for the dictionary. The project includes functions for initializing, inserting, searching, and cleaning up the dictionary, as well as support for statistics collection and execution time measurement.

---

## Features

* File compression using the LZWdR algorithm.
* Dynamic dictionary structure based on linked lists.
* Collection and display of compression statistics.
* Execution time measurement.
---

## Function Table – Compression Process

| Function                          | Purpose                                              |
| --------------------------------- | ---------------------------------------------------- |
| `void LZWDR(...)`                 | Main compression routine using LZWdR.                |
| `void Comprimir(...)`             | Performs compression using the dictionary.           |
| `void InicializarDicionario(...)` | Initializes the dictionary with base patterns.       |
| `void LimparDicionario(...)`      | Cleans up dictionary memory.                         |
| `EntradaDicionario* search(...)`  | Searches for a pattern in the dictionary.            |
| `EntradaDicionario* insert(...)`  | Inserts a new pattern into the dictionary.           |
| `void output(...)`                | Appends the code for a pattern to the output buffer. |
| `String concat(...)`              | Concatenates two pattern strings.                    |
| `String reverse(...)`             | Reverses a pattern string.                           |

---

## Function Table – Statistics and Utilities

| Function                                         | Purpose                                              |
| ------------------------------------------------ | ---------------------------------------------------- |
| `void statics()`                                 | Collects statistics during compression.              |
| `double ObterDiferencaTempoSegundos(...)`        | Calculates time difference in seconds.               |
| `void ImprimirLogotipo()`                        | Prints a logo or banner to the terminal.             |
| `void printParameters(...)`                      | Displays input/output file information.              |
| `void ImprimeArgErro()`                          | Prints error messages for invalid command-line args. |
| `void printTime()`                               | Prints the current system time.                      |
| `void imprimeDicionario(...)`                    | Displays the entire dictionary content.              |
| `void imprimePadrao(...)`                        | Prints a single dictionary entry.                    |
| `void printStatics(...)`                         | Displays final compression statistics.               |
| `unsigned int ObterFicheiroProcessadoBytes(...)` | Calculates size (in bytes) of the processed file.    |

---

## Compilation

```
g++ tp1.cpp -o tp1
```
---

## Usage

```
./tp1 input.txt output.txt <max_Block> <max_Dic> <init_Dic> <type_cleanDic>
```

`Maximum block size must be between ]0, 67109056] bytes;`
`Maximum dictionary size must be between [4096, 16777216];`
`Initial dictionary type must be 0-LWDR;`
`Dictionary cleaning type must be 0-none, 1-reset`

---


