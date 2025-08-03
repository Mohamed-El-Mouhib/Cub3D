LINKERS       = -lmlx -lXext -lX11 -lm -Llibs -lft
CFLAGS        = -Wall -Werror -Wextra -Iincludes
SRC           = $(wildcard *.c) # update accordantly
OBJ           = $(SRC:%.c=$(OBJDIR)/%.o)
LIBFT_ARCHIVE = $(LIB_DIR)/libft.a
OBJDIR        = builds
NAME          = cub3d
LIB_DIR       = libs
CC            = cc

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
	make -C libft clean

fclean: clean
	rm -rf $(NAME) $(LIB_DIR)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all clean fclean re libft_rule
