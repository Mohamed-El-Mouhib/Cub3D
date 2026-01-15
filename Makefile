CFLAGS        = -Wall -Werror -Wextra -O3 -Iincludes 
SRC           = dynamic_array_1.c dynamic_array.c src/animation.c src/map_parser_5.c src/assets_factory.c src/map_parser_6.c\
		cub.c src/map_parser.c src/cub_start_utils.c src/minimap_1.c\
		src/dda.c src/minimap.c src/dda_util_1.c src/mouse_events.c\
		src/dda_util_2.c  src/player.c src/enemy_init.c src/player_init.c\
		src/enemy_movements_1.c src/player_init_utils_1.c src/enemy_movements.c src/player_render.c\
		src/enemy_render.c src/player_state.c src/enemy_render_util_1.c src/player_update.c\
		src/game_init.c src/player_update_utils_1.c src/graphics_1.c src/release_mem.c\
		src/graphics.c src/release_mem_utis_1.c src/image_buffer_1.c src/texture_impl.c\
		src/image_buffer.c src/texture_impl_utils.c src/keyboard_events.c src/ui.c\
		src/map_parser_1.c src/utils.c src/map_parser_2.c src/vectors_1.c\
		src/map_parser_3.c src/vectors.c src/map_parser_4.c
OBJ           = $(SRC:%.c=$(OBJDIR)/%.o)
LIBFT_ARCHIVE = $(LIB_DIR)/libft.a
LINKERS 	  = -lmlx -lXext -lX11 -lm -Llibs -lft
OBJDIR        = builds
NAME          = cub3d
LIB_DIR       = libs
CC            = cc
ASSET_DIR     = textures
ASSET_ARC     = textures.zip

all: libft_rule $(NAME)

bonus: all

$(ASSET_DIR):
	unzip -oq $(ASSET_ARC)

$(NAME): $(OBJ) $(LIBFT_ARCHIVE) $(ASSET_DIR)
	$(CC) $(CFLAGS) $(OBJ) $(LINKERS) -o $(NAME)

libft_rule:
	@mkdir -p $(LIB_DIR)
	@make -C libft

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)
	make -sC libft clean

fclean: clean
	@rm -rf $(ASSET_DIR)
	@rm -rf $(NAME) $(LIB_DIR)

re: fclean all

.SECONDARY: $(OBJ)
.PHONY: all bonus clean fclean re libft_rule
