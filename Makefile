# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/06 12:31:33 by mdziadko          #+#    #+#              #
#    Updated: 2025/04/14 17:03:11 by mdziadko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 								   VARIABLES								   #
# **************************************************************************** #

NAME		= pipex
CC 			= cc
CFLAGS		= -Wall -Wextra -Werror -g
AR			= ar rcs
RM			= rm -f

# Directories
PRINTF_DIR	= printf
PRINTF_LIB	= $(PRINTF_DIR)/libftprintf.a
LIBFT_DIR	= libft
LIBFT_LIB	= $(LIBFT_DIR)/libft.a

# Compilation settings
INCLUDES	= -I. -I$(PRINTF_DIR) -I$(LIBFT_DIR)
LIBS		= -L$(PRINTF_DIR) -lftprintf -L$(LIBFT_DIR) -lft

# **************************************************************************** #
# 									FILES									   #
# **************************************************************************** #

SRCS		= pipex.c pipex_exec.c pipex_parse.c pipex_cleanup.c pipex_help.c
OBJS		= $(SRCS:.c=.o)
HEADERS		= pipex.h

# **************************************************************************** #
# 									RULES									   #
# **************************************************************************** #

all: $(PRINTF_LIB) $(LIBFT_LIB) $(NAME)

# Runs make inside the printf folder
$(PRINTF_LIB):
	$(MAKE) -C $(PRINTF_DIR)

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

# Object files (replace .c with .o)
$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Rule to build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule to remove generated files
clean:
	$(RM) $(OBJS)
	$(MAKE) clean -C $(PRINTF_DIR)
	$(MAKE) clean -C $(LIBFT_DIR)

# Rule to remove all unnecessary files
fclean: clean
	$(RM) $(NAME)
	$(MAKE) fclean -C $(PRINTF_DIR)
	$(MAKE) fclean -C $(LIBFT_DIR)

# Rule to rebuild everything
re: fclean all

.PHONY: all clean fclean re