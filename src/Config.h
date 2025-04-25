#pragma once

// Window settings
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

// Maze and world settings
const int MAZE_SIZE = 15;
const float ROOM_SIZE = 10.0f; // Room size in the game world
const float WALL_HEIGHT = 3.0f;
const float DOOR_WIDTH = 1.5f;
const float DOOR_HEIGHT = 2.5f;

// Player settings
const float PLAYER_EYE_HEIGHT = 1.7f;   // Eye height for the player
const float PLAYER_MOVE_SPEED = 0.1f;   // Movement speed
const float PLAYER_ROTATE_SPEED = 0.005f; // Rotation speed

// Ghost settings
const float GHOST_SPEED = 0.02f;
const int GHOST_APPEAR_INTERVAL_MIN = 5000;  // Minimum ghost appearance interval in ms
const int GHOST_APPEAR_INTERVAL_MAX = 15000; // Maximum ghost appearance interval in ms
const int GHOST_VISIBLE_DURATION = 3000;    // Duration the ghost is visible in ms

// Lighting settings
const int FLICKER_INTERVAL = 200; // Flicker interval in milliseconds for horror lighting effect

// Sound file paths
const char* SOUND_FOOTSTEP = "sounds/footstep.wav";
const char* SOUND_DOOR_CREAK = "sounds/door_creak.wav";
const char* SOUND_GHOST_APPEAR = "sounds/ghost_appear.wav";
const char* SOUND_AMBIENT = "sounds/ambient_horror.wav";
const char* SOUND_PICKUP_KEY = "sounds/pickup_key.wav";
const char* SOUND_WIN = "sounds/win_sound.wav";

// Texture file paths
const char* TEX_WALL = "textures/wall_texture.png";
const char* TEX_FLOOR = "textures/floor_texture.png";
const char* TEX_CEILING = "textures/ceiling_texture.png";
const char* TEX_DOOR = "textures/door_texture.png"; 
const char* TEX_BLOOD = "textures/blood_texture.png";
const char* TEX_MIRROR = "textures/mirror_texture.png";
const char* TEX_TABLE = "textures/wood_texture.jpg";
const char* TEX_CHAIR = "textures/fabric_texture.jpg";
const char* TEX_MANNEQUIN_SKIN = "textures/mannequin_skin.jpg";
const char* TEX_MANNEQUIN_CLOTH = "textures/mannequin_cloth.jpg";
const char* TEX_MANNEQUIN_EYE = "textures/mannequin_eyes.jpg";
