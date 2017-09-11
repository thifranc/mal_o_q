/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/11 10:02:59 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	dummy_init(t_block **load, int area, char flag)
{
	(*load)->size = area - BLOCKSIZE;
	(*load)->free = FALSE;
	(*load)->prev = g_mem.large;
	if (flag == 1)
		(*load)->next = g_mem.large;
	else if (flag == 2)
		(*load)->next = g_mem.large->next;
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
		dummy_init(&(g_mem.large), area, 1);
	}
	if (size_available(size, &(g_mem.large)))
		return (g_mem.large + BLOCKSIZE);
	else
	{
		if (!(new_memory = (t_block *)mmap(NULL,
			area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
			return (NULL);
		dummy_init(&(new_memory), area, 2);
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
