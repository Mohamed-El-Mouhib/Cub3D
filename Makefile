CFLAGS        = -Wall -Wextra -Iincludes -O3 -g -g3 -ggdb3 # -fsanitize=address
SRC           = $(wildcard *.c ./src/*.c) # update accordantly
OBJ           = $(SRC:%.c=$(OBJDIR)/%.o)
LIBFT_ARCHIVE = $(LIB_DIR)/libft.a
OBJDIR        = builds
NAME          = cub3d
LIB_DIR       = libs
CC            = cc
ASSET_DIR     = textures
ASSET_ARC     = textures.zip
UNAME         = $(shell uname)
ASSETS_LINK   = https://github.com/Mohamed-El-Mouhib/Cub3D/releases/download/CubAssets/textures.zip

ifeq ($(UNAME), Linux)
	LINKERS = -lmlx -lXext -lX11 -lm -Llibs -lft
else
	LINKERS = -L../mlx -lmlx -lm -Llibs -lft -framework OpenGL -framework AppKit
endif


all: libft_rule $(NAME)
	@echo -e '\nNOTE: run "make assets" to download the textures used in the game'

assets:
	rm -rf $(ASSET_DIR) $(ASSET_ARC)
	@echo Downloading the assets...
	@curl -LO $(ASSETS_LINK)
	@echo Unzipping the assets Archive...
	@unzip -oq $(ASSET_ARC)

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
.PHONY: all clean fclean re libft_rule assets
