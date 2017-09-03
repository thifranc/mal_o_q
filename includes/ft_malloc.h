/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/29 13:48:23 by thifranc          #+#    #+#             */
/*   Updated: 2017/09/03 18:08:39 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#define BLOCKSIZE sizeof(t_block) 
#define TINY 2048 //relatif au systeme normalement
#define SMALL 10


typedef enum		e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef struct		s_block
{
	t_bool			free;
	size_t			size;
	struct s_block	*next;
	struct s_block	*prev;
}					t_block;


void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif
