/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artainmo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 21:27:23 by artainmo          #+#    #+#             */
/*   Updated: 2019/12/17 02:20:47 by artainmo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
* This get_next_line has been modified specifically for the ft_ssl project.
* It returns lines containing the \n. And it handles correctly lines that do not end with a \n.
* I had to do this to handle the following case whereby -n removes the ending \n: echo -n "42 is nice" | ./../ft_ssl sha256
*/

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	static char		buffer[FOPEN_MAX][BUFFER_SIZE + 1];
	int				ret;
	int				i;

	i = 0;
	ret = 1;
	if (BUFFER_SIZE < 1 || fd < 0 || fd >= FOPEN_MAX || !line
				|| (!(*line = ft_strjoin_m(NULL, buffer[fd]))))
		return (-1);
	while (ft_strchr_m(buffer[fd], '\n') == -1 && ret)
	{
		if ((ret = read(fd, buffer[fd], BUFFER_SIZE)) == -1)
			return (-1);
		// printf("in: %s\n", buffer[fd]);
		buffer[fd][ret] = '\0';
		// printf("in2: %s\n", buffer[fd]);
		if (!(*line = ft_strjoin_m(*line, buffer[fd])))
			return (-1);
		// printf("line in (ret: %d): %s\n", ret, *line);
		if (ret && ret != BUFFER_SIZE && ft_strchr_m(*line, '\n') == -1) //If no \n to be found and arrived at end, return 1
			return 1;
	}
	// printf("out: %s\n", buffer[fd]);
	ret = ft_strchr_m(buffer[fd], '\n') + 1;
	if (buffer[fd][0] == '\0')
		return (0);
	while (buffer[fd][ret] != '\0')
		buffer[fd][i++] = buffer[fd][ret++];
	buffer[fd][i] = '\0';
	return (1);
}
