# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thifranc <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/02/04 19:24:34 by thifranc          #+#    #+#              #
#    Updated: 2017/09/04 18:34:43 by thifranc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

INC = includes/ft_malloc.h

SRCS = srcs/ft_malloc srcs/memory

SYMLINK	= libft_malloc.so

SRCS:= $(addsuffix .c, $(SRCS))
OBJ = $(SRCS:%.c=%.o)

FLAGS = -Wall -Werror -Wextra -shared -I

all: $(NAME)

$(NAME):
	make -C libft/
	gcc $(FLAGS) $(SRCS) libft/libft.a -o $(NAME)
	ln -s $(NAME) $(SYMLINK)

clean:
	@$(RM) $(OBJ)
	@echo "erasing all binary files $(HOSTTYPE)"

fclean: clean
	@$(RM) $(NAME) $(SYMLINK)
	@make clean -C libft/
	@echo "erasing $(NAME)"

re: fclean all
	@echo "fclean + all"

.PHONY: all clean fclean re
