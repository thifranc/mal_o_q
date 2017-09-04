# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thifranc <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/04 19:24:34 by thifranc          #+#    #+#              #
#    Updated: 2017/09/04 17:59:46 by thifranc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = coucou.out

INC = includes/ft_malloc.h

SRCS = srcs/ft_malloc srcs/memory

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

SYMLINK			=	libft_malloc.so

SRCS:= $(addsuffix .c, $(SRCS))
OBJ = $(SRCS:%.c=%.o)

FLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME):
	make -C libft/
	gcc $(FLAGS) $(SRCS) libft/libft.a -o $(NAME)

clean:
	@$(RM) $(OBJ)
	@echo "erasing all binary files $(HOSTTYPE)"

fclean: clean
	@$(RM) $(NAME)
	@make clean -C libft/
	@echo "erasing $(NAME)"

re: fclean all
	@echo "fclean + all"

.PHONY: all clean fclean re
