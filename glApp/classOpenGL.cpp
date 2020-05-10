#include "classOpenGL.h"

bool classOpenGL::open(HWND _hWnd)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormat;
	hRC = NULL;
	// the handle to the device context
	hDC = GetDC(_hWnd);
	if (!hDC) goto failure;
	// the pixel format descriptor
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	// set the pixel format
	iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (!iPixelFormat) goto failure;
	if (!DescribePixelFormat(hDC, iPixelFormat, sizeof(pfd), &pfd)) goto failure;
	if (!SetPixelFormat(hDC, iPixelFormat, &pfd)) goto failure;
	// handle to the render context
	hRC = wglCreateContext(hDC);
	if (!hRC) goto failure;
	if (!wglMakeCurrent(hDC, hRC)) goto failure;
	// success
	hWnd = _hWnd;
	errorCode = 0;
	isActive = true; // OpenGL is active on this window
	return true;
failure:
	errorCode = GetLastError();
	close();
	return false;
}

void classOpenGL::close()
{
	if (hRC) {
		wglMakeCurrent(NULL, NULL); // this also releases the DC (of this thread's RC)
		wglDeleteContext(hRC);
	}
	hWnd = NULL; // invalidate handles
	hDC = NULL;
	hRC = NULL;
	isActive = false; // OpenGL is not active on this window
}

bool classOpenGL::swap()
{
	if (!isActive) return false;
	return SwapBuffers(hDC);
}

void classOpenGL::init()
{
	if (!isActive) return;
	// Z-Buffer
	glDepthFunc(GL_LESS);
	glDepthRange(0.0, 1.0);
	glDepthMask(GL_TRUE); // depth-buffer is writable
	glEnable(GL_DEPTH_TEST);
	// culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	// form background
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonOffset(1.0, 1.0);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glClearDepth(1.0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDrawBuffer(GL_FRONT_AND_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /* | GL_STENCIL_BUFFER_BIT */);
	glDrawBuffer(GL_BACK);
	// render-performance
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	// depricated
//	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//	glHint(GL_FOG_HINT, GL_NICEST);
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//	glDisable(GL_NORMALIZE);
//	glShadeModel(GL_SMOOTH);

	swap(); // show it..
}

void classOpenGL::cls(GLfloat R, GLfloat G, GLfloat B, GLfloat A)
{
	glDrawBuffer(GL_BACK);
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	swap();
}
