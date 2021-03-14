#include "Class_PopupBox_ComBox_ex.h"

size_t _pb_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_PAINT)
	{
		_static_paint(hObj, pObj);
	}
	else if (uMsg == PBM_POPUP)
	{
		HMENU hMenu = CreatePopupMenu();
		AppendMenuW(hMenu, MF_BYPOSITION, 0, 0);
		MENUINFO mi;
		mi.cbSize = sizeof(MENUINFO);
		mi.fMask = MIM_MENUDATA;
		mi.dwMenuData = (ULONG_PTR)pObj;
		SetMenuInfo(hMenu, &mi);
		RECT rc;
		GetWindowRect(hWnd, &rc);
		Ex_TrackPopupMenu(hMenu, 0, rc.left + pObj->w_left_, rc.top + pObj->w_bottom_, 0, hObj, 0, &_pbm_proc, 0);
		DestroyMenu(hMenu);
	}

	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

size_t _pbm_proc(HWND hWnd, EXHANDLE hExDUI, UINT uMsg, size_t wParam, size_t lParam, void* lpResult)
{
	wnd_s* pWnd = nullptr;
	obj_s* pObj = nullptr;
	int nError = 0;
	if (_handle_validate(hExDUI, HT_DUI, (void**)&pWnd, &nError))
	{
		MENUINFO mi;
		mi.cbSize = sizeof(MENUINFO);
		mi.fMask = MIM_MENUDATA;
		pObj = (obj_s*)mi.dwMenuData;
		if (pObj != 0)
		{
			mempoolmenumsg_s ms;
			ms.hWnd = hWnd;
			ms.hExDUI = hExDUI;
			ms.pObj = pObj;
			ms.uMsg = uMsg;
			ms.wParam = wParam;
			ms.lParam = lParam;
			if (_obj_baseproc(_obj_gethWnd(pObj), pObj->hObj_, pObj, PBM_ON_POPWND_MSG, hExDUI, (size_t)&ms))
			{
				return 1;
			}
		}
		return 0;
	}
	if (uMsg == WM_DESTROY)
	{
		if (pObj != nullptr)
		{
			_obj_baseproc(_obj_gethWnd(pObj), pObj->hObj_, pObj, PBM_ON_CLOSE, (size_t)hWnd, hExDUI);
		}
		
	}
	return 0;
}

size_t _cb_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, obj_s* pObj)
{
	if (uMsg == WM_CREATE)
	{
		array_s* hArr = Array_Create(0);
		Array_BindEvent(hArr, 数组事件_删除成员, &_cb_arr_del);
		pObj->dwOwnerData_ = hArr;
	}
	else if (uMsg == WM_DESTROY)
	{
		Array_Destroy((array_s*)pObj->dwOwnerData_);
	}
	else if (uMsg == WM_PAINT)
	{
		_cb_paint(hObj, pObj);
		return 0;
	}
	else if (uMsg == WM_EX_LCLICK)
	{
		Ex_ObjSendMessage(hObj, PBM_POPUP, 0, 0);
	}
	else if (uMsg == PBM_ON_POPUP)
	{
		array_s* hArr =(array_s *) pObj->dwOwnerData_;
		EXHANDLE hLayout=_layout_create(ELT_PAGE, lParam);
		LPCWSTR class_name = L"listview";
		EXHANDLE hObjItem = Ex_ObjCreateEx(-1, (void*)class_name, 0, -1, 10, 10, 100, 100, lParam, 101, DT_VCENTER | DT_SINGLELINE,(size_t) hArr, pObj->hTheme_, &_cblv_proc);
		int nItem = Array_GetCount(hArr);
		Ex_ObjSendMessage(hObjItem, LVM_SETITEMCOUNT, nItem, nItem);
		int height = nItem * 25;
		if (height < 50) height = 50;
		if (height > 400) height = 400;
		SetWindowPos((HWND)wParam, HWND_TOP, 0, 0, pObj->c_right_, Ex_Scale(height), SWP_NOMOVE | SWP_NOACTIVATE);
		Ex_ObjMove(hObjItem, 5, 5, pObj->c_right_ - 15, height - 10, true);
		return 1;
	}
	else if (uMsg == PBM_ON_CLOSE)
	{
		array_s* hArr = (array_s*)pObj->dwOwnerData_;
		EXHANDLE hObjItem = Ex_ObjGetFromID(lParam, 101);
		int nItem = Ex_ObjSendMessage(hObjItem, LVM_GETSELECTIONMARK, 0, 0);
		if (nItem != 0)
		{
			Ex_ObjDispatchNotify(hObj, LVN_ITEMCHANGED, nItem, 0);
			Ex_ObjSetText(hObj, (void*)__get((void*)Array_GetMember(hArr, nItem), offsetof(cbi_s, wzItem_)), true);
		}
	}
	else if (uMsg == LVM_INSERTITEM)
	{
		void* pItem = Ex_MemAlloc(sizeof(cbi_s));
		__set(pItem, offsetof(cbi_s, wzItem_), (size_t)copytstr((LPCWSTR)lParam, lstrlenW((LPCWSTR)lParam)));
		__set_int(pItem, offsetof(cbi_s, nItem_), 0);
		if (wParam <= 0)
		{
			wParam = Array_GetCount((array_s*)_obj_pOwner(pObj)) + (size_t)1;
		}
		return Array_AddMember((array_s*)_obj_pOwner(pObj), (size_t)pItem, wParam);
	}
	else if (uMsg == LVM_DELETEITEM)
	{
		Array_DelMember((array_s*)_obj_pOwner(pObj), wParam);
	}
	return _pb_proc(hWnd, hObj, uMsg, wParam, lParam, pObj);
}

