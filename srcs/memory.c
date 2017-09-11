/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/11 10:02:03 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

t_block				*find_equality(void *ptr, t_block *head)
{
	t_block	*stop;

	if (head == NULL || ptr == NULL)
		return (NULL);
	stop = head->prev;
	while (head != stop)
	{
		if ((void*)head + BLOCKSIZE == ptr)
		{
			return (head);
		}
		head = head->next;
	}
	if ((void*)head + BLOCKSIZE == ptr)
	{
		return (head);
	}
	else
		return (NULL);
}

t_block				*get_min(t_block *list)
{
	t_block	*rep;
	t_block	*runner;

	runner = list;
	rep = runner;
	while (runner != list->prev)
	{
		if (runner < rep)
			rep = runner;
		runner = runner->next;
	}
	if (runner < rep)
		rep = runner;
	return (rep);
}

void				swapping(t_block **a)
{
	t_block	*swapper;

	swapper = (*a)->next;
	(*a)->next = swapper->next;
	(*a)->next->prev = (*a);
	(*a)->prev->next = swapper;
	swapper->prev = (*a)->prev;
	(*a)->prev = swapper;
	swapper->next = (*a);
}

t_block				*sort_list(t_block *head)
{
	int		swapped;
	t_block	*node;

	if (!head)
		return (NULL);
	while (1)
	{
		swapped = FALSE;
		node = get_min(head);
		while (node != (head)->prev)
		{
			if (node > node->next)
			{
				swapped = TRUE;
				swapping(&node);
			}
			node = node->next;
		}
		if (swapped == FALSE)
			break ;
	}
	return (get_min(head));
}
