#include "ft_ssl.h"

int main(int argc, char **argv)
{
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
	return 0;
}
