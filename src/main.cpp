#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include "helper.hpp"
#include "animationProcess.hpp"
#include "map.hpp"

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
    if (InitMap(state->map,*state) != 0)
    {
        SDL_Log("Could not init map: %s",SDL_GetError());
        return SDL_APP_FAILURE;
    } 
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
        UpdateEntityState(state->entities[i]);
        SetAnimation(state->entities[i],*state);
        UpdateEntityPosition(state->entities[i]);
        UpdateEntityAnimation(state->entities[state->playerIndex],*state);
        
    }
    UpdateCameraPosition(state->entities[state->playerIndex],state->camera);
    Render(*state);
    UpdateSelectedItem(*state);
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
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        state->mouseButton = 0;
        break;
    case SDL_EVENT_WINDOW_RESIZED:
        break;
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
    RenderInventory(appState);
    RenderSelectedButton(appState);
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