#include "../ft_ssl.h"

//init 32bits initialization vectors
const uint32_t g_OI_A = 0x01234567; //OI stands for original initialization vector ...A/B/C/D
const uint32_t g_OI_B = 0x89abcdef;
const uint32_t g_OI_C = 0xfedcba98;
const uint32_t g_OI_D = 0x76543210;
uint32_t g_A;
uint32_t g_B;
uint32_t g_C;
uint32_t g_D;

//init of constant K values
const uint32_t g_K[4][16] = {
      {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
      0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
      0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821},
      {0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453,
      0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
      0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a},
      {0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9,
      0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
      0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665},
      {0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
      0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
      0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391}
    };

//4 functions for the 4 rounds
static uint32_t F(uint32_t X, uint32_t Y, uint32_t Z) { return (X & Y) | (~X & Z); }
static uint32_t G(uint32_t X, uint32_t Y, uint32_t Z) { return (X & Z) | (Y & ~Z); }
static uint32_t H(uint32_t X, uint32_t Y, uint32_t Z) { return X ^ Y ^ Z; }
static uint32_t I(uint32_t X, uint32_t Y, uint32_t Z) { return Y ^ (X | ~Z); }

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
      {4, 11, 16, 23,
      4, 11, 16, 23,
      4, 11, 16, 23,
      4, 11, 16, 23},
      {6, 10, 15, 21,
      6, 10, 15, 21,
      6, 10, 15, 21,
      6, 10, 15, 21}
    };

// static void printstringasbinary(char *s, int length)
// {
//   printf("%s\n", s);
//   for (int l = 0; l < length; l++)
//   {
//     for(int i = 7; i >= 0; i--) {
//         ft_printf("%d", (s[l] >> i) & 1 ? 1 : 0 );
//     }
//     ft_printf(" ");
//   }
//   ft_printf("\n");
// }

static uint32_t binaryStringToInt(const char* binaryString) {
    uint32_t result = 0;
    int len = ft_strlen(binaryString);

    for (int i = 0; i < len; i++) {
        result <<= 1;  // Left shift result by 1 bit
        result += binaryString[i] - '0';  // Convert character to integer
    }
    return result;
}

static char *stringToBinary(char *s, int length)
{
  char *res;
  int counter = 0;

  if (!(res = malloc((length * 8) + 1)))
    ft_malloc_error();
  for (int l = 0; l < length; l++)
  {
    for(int i = 7; i >= 0; i--) {
      res[counter++] = (s[l] >> i) & 1 ? '1' : '0';
    }
  }
  return res;
}

static uint32_t rotate_left(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }

// static char *leftShift(char *binaryString, int shift)
// {
//   char *begin;
//   char *end;
//   char *ret;
//   int len = ft_strlen(binaryString);
//
//   if (shift > len)
//     ft_error("Left shift not possible");
//   if (!(begin = ft_substr(binaryString, 0, shift)))
//     ft_malloc_error();
//   if (!(end = ft_substr(binaryString, shift, len - shift)))
//     ft_malloc_error();
//   if (!(ret = ft_strjoin_len_f(end, len - shift, begin, shift, 3)))
//     ft_malloc_error();
//   return ret;
// }
//
static char *decimalToBinary(uint32_t num) {
  char *res;
  int i = 32;

  if (num == 0) {
    return 0;
  }
  if (!(res = malloc(32 + 1)))
      ft_malloc_error();
  res[i--] = 0;
  while (i >= 0) {
    res[i--] = num % 2 ? '1': '0';
    num /= 2;
  }
  return res;
}

