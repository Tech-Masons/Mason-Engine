#pragma once
#include "iInputSystem.h"

class iGameWindow
{
protected:
	std::string title;
	uint32_t width, height;
	bool running, fullscreen;
	std::map<InputType, iInputSystem*> systems;
public:
	virtual ~iGameWindow() { }

	virtual void Open() = 0;
	virtual void Close() = 0;
	virtual void Poll() = 0;
	virtual void SetDimensions(uint32_t newWidth, uint32_t newHeight) = 0;
	virtual void SetTitle(std::string newTitle) = 0;
	virtual void* GetHandle() = 0;

	uint32_t GetWidth() const { return width; }
	uint32_t GetHeight() const { return height; }
	bool IsRunning() const { return running; }
	bool IsFullscreen() const { return fullscreen; }
	std::string GetTitle() const { return title; }
	void AttachInputSystems(std::map<InputType, iInputSystem*>& systems) { this->systems = systems; }
	iInputSystem* GetSystem(InputType type) { return systems[type]; }
};