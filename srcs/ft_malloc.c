/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/06 09:38:02 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

t_bool		size_available(size_t size)
{
	t_block	*head;

	head = g_mem.tiny;
	//ideally, we should be sure that at this call head will never be the only option
	g_mem.tiny = g_mem.tiny->next;
	//this is a test to see if it's the first
	//malloc but basically only testing that head == g_mem.tiny would suffice
	if (head == g_mem.tiny)
	{
		return TRUE;
	}
	while (g_mem.tiny != head)
	{
		if (g_mem.tiny->free && size <= g_mem.tiny->size)
		{
		//dprintf(1, "size == %zu || free == %u  || asked == (%lu)\n",
		//g_mem.tiny->size, g_mem.tiny->free, size + BLOCKSIZE);

			return TRUE;
		}
		g_mem.tiny = g_mem.tiny->next;
	}
		if (g_mem.tiny->free && size <= g_mem.tiny->size)
		{
			//dprintf(1, "cur=%p &&&&&& free? %u\n", g_mem.tiny, g_mem.tiny->free);
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
		//dprintf(1, "%p addr is at node nb %d\n", node, i);
		if (node->size)
			node = node->next;
	//	dprintf(1, "size is %zu \n", node->size);
	}
}

void	*t_malloc(size_t size)
{
	t_bool rest;

	if (!(g_mem.tiny)) {
		init_lst(TINY);
	}
	//list_loop();
	rest = size_available(size);
	if (rest == TRUE)
	{
		//dprintf(1, "cur size ==%zu  rest === %d\n", g_mem.tiny->size, rest);
		return carve_block(g_mem.tiny, size);
	}
	if (!get_new_area(TINY)) //RAM is full
	{
		return (NULL);
	}
	else
	{
		return carve_block(g_mem.tiny->next, size);
	}
}

t_bool	get_new_area(int type)
{
	t_block	*new_memory;
	int	tiny_area;

	dprintf(1, "GET NEW AREA\n\\n\n\n\n\n\n\n");
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
		dprintf(1, "TOTAL PLAGE === %d\n", tiny_area);
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

void	*carve_block(t_block *cur, size_t size)
{
	t_block	*new;

	if (cur->size >= size + BLOCKSIZE)
	{
		new = (void*)cur + BLOCKSIZE + size;
		//dprintf(1, "new is at %p, so %p - %p = %ld\n", new, new, cur, (new - cur));
		new->size = cur->size - (size + BLOCKSIZE);
		new->free = TRUE;
		new->prev = cur;
		new->next = cur->next;
		cur->next->prev = new;
		cur->next = new;
	}
	cur->size = size;
	cur->free = FALSE;
	g_mem.tiny = g_mem.tiny->prev;
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