static void operation(uint32_t M, int round_, int operation_)
{
  uint32_t Y = 0;
  uint32_t rem_B = g_B;
  uint32_t rem_C = g_C;
  // char *b;
  // char *blf;

  if (round_ == 0) { Y = F(g_B, g_C, g_D); }
  else if (round_ == 1) { Y = G(g_B, g_C, g_D); }
  else if (round_ == 2) { Y = H(g_B, g_C, g_D); }
  else if (round_ == 3) { Y = I(g_B, g_C, g_D); }
  else { ft_error("Out of range round number"); }
  ft_printf("_____________________\n");
  ft_printf("Round: %d | Operation: %d\n", round_, operation_);
  ft_printf("B: %08x | ", g_B);
  ft_printf("A: %08x | ", g_A);
  ft_printf("E: %08x | ", Y);
  ft_printf("K: %08x | ", g_K[round_][operation_]);
  ft_printf("M: %08x | ", M);
  ft_printf("S: %d\n", g_S[round_][operation_]);
  Y = modular_addition(g_A, Y);
  // ft_printf("Y1: %08x\n", Y);
  Y = modular_addition(M, Y);
  // ft_printf("Y2: %08x\n", Y);
  Y = modular_addition(g_K[round_][operation_], Y);
  // ft_printf("Y3: %08x\n", Y);
  // b = decimalToBinary(Y);
  // ft_printf("b: %s\n", b);
  // blf = leftShift(b, g_S[round_][operation_]);
  // ft_printf("blf: %s\n", blf);
  // Y = binaryStringToInt(blf);
  // free(b);
  // free(blf);
  Y = rotate_left(Y, g_S[round_][operation_]);
  // ft_printf("Y4: %08x\n", Y);
  g_B = modular_addition(g_B, Y);
  g_A = g_D;
  g_C = rem_B;
  g_D = rem_C;
  ft_printf("A: %08x | B: %08x | C: %08x | D: %08x\n", g_A, g_B, g_C, g_D);
  ft_printf("_____________________\n");
  // if (operation_ == 6) { exit(0); }
}

static void freeBlocks(uint32_t **M, int _512bit_messages)
{
  for (int i = 0; i < _512bit_messages; i++) {
    free(M[i]);
  }
  free(M);
}

// static char *reorder4Bytes_little_endian(char *s) //Little and big endian are two ways of storing multibyte data-types ( int, float, etc). In little endian machines, last byte of binary representation of the multibyte data-type is stored first. (https://www.geeksforgeeks.org/little-and-big-endian-mystery/)
// {
//   char *firstByte;
//   char *secondByte;
//   char *thirdByte;
//   char *fourthByte;
//   char *res;
//
//   if (!(firstByte = ft_substr(s, 0, 8)))
//     ft_malloc_error();
//   if (!(secondByte = ft_substr(s, 8, 8)))
//     ft_malloc_error();
//   if (!(thirdByte = ft_substr(s, 16, 8)))
//     ft_malloc_error();
//   if (!(fourthByte = ft_substr(s, 24, 8)))
//     ft_malloc_error();
//   free(s);
//   res = ft_strjoin_f(fourthByte, thirdByte, 3);
//   res = ft_strjoin_f(res, secondByte, 3);
//   return ft_strjoin_f(res, firstByte, 3);
// }
//
// static uint32_t reorder4Bytes_little_endian2(uint32_t num)
// {
//   char *tmp;
//   uint32_t res;
//
//   tmp = decimalToBinary(num);
//   tmp = reorder4Bytes_little_endian(tmp);
//   res = binaryStringToInt(tmp);
//   free(tmp);
//   return res;
// }

