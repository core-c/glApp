#pragma once
#pragma comment(lib,"opengl32.lib")

#include <windows.h>
//#include <gl/gl.h>		// glad takes care of this..
#include <glad/glad.h>
//#include "gl/glext.h"
//#include "gl/wglext.h"
//#include <gl/glu.h>

class classOpenGL
{
private:
	HWND hWnd;				// this value is valid (!= 0) when OpenGL is opened successfully
	HDC hDC;				// the handle to the Device Context
	HGLRC hRC;				// the handle to the OpenGL Render Context
	bool isActive = false;	// true if OpenGL is active in the window
public:
	DWORD errorCode = 0;	// the last error code (0 == no error)
	bool open(HWND _hWnd);	// Enable OpenGL in the window
	void close();			// Disable OpenGL in the window
	void init();			// Initialize the OpenGL settings, and clear the RC
	bool swap();			// swap the FRONT & BACK buffers
};

