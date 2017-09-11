/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/11 10:05:25 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/11 10:05:31 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

void				print_line(t_block *node)
{
	ft_putstr("0x");
	ft_putnb_base(((long long)node + BLOCKSIZE) % POWER, "0123456789ABCDEF");
	ft_putstr(" - ");
	ft_putstr("0x");
	ft_putnb_base(((long long)node->next) % POWER,
			"0123456789ABCDEF");
	ft_putstr(" : ");
	ft_putnb_base((long long)node->size, "0123456789");
	ft_putstr(" octets\n");
}

void				print_head_line(int type)
{
	if (type == TINY)
		ft_putstr("TINY : ");
	else if (type == SMALL)
		ft_putstr("SMALL : ");
	else if (type == LARGE)
		ft_putstr("LARGE : ");
}

unsigned long long	print_memory(t_block *head, int type)
{
	t_block				*node;
	unsigned long long	total;

	print_head_line(type);
	if (!head)
	{
		ft_putstr("\n");
		return (0);
	}
	ft_putstr("0x");
	ft_putnb_base((long long)head % POWER, "0123456789ABCDEF");
	ft_putstr("\n");
	node = head;
	total = 0;
	head = head->prev;
	while (node != head)
	{
		print_line(node);
		total += node->size;
		node = node->next;
	}
	return (total);
}

void				show_alloc_mem(void)
{
	unsigned long long	total;
	t_block				*tiny;
	t_block				*small;
	t_block				*large;

	tiny = sort_list((g_mem.tiny));
	small = sort_list((g_mem.small));
	large = sort_list((g_mem.large));
	total = print_memory(tiny, TINY)
		+ print_memory(small, SMALL)
		+ print_memory(large, LARGE);
	ft_putstr("Total : ");
	ft_putnb_base((long long)total, "0123456789");
	ft_putstr(" octets\n");
}
