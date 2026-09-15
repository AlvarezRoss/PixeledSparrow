#include "helper.hpp"

void CleanUp(SDL_Window* window)
{
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}

void HandleAppEvent(AppState& gameState)
{
    SDL_Event event = {0};
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            gameState.state = GAME_STATE_QUIT;
            break;
        
        default:
            break;
        }        
    }
}



// The program will quit if this function reutnrs -1
int InitGraphics(AppState& appState)
{
    appState.graphics->playerTexture = IMG_LoadTexture(appState.renderer,"assets/Characters/Premium Charakter Spritesheet.png");
    if (appState.graphics->playerTexture == nullptr) return -1;
    appState.graphics->cowTexture = IMG_LoadTexture(appState.renderer,"assets/Animals/Cow/Brown cow animations.png");
    if (appState.graphics->cowTexture == nullptr) return -1;
    appState.graphics->chickenTexture = IMG_LoadTexture(appState.renderer,"assets/Animals/Chicken/chicken default.png");
    if (appState.graphics->chickenTexture == nullptr) return -1;
    appState.graphics->bushTiles = IMG_LoadTexture(appState.renderer,"assets/Tilesets/ground tiles/New tiles/Bush_Tiles.png");
    if (appState.graphics->bushTiles == nullptr) return -1;
    appState.graphics->darkGrassTiles = IMG_LoadTexture(appState.renderer,"assets/Tilesets/ground tiles/New tiles/Darker_Grass_Hills_Tiles_v2.png");
    if (appState.graphics->darkGrassTiles == nullptr) return -1;
    appState.graphics->darkHillTiles = IMG_LoadTexture(appState.renderer,"assets/Tilesets/ground tiles/New tiles/Darker_Grass_Tiles_v2.png");
    return 0;
}

void SetEntities(AppState& appState)
{
    appState.entities[0].entityType = ENTITY_PLAYER;
    for (int i = 1; i < MAX_ENTITIES; i++)
    {
        appState.entities[i].entityType = ENTITY_NONE;
        appState.entities[i].entityState = ENTITY_STATE_UNINITIALIZED;
    }
    
    return;
}
void InitEntities(AppState& appState)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        switch (appState.entities[i].entityType)
        {
        case ENTITY_PLAYER:
            appState.characterAnimation->row = IDLE_FRONT;
            appState.entities[i].entityState = ENTITY_STATE_IDLE;
            appState.entities[i].entityTexture = appState.graphics->playerTexture;
            appState.entities[i].currentAnimation = appState.characterAnimation.get();
            break;
        default:
            break;
        }
    }
}

void RenderEntity(Entity& entity, SDL_Renderer& renderer)
{
    SDL_FRect srcRect = entity.currentAnimation->frameRect;
    srcRect.y = entity.currentAnimation->frameHeight * entity.currentAnimation->row;
    SDL_FRect destRect = {
        entity.x,
        entity.y,
        entity.currentAnimation->frameWidth,
        entity.currentAnimation->frameHeight
    };
    if (!SDL_RenderTexture(&renderer,entity.entityTexture,&srcRect,&destRect))
    {
        SDL_Log("Could not render entity: %s",SDL_GetError());
    }
    return;
}

void PlayerMovement(Entity& player, AppState& appState)
{
    // keyStates is an array of booleans index by SDL_ScanCode which represent the keys true if pressed false if not
    const bool* keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates == nullptr) return;
    if (!keyStates) return;

    if (keyStates[SDL_SCANCODE_W])
    {
        player.speedY = -1;
        player.playerDirection = BACK;
    } 
    else if(keyStates[SDL_SCANCODE_S])
    {
        player.speedY = 1;
        player.playerDirection = FRONT;
    } 

    if (keyStates[SDL_SCANCODE_A])
    {
        player.speedX = -1;
        player.playerDirection = LEFT;
    } 
    else if (keyStates[SDL_SCANCODE_D])
    {
        player.speedX = 1;
        player.playerDirection = RIGHT;
    }

}

void UpdateEntityPosition(Entity& entity)
{
    entity.x += entity.speedX;
    entity.y += entity.speedY;

    entity.speedX = 0.0f;
    entity.speedY = 0.0f;
}
void UpdateEntityState(Entity& entity)
{
    if (entity.speedX == 0.0f && entity.speedY == 0.0f)
    {
        entity.entityState = ENTITY_STATE_IDLE;
        return;
    }
    entity.entityState = ENTITY_STATE_WALKING;
}