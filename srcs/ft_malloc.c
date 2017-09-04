/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 17:33:26 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/04 17:54:31 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

struct s_mem g_mem = {NULL, NULL, NULL};

int		size_available(size_t size)
{
	t_block	*head;

	head = g_mem.tiny;
	//ideally, we should be sure that at this call head will never be the only option
	g_mem.tiny = g_mem.tiny->next;
	//this is a test to see if it's the first
	//malloc but basically only testing that head == g_mem.tiny would suffice
	if (head == g_mem.tiny && g_mem.tiny->free && (size + BLOCKSIZE) < g_mem.tiny->size)
	{
		return g_mem.tiny->size - (size + BLOCKSIZE);
	}
	while (g_mem.tiny != head)
	{
		if (g_mem.tiny->free && (size + BLOCKSIZE) < g_mem.tiny->size)
		{
			return g_mem.tiny->size - (size + BLOCKSIZE);
		}
		g_mem.tiny = g_mem.tiny->next;
	}
	return -1;
}

void	*t_malloc(size_t size)
{
	int rest;

	if (!(g_mem.tiny)) {
		init_lst(TINY);
	}
	if ((rest = size_available(size)) != -1)
	{
		return carve_block(g_mem.tiny, size, rest);
	}
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
	t_block	*node;
	int		i;

	i = 0;
	while (i < 132)
	{
		lol = (char *)malloc(2048);
		lol = "mmalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussimalloc reussialloc reussi";
		i++;
	}
	i = 0;
	node = g_mem.tiny->next;
	while (node != g_mem.tiny)
	{
		dprintf(1, "i == %d && size == %zu\n", i, node->size);
		i++;
		node = node->next;
	}

	return (0);
}
