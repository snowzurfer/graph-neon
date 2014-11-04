// Lab2 - 16/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Definition of functions

// Includes
#include "window.h"




// Defines the window BEFORE CREATION. It basically defines how the window will
// look like
/*void RegisterMyWindow(HINSTANCE hInstance) {
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
    wcex.lpszClassName = "MyWindowClass";				
    wcex.hIconSm       = 0; 

	RegisterClassEx (&wcex);							
}

// Attempts to create the window and display it
BOOL InitialiseMyWindow(HINSTANCE hInstance, int nCmdShow, int x, int y, int w, int h) {
  
   hwnd = CreateWindow ("MyWindowClass",					// Classname as defined in RegClassEX
						 "My Window App",		  			// Display name of the window
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
}*/


// EO Namespace