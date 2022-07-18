#pragma once
#include "PlatformDefines.h"
#include "iGameWindow.h"

class Win32Window : public iGameWindow
{
private:
	HWND handle;
	HINSTANCE instance;
public:
	Win32Window(std::string title, uint32_t width, uint32_t height);
	~Win32Window();

	virtual void Open() override;
	virtual void Close() override;
	virtual void Poll() override;
	virtual void SetDimensions(uint32_t newWidth, uint32_t newHeight) override;
	virtual void SetTitle(std::string newTitle) override;
	virtual void* GetHandle() override;
};