#ifndef FT_SSL_H
#define FT_SSL_H

#include "utils/libft/libft.h"
#include "utils/get_next_line/get_next_line.h"
#include "utils/ft_printf/printf.h"

typedef struct s_arguments
{
  int algo; //5 for md5 and 256 for sha256
  int p; //1 for flag being present and 0 for not being present
  int q;
  int r;
  int s_;
  char **inputs;
  char **s; //Equals "1" if input at same index comes from -s flag
  char **file; //Equals "1" if input at same index comes from file
  char **fileName; //Equals filename if input at same index comes from file
  //If input neither comes from -s nor file we know it comes from stdin
  char **outputs;
} t_args;

void parsing(t_args *args, int argc, char **argv);
void visualize_args(t_args *args);
void md5(t_args *args);
void sha256(t_args *args);
void display(t_args *args);

int modular_addition(unsigned long X, unsigned long Y);
uint32_t rotate_left(uint32_t x, uint32_t n);
uint32_t rotate_right(uint32_t x, uint32_t n);
char *padding(int padding_bits, char value);
void freeBlocks(uint32_t **M, int _512bit_messages);
uint32_t reorder4Bytes_little_endian2(uint32_t num);
char *reorder4Bytes_little_endian(char *s);
char *stringToBinary(char *s, int length);
uint32_t binaryStringToInt(const char *binaryString);
char *reverse_bits_padding(char *s);

#endif
