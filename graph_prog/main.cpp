// Lab3 - 30/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Main source

// Includes
#include <stdio.h>
#include <Windows.h>
#include "input.h"
#include "scene.h"
#include "timerclass.h"
#include "app_globals.h"


// Handle to window
HWND hwnd;

// Prototypes
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void RegisterMyWindow(HINSTANCE);
BOOL InitialiseMyWindow(HINSTANCE, int, int x, int y, int w, int h);
int WINAPI WinMain (HINSTANCE, HINSTANCE, PSTR, int);


using namespace winapp;

// Input manager
Input input;

// Scene 
Scene scene;

// Timer class
TimerClass timer;






// Entry point. The program start here
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int nCmdShow) {
	// Create a message handler
    MSG         msg;	

	// Define the behaviour and characteristics of the window to be created
	RegisterMyWindow(hInstance);

	// Initialise window and define GRAPHICAL style
   	if (!InitialiseMyWindow(hInstance, nCmdShow, 200, 200, 800, 600)) {
		return FALSE;
	}

	// Initialise the scene
	scene.initialise(&hwnd, &input);

	// Initialise the timer
	timer.Initialize();

	// Control wether the game is running
	bool gameIsRunning = true;

	// Control game speed 
	DWORD next_game_tick = GetTickCount();
    int loops;
    float interp;

	// Main loop
	while (gameIsRunning) {							
		// Fetch messages from the Win32 queue
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			// If the cmd is to quit
		    if (msg.message == WM_QUIT) {
				// Exit the main loop
				break;
			}

			// Translate it into character message
		    TranslateMessage (&msg);				
			// Dispatch the message to the relative window
			DispatchMessage (&msg);
		}
		// Execute game logic
		else {		
			if(input.isKeyDown(VK_ESCAPE)) {
				// Quit application
				SendMessage(hwnd, WM_CLOSE, 0, 0);

				// Set key up so that it doesn't get continuously detected
				input.setKeyUp(VK_ESCAPE);
			}

			// Process input first in the game loop
			scene.procInput();

			loops = 0;

			// Current game milliseconds
			DWORD curr_game_tick = GetTickCount();

			while(curr_game_tick > next_game_tick && loops < MAX_FRAMESKIP) {
				scene.update();

				next_game_tick += SKIP_TICKS;
				++loops;

				curr_game_tick = GetTickCount();
			}

			// Interpolate between update frames
			interp = float(GetTickCount() + SKIP_TICKS - next_game_tick )
                        / float( SKIP_TICKS );

			// Render scene
			scene.render(interp);
		}
    }

	return msg.wParam ;										
}

// Handles window messages			
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Depending on the message received
    switch (message){
	case WM_CLOSE: {
			// Ask user if he wants to proceed in closing the window	
			int msgBoxResult = MessageBox(hwnd,											// Handle to parent window
										(LPCSTR)"Are you sure you want to quit?",		// Text
										(LPCSTR)"Take your final, ultimate decision.",	// Caption
										MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2);		// Aspect and behaviour

			// Handle user decision
			switch(msgBoxResult) {
				case IDYES: {
					DestroyWindow(hwnd);
					break;
				}
				case IDNO: {
					return true;
					break;
				}
				
			}

			break;
		}
		case WM_KEYDOWN: {
			input.setKeyDown(wParam);			// Pass key

			break;
		}
		case WM_KEYUP: {
			input.setKeyUp(wParam);				// Pass key

			break;
		}
		case WM_MOUSEMOVE: {
			input.setMouseX(LOWORD (lParam));	// Pass X position
			input.setMouseY(HIWORD (lParam));	// Pass Y position

			break;
		}
		case WM_CREATE:											
			break;

		case WM_SIZE:
			
			break;	
		

		case WM_PAINT:
		    break;		

		case WM_DESTROY:	
			PostQuitMessage(0);	
			break;				
	}													

	return DefWindowProc (hwnd, message, wParam, lParam);										
}

// Defines the window BEFORE CREATION. It basically defines how the window will
// look like
void RegisterMyWindow(HINSTANCE hInstance) {
    WNDCLASSEX  wcex;									// Class information

    wcex.cbSize        = sizeof (wcex);					// Size in bytes of structure
    wcex.style         = CS_HREDRAW | CS_VREDRAW;		// Class style (how the window behaves)
    wcex.lpfnWndProc   = WndProc;						// Pointer to window procedure 
														// (Observer pattern)
    wcex.cbClsExtra    = 0;								// Extra bytes to allocate following
														// the window-class STRUCTURE
    wcex.cbWndExtra    = 0;								// Extra bytes to allocate following
														// the window-class INSTANCE
    wcex.hInstance     = hInstance;						// Handle to instance that contains Window Proc
    wcex.hIcon         = 0;								// Handle to default icon; 0 == DEFAULT
    wcex.hCursor       = LoadCursor (NULL, IDC_ARROW);													
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);
    wcex.lpszMenuName  = NULL;							
    wcex.lpszClassName = reinterpret_cast<LPCSTR>("MyWindowClass");				
    wcex.hIconSm       = 0; 

	RegisterClassEx (&wcex);							
}

// Attempts to create the window and display it
BOOL InitialiseMyWindow(HINSTANCE hInstance, int nCmdShow, int x, int y, int w, int h) {
  
   hwnd = CreateWindow (reinterpret_cast<LPCSTR>("MyWindowClass"),					// Classname as defined in RegClassEX
						 reinterpret_cast<LPCSTR>("My Window App"),		  			// Display name of the window
						 WS_SYSMENU | WS_THICKFRAME,	
						 x,									// xPos
						 y,									// yPos
						 w,									// Widht
						 h,									// Height
						 NULL,								// Parent window
						 NULL,								// Handle to Windows Menu
						 hInstance,							// Handle to instance associated with
															// Window
						 NULL);								// Pointer to value to pass to window
	
   // If the window didn't get created
	if (!hwnd) {
		// Exit the program
		return FALSE;
	}
	
	// Set the specified window's show state (by using nCmdShow
    ShowWindow (hwnd, nCmdShow);		

	// Update the window by sending a WM_PAINT message to it
    UpdateWindow (hwnd);			

	// Exit the function successfully
	return TRUE;
}


// EO Namespace