#include "help_ex.h"
#include "Class_MenuButton_ex.h"

size_t CALLBACK _menubutton_menu_proc(HWND hWnd, EXHANDLE hExDUI, UINT uMsg, size_t wParam, size_t lParam, void* lpResult)
{
    menu_s* lpMenuParams; // [esp+28h] [ebp-8h] BYREF
    wnd_s* pWnd; // [esp+2Ch] [ebp-4h] BYREF
    HWND hWnda; // [esp+38h] [ebp+8h]
    int nError = 0;
    POINT point = { 0 };
    obj_s* pObj;
    obj_s* pObj2;
    HWND currentWnd;
    EXHANDLE hObj = 0;

    if (uMsg == 485 && wParam == -1)
    {
        if (_handle_validate(hExDUI, HT_DUI, (void**)&pWnd, &nError))
        {
            lpMenuParams = pWnd->lpMenuParams_;
            if (lpMenuParams)
            {
                GetCursorPos(&point);
                currentWnd = WindowFromPoint(point);
                if ((HWND)lpMenuParams->nReserved_ == currentWnd)
                {
                    ScreenToClient(currentWnd, &point);
                    hObj = Ex_DUIGetObjFromPoint(currentWnd, point);
                    if (hObj && hObj != lpMenuParams->handle_)
                    {
                            if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
                            {
                                if (pObj->pCls_->atomName_ == 371568388)
                                {
                                    nError = 0;
                                    if (_handle_validate(lpMenuParams->handle_, HT_OBJECT, (void**)&pObj2, &nError))
                                    {
                                        if (pObj->objParent_ == pObj2->objParent_ && pObj->dwUserData_ == pObj2->dwUserData_)
                                        {
                                            //EndMenu();
                                            //_obj_sendmessage(currentWnd, hObj, pObj, 0x1E1B9u, (size_t)pObj->dwUserData_, 0, 0);
                                        }
                                    }
                                }
                            }
                    }
                }
            }
        }
    }
    return 0;
}

void _menubutton_paint(EXHANDLE hObj, obj_s* pObj)
{
    paintstruct_s ps; // eax
    int nColor; // [esp+38h] [ebp-8h]

    nColor = 0;

    if (Ex_ObjBeginPaint(hObj, &ps))
    {
        if (FLAGS_CHECK(pObj->dwState_, 8) || FLAGS_CHECK(pObj->dwState_, 16))
        {
            nColor = _obj_getcolor(pObj, 4);
        }
        else if (FLAGS_CHECK(pObj->dwState_, 128))
        {
            nColor = _obj_getcolor(pObj, 3);
        }
        else
        {
            nColor = _obj_getcolor(pObj, 0);
        }
        if (nColor)
            _canvas_clear(ps.hCanvas_, nColor);
        //sub_1005C7D3(ps.p_left_, ps.p_top_, ps.p_right_, ps.p_bottom_, (RECT**)&rect);

        _canvas_drawtext(ps.hCanvas_, pObj->hFont_, _obj_getcolor(pObj, 2), pObj->pstrTitle_, -1, ps.dwTextFormat_, ps.p_left_, ps.p_top_, ps.p_right_ + ps.p_left_, ps.p_bottom_ + ps.p_top_);
        Ex_ObjEndPaint(hObj, &ps);
    }
}

size_t CALLBACK _menubutton_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
    int ret; // [esp-4h] [ebp-4Ch]
    int v11; // [esp+0h] [ebp-48h]

    int v14; // [esp+3Ch] [ebp-Ch]
    int nID; // [esp+40h] [ebp-8h]
    int nIDa; // [esp+40h] [ebp-8h]
    RECT lpRect = { 0 }; // [esp+44h] [ebp-4h]

    switch (uMsg)
    {
    case WM_PAINT:
        _menubutton_paint(hObj, pObj);
        break;
    case WM_MOUSEHOVER:
        _obj_setuistate(pObj, 128, 0, 0, 1, 0);
        break;
    case WM_MOUSELEAVE:
        _obj_setuistate(pObj, 128, 1, 0, 1, 0);
        break;
    case WM_LBUTTONDOWN:
        _obj_setuistate(pObj, 8, 0, 0, 1, 0);
        _obj_sendmessage(hWnd, hObj, pObj, 0x1E1B9u, (size_t)pObj->dwUserData_, pObj->lParam_, 0);
        break;
    case WM_LBUTTONUP:
        _obj_setuistate(pObj, 8, 1, 0, 1, 0);
        break;
    default:
        if (uMsg == 123321 && wParam == (size_t)pObj->dwUserData_)
        {
            if (!lParam)
                lParam = pObj->lParam_;
            if (IsMenu((HMENU)lParam) && !FLAGS_CHECK(pObj->dwState_, 16))
            {
                EndMenu();
                _obj_setuistate(pObj, 16, 0, 0, 0, 0);
                if (!_obj_dispatchnotify(hWnd, pObj, hObj, pObj->id_, 102401, wParam, lParam))
                {
                    GetWindowRect(hWnd, &lpRect);
                    Ex_TrackPopupMenu((void*)lParam, 0, lpRect.left + pObj->w_left_, lpRect.top + pObj->w_bottom_, (int)hWnd, hObj, 0, _menubutton_menu_proc, 0);
                }
                _obj_setuistate(pObj, 16, 1, 0, 1, 0);
            }
        }
        break;
    }
    ret = Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
    return ret;
}

void _menubutton_register() {
	Ex_ObjRegister(L"menubutton", 0x10000000, 0x8000000, 1 | 4 | 32, 0, 0, 0, _menubutton_proc);
}