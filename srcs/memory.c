/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 09:50:01 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/08 14:51:41 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void	*malloc(size_t size)
{
	if (size == 0)
	{
		return (NULL);
	}
	else if (0 < size && size <= TINY)
	{
		return (t_malloc(size));
	}
	else if (TINY < size && size <= SMALL)
	{
		return (s_malloc(size));
	}
	else
	{
		return (l_malloc(size));
	}
}

t_block	*find_equality(void *ptr, t_block *head)
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

void	real_free(t_block *node)
{
	node->free = TRUE;
}

void	free(void *ptr)
{
	t_block	*node;

	if (ptr == NULL)
		return ;
	else if ((node = find_equality(ptr, g_mem.tiny)) != NULL)
	{
		real_free(node);
	}
	else if ((node = find_equality(ptr, g_mem.small)) != NULL)
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

	if (!head)
		return (0);
	if (type == TINY)
		ft_putstr("TINY : ");
	else if (type == SMALL)
		ft_putstr("SMALL : ");
	else if (type == LARGE)
		ft_putstr("LARGE : ");
	if (!head)
		return 0;
	ft_putnb_base((long long)head, "0123456789ABCDEF");
	ft_putstr("\n");
	node = head;
	total = 0;
	head = head->prev;
	while (node != head)
	{
		ft_putnb_base((long long)node, "0123456789ABCDEF");
		ft_putstr(" - ");
		ft_putnb_base((long long)node->next - BLOCKSIZE, "0123456789ABCDEF");
		ft_putstr(" : ");
		ft_putnb_base((long long)node->size, "0123456789");
		ft_putstr(" octets\n");
		total += node->size;
		node = node->next;
	}
	return total;
}

int		ft_power(int nb, int power)
{
	if (power == 0)
		return (1);
	else
		return (nb * ft_power(nb, power - 1));
}

void	show_alloc_mem()
{
	unsigned long long	total;
	t_block				*tiny;
	t_block				*small;
	t_block				*large;

	tiny = sort_list((g_mem.tiny));
	small = sort_list((g_mem.small));
	large = sort_list((g_mem.large));

	total = print_memory(tiny, TINY) + print_memory(small, SMALL) + print_memory(large, LARGE);
	ft_putstr("Total : ");
	ft_putnb_base((long long)total, "0123456789");
	ft_putstr(" octets\n");

}

t_block	*get_min(t_block *list)
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
	return rep;
}

t_block	*sort_list(t_block *head)
{
	int		swapped;
	t_block	*node;
	t_block	*swapper;

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
	return get_min(head);
}
