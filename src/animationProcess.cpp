#include "animationProcess.hpp"
#include "map.hpp"

void InitAnimations(AppState& appState)
{
    for(int i = 0; i < MAX_ENTITIES; i++)
    {
        switch (appState.entities[i].entityType)
        {
        case ENTITY_PLAYER:
            InitPlayerAnimations(appState.characterAnimation,*appState.graphics->playerTexture);
            break;
        
        default:
            break;
        }
    }
    return;
}

void InitPlayerAnimations(std::unique_ptr<Animation>& animation,SDL_Texture& texture)
{
    
    float textureHegiht = 0.0f;
    float textureWidth = 0.0f;
    SDL_GetTextureSize(&texture,&textureWidth,&textureHegiht);
    animation->frameNumer = FRAMES_IN_PLAYER_TEXTURE;
    animation->currentFrame = 0;
    animation->frameHeight = textureHegiht/ROWS_IN_PLAYER_TEXTURE;
    animation->frameWidth = textureWidth/FRAMES_IN_PLAYER_TEXTURE;
    animation->frameSpeed = 10;
    animation->frameRect = {
                        0,
                        animation->frameHeight*animation->row,
                        animation->frameWidth,
                        animation->frameHeight
                    }; 
    animation->state = ANIMATION_STATE_IDLE;

    return;
}

void UpdateEntityAnimation(Entity& entity, AppState& appState)
{
    if (entity.entityType == ENTITY_NONE) return;
    entity.currentAnimation->frameCount ++; // increases every frame - So I need to a set a framerate naturally
    if (entity.currentAnimation->frameCount >= ANIMATION_WAIT_VALUE/entity.currentAnimation->frameSpeed)
    {
        entity.currentAnimation->currentFrame ++;
        entity.currentAnimation->frameCount = 0;

        if (entity.currentAnimation->currentFrame >= entity.currentAnimation->frameNumer) // Animation End
        {
            entity.currentAnimation->currentFrame = 0;
            if (entity.entityType == ENTITY_PLAYER) PlayerAnimationEnd(entity,appState);
        }
        entity.currentAnimation->frameRect.x = entity.currentAnimation->currentFrame*entity.currentAnimation->frameWidth;
    }
}

void SetAnimation(Entity& entity,AppState& appState)
{
    switch (entity.entityType)
    {
    case ENTITY_PLAYER:
        SetPlayerAnimation(entity,appState);
        break;
    
    default:
        break;
    }
}

void SetPlayerAnimation(Entity& player, AppState& appState)
{
    CHARACTER_ANIMATION_ROW currentRow = player.characterAnimationRow;
    player.characterAnimationRow = static_cast<CHARACTER_ANIMATION_ROW>(
        static_cast<int>(player.entityState) * CHARACTER_DIRECTIONS +
        static_cast<int>(player.playerDirection)  
    );
    appState.characterAnimation->row = player.characterAnimationRow;
    if (currentRow != appState.characterAnimation->row) appState.characterAnimation->currentFrame = 0;  // Resets the player animation if it is different
}
// 
void PlayerAnimationEnd(Entity& player, AppState& appState)
{
    if (player.entityState == ENTITY_STATE_IDLE || player.entityState == ENTITY_STATE_WALKING || player.entityState == ENTITY_STATE_IDLE) return;
    UpdateTile(*appState.map,player,appState);
}

void InitAnimalAnimations(AnimalAnimations& animations, ENTITY_TYPE animalType, SDL_Texture& texture)
{
    switch (animalType)
    {
    case ENTITY_COW:
        InitCowAnimations(animations,texture);
        break;
    
    default:
        break;
    }
}
// Only the row and the count would be different for each animation ---- this requires rethink --- Could probably do it similarly to how the player animation processing is handled
void InitCowAnimations(AnimalAnimations& animations, SDL_Texture& texture)
{
    float textureHeight = 0.0f;
    float textureWidht = 0.0f;
    SDL_GetTextureSize(&texture,&textureWidht,&textureHeight);
    animations.idle.frameCount = COW_IDLE_FRAMES;
    animations.idle.row = 1;
    animations.idle.frameHeight = textureHeight/COW_TEXTURE_ROWS;
    animations.idle.frameWidth = textureWidht/COW_TEXTURE_ROWS;

}