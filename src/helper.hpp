#pragma once


#include <array>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <array>
#include <iostream>
#include <string>
#include "inventory.hpp"
#include "items.hpp"

#define MAX_ENTITIES 255
#define SPRITE_SIZE 16
#define GAME_ITEMS 7
#define FRAME_TO_CHARACTER_OFFSET 8
#define CURSOR_TILE 2982


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
    ENTITY_STATE_RUNNING, // Not used added so calculation works
    ENTITY_STATE_FARMING,
    ENTITY_STATE_CHOPPING,
    ENTITY_STATE_WATERING,
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


    HOE_FRONT,
    HOE_BACK,
    HOE_RIGHT,
    HOE_LEFT,

    CHOP_FRONT,
    CHOP_BACK,
    CHOP_RIGHT,
    CHOP_LEFT,

    WATERING_FRONT,
    WATERING_BACK,
    WATERING_RIGHT,
    WATERING_LEFT,

    TOTAL,
};

struct Vector2
{
    float x;
    float y;
};

struct Vector2Int
{
    int x;
    int y;
};

struct Graphics
{
    SDL_Texture *playerTexture;
    SDL_Texture *chickenTexture;
    SDL_Texture *cowTexture;
    SDL_Texture *inventoryUi;
    SDL_Texture *itemSelector;
    SDL_Texture *selectedUi;
    SDL_Texture *inventorySlot;
    SDL_Texture *mapTexture;
    SDL_Texture *cursorTexture;
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

struct Map; // Forward Declaration

struct AnimalAnimations
{
    Animation idle;
    Animation walk;
    Animation eat;
};

struct Cursor
{
    Vector2 position;
    SDL_Texture *texture; // Non-Owning -- see graphics struct
};
struct Entity
{
    Animation* currentAnimation;
    CHARACTER_ANIMATION_ROW characterAnimationRow; //Only used for humanoids
    ENTITY_TYPE entityType;
    ENTITY_STATE entityState;
    SDL_Texture* entityTexture;
    PLAYER_DIRECTION playerDirection;
    Vector2 tilemapIndex;
    Vector2 placementTarget;
    SDL_FRect hitBox;
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
    std::array<Item,GAME_ITEMS> gameItems{};
    int currentItemIndex = 0;
    SDL_Event* event;
    std::unique_ptr<Map> map;
    Camera camera;
    Cursor cursor;
    Inventory inventory; // This should probable be moved to the player
    int playerIndex;
    int windowHeight;
    int windowWidth;
    int selectedItemUiIndex;
    Uint8 mouseButton;
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
void UpdateEntityState(AppState& appState, Entity& entity);
void UpdateCameraPosition(Entity& player, Camera& camera);
void WorldToScreen(Camera& camera, float entityWorldX, float entityWorldY, float& drawX, float& drawY);
void HandlePlayerState(AppState& appState, Entity& player);
void HandlePlayerAction(Map& map, Inventory& inventory, Entity& player);
void InputHandler(AppState& appState, Entity& player, Inventory& inventory, SDL_Event& event);
void RenderCursor(SDL_Renderer& renderer, Cursor& cursor, Map& map, Camera& camera);