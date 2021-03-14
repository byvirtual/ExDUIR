#pragma once
#include "help_ex.h"

#define PBM_POPUP 10020
#define PBM_ON_POPUP 10010
#define PBM_ON_POPWND_MSG 10011
#define PBM_ON_CLOSE 10012

struct cbi_s
{
	void* wzItem_;
	int nItem_;
};

size_t _pbm_proc(HWND hWnd, EXHANDLE hExDUI, UINT uMsg, size_t wParam, size_t lParam, void* lpResult);
size_t _pb_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
size_t _cb_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj);
void _cb_arr_del(array_s* hArr, int nIndex, void* pvData, int nType);
void _cb_paint(EXHANDLE hObj, obj_s* pObj);
int _cblv_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, int* lpResult);
