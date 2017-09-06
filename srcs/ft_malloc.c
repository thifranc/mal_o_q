/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/06 11:51:29 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

t_bool		size_available(size_t size, t_block **src)
{
	t_block	*head;

	head = *src;
	*src = (*src)->next;
	/*
	 * this is a test to see if it's the first
	 * malloc but basically only testing that head == *src would suffice
	 */
	dprintf(1, "free == %u && size == %zu asked == %zu test size avail\n", head->free, head->size, size);
	if (head == *src && (*src)->free && (*src)->size >= size)
	{
		return TRUE;
	}
	while (*src != head)
	{
		if ((*src)->free && size <= (*src)->size)
		{
			return TRUE;
		}
		*src = (*src)->next;
	}
	if ((*src)->free && size <= (*src)->size)
	{
		return TRUE;
	}
	return FALSE;
}

void	list_loop()
{
	t_block	*head;
	t_block	*node;
	int i;

	head = g_mem.tiny->prev;
	node = g_mem.tiny;
	i = 0;
	while (node != head)
	{
		i++;
		if (node->size)
			node = node->next;
	}
}

void	*l_malloc(size_t size)
{
	t_block *new_memory;
	int		area;

	area = (((size  + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	dprintf(1, "area = %d called\n", area);
	if (!g_mem.large)
	{
		g_mem.large = (t_block *)mmap(NULL, area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		g_mem.large->size = area - BLOCKSIZE;
		g_mem.large->free = FALSE;
		g_mem.large->next = g_mem.large;
		g_mem.large->prev = g_mem.large;
	}
	if (size_available(size, &(g_mem.large)))
	{
		dprintf(1, "SIZE AVAILABLE\n");
		return g_mem.large + BLOCKSIZE;
	} else {
		new_memory = (t_block *)mmap(NULL, area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		new_memory->size = area = BLOCKSIZE;
		new_memory->free = FALSE;
		new_memory->next = g_mem.large->next;
		new_memory->prev = g_mem.large;
		g_mem.large->next->prev = new_memory;
		g_mem.large->next = new_memory;
	}
	return (void*)new_memory + BLOCKSIZE;

}


void	*t_malloc(size_t size)
{
	t_bool	rest;
	void	*ret;

	if (!(g_mem.tiny)) {
		init_lst(TINY);
	}
	rest = size_available(size, &(g_mem.tiny));
	if (rest == TRUE)
	{
		ret = carve_block(g_mem.tiny, size);
		g_mem.tiny = g_mem.tiny->prev;
		return ret;
	}
	if (!get_new_area(TINY)) //RAM is full
	{
		return (NULL);
	}
	else
	{
		ret = carve_block(g_mem.tiny->next, size);
		g_mem.tiny = g_mem.tiny->prev;
		return ret;
	}
}

void	*s_malloc(size_t size)
{
	t_bool	rest;
	void	*ret;

	if (!(g_mem.small)) {
		init_lst(SMALL);
	}
	rest = size_available(size, &(g_mem.small));
	if (rest == TRUE)
	{
		ret = carve_block(g_mem.small, size);
		g_mem.small = g_mem.small->prev;
		return ret;
	}
	if (!get_new_area(SMALL)) //RAM is full
	{
		return (NULL);
	}
	else
	{
		ret = carve_block(g_mem.small->next, size);
		g_mem.small = g_mem.small->prev;
		return ret;
	}
}

t_bool	get_new_area(int type)
{
	t_block	*new_memory;
	int	area;

	area = ((100 * (TINY + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	new_memory = (t_block *)mmap(NULL, area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	new_memory->size = area - BLOCKSIZE;
	new_memory->free = TRUE;
	if (type == TINY) {
		new_memory->next = g_mem.tiny->next;
		new_memory->prev = g_mem.tiny;
		g_mem.tiny->next->prev = new_memory;
		g_mem.tiny->next = new_memory;
	} else if (type == SMALL)
	{
		new_memory->next = g_mem.small->next;
		new_memory->prev = g_mem.small;
		g_mem.small->next->prev = new_memory;
		g_mem.small->next = new_memory;
	}
	return TRUE;
}

void	init_lst(int type)
{
	int	area;

	area = ((100 * (type + BLOCKSIZE) / getpagesize()) + 1) * getpagesize();
	if (type == TINY) {
		dprintf(1, "TOTAL PLAGE === %d\n", area);
		g_mem.tiny = mmap(NULL, area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		g_mem.tiny->size = area - BLOCKSIZE;
		g_mem.tiny->free = TRUE;
		g_mem.tiny->next = g_mem.tiny;
		g_mem.tiny->prev = g_mem.tiny;
	} else if (type == SMALL) {
		dprintf(1, "TOTAL PLAGE === %d\n", area);
		g_mem.small = mmap(NULL, area, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
		g_mem.small->size = area - BLOCKSIZE;
		g_mem.small->free = TRUE;
		g_mem.small->next = g_mem.small;
		g_mem.small->prev = g_mem.small;
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
	return (void *)cur + BLOCKSIZE;
}

/*
   int main(){
   char	*lol;
   t_block	*node;
   int		i;

   i = 0;
   while (i < 1024)
   {
   lol = (char *)malloc(1024);
   lol = "reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussialloc reussi";
   i++;
   }

   return (0);
   }
   */
