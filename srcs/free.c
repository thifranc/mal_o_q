/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/08 15:06:12 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/08 15:07:49 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	real_free(t_block *node)
{
	node->free = TRUE;
}

void	free(void *ptr)
{
	t_block	*node;

	if (ptr == NULL)
		return ;
	else if ((node = find_equality(ptr, g_mem.tiny)) != NULL)
	{
		real_free(node);
	}
	else if ((node = find_equality(ptr, g_mem.small)) != NULL)
	{
		real_free(node);
	}
	else if ((node = find_equality(ptr, g_mem.large)))
	{
		node->free = TRUE;
	}
	else
		return ;
}

void	*realloc(void *ptr, size_t size)
{
	void	*memory;

	if (ptr == NULL)
	{
		return (malloc(size));
	}
	else
	{
		if (!find_equality(ptr, g_mem.tiny)
				&& !find_equality(ptr, g_mem.small)
				&& !find_equality(ptr, g_mem.large))
			return (NULL);
		if (size == 0)
			size = 1;
		memory = malloc(size);
		ft_memcpy(memory, ptr, size);
		free(ptr);
		return (memory);
	}
}