void _cb_arr_del(array_s* hArr, int nIndex, void* pvData, int nType)
{
	_struct_destroyfromaddr(pvData, offsetof(cbi_s, wzItem_));
	Ex_MemFree(pvData);
}

void _cb_paint(EXHANDLE hObj, obj_s* pObj)
{
	_static_paint(hObj, pObj);
}

int _cblv_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, int* lpResult)
{
	if (uMsg == WM_NOTIFY_SELF)
	{
		int nCode = __get_int((void*)lParam, 8);
		size_t nwParam = __get((void*)lParam, 12);
		size_t nlParam= __get((void*)lParam, 12+sizeof(size_t));
		if (nCode == NM_CALCSIZE)
		{
			__set_int((void*)nlParam, 4, 22);
			*lpResult = 1;
			return 1;
		}
		else if (nCode == NM_CUSTOMDRAW)
		{
			EX_CUSTOMDRAW ecd;
			RtlMoveMemory(&ecd, (void*)nlParam, sizeof(EX_CUSTOMDRAW));
			array_s* hArr = (array_s*)Ex_ObjGetLong(hObj, EOL_LPARAM);
			void* pItemInfo = (void*)Array_GetMember(hArr, nwParam);
			if (pItemInfo != 0)
			{
				obj_s* pObj = nullptr;
				int nError = 0;
				if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
				{
					if ((ecd.dwState & 状态_点燃) != 0)
					{
						void* brush = _brush_create(ExRGB2ARGB(8421504, 125));
						_canvas_fillrect(ecd.hCanvas, brush, ecd.rcDraw.left, ecd.rcDraw.top, ecd.rcDraw.right, ecd.rcDraw.bottom);
						_brush_destroy(brush);
					}
					_canvas_drawtext(ecd.hCanvas, pObj->hFont_, ExRGB2ARGB(0, 255), (LPCWSTR)__get(pItemInfo, offsetof(cbi_s, wzItem_)), -1, DT_SINGLELINE | DT_VCENTER | DT_LEFT, ecd.rcDraw.left, ecd.rcDraw.top, ecd.rcDraw.right, ecd.rcDraw.bottom);
				}
			}
			*lpResult = 1;
			return 1;
		}
		else if (nCode == NM_CLICK)
		{
			EndMenu();
			return 1;
		}
	}
	return 0;
}