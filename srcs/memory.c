/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/07 17:17:21 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	*malloc(size_t size)
{
	//dprintf(1, "malloc with size %zu, function called : %s\n", size, __func__);
	if (size == 0)
	{
		return (NULL);
	}
	else if (0 < size && size <= TINY)
	{
		//dprintf(1, "type 1\n");
		return (t_malloc(size));
	}
	else if (TINY < size && size <= SMALL)
	{
		//dprintf(1, "type 2\n");
		return (s_malloc(size));
	}
	else
	{
		//dprintf(1, "type 3\n");
		return (l_malloc(size));
	}
}

t_block	*find_equality(void *ptr, t_block *head)
{
	t_block	*stop;

	//dprintf(1, "function called : %s\n", __func__);
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
	else if ((node = find_equality(ptr, g_mem.tiny)) != NULL
			|| (node = find_equality(ptr, g_mem.small)) != NULL)
	{
		real_free(node);
	}
	else if ((node = find_equality(ptr, g_mem.large)))
	{
		node->free = TRUE;
	}
	else
		return ;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t		i;
	char		*src2;
	char		*dst2;

	i = 0;
	src2 = (char*)src;
	dst2 = (char*)dst;
	while (i < n)
	{
		dst2[i] = src2[i];
		i++;
	}
	return ((void*)dst);
}

void	*realloc(void *ptr, size_t size)
{
	void	*memory;

	if (ptr == NULL)
	{
		return (malloc(size));
	}
	else
	{
		if (!find_equality(ptr, g_mem.tiny)
				&& !find_equality(ptr, g_mem.small)
				&& !find_equality(ptr, g_mem.large))
			return (NULL);
		if (size == 0)
			size = 1;
		memory = malloc(size);
		ft_memcpy(memory, ptr, size);
		free(ptr);
		return (memory);
	}
}

unsigned long long	print_memory(t_block *head, int type)
{
	t_block	*node;
	unsigned long long	total;

	if (type == TINY)
		dprintf(1, "TINY : %p\n", g_mem.tiny);
	else if (type == SMALL)
		dprintf(1, "SMALL : %p\n", g_mem.small);
	else if (type == LARGE)
		dprintf(1, "LARGE : %p\n", g_mem.large);
	if (!head)
		return 0;
	node = head;
	total = 0;
	head = head->prev;
	while (node != head)
	{
		dprintf(1, "%p - %p : %ld octets\n", (void*)node + BLOCKSIZE, node->next,
				(void*)node->next - ((void*)node + BLOCKSIZE));
		total += (void*)node->next - ((void*)node + BLOCKSIZE);
		node = node->next;
	}
	return total;
}

void	show_alloc_mem()
{
	unsigned long long	total;



	sort_list(&(g_mem.l_head));
	sort_list(&(g_mem.s_head));
	sort_list(&(g_mem.t_head));

	total = print_memory(g_mem.t_head, TINY) + print_memory(g_mem.s_head, SMALL) + print_memory(g_mem.l_head, LARGE);
	dprintf(1, "Total : %llu octets\n", total);

}

void	sort_list(t_block **head)
{
	int		swapped;
	t_block	*node;
	t_block	*swapper;

	while (1)
	{
		swapped = FALSE;
		node = *head;
		while (node != (*head)->prev)
		{
			if (node > node->next)
			{
				swapped = TRUE;
				swapper = node->next;
				node->next = swapper->next;
				node->next->prev = node;
				node->prev->next = swapper;
				swapper->prev = node->prev;
				node->prev = swapper;
				swapper->next = node;
			}
			node = node->next;
		}
		if (swapped == FALSE)
			break ;
	}
}
