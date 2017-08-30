/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 13:48:23 by thifranc          #+#    #+#             */
/*   Updated: 2017/08/30 18:02:00 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

#define BLOCKSIZE sizeof(t_block) //relatif au systeme normalement
#define TINY 2 //relatif au systeme normalement
#define SMALL 10

typedef struct		s_block
{
	void			*content;
	char			free;
	size_t			size;
	struct s_block	*next;
	struct s_block	*prev;
}					t_block;


void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif
