# TODO

This file lists the areas of the project that need improvement and features that could be added.

## Bugs and Fixes

*   **Collision Detection**: The player can walk through walls. Implement collision detection to prevent this.
*   **Player Movement**: The player movement is not smooth. The movement speed is also inconsistent.
*   **Texture Rendering**: Textures are distorted (fisheye effect). This can be fixed by using the perpendicular distance to the wall instead of the Euclidean distance.
*   **Map Parsing**: The map parser is very basic. It does not handle invalid maps gracefully. For example, a map that is not enclosed by walls will cause the game to crash.
*   **Error Handling**: The program often crashes without a proper error message. For example, if a texture file is not found, the program will crash with a segmentation fault.

## Improvements

*   **Code Structure**: The code could be better organized. Some files are too long and could be split into smaller files. For example, `dda.c` could be split into multiple files.
*   **Constants**: Many magic numbers are used in the code. These should be replaced with named constants.
*   **Header Files**: The header files are not well organized. There is a circular dependency between `cub3d.h` and `graphics.h`.
*   **Performance**: The game's performance could be improved. For example, the minimap is redrawn every frame, which is not necessary.

## New Features

*   **Sprites**: Add support for sprites to the game. This would allow for objects like enemies, items, and decorations to be placed in the map.
*   **Floor and Ceiling Textures**: Add support for floor and ceiling textures.
*   **Animated Sprites**: Add support for animated sprites.
*   **Sound**: Add sound effects and music to the game.
*   **HUD**: Add a heads-up display (HUD) to the game. This could display information such as the player's health, ammo, and score.
*   **Doors**: Add support for doors that can be opened and closed by the player.
*   **Map Editor**: Create a map editor to make it easier to create new maps.
