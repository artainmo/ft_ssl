#include "ft_ssl.h"

void init_struct(t_args *args)
{
	args->algo = 0;
	args->p = 0;
	args->q = 0;
	args->r = 0;
	args->s_ = 0;
	args->inputs = NULL;
	args->s = NULL;
	args->file = NULL;
	args->fileName = NULL;
	args->outputs = NULL;
}

int main(int argc, char **argv)
{
	t_args args;

	if (argc < 2)
	{
		ft_printf("usage: ft_ssl command [flags] [file/string]\n");
		exit(0);
	} else if (!ft_strcmp(argv[1], "md5") && !ft_strcmp(argv[1], "sha256")) {
		ft_printf("ft_ssl: Error: 'foobar' is an invalid command.\n");
		ft_printf("\n");
		ft_printf("Commands:\n");
		ft_printf("md5\n");
		ft_printf("sha256\n");
		ft_printf("\n");
		ft_printf("Flags:\n");
		ft_printf("-p -q -r -s\n");
		exit(0);
	}
	init_struct(&args);
	parsing(&args, argc, argv);
	// visualize_args(&args);
	if (args.algo == 5)
		md5(&args);
	else
		sha256(&args);
	display(&args);
	free_ds(args.inputs);
	free_ds(args.s);
	free_ds(args.file);
	free_ds(args.fileName);
	free_ds(args.outputs);
	return 0;
}
