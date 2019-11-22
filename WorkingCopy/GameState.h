#pragma once
#include <string>
#include "DXCore.h"
#include <memory>
#include <unordered_map>
class GameState
{
	std::wstring name; // name of menu

protected:
	std::unique_ptr<DXCore> const directXApp; // pointer to main APP Class
	bool isPaused; // true if scene is paused

	GameState(std::unique_ptr<DXCore> const app, std::wstring& name);

public:
	virtual ~GameState();
	// delete copy and assignment operators
	GameState(GameState const&) = delete;
	GameState& operator = (GameState const&) = delete;

	// initialization
	virtual void initialize() = 0;
	virtual void shutdown() = 0;

	// pause and resume
	virtual void pause() = 0;
	virtual void resume() = 0;

	// user input
	//virtual bool handleInput(std::unordered_map<input::GameCommands, input::GameCommand&>& activeKeyMap) = 0;	// returns false if the observer stack of the input handler was changed
	virtual void update(const double deltaTime) = 0;

	// render the scene
	virtual void render(const double farSeer) = 0;

	// get name
	std::wstring& getStateName() { return name; };
};


