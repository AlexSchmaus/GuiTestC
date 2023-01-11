#include <windows.h>
#include <tchar.h>		// abstraction that handles unicode.... I think. It defines the TCHAR macro, _T()

// Alex Schmaus 1/1/2023
// follow along from this tutorial: https://learn.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-170


// Window Procedure Function - handles messages the app recieves from Windows when events occur
LRESULT CALLBACK WndProc(
	_In_	HWND	hWnd,						// a handle to the window
	_In_	UINT	message,					// the message sent. Messages >> https://learn.microsoft.com/en-us/windows/win32/winmsg/about-messages-and-message-queues#system-defined-messages
	_In_	WPARAM	wParam,						// Additional info in the message, defined by the param: message
	_In_	LPARAM	lParam						// Additional info in the message, defined by the param: message
) {
	PAINTSTRUCT ps;
	HDC hdc;			// a handle to the context of the device - like, where to know where the window goes (screens, but we have to tell it that, basically)
	TCHAR greeting[] = _T("Hello World!");

	switch (message) {
	// WM_PAINT is occurs when a displayed window must be updated. This can occur when the user moves a window over our.
	// Also, when the window is first displayed...... LOL
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Lay out window text, buttons, etc.
		TextOut(hdc, 5, 5, greeting, _tcslen(greeting));   // For now, just put text in the top left

		// end app layout

		EndPaint(hWnd, &ps);
		break;
	// happens when the window is closed.
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	// happens when a window is first created
	//case WM_CREATE:
	//	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}




// main function / entry point -> _In_ is SAL Annotation https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2012/hh916383(v=vs.110)?redirectedfrom=MSDN
int WINAPI WinMain(
	_In_		HINSTANCE	hInstance,			// a handle to the current instance of the app
	_In_opt_	HINSTANCE	hPrevInstance,		// a handle to the previous instance. Always NULL. Can be helpful for limiting the app to one instance only
	_In_		LPSTR		lppCmdLine,			// the command line for the application..... wut?
	_In_		int			nCmdShow			// controls how the window is shown. Hidden, Minimzed, etc. https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
) {

	//These are arrays of TCHAR
	static TCHAR szWindowClass[] = _T("DesktopApp");
	static TCHAR szTitle[] = _T("Windows Tutorial");



	// Structure of type WNDCLASSEX that gives info about the window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);							//Size, in bytes of this structure
	wcex.style = CS_HREDRAW | CS_VREDRAW;						//The class styles, combination of these https://learn.microsoft.com/en-us/windows/win32/winmsg/about-window-classes - CS_HREDRAW | CS_VREDRAW are the styles of "redraw window on Horizontal resize OR redraw window on Verticle resize"
	wcex.lpfnWndProc = WndProc;									//Pointer to the window procedure function (below). 
	wcex.cbClsExtra = 0;										//Number of extra bytes of allocate, after this structure
	wcex.cbWndExtra = 0;										//Number of extra bytes to allocate, after the window instance. if application uses WNDCLASSEX for a dialog box, this should be = DLGWINDOWEXTRA
	wcex.hInstance = hInstance;									//A handle to the applcation instnace that contains the window procedure for the class
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);		//A handle to the class icon. Must be an icon. NULL = system default
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//A handle to the class cursor. Must be a cursor. NULL is NOT equal to default
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			//background brush. COLOR_WINDOW is a system color. System colors must have 1 added to them, and be converted to a HBRUSH, since that's the type of this  member
	wcex.lpszMenuName = NULL;									//Null termincated character string that defines the resource of the class menu. If NULL, windows belonging tot his class have no default menu
	wcex.lpszClassName = szWindowClass;							//null terminated string OR atom. String specifies window class name. (this one is confusing, given the value the tutrioal provided)
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);	//handle to a small icon, assocaite with this window class. 

	// Now, take this and register it with Windows, so the OS knows about the window and how to send messages to it. 
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("RegisterClassEx failed!"), _T("Tutorial"), NULL);	// _T() is a macro, related to the TCHAR data type. Basically, it seems to be converting the string into the "Correct" type of character, or the needed type of char
		return 1;
	}

	// Next, create a window.
	HWND hWnd = CreateWindowEx(		// returns an object of the type HWND, which is a handle to a window. A handle is somewhat like a pointer that windows uses to keep track of open windows
		WS_EX_OVERLAPPEDWINDOW,		// Another window style...
		szWindowClass,				// Name of the app, as defined above
		szTitle,					// title bar text
		WS_OVERLAPPEDWINDOW,		// type of window to create
		CW_USEDEFAULT,				// initial X position
		CW_USEDEFAULT,				// inital Y position,
		500,						// initial width
		100,						// initial length (do they mean height?)
		NULL,						// this window's parent
		NULL,						// menu bar? I think....
		hInstance,					// the instance of this app, as defined in WinMain
		NULL						// ??????????
	);

	if (!hWnd) {
		MessageBox(NULL, _T("call to CreateWindowEx failed"), _T("Tutorial"), NULL);
		return 1;
	}

	// Make the window we just created visable
	ShowWindow(
		hWnd,		// handle to the window we just created
		nCmdShow	// fourth param in WinMain, which controls the state of the window
	);
	UpdateWindow(
		hWnd
	);

	// Handle messages. Loop. Listen for messages that Windows OS sends. Then send the message to WndProc function
	// &msg is the memory address of msg
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);		//
		DispatchMessage(&msg);		//this sends it to the application's Window Procedure Function
	}

	return (int)msg.wParam;
}






