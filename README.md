# Cub3D

This project is a 3D game built using raycasting techniques, similar to the classic game Wolfenstein 3D. The player can navigate a maze-like map, rendered in a first-person perspective.

## Features

*   **Raycasting Engine**: The core of the project is a raycasting engine that renders a 2D map into a 3D perspective.
*   **Map Parsing**: The game loads maps from `.txt` files. The map format is a simple grid of characters, where '1' represents a wall and '0' represents an empty space.
*   **Player Movement**: The player can move forward, backward, and strafe left and right.
*   **Player Rotation**: The player can rotate the camera to look around the environment.
*   **Minimap**: A 2D minimap is displayed on the screen, showing the player's position and the layout of the map.
*   **Texture Mapping**: Walls are rendered with textures loaded from XPM files.
*   **Mouse Control**: The player can control the camera with the mouse.

## How to Compile and Run

1.  **Compile the project**:
    ```bash
    make
    ```
2.  **Run the game**:
    ```bash
    ./cub3d
    ```

## Controls

*   **W**: Move forward
*   **S**: Move backward
*   **A**: Strafe left
*   **D**: Strafe right
*   **Left Arrow**: Rotate left
*   **Right Arrow**: Rotate right
*   **Mouse**: Look around
*   **R**: Reset player position
*   **ESC**: Exit the game
