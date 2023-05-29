#include "../ft_ssl.h"

void visualize_args(t_args *args)
{
  int i;

  i = 0;
  ft_printf("Algo: %d\n", args->algo);
  ft_printf("Flag p: %d\n", args->p);
  ft_printf("Flag q: %d\n", args->q);
  ft_printf("Flag r: %d\n", args->r);
  ft_printf("Flag s: %d\n", args->s);
  while (args->inputs && args->inputs[i])
  {
    ft_printf("Input %d: %s\n", i, args->inputs[i]);
    i++;
  }
}

static char *file_content(char *file)
{
	int		fd;
	int		ret;
	char	*line;
	char *result;

	result = NULL;
	if ((fd = open(file, O_RDONLY)) == -1)
		ft_errno();
	else
	{
		while ((ret = get_next_line(fd, &line)))
		{
			if (ret == -1)
				ft_errno();
			if (!(result = ft_strjoin_f(result, line, 3)))
				ft_malloc_error();
		}
		close(fd);
	}
  return result;
}

static void take_piped_stdin_input(t_args *args)
{
  struct stat st;
  char	*line;

  fstat(fileno(stdin), &st); //By using this function we can see the size of the stdin input buffer. Allowing us to see if input has been piped to the stdin of our program.
  if (st.st_size > 0)
  {
    if (get_next_line(0, &line) == -1)
      ft_errno();
    if (!(line = ft_strjoin_f(line, "\n", 1))) //When comparing with functional algorithm, a \n exists at end of piped input, knowing the get_next_line I use removes the \n, I added it back.
      ft_malloc_error();
    if (!(args->inputs = add_end_ds(args->inputs, line)))
      ft_malloc_error();
    free(line);
  }
}

void parsing(t_args *args, int argc, char **argv)
{
	int i;

	i = 2;
	if (ft_strcmp(argv[1], "md5"))
		args->algo = 5;
	else
		args->algo = 256;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-p"))
			args->p = 1;
		else if (ft_strcmp(argv[i], "-q"))
			args->q = 1;
		else if (ft_strcmp(argv[i], "-r"))
			args->r = 1;
		else if (ft_strcmp(argv[i], "-s"))
		{
				args->s = 1;
				if (++i < argc)
				{
					if (!(args->inputs = add_end_ds(args->inputs, argv[i])))
						ft_malloc_error();
				} else {
					ft_error("ft_ssl: Error: Missing string after '-s' flag.\n");
				}
		}
		else
    {
			if (!(args->inputs = add_end_ds(args->inputs, file_content(argv[i]))))
        ft_malloc_error();
    }
		i++;
	}
  take_piped_stdin_input(args);
}
