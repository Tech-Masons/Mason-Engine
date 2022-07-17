#include "window.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "windowsx.h"

Window::Window(const std::wstring& _title, int _width, int _height)
	:  iSurface(_title, _width, _height)
{
}

Window::~Window()
{
}

LRESULT Window::MessageHandler(UINT msg, WPARAM w, LPARAM l)
{
	Keyboard* kbd = reinterpret_cast<Keyboard*>(systems[InputType::Keyboard]);
	Mouse* mouse = reinterpret_cast<Mouse*>(systems[InputType::Mouse]);

	switch (msg)
	{
		if (kbd) {

			// default keypress
			case WM_KEYDOWN:
				kbd->OnKeyDown((KeyCode)w);
				break;

			case WM_KEYUP:
				kbd->OnKeyUp((KeyCode)w);
				break;
			
		}

		if (mouse) {
			
			case WM_MOUSEMOVE:
				
				mouse->OnMouseMove(GET_X_LPARAM(l), GET_Y_LPARAM(l));
				
				break;
			
			
			case WM_RBUTTONDOWN:
				mouse->OnMouseButtonDown(MouseButton::RMB);
				SetCapture(GetSurfacePointer());
				break;


			case WM_RBUTTONUP:
				mouse->OnMouseButtonUp(MouseButton::RMB);
				ReleaseCapture();
				break;


		}

	// Handle All Default Messages
	default:
		return iSurface::MessageHandler(msg, w, l);
	}
}

void Window::AttachInputSystems(std::map<InputType, iInputSystem*>& _systems)
{
	systems = _systems;
}