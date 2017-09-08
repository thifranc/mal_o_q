/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/08 14:01:04 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

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

void	*l_malloc(size_t size)
{
	t_block *new_memory;
	int		area;

	area = (((size + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	if (!g_mem.large)
	{
		if (!(g_mem.large = (t_block *)mmap(NULL,
			area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
			return (NULL);
		g_mem.large->size = area - BLOCKSIZE;
		g_mem.large->free = FALSE;
		g_mem.large->next = g_mem.large;
		g_mem.large->prev = g_mem.large;
	}
	if (size_available(size, &(g_mem.large)))
	{
		return (g_mem.large + BLOCKSIZE);
	}
	else
	{
		if (!(new_memory = (t_block *)mmap(NULL,
			area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
		{
			return (NULL);
		}
		new_memory->size = area - BLOCKSIZE;
		new_memory->free = FALSE;
		new_memory->next = g_mem.large->next;
		new_memory->prev = g_mem.large;
		g_mem.large->next->prev = new_memory;
		g_mem.large->next = new_memory;
		return ((void*)new_memory + BLOCKSIZE);
	}
}

void	*t_malloc(size_t size)
{
	t_bool	rest;
	void	*ret;

	if (!(g_mem.tiny))
	{
		init_lst(TINY);
	}
	rest = size_available(size, &(g_mem.tiny));
	if (rest == TRUE)
	{
		ret = carve_block(g_mem.tiny, size);
		g_mem.tiny = g_mem.tiny->prev;
		return (ret);
	}
	if (!get_new_area(TINY))
	{
		return (NULL);
	}
	else
	{
		ret = carve_block(g_mem.tiny->next, size);
		g_mem.tiny = g_mem.tiny->prev;
		return (ret);
	}
}

void	*s_malloc(size_t size)
{
	t_bool	rest;
	void	*ret;

	if (!(g_mem.small))
	{
		init_lst(SMALL);
	}
	rest = size_available(size, &(g_mem.small));
	if (rest == TRUE)
	{
		ret = carve_block(g_mem.small, size);
		g_mem.small = g_mem.small->prev;
		return (ret);
	}
	if (!get_new_area(SMALL))
	{
		return (NULL);
	}
	else
	{
		ret = carve_block(g_mem.small->next, size);
		g_mem.small = g_mem.small->prev;
		return (ret);
	}
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

void	init_lst(int type)
{
	int		area;
	t_block	*new_memory;

	area = ((100 * (type + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	if (!(new_memory = mmap(NULL,
		area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
		return ;
	new_memory->size = area - BLOCKSIZE;
	new_memory->free = TRUE;
	new_memory->next = new_memory;
	new_memory->prev = new_memory;
	if (type == TINY)
	{
		g_mem.tiny = new_memory;
	}
	else if (type == SMALL)
	{
		g_mem.small = new_memory;
	}
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
