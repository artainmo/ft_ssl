#include "../ft_ssl.h"

//init 32bits initialization vectors
const int g_OI_A = 0x01234567; //OI stands for original initialization vector ...A/B/C/D
const int g_OI_B = 0x89abcdef;
const int g_OI_C = 0xfedcba98;
const int g_OI_D = 0x76543210;
int g_A;
int g_B;
int g_C;
int g_D;

//init of constant K values
const int g_K[4][16] = {
      {0xd76aa478, 0xe8c7b756, 0x242070db, 0xC1bdceee, 0xf57c0faf, 0x4787c62a,
      0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
      0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821},
      {0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x2441453,
      0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
      0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a},
      {0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9,
      0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05,
      0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665},
      {0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
      0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
      0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}
    };

//4 functions for the 4 rounds
static int F(int B, int C, int D) { return (B & C) | ((~B) & D); }
static int G(int B, int C, int D) { return (B & D) | (C & (~D)); }
static int H(int B, int C, int D) { return B ^ C ^ D; }
static int I(int B, int C, int D) { return C ^ (B | (~D)); }

//Modular addition
const unsigned long g_Z = 0x100000000;
static int modular_addition(unsigned long X, unsigned long Y)
{
  return (X + Y) % g_Z;
}

//For left shift operations
const int g_S[4][16] = {
      {7, 12, 17, 22,
      7, 12, 17, 22,
      7, 12, 17, 22,
      7, 12, 17, 22},
      {5, 9, 14, 20,
      5, 9, 14, 20,
      5, 9, 14, 20,
      5, 9, 14, 20},
      {4, 11, 16, 13,
      4, 11, 16, 13,
      4, 11, 16, 13,
      4, 11, 16, 13},
      {6, 10, 15, 21,
      6, 10, 15, 21,
      6, 10, 15, 21,
      6, 10, 15, 21}
    };

static void operation(int M, int round_, int operation_)
{
  int Y = 0;
  int rem_B = g_B;
  int rem_C = g_C;

  if (round_ == 0) { Y = F(g_B, g_C, g_D); }
  else if (round_ == 1) { Y = G(g_B, g_C, g_D); }
  else if (round_ == 2) { Y = H(g_B, g_C, g_D); }
  else if (round_ == 3) { Y = I(g_B, g_C, g_D); }
  else { ft_error("Out of range round number"); }
  Y = modular_addition(g_A, Y);
  Y = modular_addition(M, Y);
  Y = modular_addition(g_K[round_][operation_], Y);
  Y = Y << g_S[round_][operation_];
  g_B = modular_addition(g_B, Y);
  g_A = g_D;
  g_C = rem_B;
  g_D = rem_C;
}

static void freeBlocks(int **M, int _512bit_messages)
{
  for (int i = 0; i < _512bit_messages; i++) {
    free(M[i]);
  }
  free(M);
}

static int **toBlocks(char *message, int _512bit_messages)
{
  int **M;
  char *rem = message;

  if (!(M = malloc(_512bit_messages)))
    ft_malloc_error();
  for (int i = 0; i < _512bit_messages; i++) { //Make distinction between the 512bit-messages
    if (!(M[i] = malloc(sizeof(int) * 16)))
      ft_malloc_error();
    for (int l = 0; l < 16 ; l++) { //Each 512bit message should be split in 16 32bit-words
      M[i][l] = *(int*)message++; //We cast to int because an int contains 32bits
    }
  }
  free(rem);
  return M;
}

static char *padding(int padding_bits, char value)
{
  char *res;

  if (!(res = malloc(padding_bits / 8 + 1)))
    ft_malloc_error();
  for (int i = 0; i < padding_bits / 8; i++) {
    res[i] = value;
  }
  res[padding_bits / 8] = '\0';
  return res;
}

static void md5_transform(char *input)
{
  char *res;
  uint64_t input_bits_64;
  char *input_length_64bits;
  int **M;

  //step 1: append padding bits
  int input_bits = ft_strlen(input) * 8; //One char equals 1 byte which equals 8 bits
  int _512bit_messages = ft_round_up(((float)input_bits / (512 - 64)));
  int padding_bits = ((512 * _512bit_messages) - 64) - input_bits; //Total length to be 64 bits less than multiple of 512
  ft_printf("input_bits: %d\n_512bit_messages: %d\npadding_bits: %d\n", input_bits, _512bit_messages, padding_bits);
  if (padding_bits == 0)
    padding_bits = 512;
  res = ft_strjoin_2f(input, padding(8, 0b10000000)); //First byte should equal '10000000'. We use 0b as prefix to represent binary literals in c
  res = ft_strjoin_df(res, padding(padding_bits - 8, 0b00000000)); //Rest of bytes should equal '00000000'
  //step 2: append length
  if (!(input_length_64bits = malloc(64 / 8 + 1)))
    ft_malloc_error();
  input_bits_64 = (uint64_t)input_bits;
  input_length_64bits[0] = (char)input_bits_64++;
  input_length_64bits[1] = (char)input_bits_64++;
  input_length_64bits[2] = (char)input_bits_64++;
  input_length_64bits[3] = (char)input_bits_64++;
  input_length_64bits[4] = (char)input_bits_64++;
  input_length_64bits[5] = (char)input_bits_64++;
  input_length_64bits[6] = (char)input_bits_64++;
  input_length_64bits[7] = (char)input_bits_64++;
  input_length_64bits[8] = '\0';
  res = ft_strjoin_df(res, input_length_64bits);
  //step 3: process each 512-bit block
  M = toBlocks(res, _512bit_messages);
  for (int block = 0; block < _512bit_messages; block++) {
    for (int round_ = 0; round_ < 4; round_++) {
      for (int operation_ = 0; operation_ < 16; operation_++) {
        operation(M[block][operation_], round_, operation_);
      }
    }
    g_A = modular_addition(g_A, g_OI_A);
    g_B = modular_addition(g_B, g_OI_B);
    g_C = modular_addition(g_C, g_OI_C);
    g_D = modular_addition(g_D, g_OI_D);
  }
  ft_printf("%s%s%s%s\n", ft_dec_to_hex(g_A), ft_dec_to_hex(g_B),
        ft_dec_to_hex(g_C), ft_dec_to_hex(g_D));
  freeBlocks(M, _512bit_messages);
}

void md5(t_args *args)
{
  int i;

  i = 0;
  while (args->inputs && args->inputs[i])
  {
    g_A = g_OI_A;
    g_B = g_OI_B;
    g_C = g_OI_C;
    g_D = g_OI_D;
    md5_transform(args->inputs[i]);
    i++;
  }
}
