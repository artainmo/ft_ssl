#include "../ft_ssl.h"

char *from_file(t_args *args, int i)
{
  char *res;

  if (args->r) {
    res = ft_strjoin_f(
        ft_strjoin_f(args->outputs[i], " ", 1),
        args->fileName[i],
        1);
  } else {
    res = ft_strjoin_f(
        ft_strjoin("MD5 (", args->fileName[i]),
        ft_strjoin_f(") = ", args->outputs[i], 2),
        3);
  }
  return res;
}

char *file_error(t_args *args, int i)
{
  return ft_strjoin_f(
        ft_strjoin("ft_ssl: md5: ", args->fileName[i]),
        ": No such file or directory", 1);
}

char *from_string(t_args *args, int i)
{
  char *res;

  if (args->r) {
    res = ft_strjoin_f(
        ft_strjoin_f(args->outputs[i], " \"", 1),
        ft_strjoin(args->inputs[i], "\""),
        3);
  } else {
    res = ft_strjoin_f(
        ft_strjoin("MD5 (\"", args->inputs[i]),
        ft_strjoin_f("\") = ", args->outputs[i], 2),
        3);
  }
  return res;
}

char *from_stdin(t_args *args, int i)
{
  char *res;

  if (args->p) {
    res = ft_strjoin_f(
        ft_strjoin_f("(\"", ft_substr(args->inputs[i], 0, (ft_strlen(args->inputs[i]) - 1)), 2), //We use substr to remove ending \n
        ft_strjoin_f("\")= ", args->outputs[i], 2),
        3);
  } else {
    res = ft_strjoin_f("(stdin)= ", args->outputs[i], 2);
  }
  return res;
}

void display(t_args *args)
{
    int i;

    i = 0;
    while (args->outputs && args->outputs[i])
    {
      if (!args->q && ft_strcmp(args->file[i], "1"))
        args->outputs[i] = from_file(args, i);
      else if (ft_strcmp(args->file[i], "-1"))
        args->outputs[i] = file_error(args, i);
      else if (!args->q && ft_strcmp(args->s[i], "1"))
        args->outputs[i] = from_string(args, i);
      else if (!args->q)
        args->outputs[i] = from_stdin(args, i);
      ft_printf("%s\n", args->outputs[i]);
      i++;
    }
}
