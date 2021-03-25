#include "help_ex.h"
#include "Class_MenuButton_ex.h"

size_t CALLBACK _menubutton_menu_proc(HWND hWnd, EXHANDLE hExDUI, UINT uMsg, size_t wParam, size_t lParam, void* lpResult)
{
	menu_s* lpMenuParams;
	wnd_s* pWnd;
	int nError = 0;
	POINT point = { 0 };
	obj_s* pObj;
	obj_s* pObj2;
	HWND currentWnd;
	EXHANDLE hObj = 0;

	if (uMsg == 485 && LODWORD(wParam) == -1)
	{
		if (_handle_validate(hExDUI, HT_DUI, (void**)&pWnd, &nError))
		{
			lpMenuParams = pWnd->lpMenuParams_;
			if (!lpMenuParams)
			{
				return 0;
			}
			GetCursorPos(&point);
			currentWnd = WindowFromPoint(point);
			if ((HWND)lpMenuParams->nReserved_ != currentWnd)
			{
				return 0;
			}
			ScreenToClient(currentWnd, &point);
			hObj = Ex_DUIGetObjFromPoint(currentWnd, point);
			if (!hObj || hObj == lpMenuParams->handle_)
			{
				return 0;
			}
			if (!_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
			{
				return 0;
			}
			if (pObj->pCls_->atomName_ != ATOM_MENUBUTTON)
			{
				return 0;
			}
			nError = 0;
			if (!_handle_validate(lpMenuParams->handle_, HT_OBJECT, (void**)&pObj2, &nError))
			{
				return 0;
			}
			if (pObj->objParent_ == pObj2->objParent_ && pObj->dwUserData_ == pObj2->dwUserData_)
			{
				EndMenu();
				_obj_postmessage(currentWnd, hObj, pObj, 123321, (size_t)pObj->dwUserData_, 0, 0);
			}
		}
	}
	return 0;
}

void _menubutton_paint(EXHANDLE hObj, obj_s* pObj)
{
	paintstruct_s ps;
	int nColor = 0;

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		if (FLAGS_CHECK(pObj->dwState_, STATE_DOWN) || FLAGS_CHECK(pObj->dwState_, STATE_CHECKED))
		{
			nColor = _obj_getcolor(pObj, COLOR_EX_TEXT_DOWN);
		}
		else if (FLAGS_CHECK(pObj->dwState_, STATE_HOVER))
		{
			nColor = _obj_getcolor(pObj, COLOR_EX_TEXT_HOVER);
		}
		else
		{
			nColor = _obj_getcolor(pObj, COLOR_EX_BACKGROUND);
		}
		if (nColor)
			_canvas_clear(ps.hCanvas_, nColor);

		_canvas_drawtext(ps.hCanvas_, pObj->hFont_, _obj_getcolor(pObj, COLOR_EX_TEXT_NORMAL), pObj->pstrTitle_, -1, ps.dwTextFormat_, ps.p_left_, ps.p_top_, ps.p_right_ + ps.p_left_, ps.p_bottom_ + ps.p_top_);
		Ex_ObjEndPaint(hObj, &ps);
	}
}

size_t CALLBACK _menubutton_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	size_t ret = 0;
	RECT lpRect = { 0 };

	switch (uMsg)
	{
	case WM_PAINT:
		_menubutton_paint(hObj, pObj);
		break;
	case WM_MOUSEHOVER:
		_obj_setuistate(pObj, STATE_HOVER, FALSE, NULL, TRUE, NULL);
		break;
	case WM_MOUSELEAVE:
		_obj_setuistate(pObj, STATE_HOVER, TRUE, NULL, TRUE, NULL);
		break;
	case WM_LBUTTONDOWN:
		_obj_setuistate(pObj, STATE_DOWN, FALSE, NULL, TRUE, NULL);
		_obj_postmessage(hWnd, hObj, pObj, 123321, (size_t)pObj->dwUserData_, pObj->lParam_, NULL);
		break;
	case WM_LBUTTONUP:
		_obj_setuistate(pObj, STATE_DOWN, TRUE, NULL, TRUE, NULL);
		break;
	default:
		if (uMsg == 123321 && wParam == (size_t)pObj->dwUserData_)
		{
			if (!lParam)
				lParam = pObj->lParam_;
			if (IsMenu((HMENU)lParam) && !FLAGS_CHECK(pObj->dwState_, STATE_CHECKED))
			{
				EndMenu();
				_obj_setuistate(pObj, STATE_CHECKED, FALSE, NULL, FALSE, NULL);
				if (!_obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, MBN_POPUP, wParam, lParam))
				{
					GetWindowRect(hWnd, &lpRect);
					Ex_TrackPopupMenu((void*)lParam, 0, lpRect.left + pObj->w_left_, lpRect.top + pObj->w_bottom_, (int)hWnd, hObj, NULL, _menubutton_menu_proc, 0);
				}
				_obj_setuistate(pObj, STATE_CHECKED | STATE_DOWN, TRUE, NULL, TRUE, NULL);
			}
		}
		break;
	}
	ret = Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
	return ret;
}

void _menubutton_register() {
	Ex_ObjRegister(L"menubutton", EOS_VISIBLE, EOS_EX_FOCUSABLE, DT_CENTER | DT_VCENTER | DT_SINGLELINE, 0, 0, 0, _menubutton_proc);
}