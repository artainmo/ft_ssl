/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artainmo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/23 18:06:13 by artainmo          #+#    #+#             */
/*   Updated: 2020/02/23 18:08:19 by artainmo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_len_ds(char **double_str)
{
	int	i;

	i = 0;
	if (!double_str)
		return (0);
	while (double_str[i])
		i++;
	return (i);
}
