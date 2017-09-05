/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/05 14:54:53 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

int		size_available(size_t size)
{
	t_block	*head;

	dprintf(1, "lol3\n");
	head = g_mem.tiny;
	//ideally, we should be sure that at this call head will never be the only option
	g_mem.tiny = g_mem.tiny->next;
	//this is a test to see if it's the first
	//malloc but basically only testing that head == g_mem.tiny would suffice
	if (head == g_mem.tiny && g_mem.tiny->free && (size + BLOCKSIZE) < g_mem.tiny->size)
	{
	dprintf(1, "lol4\n");
		return g_mem.tiny->size - (size + BLOCKSIZE);
	}
	while (g_mem.tiny != head)
	{
	dprintf(1, "%p addr \n", g_mem.tiny);
		if (g_mem.tiny->free && (size + BLOCKSIZE) < g_mem.tiny->size)
		{
			//dprintf(1, "cur=%p &&&&&& free? %u\n", g_mem.tiny, g_mem.tiny->free);
			dprintf(1, "node trouvé ! dispo size et free :) %zu, %u\n", g_mem.tiny->size, g_mem.tiny->free);
	dprintf(1, "lol9\n");
			return g_mem.tiny->size - (size + BLOCKSIZE);
		}
		g_mem.tiny = g_mem.tiny->next;
	}
	dprintf(1, "nedd to get new area\n");
	return -1;
}

void	*t_malloc(size_t size)
{
	int rest;

	dprintf(1, "coucou first\n");
	if (!(g_mem.tiny)) {
		init_lst(TINY);
	}
	if ((rest = size_available(size)) != -1)
	{
		//dprintf(1, "cur size ==%zu  rest === %d\n", g_mem.tiny->size, rest);
		return carve_block(g_mem.tiny, size, rest);
	}
	if (!get_new_area(TINY)) //RAM is full
	{
		dprintf(1, "\n\n\n\n\nRNOME PROBLEME\n\n\n\n\n");
		return (NULL);
	}
	else
		return t_malloc(size);
}

t_bool	get_new_area(int type)
{
	t_block	*new_memory;
	int	tiny_area;

	dprintf(1, "\n\nget new area called !!!!\n\n");
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
		dprintf(1, "plage memoire de %p jusque %p\n", g_mem.tiny, g_mem.tiny + tiny_area);
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

void	*carve_block(t_block *cur, size_t size, int rest)
{
	t_block	*new;

	cur->free = FALSE;
	new = (void*)cur + BLOCKSIZE + size;
	//dprintf(1, "new is at %p, so %p - %p = %ld\n", new, new, cur, (new - cur));
	new->size = rest;
	new->free = TRUE;
	new->prev = cur;
	new->next = cur->next;
	cur->next->prev = new;
	cur->next = new;
	g_mem.tiny = g_mem.tiny->prev;
	dprintf(1, "coucou last\n");
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
		lol = (char *)malloc(2048);
		lol = "mmalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussialloc reussi";
		i++;
	}
	return 0;
	i = 0;
	node = g_mem.tiny->next;
	while (node != g_mem.tiny)
	{
		//dprintf(1, "i == %d && size == %zu\n", i, node->size);
		i++;
		node = node->next;
	}

	return (0);
}
	*/
