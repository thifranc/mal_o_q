/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/06 16:36:56 by thifranc         ###   ########.fr       */
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
		dprintf(1, "%p - %p : %ld octets\n", node + BLOCKSIZE, node->next, node->next - (node + BLOCKSIZE));
		total += node->next - (node + BLOCKSIZE);
		node = node->next;
	}
	return total;
}

void	first_in_head(t_block **list)
{
	t_block	*node;
	t_block	*head;

	if (!*list)
		return ;
	head = *list;
	dprintf(1, "lol1\n");
	node = (*list)->next;
	while (node != *list)
	{
		if (node < head)
			head = node;
		node = node->next;
	}
	*list = head;
}

void	show_alloc_mem()
{
	unsigned long long	total;

	dprintf(1, "lol1\n");
	first_in_head(&(g_mem.tiny));
	dprintf(1, "lol2\n");
	first_in_head(&(g_mem.small));
	dprintf(1, "lol3\n");
	first_in_head(&(g_mem.large));
	dprintf(1, "lol4\n");

	total = print_memory(g_mem.tiny, TINY) + print_memory(g_mem.small, SMALL) + print_memory(g_mem.large, LARGE);
	dprintf(1, "Total : %llu octets\n", total);

}
