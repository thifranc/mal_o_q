/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/07 17:47:23 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/07 18:20:28 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_malloc.h"

size_t	ft_strlen(const char *c)
{
	size_t	i;

	i = 0;
	while (c && c[i])
		i++;
	return (i);
}

void	ft_putstr(char const *s)
{
	write(1, s, ft_strlen(s));
}
void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnb_base(long long n, char *base)
{
	if (n < 0)
	{
		ft_putchar('-');
		ft_putnb_base(-n, base);
	}
	else if (0 <= n && (size_t)n < ft_strlen(base))
		ft_putchar(base[n]);
	else
	{
		ft_putnb_base(n / ft_strlen(base), base);
		ft_putnb_base(n % ft_strlen(base), base);
	}
}
