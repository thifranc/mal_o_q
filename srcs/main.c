/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/11 10:02:38 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/11 10:03:01 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

void	*malloc(size_t size)
{
	if (size == 0)
	{
		return (NULL);
	}
	else if (0 < size && size <= TINY)
	{
		return (t_malloc(size));
	}
	else if (TINY < size && size <= SMALL)
	{
		return (s_malloc(size));
	}
	else
	{
		return (l_malloc(size));
	}
}
