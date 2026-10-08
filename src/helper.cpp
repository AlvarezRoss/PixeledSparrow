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
    appState.graphics->itemSelector = IMG_LoadTexture(appState.renderer,"assets/Inventory_Light_example_with_slots_2.png");
    if (appState.graphics->itemSelector == nullptr) return -1;
    appState.graphics->inventoryUi = IMG_LoadTexture(appState.renderer,"assets/Setting menu (1).png");
    if (appState.graphics->inventoryUi == nullptr) return -1;
    appState.graphics->selectedUi = IMG_LoadTexture(appState.renderer,"assets/SelectedButton.png");
    if (appState.graphics->selectedUi == nullptr) return -1;
    appState.graphics->inventorySlot = IMG_LoadTexture(appState.renderer,"assets/inventory_example_with_slots.png");
    if (appState.graphics->inventorySlot == nullptr) return -1;
    appState.graphics->mapTexture = IMG_LoadTexture(appState.renderer,"assets/spr_tileset_sunnysideworld_16px.png");
    if (appState.graphics->inventorySlot == nullptr) return -1; 
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

    if (player.x < 0 && player.speedX == -1) player.speedX = 0;
    if (player.y < 0 && player.speedY == -1) player.speedY = 0;
    if (player.x > WORLD_WIDTH && player.speedX == 1) player.speedX = 0;
    if (player.y > WORLD_HEIGHT && player.speedY == 1) player.speedY = 0;
}

void UpdateEntityPosition(Entity& entity)
{
    entity.x += entity.speedX;
    entity.y += entity.speedY;

    entity.speedX = 0.0f;
    entity.speedY = 0.0f;
}
void UpdateEntityState(AppState& appState, Entity& entity)
{
    if (entity.entityType == ENTITY_PLAYER)
    {
        HandlePlayerState(appState, entity);
        return;
    }
    if (entity.speedX != 0.0f || entity.speedY != 0.0f)
    {
        entity.entityState = ENTITY_STATE_WALKING;
        return;
    }
    entity.entityState = ENTITY_STATE_IDLE;    
}

void UpdateCameraPosition(Entity& player, Camera& camera)
{
    camera.Position.x = player.x + player.currentAnimation->frameWidth / 2.0f - camera.width/2.0f;
    camera.Position.y = player.y + player.currentAnimation->frameHeight / 2.0f - camera.height/2.0f;
    // Camera boundries
    
    if (camera.Position.x < 0) camera.Position.x = 0;
    if (camera.Position.y < 0) camera.Position.y = 0;

    if (camera.Position.x + camera.width > WORLD_WIDTH) camera.Position.x = WORLD_WIDTH - camera.width;
    if (camera.Position.y + camera.height > WORLD_HEIGHT) camera.Position.y = WORLD_HEIGHT - camera.height;
}

void WorldToScreen(Camera& camera, float entityWorldX, float entityWorldY, float& drawX, float& drawY)
{
    drawX = entityWorldX - camera.Position.x; //+ camera.width /2.0f;
    drawY = entityWorldY - camera.Position.y;// + camera.height /2.0f;
}

void HandlePlayerState(AppState& appState, Entity& player)
{   
    if (player.speedX != 0 || player.speedY != 0)
    {
        player.entityState = ENTITY_STATE_WALKING;
        return; 
    }
    if (appState.mouseButton != SDL_BUTTON_LMASK)
    {
        player.entityState = ENTITY_STATE_IDLE;
    }
    else
    {
        HandlePlayerAction(appState.inventory,player);
    }
}

void HandlePlayerAction(Inventory& inventory, Entity& player)
{
    if (inventory.itemInUse == nullptr) return;
    if (inventory.open) return;
    switch (inventory.itemInUse->name)
    {
    case ITEM_AXE:
        player.entityState = ENTITY_STATE_CHOPPING;
        break;
    case ITEM_HOE:
        player.entityState = ENTITY_STATE_FARMING;
        break;
    case ITEM_WATERCAN:
        player.entityState = ENTITY_STATE_WATERING;
        break;
    default:
        break;
    }
}