#ifndef GAMELOOP_H
#define GAMELOOP_H

enum state_t
{
    STATE_START = 0,
    STATE_PLAYING,
    STATE_GAMEOVER
};

void Cleanup();

int  Init();

void Render();

void StartLoop();

void InitGame();

void StartStateExec(float deltaTime);
void PlayingStateExec(float deltaTime);
void GameoverStateExec(float deltaTime);

void HandleInputStartState();
void HandleInputPlayingState();
void HandleInputGameOverState();

void UpdateStartState(float deltaTime);
void UpdatePlayingState(float deltaTime);
void UpdateGameOverState(float deltaTime);

void RenderStartState();
void RenderPlayingState();
void RenderGameOverState();

#endif