#pragma once


#include <array>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <array>
#include <iostream>
#include <string>

#define MAX_ENTITIES 255
#define MAP_LAYERS 3
#define MAP_WIDTH 100
#define MAP_HEIGHT 100
#define TILE_MAP_WIDTH 1024
#define TILE_MAP_HEIGHT 1024
#define TILE_SIZE 16
#define TILED_OFFSET 1
#define MAP_PIXEL_COUNT 1600
#define SPRITE_SIZE 16
#define INVENTORY_TOP_TO_BUTTONS 35
#define INVENTORY_OFFSET 11

enum ENTITY_TYPE
{
    ENTITY_PLAYER,
    ENTITY_NPC,
    ENTITY_COW,
    ENTITY_CHICKEN,

    ENTITY_TOTAL,
    ENTITY_NONE
};
enum GAME_STATE
{
    GAME_STATE_RUNNING,
    GAME_STATE_QUIT
};
enum ENTITY_STATE
{
    ENTITY_STATE_IDLE,
    ENTITY_STATE_WALKING,

    ENTITY_STATE_TOTAL,
    ENTITY_STATE_INACTIVE,
    ENTITY_STATE_UNINITIALIZED
};

enum PLAYER_DIRECTION
{
    FRONT,
    BACK,
    RIGHT,
    LEFT
};

enum ANIMATION_STATE
{
    ANIMATION_STATE_WALKING,
    ANIMATION_STATE_IDLE,
};

enum CHARACTER_ANIMATION_ROW
{
    IDLE_FRONT,
    IDLE_BACK,
    IDLE_RIGHT,
    IDLE_LEFT,


    WALKING_FRONT,
    WALKING_BACK,
    WALKING_RIGHT,
    WALKING_LEFT,

    TOTAL,
};

struct Vector2
{
    float x;
    float y;
};

struct Graphics
{
    SDL_Texture *playerTexture;
    SDL_Texture *chickenTexture;
    SDL_Texture *cowTexture;
    SDL_Texture *inventoryUi;
    SDL_Texture *selectedUi;
};
struct Animation
{
    SDL_FRect frameRect;
    int frameCount; // Used to control the frame changes based on the number of frames that have passed
    int currentFrame;
    int frameSpeed;
    int frameNumer;
    float frameHeight;
    float frameWidth;
    float frameSize;
    int row;
    ANIMATION_STATE state;
};

struct Map
{
    int map[MAP_LAYERS][MAP_HEIGHT][MAP_WIDTH];
    const std::string path = "assets/Tiled/PixeledSparrow.tmx";
    const std::string pngPath = "assets/spr_tileset_sunnysideworld_16px.png";
    int numberOfColumns = TILE_MAP_WIDTH/TILE_SIZE;
    SDL_Texture *mapTexture = nullptr;
};

struct AnimalAnimations
{
    Animation idle;
    Animation walk;
    Animation eat;
};
struct Entity
{
    Animation* currentAnimation;
    CHARACTER_ANIMATION_ROW characterAnimationRow; //Only used for humanoids
    ENTITY_TYPE entityType;
    ENTITY_STATE entityState;
    SDL_Texture* entityTexture;
    PLAYER_DIRECTION playerDirection;
    float speedX;
    float speedY;
    float x;
    float y;
};

struct Camera
{
    Vector2 Position;
    float width;
    float height; 
};

struct AppState
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    GAME_STATE state;
    std::unique_ptr<Graphics> graphics;
    std::unique_ptr<AnimalAnimations> cowAnimations;
    std::unique_ptr<AnimalAnimations> chickenAnimations;
    std::unique_ptr<Animation> characterAnimation;
    std::array<Entity,MAX_ENTITIES> entities{};
    Map map;
    Camera camera;
    int playerIndex;
    int windowHeight;
    int windowWidth;
    int selectedItemUiIndex;
    Uint8 mouseButton;
    float mouseX;
    float mouseY;
    float uiX;
    float uiY;
};

void HandleAppEvent(AppState& gameState);
void CleanUp(SDL_Window* window);
int InitGraphics(AppState& appState);
void SetEntities(AppState& appState);
void RenderEntity(Entity& entity, SDL_Renderer& renderer, float drawX, float drawY);
void InitEntities(AppState& appState);
// player is contained wihtin the entity array inside appState but for ease of use in the function it is passed as a separete parameter
void PlayerMovement(Entity& player, AppState& appState);
void UpdateEntityPosition(Entity& entity);
void UpdateEntityState(Entity& entity);
void UpdateCameraPosition(Entity& player, Camera& camera);
void WorldToScreen(Camera& camera, float entityWorldX, float entityWorldY, float& drawX, float& drawY);
void RenderInventory(AppState& appState);
void RenderSelectedButton(AppState& appState);
void UpdateSelectedItem(AppState& appState);