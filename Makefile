SRC=cub.c 
OBJ= $(SRC:%.c=%.o)
CC=cc
CFLAGS= -Wall -Werror -Wextra -Iincludes -Imlx
NAME=Cub3d
LIBS=./libs/libft
LIB_DIR=libs

all: $(NAME)

$(NAME): $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) $(OBJ) -Llibs -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz -lft -o $(NAME)

$(LIBS): $(LIB_DIR)
	make -C libft

$(LIB_DIR):
	mkdir -p libs

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
	make -C libft clean

fclean: clean
	rm -rf $(NAME) $(LIB_DIR)

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJ)
