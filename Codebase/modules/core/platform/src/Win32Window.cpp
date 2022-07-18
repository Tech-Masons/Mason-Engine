#include "Win32Window.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	Win32Window* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (window != NULL)
	{
		Keyboard* kbd = reinterpret_cast<Keyboard*>(window->GetSystem(InputType::Keyboard));
		Mouse* mouse = reinterpret_cast<Mouse*>(window->GetSystem(InputType::Mouse));
		
		switch (msg)
		{
			if (kbd)
			{
				// default keypress
				case WM_KEYDOWN:
					kbd->OnKeyDown((KeyCode)w);
					break;
				case WM_KEYUP:
					kbd->OnKeyUp((KeyCode)w);
					break;
			}

			if (mouse)
			{
				case WM_MOUSEMOVE:
					mouse->OnMouseMove(GET_X_LPARAM(l), GET_Y_LPARAM(l));
					break;

				case WM_RBUTTONDOWN:
					mouse->OnMouseButtonDown(MouseButton::RMB);
					SetCapture((HWND)window->GetHandle());
					break;

				case WM_RBUTTONUP:
					mouse->OnMouseButtonUp(MouseButton::RMB);
					ReleaseCapture();
					break;
			}
		}
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, msg, w, l);
}

Win32Window::Win32Window(std::string title, uint32_t width, uint32_t height)
{
	this->title = title;
	this->width = width;
	this->height = height;
	this->fullscreen = false;
	this->instance = GetModuleHandle(NULL);
}

Win32Window::~Win32Window()
{
	Close();
}

void Win32Window::Open()
{
	const LPCWSTR className = L"MasonEngineClass";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.lpszClassName = className;
	wc.cbClsExtra = 0;
	wc.hInstance = instance;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = MessageHandler;
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wc);

	DWORD styles = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	handle = CreateWindowEx(0, className, L"", styles, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, instance, this);
	ShowWindow(handle, SW_NORMAL);
	this->running = true;
	SetTitle(title);

	SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void Win32Window::Close()
{
	DestroyWindow(handle);
}

void Win32Window::Poll()
{
	MSG msg = { };
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			running = false;
	}
}

void Win32Window::SetDimensions(uint32_t newWidth, uint32_t newHeight)
{
	this->width = newWidth;
	this->height = newHeight;
	SetWindowPos(handle, NULL, 0, 0, newWidth, newHeight, SWP_NOREPOSITION | SWP_SHOWWINDOW);
}

void Win32Window::SetTitle(std::string newTitle)
{
	this->title = newTitle;
	SetWindowTextA(handle, newTitle.c_str());
}

void* Win32Window::GetHandle()
{
	return (void*)handle;
}