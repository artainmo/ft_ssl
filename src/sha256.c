#include "../ft_ssl.h"

//init 32bits initialization vectors
static const uint32_t g_OI_A = 0x6a09e667; //OI stands for original initialization vector ...A/B/C/D
static const uint32_t g_OI_B = 0xbb67ae85;
static const uint32_t g_OI_C = 0x3c6ef372;
static const uint32_t g_OI_D = 0xa54ff53a;
static const uint32_t g_OI_E = 0x510e527f;
static const uint32_t g_OI_F = 0x9b05688c;
static const uint32_t g_OI_G = 0x1f83d9ab;
static const uint32_t g_OI_H = 0x5be0cd19;
static uint32_t g_BL_A; //BL stands for blocks because those are used in calculations between blocks
static uint32_t g_BL_B;
static uint32_t g_BL_C;
static uint32_t g_BL_D;
static uint32_t g_BL_E;
static uint32_t g_BL_F;
static uint32_t g_BL_G;
static uint32_t g_BL_H;
static uint32_t g_A;
static uint32_t g_B;
static uint32_t g_C;
static uint32_t g_D;
static uint32_t g_E;
static uint32_t g_F;
static uint32_t g_G;
static uint32_t g_H;

//init of constant K values
static const uint32_t g_K[64] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
      0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
      0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
      0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
      0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
      0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t maj(uint32_t a, uint32_t b, uint32_t c)
{
  return (a & b) ^ (a & c) ^ (b & c);
}

static uint32_t ch(uint32_t e, uint32_t f, uint32_t g)
{
  return (e & f) ^ (~e & g);
}

static uint32_t capitalSigmaOne(uint32_t x)
{
  return rotate_right(x, 6) ^ rotate_right(x, 11) ^ rotate_right(x, 25);
}

static uint32_t capitalSigmaZero(uint32_t x)
{
  return rotate_right(x, 2) ^ rotate_right(x, 13) ^ rotate_right(x, 22);
}

static void operation(uint32_t M, int operation_)//, int block_)
{
  uint32_t T1;
  uint32_t T2;

  T1 = modular_addition(
          modular_addition(
            modular_addition(g_H, capitalSigmaOne(g_E)),
            modular_addition(ch(g_E, g_F, g_G), g_K[operation_])),
          M);
  T2 = modular_addition(capitalSigmaZero(g_A), maj(g_A, g_B, g_C));
  g_H = g_G;
  g_G = g_F;
  g_F = g_E;
  g_E = modular_addition(g_D, T1);
  g_D = g_C;
  g_C = g_B;
  g_B = g_A;
  g_A = modular_addition(T1, T2);
}

uint32_t rotate_right(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }

static uint32_t sigmaOne(uint32_t x)
{
  return rotate_right(x, 17) ^ rotate_right(x, 19) ^ (x >> 10);
}

static uint32_t sigmaZero(uint32_t x)
{
  return rotate_right(x, 7) ^ rotate_right(x, 18) ^ (x >> 3);
}

static uint32_t **toBlocks(char *message, int _512bit_messages)
{
  uint32_t **M;
  char *rem = message;
  char *tmp;

  if (!(M = malloc(_512bit_messages * sizeof(char *))))
    ft_malloc_error();
  for (int i = 0; i < _512bit_messages; i++) { //Make distinction between the 512bit-messages
    if (!(M[i] = malloc(sizeof(uint32_t) * 64)))
      ft_malloc_error();
    for (int l = 0; l < 64 ; l++) { //Each 512bit message should be split in 16 32bit-words
      if (l < 16) {
        tmp = stringToBinary(message, 4);
        // printBinaryString(tmp, 32);
        M[i][l] = binaryStringToInt(tmp);
        free(tmp);
        message = message + 4; //One int equals 4 chars thus we move 4 chars forward to go to next int
      } else {
        // ft_printf("sigma1: %08x | ", sigmaOne(M[i][l-2]));
        // ft_printf("M(n-7): %08x | ", M[i][l-7]);
        // ft_printf("M(n-15): %08x | ", M[i][l-15]);
        // ft_printf("sigma0: %08x | ", sigmaZero(M[i][l-15]));
        // ft_printf("M(n-16): %08x\n", M[i][l-16]);
        M[i][l] = modular_addition(
                    modular_addition(sigmaOne(M[i][l-2]), M[i][l-7]),
                    modular_addition(sigmaZero(M[i][l-15]), M[i][l-16]));
      }
      // if (i == 43) exit(0);
      // ft_printf("M[%d][%d]: %08x\n", i, l, M[i][l]);
    }
  }
  free(rem);
  return M;
}