static uint32_t **toBlocks(char *message, int _512bit_messages)
{
  uint32_t **M;
  char *rem = message;
  char *tmp;

  if (!(M = malloc(_512bit_messages)))
    ft_malloc_error();
  for (int i = 0; i < _512bit_messages; i++) { //Make distinction between the 512bit-messages
    if (!(M[i] = malloc(sizeof(uint32_t) * 16)))
      ft_malloc_error();
    for (int l = 0; l < 16 ; l++) { //Each 512bit message should be split in 16 32bit-words
        tmp = stringToBinary(message, 4);
        // printBinaryString(tmp, 32);
        // tmp = reorder4Bytes_little_endian(tmp); //When comparing my algo with someone else's functional algo, we have the same 4byte/32bits-word content but the bytes are in a different order, they are stored in little-endian order
        // printBinaryString(tmp, 32);
        M[i][l] = binaryStringToInt(tmp);
        free(tmp);
        message = message + 4; //One int equals 4 chars thus we move 4 chars forward to go to next int
      ft_printf("M[%d][%d]: %08x\n", i, l, M[i][l]);
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

// static char *reverse_bits_padding(char *s)
// {
//   char *active;
//   int ia = 0;
//   char *empty;
//   int ie = 0;
//
//   if (!(active = malloc(sizeof(uint64_t))))
//     ft_malloc_error();
//   if (!(empty = malloc(sizeof(uint64_t))))
//     ft_malloc_error();
//   for (int i = 0; i < 64/8; i++) {
//     if (s[i] != 0) {
//       active[ia++] = s[i];
//     } else {
//       empty[ie++] = s[i];
//     }
//   }
//   return ft_strjoin_len_f(empty, ie, active, ia, 3);
// }

static void md5_transform(char *input)
{
  char *res;
  uint64_t *input_length_64bits;
  void *pointer;
  uint32_t **M;
  int M_number;

//step 1: append padding bits
  uint64_t input_bits = ft_strlen(input) * 8; //One char equals 1 byte which equals 8 bits
  int _512bit_messages = ft_round_up(((float)input_bits / (512 - 64)));
  int padding_bits = ((512 * _512bit_messages) - 64) - input_bits; //Total length to be 64 bits less than multiple of 512
  // ft_printf("input_bits: %d\n_512bit_messages: %d\npadding_bits: %d\n", input_bits, _512bit_messages, padding_bits);
  if (padding_bits == 0)
    padding_bits = 512;
  res = ft_strjoin_len_f(input, input_bits/8, padding(8, 0b10000000), 1, 2); //First byte should equal '10000000'. We use 0b as prefix to represent binary literals in c
  res = ft_strjoin_len_f(res, input_bits/8 + 1, padding(padding_bits - 8, 0b00000000), (padding_bits - 8) / 8, 3); //Rest of bytes should equal '00000000'
//step 2: append length
  if (!(input_length_64bits = malloc(sizeof(uint64_t))))
    ft_malloc_error();
  *input_length_64bits = input_bits;
  pointer = input_length_64bits;
  res = ft_strjoin_len_f(res, 448/8, pointer, 64/8, 3);
  // printstringasbinary(res, 512/8);
  // exit(0);
//step 3: process each 512-bit block
  M = toBlocks(res, _512bit_messages);
  for (int block = 0; block < _512bit_messages; block++) {
    for (int round_ = 0; round_ < 4; round_++) {
      for (int operation_ = 0; operation_ < 16; operation_++) {
        if (round_ == 0) { M_number = operation_; }
        else if (round_ == 1) { M_number = (((16 + operation_) * 5) + 1) % 16; }
        else if (round_ == 2) { M_number = (((32 + operation_) * 3) + 5) % 16; }
        else if (round_ == 3) { M_number = ((48 + operation_) * 7) % 16; }
        else { ft_error("Out of range round number"); }
        operation(M[block][M_number], round_, operation_);
      }
    }
    g_A = modular_addition(g_A, g_OI_A);
    g_B = modular_addition(g_B, g_OI_B);
    g_C = modular_addition(g_C, g_OI_C);
    g_D = modular_addition(g_D, g_OI_D);
    ft_printf("Final Addition\n");
    ft_printf("A: %08x | B: %08x | C: %08x | D: %08x\n", g_A, g_B, g_C, g_D);
    // g_A = reorder4Bytes_little_endian2(g_A);
    // g_B = reorder4Bytes_little_endian2(g_B);
    // g_C = reorder4Bytes_little_endian2(g_C);
    // g_D = reorder4Bytes_little_endian2(g_D);
  }
  ft_printf("%08x%08x%08x%08x\n", g_A, g_B, g_C, g_D);
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
