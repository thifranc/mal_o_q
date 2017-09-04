/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/04 18:33:14 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	*t_malloc(size_t size);

void	*carve_block(t_block *cur, size_t size, int rest)
{
	t_block	*new;

	cur->free = FALSE;
	dprintf(1, "%lu == blocksize\n", BLOCKSIZE);
	new = (void*)cur + BLOCKSIZE + size;
	dprintf(1, "new is at %p, so %p - %p = %ld\n", new, new, cur, (new - cur));
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

void	free(void *ptr)
{
	if (ptr == NULL)
		return ;
}
