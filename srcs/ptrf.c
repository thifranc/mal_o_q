/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ptrf.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/07 17:47:23 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/08 15:07:49 by thifranc         ###   ########.fr       */
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

