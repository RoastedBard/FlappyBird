#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL.h>

#include "GameLoop.h"
#include "Constants.h"
#include "Tubes.h"
#include "CollisionDetection.h"

#define TUBES_ARRAY_SIZE  ((levelWidth / (tubeWidth + distanceBetweenTubes)) + 1)

static SDL_Window   *_mainWindow = NULL;
static SDL_Renderer *_renderer = NULL;

static int           _isRunning = 0;

static SDL_Rect      _player;
static SDL_Rect      _tubesRect;
static SDL_Rect      _floorRect;

static float         _vertSpeed       =  0.0f;
static const float   _jumpSpeed       = -3.5f;
static float         _fallingConstant =  0.0f;

static float         _tubesMovingSpeed = 0.0f;

static tubes_t      *_tubesArray;

static int           _gameState = STATE_START;

void Cleanup()
{
	free(_tubesArray);
	SDL_DestroyWindow(_mainWindow);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}

int Init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("%s\n", ">SDL init success");

		_mainWindow = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, levelWidth, levelHeight, SDL_WINDOW_SHOWN);

		if(_mainWindow != NULL)
		{
			printf("%s\n", ">window creation success");

			_renderer = SDL_CreateRenderer(_mainWindow, -1, 0);

			if(_renderer != NULL)
			{
				printf("%s\n", ">renderer creation success");
			}
			else
			{
				printf("%s\n", ">renderer creation failed");
				return 1;	
			}
		}
		else
		{
			printf("%s\n", ">window creation failed");
			return 1;
		}
	}
	else
	{
		printf("%s\n", ">SDL init failed");
		return 1;	
	}

	_tubesArray = malloc(TUBES_ARRAY_SIZE * sizeof(tubes_t));

    _floorRect.x = 0;
    _floorRect.y = levelHeight - floorHight;
    _floorRect.w = levelWidth;
    _floorRect.h = floorHight;

    InitGame();

	_isRunning = 1;

    printf("%s\n", ">init success");

	return 0;
}

void InitGame()
{
    int i = 0;

    srand(time(0));

    TubesSetCoords(&_tubesArray[0], levelWidth / 2, 0, levelWidth / 2, 0);
    TubesSetCoords(&_tubesArray[TUBES_ARRAY_SIZE - 1], (TUBES_ARRAY_SIZE - 1) * (tubeWidth + distanceBetweenTubes), 0, (TUBES_ARRAY_SIZE - 1) * (tubeWidth + distanceBetweenTubes), 0);

    for(i = 0; i < TUBES_ARRAY_SIZE ; ++i)
    {
        if(i == 0)
            TubesGenerate(&_tubesArray[i], _tubesArray[TUBES_ARRAY_SIZE - 1].topTubeCoords.x);
        else
            TubesGenerate(&_tubesArray[i], _tubesArray[i - 1].topTubeCoords.x);
    }

    _player.x = levelWidth / 2;
    _player.y = levelHeight / 2;
    _player.w = 20;
    _player.h = 20;

    _vertSpeed = 0.0;
    _tubesMovingSpeed = 1.0f;
    _fallingConstant = 10.0f;

    _gameState = STATE_START;
}

void StartLoop()
{
    int simulaionTime = 0;
    
    Init();
    
    while(_isRunning)
    {
        int realTime = SDL_GetTicks();

        while(simulaionTime < realTime)
        {
            simulaionTime += 16;

            switch(_gameState)
            {
                case STATE_START:
                    StartStateExec(16.0f / 1000.0f);
                    break;

                case STATE_PLAYING:
                    PlayingStateExec(16.0f / 1000.0f);
                    break;

                case STATE_GAMEOVER:
                    GameoverStateExec(16.0f / 1000.0f);
                    break;
            }
        }

        Render();
    }

    Cleanup();
}

void Render()
{
    switch(_gameState)
    {
        case STATE_START:
            RenderStartState();
            break;

        case STATE_PLAYING:
            RenderPlayingState();
            break;

        case STATE_GAMEOVER:
            RenderGameOverState();
            break;
    }
}

void StartStateExec(float deltaTime)
{
    HandleInputStartState();
    UpdateStartState(deltaTime);
}

void PlayingStateExec(float deltaTime)
{
    HandleInputPlayingState();
    UpdatePlayingState(deltaTime);
}

void GameoverStateExec(float deltaTime)
{
    HandleInputGameOverState();
    UpdateGameOverState(deltaTime);
}

/*
 =========================
 Handling input for states
 =========================
 */
void HandleInputStartState()
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                _isRunning = 0;
                break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        _isRunning = 0;
                        break;

                    case SDLK_SPACE:
                        _gameState = STATE_PLAYING;
                        break;

                }
                break;
        }
    }
}

void HandleInputPlayingState()
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                _isRunning = 0;
                break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        _isRunning = 0;
                        break;

                    case SDLK_SPACE:
                        _vertSpeed = _jumpSpeed;
                        break;

                    case SDLK_UP:
                        _vertSpeed = _jumpSpeed;
                        break;
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(e.button.button == SDL_BUTTON_LEFT)
                    _vertSpeed = _jumpSpeed;
                break;
        }
    }
}

