#pragma once


enum class KeyState { StillReleased, JustPressed, StillPressed, JustReleased };		// enumeration of all possible key states

/*
Combines a keycode to a keystate
*/
struct BindInfo
{
private:
	unsigned int keyCode;
	KeyState keyState;
public:
	BindInfo();
	BindInfo(const unsigned int keyCode, const KeyState keyState);

};

class InputHandler
{
};

