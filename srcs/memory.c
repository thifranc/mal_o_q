/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/06 11:51:57 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	*malloc(size_t size)
{
	if (size == 0) {
		return (NULL);
	} else if (0 < size && size <= TINY) {
		return t_malloc(size);
	}
	else if (TINY < size && size <= SMALL) {
		return s_malloc(size);
	} else {
		return l_malloc(size);
	}
}

t_block	*find_equality(void *ptr, t_block *head)
{
	t_block	*stop;

	dprintf(1, "free call\n");
	if (head == NULL)
		return NULL;
	stop = head->prev;
	while (head != stop)
	{
		if ((void*)head + BLOCKSIZE == ptr)
		{
			return head;
		}
		head = head->next;
	}
	if ((void*)head + BLOCKSIZE == ptr)
	{
		return head;
	}
	else
		return NULL;
}

void	real_free(t_block *node)
{
	node->free = TRUE;
	/*
	if (node->prev->free == 1)
	{
		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->prev->size += node->size + BLOCKSIZE;
	} else if (node->next->free == 1) {
		real_free(node->next);
	}
	*/
}

void	free(void *ptr)
{
	t_block	*node;

	if (ptr == NULL)
		return ;
	else if (
			(node = find_equality(ptr, g_mem.tiny)) != NULL
			|| 	(node = find_equality(ptr, g_mem.small)) != NULL
			)
	{
		real_free(node);
	}
	else if ((node = find_equality(ptr, g_mem.large)))
	{
		dprintf(1, "large is freed\n");
		node->free = TRUE;
	}
	else 
		return ;
}