static char *sha256_transform(char *input)
{
  char *res;
  uint64_t *input_length_64bits;
  uint32_t **M;

//step 1: append padding bits
  uint64_t input_bits = ft_strlen(input) * 8; //One char equals 1 byte which equals 8 bits
  // ft_printf("len: %llu | bits: %llu\n", input_bits / 8, input_bits);
  int _512bit_messages = input_bits / 512;
  if (input_bits % 512 >= 440) { _512bit_messages += 2; } else { _512bit_messages += 1; } //At least 1 byte of padding and 64 bytes of length appending should be kept in last block, thus if not enough space for those add one block
  int padding_bits = ((512 * _512bit_messages) - 64) - input_bits; //Total length to be 64 bits less than multiple of 512
  // ft_printf("input_bits: %d\n_512bit_messages: %d\npadding_bits: %d\n", input_bits, _512bit_messages, padding_bits);
  res = ft_strjoin_len_f(input, input_bits/8, padding(8, 0b10000000), 1, 2); //First byte should equal '10000000'. We use 0b as prefix to represent binary literals in c
  res = ft_strjoin_len_f(res, input_bits/8 + 1, padding(padding_bits - 8, 0b00000000), (padding_bits - 8) / 8, 3); //Rest of bytes should equal '00000000'
//step 2: append length
  if (!(input_length_64bits = malloc(sizeof(uint64_t))))
    ft_malloc_error();
  *input_length_64bits = input_bits;
  res = ft_strjoin_len_f(res, (((512 * _512bit_messages) - 64) / 8),
        reverse_bits_padding((void *)input_length_64bits), 64/8, 3);
  // printstringasbinary(res, 512/8);
  // exit(0);
//step 3: process each 512-bit block
  M = toBlocks(res, _512bit_messages);
  for (int block = 0; block < _512bit_messages; block++) {
    for (int operation_ = 0; operation_ < 64; operation_++) {
      operation(M[block][operation_], operation_);//, block);
    }
    g_BL_A = modular_addition(g_A, g_BL_A);
    g_BL_B = modular_addition(g_B, g_BL_B);
    g_BL_C = modular_addition(g_C, g_BL_C);
    g_BL_D = modular_addition(g_D, g_BL_D);
    g_BL_E = modular_addition(g_E, g_BL_E);
    g_BL_F = modular_addition(g_F, g_BL_F);
    g_BL_G = modular_addition(g_G, g_BL_G);
    g_BL_H = modular_addition(g_H, g_BL_H);
    g_A = g_BL_A;
    g_B = g_BL_B;
    g_C = g_BL_C;
    g_D = g_BL_D;
    g_E = g_BL_E;
    g_F = g_BL_F;
    g_G = g_BL_G;
    g_H = g_BL_H;
    // ft_printf("A: %08x\nB: %08x\nC: %08x\nD: %08x\nE: %08x\nF: %08x\nG: %08x\nH: %08x\n\n\n", g_A, g_B, g_C, g_D, g_E, g_F, g_G, g_H);
  }
  freeBlocks(M, _512bit_messages);
  return ft_strjoin_f(
            ft_strjoin_f(ft_strjoin_f(ft_dec_to_hex(g_A), ft_dec_to_hex(g_B), 3),
              ft_strjoin_f(ft_dec_to_hex(g_C), ft_dec_to_hex(g_D), 3), 3),
            ft_strjoin_f(ft_strjoin_f(ft_dec_to_hex(g_E), ft_dec_to_hex(g_F), 3),
              ft_strjoin_f(ft_dec_to_hex(g_G), ft_dec_to_hex(g_H), 3), 3), 3);
}

void sha256(t_args *args)
{
  int i;
  char *output;

  i = 0;
  while (args->inputs && args->inputs[i])
  {
    g_A = g_OI_A;
    g_B = g_OI_B;
    g_C = g_OI_C;
    g_D = g_OI_D;
    g_E = g_OI_E;
    g_F = g_OI_F;
    g_G = g_OI_G;
    g_H = g_OI_H;
    g_BL_A = g_OI_A;
    g_BL_B = g_OI_B;
    g_BL_C = g_OI_C;
    g_BL_D = g_OI_D;
    g_BL_E = g_OI_E;
    g_BL_F = g_OI_F;
    g_BL_G = g_OI_G;
    g_BL_H = g_OI_H;
    output = sha256_transform(args->inputs[i]);
    if (!(args->outputs = add_end_ds(args->outputs, output)))
      ft_malloc_error();
    free(output);
    i++;
  }
}
