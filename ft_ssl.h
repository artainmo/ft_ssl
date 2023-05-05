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
  int s;
  char **inputs;
} t_args;

void parsing(t_args *args, int argc, char **argv);
void visualize_args(t_args *args);
void md5(t_args *args);
void sha256(t_args *args);

#endif
