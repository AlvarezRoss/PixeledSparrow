#pragma once
#include "helper.hpp"

// Animation struct is in helper.hpp
#define ROWS_IN_PLAYER_TEXTURE 24
#define FRAMES_IN_PLAYER_TEXTURE 8
#define ANIMATION_WAIT_VALUE 60
#define CHARACTER_DIRECTIONS 4

void InitAnimations(AppState& appState);
void InitPlayerAnimations(std::unique_ptr<Animation>& animation, SDL_Texture& texture);
void AnimationHandler(AppState& appState);
void UpdateEntityAnimation(Entity& entity, AppState& appState);
void SetAnimation(Entity& entity,AppState& appState);
void SetPlayerAnimation(Entity& player, AppState& appState);
void PlayerAnimationEnd(Entity& player, AppState& appState);
void SetIdleAnimation(Entity&player, AppState& appState);
void SetWalkAnimation(Entity&player, AppState& appState);