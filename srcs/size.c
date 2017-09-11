/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 17:57:49 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/10 18:00:23 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_bool	size_available(size_t size, t_block **src)
{
	t_block	*head;

	head = *src;
	*src = (*src)->next;
	if (head == *src && (*src)->free && (*src)->size >= size)
	{
		return (TRUE);
	}
	while (*src != head)
	{
		if ((*src)->free && size <= (*src)->size)
		{
			return (TRUE);
		}
		*src = (*src)->next;
	}
	if ((*src)->free && size <= (*src)->size)
	{
		return (TRUE);
	}
	return (FALSE);
}

size_t	space_for_munmap(t_block *list)
{
	t_block	*runner;
	size_t	ret;

	runner = list->prev;
	ret = 0;
	while (runner != list)
	{
		ret += runner->size;
		runner = runner->next;
	}
	return (ret);
}

t_bool	check_munmap(void)
{
	t_bool	ret;
	size_t	len;

	ret = FALSE;
	if ((len = space_for_munmap(g_mem.tiny)))
	{
		munmap(g_mem.tiny, len);
		ret = TRUE;
	}
	if ((len = space_for_munmap(g_mem.small)))
	{
		munmap(g_mem.small, len);
		ret = TRUE;
	}
	if ((len = space_for_munmap(g_mem.large)))
	{
		munmap(g_mem.large, len);
		ret = TRUE;
	}
	return (ret);
}

void	*carve_block(t_block *cur, size_t size)
{
	t_block	*new;

	if (cur->size >= size + BLOCKSIZE)
	{
		new = (void*)cur + BLOCKSIZE + size;
		new->size = cur->size - (size + BLOCKSIZE);
		new->free = TRUE;
		new->prev = cur;
		new->next = cur->next;
		cur->next->prev = new;
		cur->next = new;
	}
	cur->size = size;
	cur->free = FALSE;
	return ((void *)cur + BLOCKSIZE);
}

t_bool	get_new_area(int type)
{
	t_block	*new_memory;
	int		area;

	area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	if (!(new_memory = (t_block *)mmap(NULL,
		area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
		return (FALSE);
	new_memory->size = area - BLOCKSIZE;
	new_memory->free = TRUE;
	if (type == TINY)
	{
		new_memory->next = g_mem.tiny->next;
		new_memory->prev = g_mem.tiny;
		g_mem.tiny->next->prev = new_memory;
		g_mem.tiny->next = new_memory;
	}
	else if (type == SMALL)
	{
		new_memory->next = g_mem.small->next;
		new_memory->prev = g_mem.small;
		g_mem.small->next->prev = new_memory;
		g_mem.small->next = new_memory;
	}
	return (TRUE);
}
