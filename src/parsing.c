#include "../ft_ssl.h"

static int stdin_input_exists(t_args *args)
{
  int i;

  i = 0;
  while (args->file && args->file[i])
  {
    if (ft_strcmp(args->file[i], "0") && ft_strcmp(args->s[i], "0"))
      return 1;
    i++;
  }
  return 0;
}


static int file_input_exists(t_args *args)
{
  int i;

  i = 0;
  while (args->file && args->file[i])
  {
    if (ft_strcmp(args->file[i], "1"))
      return 1;
    i++;
  }
  return 0;
}

void visualize_args(t_args *args)
{
  int i;

  i = 0;
  ft_printf("Algo: %d\n", args->algo);
  ft_printf("Flag p: %d\n", args->p);
  ft_printf("Flag q: %d\n", args->q);
  ft_printf("Flag r: %d\n", args->r);
  ft_printf("Flag s: %d\n", args->s_);
  while (args->inputs && args->inputs[i])
  {
    ft_printf("Input %d: %s\n", i, args->inputs[i]);
    i++;
  }
}

static char *file_content(char *file, t_args *args)
{
	int		fd;
	int		ret;
	char	*line;
	char *result;
  char *f;

	result = NULL;
	if ((fd = open(file, O_RDONLY)) == -1)
	{
    result = ft_strdup("null");
    f = "-1";
  }
	else
	{
    f = "1";
		while ((ret = get_next_line(fd, &line)))
		{
			if (ret == -1)
				ft_errno();
			if (!(result = ft_strjoin_f(result, line, 3)))
				ft_malloc_error();
		}
		close(fd);
	}
  if (!(args->s = add_end_ds(args->s, "0")) ||
        !(args->file = add_end_ds(args->file, f)) ||
        !(args->fileName = add_end_ds(args->fileName, file)))
    ft_malloc_error();
  if (result == NULL)
    result = ft_strdup("");
  return result;
}

static void take_piped_stdin_input(t_args *args)
{
  struct stat st;
  char	*line;
  char *result;
  int ret;

  result = NULL;
  fstat(fileno(stdin), &st); //By using this function we can see the size of the stdin input buffer. Allowing us to see if input has been piped to the stdin of our program.
  if (st.st_size > 0)
  {
    while ((ret = get_next_line(0, &line)))
		{
      // printf("%d\n%s\n", ret, line);
			if (ret == -1)
				ft_errno();
			if (!(result = ft_strjoin_f(result, line, 3)))
				ft_malloc_error();
		}
    if (result == NULL)
      result = ft_strdup("");
    if (!(args->inputs = add_end_ds(args->inputs, result)))
      ft_malloc_error();
    if (!(args->s = add_end_ds(args->s, "0")) ||
          !(args->file = add_end_ds(args->file, "0")) ||
          !(args->fileName = add_end_ds(args->fileName, "null")))
      ft_malloc_error();
  }
}

void parsing(t_args *args, int argc, char **argv)
{
	int i;
  char *fc;

	i = 2;
	if (ft_strcmp(argv[1], "md5"))
		args->algo = 5;
	else if (ft_strcmp(argv[1], "sha256"))
		args->algo = 256;
  else
    ft_error("ft_ssl: Error: First argument should specify md5 or sha256.\n");
  take_piped_stdin_input(args);
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-p") && !args->p)
			args->p = 1;
		else if (ft_strcmp(argv[i], "-q") && !args->q)
			args->q = 1;
		else if (ft_strcmp(argv[i], "-r") && !args->r)
			args->r = 1;
		else if (ft_strcmp(argv[i], "-s") && !args->s_)
		{
      args->s_ = 1;
			if (++i < argc)
			{
				if (!(args->inputs = add_end_ds(args->inputs, argv[i])))
					ft_malloc_error();
        if (!(args->s = add_end_ds(args->s, "1")) ||
              !(args->file = add_end_ds(args->file, "0")) ||
              !(args->fileName = add_end_ds(args->fileName, "null")))
  				ft_malloc_error();
			} else {
				ft_error("ft_ssl: Error: Missing string after '-s' flag.\n");
			}
		}
		else
    {
      fc = file_content(argv[i], args);
			if (!(args->inputs = add_end_ds(args->inputs, fc)))
        ft_malloc_error();
      free(fc);
    }
		i++;
	}
  if (!stdin_input_exists(args)) take_piped_stdin_input(args); //Sometimes the fstat function does not work correctly and thus it is better to recheck
  if (stdin_input_exists(args) && !args->p && file_input_exists(args))
  {
    if (!(args->inputs = remove_begin_ds(args->inputs)) ||
          !(args->s = remove_begin_ds(args->s)) ||
          !(args->file = remove_begin_ds(args->file)) ||
          !(args->fileName = remove_begin_ds(args->fileName)))
      ft_malloc_error();
  }
}
