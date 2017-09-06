/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/06 15:14:19 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

t_bool	size_available(size_t size, t_block **src)
{
	t_block	*head;

	//dprintf(1, "function called : %s\n", __func__);
	head = *src;
	//dprintf(1, "lol3\n");
	*src = (*src)->next;
	//dprintf(1, "lol4\n");
	if (head == *src && (*src)->free && (*src)->size >= size)
	{
	//dprintf(1, "lol5\n");
		return (TRUE);
	}
	while (*src != head)
	{
	//dprintf(1, "lol6\n");
		if ((*src)->free && size <= (*src)->size)
		{
	//dprintf(1, "lol7\n");
			return (TRUE);
		}
		*src = (*src)->next;
	}
	if ((*src)->free && size <= (*src)->size)
	{
	//dprintf(1, "lol8\n");
		return (TRUE);
	}
	//dprintf(1, "lol9\n");
	return (FALSE);
}

void	*l_malloc(size_t size)
{
	t_block *new_memory;
	int		area;

	area = (((size + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	////dprintf(1, "mmap will be of %d function called : %s\n", area, __func__);
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
		//dprintf(1, "bonjour\n");
		return (g_mem.large + BLOCKSIZE);
	}
	else
	{
		//dprintf(1, "noij1\n");
		if (!(new_memory = (t_block *)mmap(NULL,
			area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)))
		{
		//dprintf(1, "noij2\n");
			return (NULL);
		}
		//dprintf(1, "noij3\n");
		new_memory->size = area - BLOCKSIZE;
		//dprintf(1, "noij4\n");
		new_memory->free = FALSE;
		//dprintf(1, "noij5\n");
		new_memory->next = g_mem.large->next;
		//dprintf(1, "noij6\n");
		new_memory->prev = g_mem.large;
		//dprintf(1, "noij7\n");
		g_mem.large->next->prev = new_memory;
		//dprintf(1, "noij8\n");
		g_mem.large->next = new_memory;
		//dprintf(1, "noij9\n");
		return ((void*)new_memory + BLOCKSIZE);
	}
}

void	*t_malloc(size_t size)
{
	t_bool	rest;
	void	*ret;

	//dprintf(1, "function called : %s\n", __func__);
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

	//dprintf(1, "function called : %s\n", __func__);
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

	//dprintf(1, "function called : %s\n", __func__);
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

	//dprintf(1, "function called : %s\n", __func__);
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

	//dprintf(1, "function called : %s\n", __func__);
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