void HandleInputGameOverState()
{
    SDL_Event e;

    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                _isRunning = 0;
                break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        _isRunning = 0;
                        break;

                    case SDLK_SPACE:
                        _gameState = STATE_START;
                        InitGame();
                        break;

                }
                break;
        }
    }
}

/*
=========================
   Updating game states
=========================
*/
void UpdateStartState(float deltaTime)
{
    if(_player.y >= (levelHeight / 3))
        _vertSpeed -= (_fallingConstant / 2) * deltaTime;
    if(_player.y <= (levelHeight / 4))
        _vertSpeed += (_fallingConstant / 2) * deltaTime;

    _player.y += _vertSpeed;
}

void UpdatePlayingState(float deltaTime)
{
    int i = 0;

    _player.y += _vertSpeed;
    _vertSpeed += _fallingConstant * deltaTime;

    if(_player.y + _player.h >= levelHeight - floorHight)
        _player.y = (levelHeight - floorHight) - _player.h;

    for(i = 0; i < TUBES_ARRAY_SIZE; ++i)
    {
        if(doRectanglesCollide(_player.x, _player.y, _player.w, _player.h, _tubesArray[i].topTubeCoords.x, _tubesArray[i].topTubeCoords.y,
                _tubesArray[i].topTubeCoords.x + tubeWidth,
                _tubesArray[i].topTubeCoords.y + _tubesArray[i].topTubeHeight) ||
                doRectanglesCollide(_player.x, _player.y, _player.w, _player.h, _tubesArray[i].bottomTubeCoords.x, _tubesArray[i].bottomTubeCoords.y,
                _tubesArray[i].bottomTubeCoords.x + tubeWidth,
                _tubesArray[i].bottomTubeCoords.y + _tubesArray[i].topTubeHeight) ||
                doRectanglesCollide(_player.x, _player.y, _player.w, _player.h, _floorRect.x, _floorRect.y, _floorRect.x + _floorRect.w, _floorRect.y + _floorRect.h))
        {
            _gameState = STATE_GAMEOVER;
        }
    }

    for(i = 0; i < TUBES_ARRAY_SIZE; ++i)
    {
        /* For some reason when multiplying _tubesMovingSpeed by deltaTime, tubes stops at 0 xCoord */
        _tubesArray[i].topTubeCoords.x -= _tubesMovingSpeed;
        _tubesArray[i].bottomTubeCoords.x -= _tubesMovingSpeed;

        if((_tubesArray[i].topTubeCoords.x + tubeWidth) <= 0) /* You can check only one of the tubes here cause they moving synchronically */
        {
            if(i == 0)
                TubesGenerate(&_tubesArray[i], _tubesArray[TUBES_ARRAY_SIZE - 1].topTubeCoords.x);
            else
                TubesGenerate(&_tubesArray[i], _tubesArray[i - 1].topTubeCoords.x);
        }
    }
}

void UpdateGameOverState(float deltaTime)
{
    _player.y += _vertSpeed;
    _vertSpeed += _fallingConstant * deltaTime;

    if(_player.y + _player.h >= levelHeight - floorHight)
        _player.y = (levelHeight - floorHight) - _player.h;
}

/*
=========================
  Rendering game states
=========================
*/
void RenderStartState()
{
    /* Set background color */
    SDL_SetRenderDrawColor(_renderer, 30, 144, 255, 255);

    SDL_RenderClear(_renderer);

    /* Set color of the floor */
    SDL_SetRenderDrawColor(_renderer, 34, 139, 34, 255);
    /* Draw the floor */
    SDL_RenderFillRect(_renderer, &_floorRect);

    /* Set color of the _player */
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    /* Draw the _player */
    SDL_RenderFillRect(_renderer, &_player);

    SDL_RenderPresent(_renderer);
}

void RenderPlayingState()
{
    int i = 0;

    /* Set background color */
    SDL_SetRenderDrawColor(_renderer, 30, 144, 255, 255);

    SDL_RenderClear(_renderer);

    /* Set color of the tubes */
    SDL_SetRenderDrawColor(_renderer, 50, 205, 50, 255);

    /* Draw tubes */
    for(i = 0; i < TUBES_ARRAY_SIZE ; ++i)
    {
        /* Top tube */
        _tubesRect.x = _tubesArray[i].topTubeCoords.x;
        _tubesRect.y = _tubesArray[i].topTubeCoords.y;
        _tubesRect.w = tubeWidth;
        _tubesRect.h = _tubesArray[i].topTubeHeight;

        SDL_RenderFillRect(_renderer, &_tubesRect);

        /* Bottom tube */
        _tubesRect.x = _tubesArray[i].bottomTubeCoords.x;
        _tubesRect.y = _tubesArray[i].bottomTubeCoords.y;
        _tubesRect.w = tubeWidth;
        _tubesRect.h = _tubesArray[i].bottomTubeHeight;

        SDL_RenderFillRect(_renderer, &_tubesRect);
    }

    /* Set color of the floor */
    SDL_SetRenderDrawColor(_renderer, 34, 139, 34, 255);
    /* Draw the floor */
    SDL_RenderFillRect(_renderer, &_floorRect);

    /* Set color of the _player */
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
    /* Draw the _player */
    SDL_RenderFillRect(_renderer, &_player);

    SDL_RenderPresent(_renderer);
}

void RenderGameOverState()
{
    /* Render All the same */
    RenderPlayingState();
}
