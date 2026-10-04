#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include "helper.hpp"
#include "animationProcess.hpp"
#include "map.hpp"
#include "inventory.hpp"
#include "items.hpp"

void Render(AppState& appState);

SDL_AppResult SDL_AppInit(void **appstate, int arc, char **argv)
{
    
    SDL_SetAppMetadata("PixeledSparrow","0.01","com.test.PixeledSparrow");
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Could not init SDL %s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE,"60"))
    {
        SDL_Log("Could not set framerate: %s",SDL_GetError());
    }
    std::unique_ptr<AppState> state = std::make_unique<AppState>();
    state->graphics = std::make_unique<Graphics>();
    state->cowAnimations = std::make_unique<AnimalAnimations>();
    state->chickenAnimations = std::make_unique<AnimalAnimations>();
    state->characterAnimation = std::make_unique<Animation>();
    state->windowWidth = 800;
    state->windowHeight = 600;
    state->mouseButton = 0;
    state->selectedItemUiIndex = 6;
    if (!SDL_CreateWindowAndRenderer("PixeledSparrow",state->windowWidth,state->windowHeight,SDL_WINDOW_RESIZABLE,&state->window,&state->renderer))
    {
        SDL_Log("Could not create window and renderer: %s",SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->camera = {{0,0},800.0f,600.0f};

    if (InitGraphics(*state) != 0)
    {
        SDL_Log("Could not init graphics: %s",SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(state->renderer,800,600,SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SetEntities(*state);
    InitEntities(*state);
    InitAnimations(*state);
    InitInventory(*state,*state->graphics.get());
    if (InitMap(state->map,*state) != 0)
    {
        SDL_Log("Could not init map: %s",SDL_GetError());
        return SDL_APP_FAILURE;
    } 
    InitItems(*state);
    *appstate = state.release();
    return SDL_APP_CONTINUE;
}

// This function is the heart of our program. -> It runs every frame aka - GAME LOOP
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = (AppState*)appstate;
    
    
    PlayerMovement(state->entities[state->playerIndex],*state);
    for (int i = 0; i < MAX_ENTITIES ; i++)
    {
        if (state->entities[i].entityType == ENTITY_NONE) continue;
        UpdateEntityState(*state,state->entities[i]);
        SetAnimation(state->entities[i],*state);
        UpdateEntityPosition(state->entities[i]);
        UpdateEntityAnimation(state->entities[state->playerIndex],*state);
        
    }
    UpdateCameraPosition(state->entities[state->playerIndex],state->camera);
    Render(*state);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *state = (AppState*)appstate;
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        state->mouseButton = event->button.button;
        if (state->inventory.open) ProcessInventory(*state,state->inventory,*event);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        state->mouseButton = 0;
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        break;
    case SDL_EVENT_KEY_DOWN:
        if (event->button.button == SDL_SCANCODE_I) UpdateInventoryState(*state,*event);
        UpdateItemSelector(state->inventory);
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL will clean itself up we don't need to do anything SDL Specific here.
    // However we will need to free our heap memory here
    std::unique_ptr<AppState> state (static_cast<AppState*>(appstate));
    if (state->graphics->chickenTexture != nullptr) SDL_DestroyTexture(state->graphics->chickenTexture);
    if (state->graphics->cowTexture != nullptr) SDL_DestroyTexture(state->graphics->cowTexture);
    if (state->graphics->inventorySlot != nullptr) SDL_DestroyTexture(state->graphics->inventorySlot);
    if (state->graphics->itemSelector != nullptr) SDL_DestroyTexture(state->graphics->itemSelector);
    if (state->graphics->mapTexture != nullptr) SDL_DestroyTexture(state->graphics->mapTexture);
    if (state->graphics->playerTexture != nullptr) SDL_DestroyTexture(state->graphics->playerTexture);
    if (state->graphics->selectedUi != nullptr) SDL_DestroyTexture(state->graphics->selectedUi);
    if (state->renderer != nullptr) SDL_DestroyRenderer(state->renderer);
    if (state->window != nullptr) SDL_DestroyWindow(state->window);

}

void Render(AppState& appState)
{
    if (!SDL_RenderClear(appState.renderer)) // Clears renderer
    {
        SDL_Log("Could not clear renderer: %s",SDL_GetError());
    }     // Rendering //
    //SDL_SetRenderDrawColor(appState.renderer,0,255,0,255);
    DrawMapGrid(appState);
    DrawMap(appState);
    RenderItemSelector(appState);
    RenderItemSlots(appState,appState.inventory);
    if (appState.inventory.open) RenderInventoryWindow(appState,appState.inventory);
    float drawX = 0.0f;
    float drawY = 0.0f;
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        drawX = appState.entities[i].x;
        drawY = appState.entities[i].y;

        switch (appState.entities[i].entityType)
        {
        case ENTITY_PLAYER:
            WorldToScreen(appState.camera,appState.entities[i].x,appState.entities[i].y, drawX,drawY);
            RenderEntity(appState.entities[i],*appState.renderer,drawX,drawY);
            break;
        
        default:
            break;
        }
        break;
    }

    // Rendering //
    SDL_RenderPresent(appState.renderer); // Draws
}