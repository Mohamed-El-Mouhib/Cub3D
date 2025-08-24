CFLAGS        = -Wall -Wextra -Iincludes -O3#-g -g3 -ggdb3 # -fsanitize=address
SRC           = $(wildcard *.c ./src/*.c) # update accordantly
OBJ           = $(SRC:%.c=$(OBJDIR)/%.o)
LIBFT_ARCHIVE = $(LIB_DIR)/libft.a
OBJDIR        = builds
NAME          = cub3d
LIB_DIR       = libs
CC            = cc
UNAME         = $(shell uname)

ifeq ($(UNAME), Linux)
	LINKERS = -lmlx -lXext -lX11 -lm -Llibs -lft
else
	LINKERS = -L../mlx -lmlx -lm -Llibs -lft -framework OpenGL -framework AppKit
endif

all: libft_rule $(NAME)

$(NAME): $(OBJ) $(LIBFT_ARCHIVE)
	$(CC) $(CFLAGS) $(OBJ) $(LINKERS) -o $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

libft_rule:
	@mkdir -p $(LIB_DIR)
	make -C libft

clean:
	rm -rf $(OBJ)
	@echo "\033[31m""it suppose to run make -C libft clean""\033[0m"
	# make -C libft clean

fclean: clean
	rm -rf $(NAME) $(LIB_DIR)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all clean fclean re libft_rule
