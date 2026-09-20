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
    appState.graphics->inventoryUi = IMG_LoadTexture(appState.renderer,"assets/InventoryScreen.png");
    if (appState.graphics->inventoryUi == nullptr) return -1;
    appState.graphics->selectedUi = IMG_LoadTexture(appState.renderer,"assets/SelectedButton.png");
   if (appState.graphics->selectedUi == nullptr) return -1;
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

void RenderEntity(Entity& entity, SDL_Renderer& renderer, float drawX, float drawY)
{
    SDL_FRect srcRect = entity.currentAnimation->frameRect;
    srcRect.y = entity.currentAnimation->frameHeight * entity.currentAnimation->row;
    drawX -= SPRITE_SIZE/2.0f;
    drawY -= SPRITE_SIZE/2.0f;
    SDL_FRect destRect = {
        drawX,
        drawY,
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

void UpdateCameraPosition(Entity& player, Camera& camera)
{
    camera.Position.x = player.x + player.currentAnimation->frameWidth / 2.0f;
    camera.Position.y = player.y + player.currentAnimation->frameHeight / 2.0f;
}

void WorldToScreen(Camera& camera, float entityWorldX, float entityWorldY, float& drawX, float& drawY)
{
    drawX = entityWorldX - camera.Position.x + camera.width /2.0f;
    drawY = entityWorldY - camera.Position.y + camera.height /2.0f;
}

void RenderInventory(AppState& appState)
{
    const float yDistanceFromPlayer = 2;

    float uiLayerWidth = appState.graphics->inventoryUi->w;
    float uiLayerHeight = appState.graphics->inventoryUi->h;
    SDL_FRect src = {0,0,uiLayerWidth,uiLayerHeight};

    appState.uiX = appState.entities[appState.playerIndex].x - uiLayerWidth/2;
    appState.uiY = appState.entities[appState.playerIndex].y + uiLayerHeight * yDistanceFromPlayer;

    float drawX;
    float drawY;
    WorldToScreen(appState.camera,appState.uiX,appState.uiY,drawX,drawY);

    SDL_FRect dest = {drawX,drawY,uiLayerWidth,uiLayerHeight};

    if (!SDL_RenderTexture(appState.renderer,appState.graphics->inventoryUi,&src,&dest))
    {
        SDL_Log("Cannot render UI Layer: %s",SDL_GetError());
    }

    return;
}

void RenderSelectedButton(AppState& appState)
{
    float buttonWidth = appState.graphics->selectedUi->w;
    float buttonHeight = appState.graphics->selectedUi->h;

    SDL_FRect src = {0,0,buttonWidth,buttonHeight};

    

    float worldX = appState.uiX + INVENTORY_OFFSET + buttonWidth*appState.selectedItemUiIndex;
    float worldY = appState.uiY + INVENTORY_TOP_TO_BUTTONS;

    float drawX;
    float drawY;

    WorldToScreen(appState.camera,worldX,worldY,drawX,drawY);
    SDL_FRect dest = {drawX,drawY,buttonWidth,buttonHeight};

    if (!SDL_RenderTexture(appState.renderer,appState.graphics->selectedUi,&src,&dest))
    {
        SDL_Log("Could not render selected button: %s ",SDL_GetError());
    }

    return;
}

void UpdateSelectedItem(AppState& appState)
{
    const bool* keyStates = SDL_GetKeyboardState(nullptr);

    if (keyStates == nullptr) return;
    if (!keyStates) return;

    if (keyStates[SDL_SCANCODE_1]) appState.selectedItemUiIndex = 0;
    else if (keyStates[SDL_SCANCODE_2]) appState.selectedItemUiIndex = 1;
    else if (keyStates[SDL_SCANCODE_3]) appState.selectedItemUiIndex = 2;
    else if (keyStates[SDL_SCANCODE_4]) appState.selectedItemUiIndex = 3;
    else if (keyStates[SDL_SCANCODE_5]) appState.selectedItemUiIndex = 4;
    else if (keyStates[SDL_SCANCODE_6]) appState.selectedItemUiIndex = 5;
}