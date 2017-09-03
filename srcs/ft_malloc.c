/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/03 18:14:28 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct	s_mem {
	t_block *tiny;
	t_block *small;
	t_block *large;
};

struct s_mem g_mem = {NULL, NULL, NULL};

t_bool	get_new_area(int type)	
{
	t_block	*new_memory;
	int	tiny_area;

	if (type == TINY) {
		tiny_area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
		new_memory = (t_block *)mmap(NULL, tiny_area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		new_memory->size = tiny_area - BLOCKSIZE;
		new_memory->free = TRUE;
		new_memory->next = g_mem.tiny->next;
		new_memory->prev = g_mem.tiny;
		g_mem.tiny->next->prev = new_memory;
		g_mem.tiny->next = new_memory;
	}
	return TRUE;
}

void	init_lst(int type)
{
	int	tiny_area;

	if (type == TINY) {
		tiny_area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
		g_mem.tiny = mmap(NULL, tiny_area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		g_mem.tiny->size = tiny_area - BLOCKSIZE;
		g_mem.tiny->free = TRUE;
		g_mem.tiny->next = g_mem.tiny;
		g_mem.tiny->prev = g_mem.tiny;
		/*
	} else if (type == SMALL) {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	} else {
		mmap(NULL, 9999, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		*/
	}

}

int		size_available(size_t size)
{
	t_block	*head;
	t_block	*cur;

	head = g_mem.tiny;
	cur = g_mem.tiny->next;
	//this is a test to see if it's the first
	//malloc but basically only testing that head == cur would suffice
	if (head == cur && cur-> free && (size + BLOCKSIZE) < cur->size)
		return cur->size - (size + BLOCKSIZE);
	while (cur != head && !cur->free && (size + BLOCKSIZE) > cur->size)
		cur = cur->next;
	if (cur == head) {
		return -1;
	} else {
		return cur->size - (size + BLOCKSIZE);
	}
}

void	*carve_block(t_block *cur, size_t size, int rest)
{
	t_block	*new;

	cur->free = FALSE;
	cur->next->prev = cur + size;
	new = cur + size;
	new->size = rest;
	new->prev = cur;
	new->free = TRUE;
	new->next = cur->next;
	cur->next = new;
	return (void *)cur + BLOCKSIZE;
}

void	*t_malloc(size_t size)
{
	int rest;

	if (!(g_mem.tiny))
		init_lst(TINY);
	if ((rest = size_available(size)) != -1)
		return carve_block(g_mem.tiny, size, rest);
	if (!get_new_area(TINY)) //RAM is full
		return (NULL);
	else
		return t_malloc(size);
}

void	*malloc(size_t size)
{
	if (size == 0) {
		return (NULL);
	} else if (0 < size && size <= TINY) {
		return t_malloc(size);
	}
	else return(NULL);
	/* else if (size < TINY && size <= SMALL) {
		return s_malloc(size);
	} else
		return l_malloc(size);
	}*/

}

void	free(void *ptr)
{
	if (ptr == NULL)
		return ;
}

int main(){
	char	*lol;
	char	*lol2;

	lol = (char*)malloc(90);
	lol2 = (char*)malloc(200);
	lol = "ewoiuhrvoiweuhrvoiewuhrvoiweuhrvoiewuhrveioruhv";
	lol2 = "88888888888888888888888888888888oiewuhrveioruhv";
	dprintf(1, "bonjour %s et %s\n", lol, lol2);

	return (0);
}
