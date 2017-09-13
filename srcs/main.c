/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/11 10:02:38 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/13 17:12:59 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

pthread_mutex_t g_mutex;
struct s_mem g_mem = {NULL, NULL, NULL};

void	*malloc(size_t size)
{
	void	*ret;

	if (pthread_mutex_init(&g_mutex, NULL) != 0 || size == 0)
		return (NULL);
	else
		pthread_mutex_lock(&g_mutex);
	if (0 < size && size <= TINY)
		ret = (t_malloc(size));
	else if (TINY < size && size <= SMALL)
		ret = (s_malloc(size));
	else
		ret = (l_malloc(size));
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
