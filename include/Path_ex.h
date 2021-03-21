#pragma once
#include "help_ex.h"


#define epf_bOpened 2147483648
#define EPF_DISABLESCALE 1

struct path_s
{
	UINT dwFlags_;
	ID2D1GeometrySink* pObj_;
	ID2D1PathGeometry* pGeometry_;
};

int _path_destroy(EXHANDLE hPath);
int _path_reset(EXHANDLE hPath);
int _path_create(int dwFlags, EXHANDLE* hPath);
int _path_getbounds(EXHANDLE hPath, void* lpBounds);
int _path_open(EXHANDLE hPath);
int _path_close(EXHANDLE hPath);
int _path_beginfigure(EXHANDLE hPath);
int _path_beginfigure2(EXHANDLE hPath, float x, float y);
int _path_endfigure(EXHANDLE hPath, bool fCloseFigure);
bool _path_hittest(EXHANDLE hPath, float x, float y);
int _path_addline(EXHANDLE hPath, float x1, float y1, float x2, float y2);
int _path_addarc(EXHANDLE hPath, float x1, float y1, float x2, float y2, float radiusX, float radiusY, bool fClockwise);
int _path_addrect(EXHANDLE hPath, float left, float top, float right, float bottom);
int _path_addroundedrect(EXHANDLE hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight);
