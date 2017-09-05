/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/05 11:11:06 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

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
	return (void *)cur + BLOCKSIZE;
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

t_block	*find_equality(void *ptr, t_block *head)
{
	t_block	*stop;

	dprintf(1, "ptr == %p -- node== %p\n", ptr, (void*)head + BLOCKSIZE);
	stop = head->prev;
	while (head != stop)
	{
		if ((void*)head + BLOCKSIZE == ptr)
		{
			dprintf(1, "coucou\n");
			return head;
		}
		head = head->next;
	}
	if ((void*)head + BLOCKSIZE == ptr)
	{
		dprintf(1, "coucou\n");
		return head;
	}
	else
		return NULL;
}

void	real_free(t_block *node)
{
	node->free = 1;
	if (node->prev->free == 1)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->prev->size += node->size + BLOCKSIZE;
	} else if (node->next->free == 1) {
		real_free(node->next);
	}
}

void	free(void *ptr)
{
	t_block	*node;
	//dprintf(1, "ptr == %p -- node== %p\n", ptr, (void*)g_mem.tiny + BLOCKSIZE);

	if (ptr == NULL)
		return ;
	else if (
			(node = find_equality(ptr, g_mem.tiny)) != NULL
			/*
			   || 	(node = find_equality(ptr, g_mem.small)) != NULL
			   */
			)
	{
		dprintf(1, "real free called \n");
		real_free(node);
	}
	/*
	   else if ((node = find_equality(ptr, g_mem.large)))
	   return ;
	   */
	else 
		return ;
}
