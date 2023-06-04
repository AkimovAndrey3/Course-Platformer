#pragma once

#include <string>
#include <box2d/box2d.h>

const float FIXED_DELTA_TIME = 1 / 30.f;

const bool GODMODE = false;

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 960;

const short TILE_HEIGHT = 32;
const short TILE_WIDTH = 32;
const short TILES_AMOUNT_WIDTH = 50;
const short TILES_AMOUNT_HEIGHT = 30;
const short LEVEL_WIDTH = TILES_AMOUNT_WIDTH * TILE_WIDTH;

const int LEVELS_AMOUNT = 5;
const short CHUNKS_AMOUNT = 6;

const short SWEETS_PER_LEVEL = 10;
const short SWEETS_MAXIMUM = LEVELS_AMOUNT * SWEETS_PER_LEVEL;

const std::string INITIAL_MAP_FILENAME_START = "../Assets/Levels/";
const std::string INITIAL_MAP_FILENAME_END = ".tmx";
const std::string FINAL_MAP_FILENAME = "../Assets/Levels/final.tmx";

const std::string FONT_FILE_PATH = "../Assets/Copperplate Bold.ttf";

const std::string PLAYER_TEXTURE_FILE_PATH = "../Assets/Player.png";
const std::string ENEMY_TEXTURE_FILE_PATH = "../Assets/enemy.png";
const std::string LIVE_SPRITE_FILE_PATH = "../Assets/heart.png";


typedef unsigned int ObjectId;
typedef unsigned int LevelId;
typedef unsigned int ListenerId;
typedef unsigned int EnemyId;

const short LIVES_MAXIMUM = 3;

const short DIFFICULTIES_AMOUNT = 3;

const short LIVES_MAXIMUM_EASY = 3;
const short LIVES_MAXIMUM_MEDIUM = 2;
const short LIVES_MAXIMUM_HARD = 1;

const short GAME_DIFFICULTY_LIVES[DIFFICULTIES_AMOUNT] = { LIVES_MAXIMUM_EASY , LIVES_MAXIMUM_MEDIUM , LIVES_MAXIMUM_HARD };

const short SWEETS_MINIMUM_EASY = LEVELS_AMOUNT * (SWEETS_PER_LEVEL - 5);
const short SWEETS_MINIMUM_MEDIUM = LEVELS_AMOUNT * (SWEETS_PER_LEVEL - 2);
const short SWEETS_MINIMUM_HARD = LEVELS_AMOUNT * SWEETS_PER_LEVEL;

const short GAME_DIFFICULTY_SWEETS[DIFFICULTIES_AMOUNT] = { SWEETS_MINIMUM_EASY , SWEETS_MINIMUM_MEDIUM , SWEETS_MINIMUM_HARD };

const short FRAME_HEIGHT = 32;
const short FRAME_WIDTH = 32;

const int IDLE_FRAMES = 10;
const int MOVING_FRAMES = 7;

const float IDLE_FRAME_TIME = 0.3f;
const float MOVE_FRAME_TIME = 0.2f;

const short ANIM_FRAMES[2] = { IDLE_FRAMES , MOVING_FRAMES };
const float ANIM_FRAME_TIMES[2] = { IDLE_FRAME_TIME , MOVE_FRAME_TIME };

const short ENEMY_ANIM_FRAMES = 6;
const float ENEMY_ANIM_FRAME_TIMES = 0.3f;

//const float ENEMY_VELOCITY = 10.f;
const float ENEMY_VELOCITY = 3.f;

//const float PLAYER_SPRINT_VELOCTIY = 1000.f;
//const float PLAYER_NORMAL_VELOCITY = 500.f;
//const float PLAYER_JUMP_MAX_TIME = 0.5f;
//const float PLAYER_JUMP_SPEED = 40.f;

const float PLAYER_SPRINT_VELOCTIY = 250.f;
const float PLAYER_NORMAL_VELOCITY = 150.f;
const float PLAYER_JUMP_MAX_TIME = 0.5f;
const float PLAYER_JUMP_SPEED = 20.f;

const b2Vec2 PLAYER_SPAWN_POSITION = { 20.f, 40.f };

enum class CollisionBits
{
	None = 0,
	Player,
	Item,
	PlayerLevelSwitcher,
	LevelSensor,
	Spikes,
	Enemy,
	EnemySensor,
	FinishSensor
};

enum class FaceDirection
{
	Left = -1,
	Right = 1,
};

enum class AnimStates
{
	Idle = 0,
	Move = 1,
};

enum class SceneTypes
{
	StartScene,
	GameScene,
	FinishScene,
};

enum class GameDifficulty
{
	Easy,
	Medium,
	Hard,
};