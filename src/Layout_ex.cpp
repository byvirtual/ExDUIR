#include "Layout_ex.h"

bool _layout_register(int nType, void* lpfnLayoutProc)
{
	bool ret = false;
	if (!HashTable_IsExit(g_Li.hTableLayout, nType))
	{
		ret = HashTable_Set(g_Li.hTableLayout, nType, (size_t)lpfnLayoutProc);
	}
	return ret;
}

bool _layout_unregister(int nType)
{
	bool ret = false;
	if (!HashTable_IsExit(g_Li.hTableLayout, nType))
	{
		ret = HashTable_Remove(g_Li.hTableLayout, nType);
	}
	return ret;
}

void _layout_free_info(array_s* hArr, int nIndex, void* pvItem, int nType)
{
	layout_s* pLayout = (layout_s*)Array_GetExtra(hArr);
	((LayoutTwoPROC)pLayout->lpfnProc_)(ELN_UNINITCHILDPROPS, __get(pvItem, 0), (size_t)pvItem);
	Ex_MemFree((void*)((size_t)pvItem - 16));
}

EXHANDLE _layout_create(int nType, EXHANDLE hObjBind)
{
	EXHANDLE hLayout = 0;
	int nError = 0;
	if (nType > 0 && hObjBind != 0)
	{
		size_t lpfnProc = 0;
		HashTable_Get(g_Li.hTableLayout, nType, &lpfnProc);
		if (lpfnProc != 0)
		{
			layout_s* pLayout = (layout_s*)Ex_MemAlloc(sizeof(layout_s));
			obj_s* pObj = nullptr;
			if (pLayout != 0)
			{
				if (_handle_validate(hObjBind, HT_OBJECT, (void**)&pObj, &nError))
				{
					pLayout->nBindType_ = HT_OBJECT;
				}
				else if (_handle_validate(hObjBind, HT_DUI, (void**)&pObj, &nError))
				{
					pLayout->nBindType_ = HT_DUI;
				}
				else {
					nError = ERROR_EX_INVALID_OBJECT;
				}
				if (nError == 0)
				{
					pLayout->nType_ = nType;
					pLayout->fUpdateable_ = true;
					pLayout->lpfnProc_ = (void*)lpfnProc;
					pLayout->hBind_ = hObjBind;
					int nCount = ((LayoutThreePROC)lpfnProc)(NULL, ELN_GETPROPSCOUNT, NULL, NULL);
					void* pInfo = (void*)((size_t)Ex_MemAlloc((nCount + (size_t)4) * (size_t)4) + (size_t)16);
					pLayout->lpLayoutInfo_ = pInfo;
					((LayoutThreePROC)lpfnProc)(NULL, ELN_INITPROPS, NULL, (size_t)pInfo);
					nCount = ((LayoutThreePROC)lpfnProc)(NULL, ELN_GETCHILDPROPCOUNT, NULL, NULL);
					pLayout->cbInfoLen_ = (nCount + 5) * 4;
					array_s* hArr = Array_Create(0);
					Array_BindEvent(hArr, 数组事件_删除成员, &_layout_free_info);
					Array_SetExtra(hArr, (size_t)pLayout);
					pLayout->hArrChildrenInfo_ = hArr;
					hLayout = _handle_create(HT_LAYOUT, pLayout, &nError);
					pLayout->hLayout_ = hLayout;
					((wnd_s*)pObj)->hLayout_ = hLayout;
				}
				else {
					Ex_MemFree(pLayout);
				}
			}
			else {
				nError = ERROR_EX_MEMORY_ALLOC;
			}
		}
		else {
			nError = ERROR_EX_LAYOUT_INVALID;
		}
	}
	else {
		nError = ERROR_EX_LAYOUT_INVALID;
	}
	Ex_SetLastError(nError);
	return hLayout;
}

EXHANDLE _layout_get_parent_layout(EXHANDLE hObj)
{
	int nError = 0;
	obj_s* pObj = nullptr;
	EXHANDLE hLayoutParent = 0;
	if (_handle_validate(hObj, HT_OBJECT, (void**)&pObj, &nError))
	{
		EXHANDLE hObj = pObj->objParent_;
		if (hObj == 0)
		{
			auto pWnd = pObj->pWnd_;
			hObj = pWnd->hexdui_;
		}
		hLayoutParent = Ex_ObjLayoutGet(hObj);
	}
	return hLayoutParent;
}

bool _layout_destory(EXHANDLE hLayout)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		void* lpfnProc = pLayout->lpfnProc_;
		((LayoutTwoPROC)lpfnProc)(ELN_UNINITPROPS, 0, (size_t)pLayout->lpLayoutInfo_);
		Array_Destroy(pLayout->hArrChildrenInfo_);
		Ex_MemFree((void*)((size_t)pLayout->lpLayoutInfo_ - 16));
		Ex_MemFree(pLayout);
		_handle_destroy(hLayout, &nError);
	}
	return nError == 0;
}

bool _layout_enum_find_obj(void* hArr, int nIndex, void* pvItem, int nType, void* pvParam)
{
	return ((void*)__get(pvItem, 0) == pvParam);
}

void* _layout_get_child(layout_s* pLayout, EXHANDLE hObj)
{
	auto hObjP = pLayout->hBind_;
	EXHANDLE hExDUI = 0;
	void* pInfo = nullptr;
	if (Ex_ObjGetParentEx(hObj, &hExDUI) == hObjP)
	{
		if (hExDUI == hObjP)
		{
			array_s* hArr = pLayout->hArrChildrenInfo_;
			int nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
			if (nIndex > 0)
			{
				pInfo = (void*)Array_GetMember(hArr, nIndex);
			}
		}
	}
	return pInfo;
}

bool _layout_update(EXHANDLE hLayout)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		if (pLayout->fUpdateable_)
		{
			void* lpfnProc = pLayout->lpfnProc_;
			((LayoutThreePROC)lpfnProc)(pLayout, ELN_UPDATE, pLayout->hBind_, 0);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int _layout_gettype(EXHANDLE hLayout)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	int nType = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		nType = pLayout->nType_;
	}
	Ex_SetLastError(nError);
	return nType;
}

bool _layout_enableupdate(EXHANDLE hLayout, bool fUpdateable)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	int nType = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		pLayout->fUpdateable_ = fUpdateable;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

size_t _layout_notify(EXHANDLE hLayout, int nEvent, void* wParam, void* lParam)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	int ret = 1;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		if (pLayout->fUpdateable_)
		{
			void* lpfnProc = pLayout->lpfnProc_;
			ret = ((LayoutThreePROC)lpfnProc)(pLayout, nEvent, (size_t)wParam, (size_t)lParam);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _layout_table_setinfo(EXHANDLE hLayout, void* aRowHeight, int cRows, void* aCellWidth, int cCells)
{
	int nError = 0;
	layout_s* pLayout = nullptr;
	int ret = 1;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		if (pLayout->nType_ == ELT_TABLE)
		{
			void* pInfo = pLayout->lpLayoutInfo_;
			array_s* hArr = (array_s*)__get(pInfo, (ELP_TABLE_ARRAY_ROW - 1) * sizeof(void*));
			Array_Redefine(hArr, cRows, false);
			for (int i = 0; i < cRows; i++)
			{
				Array_SetMember(hArr, i + 1, __get(aRowHeight, i * sizeof(void*)));
			}
			hArr = (array_s*)__get(pInfo, (ELP_TABLE_ARRAY_CELL - 1) * sizeof(void*));
			Array_Redefine(hArr, cCells, false);
			for (int i = 0; i < cRows; i++)
			{
				Array_SetMember(hArr, i + (size_t)1, __get(aCellWidth, i * sizeof(void*)));
			}
		}
		else {
			nError = ERROR_EX_LAYOUT_INVALID;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _layout_setchildprop(EXHANDLE hLayout, EXHANDLE hObj, int dwPropID, size_t pvValue)
{
	layout_s* pLayout = nullptr;
	int nError = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		if (hObj != 0)
		{
			array_s* hArr = pLayout->hArrChildrenInfo_;
			size_t nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
			void* pInfo = nullptr;
			if (nIndex == 0)
			{
				pInfo = Ex_MemAlloc(pLayout->cbInfoLen_);
				if (pInfo != 0)
				{
					pInfo = (void*)((size_t)pInfo + 16);
					__set_int(pInfo, 0, hObj);
					((LayoutThreePROC)pLayout->lpfnProc_)(pLayout, ELN_INITCHILDPROPS, hObj, (size_t)pInfo);
					nIndex = Array_AddMember(hArr, (size_t)pInfo);
				}
			}
			else {
				pInfo = (void*)Array_GetMember(hArr, nIndex);
			}
			if (pInfo != 0)
			{
				if (((LayoutThreePROC)pLayout->lpfnProc_)(pLayout, ELN_CHECKCHILDPROPVALUE, MAKELONG(nIndex, dwPropID), pvValue) == 0)
				{
					__set(pInfo, dwPropID * sizeof(void*), pvValue);
				}
			}
			else {
				nError = ERROR_EX_MEMORY_ALLOC;
			}
		}
		else {
			nError = ERROR_EX_INVALID_OBJECT;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _layout_getchildprop(EXHANDLE hLayout, EXHANDLE hObj, int dwPropID, size_t* pvValue)
{
	layout_s* pLayout = nullptr;
	int nError = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		array_s* hArr = pLayout->hArrChildrenInfo_;
		size_t nIndex = Array_Emum(hArr, &_layout_enum_find_obj, hObj);
		void* pInfo = nullptr;
		if (nIndex > 0)
		{
			pInfo = (void*)Array_GetMember(hArr, nIndex);
		}
		if (pInfo != 0)
		{
			dwPropID = dwPropID * sizeof(void*);
			if (dwPropID >= -16 && dwPropID < pLayout->cbInfoLen_)
			{
				*pvValue = __get(pInfo, dwPropID);
			}
			else {
				nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
			}
		}
		else {
			nError = ERROR_EX_LAYOUT_INVALID;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _layout_setprop(EXHANDLE hLayout, int dwPropID, size_t pvValue)
{
	layout_s* pLayout = nullptr;
	int nError = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		void* pInfo = pLayout->lpLayoutInfo_;
		void* lpfnProc = pLayout->lpfnProc_;
		if (((LayoutThreePROC)lpfnProc)(pLayout, ELN_CHECKCHILDPROPVALUE, dwPropID, pvValue) == 0)
		{
			if (dwPropID > 0)
			{
				dwPropID = dwPropID - 1;
			}
			__set(pInfo, dwPropID * sizeof(void*), pvValue);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

size_t _layout_getprop(EXHANDLE hLayout, int dwPropID)
{
	layout_s* pLayout = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		void* pInfo = pLayout->lpLayoutInfo_;
		if (dwPropID > 0)
		{
			dwPropID = dwPropID - 1;
		}
		ret = __get(pInfo, dwPropID * sizeof(void*));
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _layout_absolute_setedge(EXHANDLE hLayout, EXHANDLE hObjChild, int dwEdge, int dwType, size_t nValue)
{
	layout_s* pLayout = nullptr;
	int nError = 0;
	if (_handle_validate(hLayout, HT_LAYOUT, (void**)&pLayout, &nError))
	{
		if (pLayout->nType_ == ELT_ABSOLUTE)
		{
			dwEdge = (dwEdge + 1) / 2;
			if (dwEdge >= 1 && dwEdge <= 8)
			{
				_layout_setchildprop(hLayout, hObjChild, dwEdge * 2, dwType);
				_layout_setchildprop(hLayout, hObjChild, dwEdge * 2 - 1, nValue);
			}
			else {
				nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
			}
		}
		else {
			nError = ERROR_EX_LAYOUT_UNSUPPORTED_PROP;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _layout_move_margin(EXHANDLE hObj, RECT* lpObjRc, void* lpMargin, int dwLockFlags, int dwOrgFlags)
{
	RECT rcObj = *lpObjRc;
	RECT rcMargin{ 0 };
	rcMargin.left = __get_int(lpMargin, 12);
	rcMargin.top = __get_int(lpMargin, 8);
	rcMargin.right = __get_int(lpMargin, 4);
	rcMargin.bottom = __get_int(lpMargin, 0);
	rcObj.left = rcObj.left + rcMargin.left;
	if ((dwLockFlags & 1) == 1)
	{
		rcObj.right = rcObj.right + rcMargin.left;
		lpObjRc->right = lpObjRc->right + rcMargin.left;
	}
	rcObj.top = rcObj.top + rcMargin.top;
	if ((dwLockFlags & 2) == 2)
	{
		rcObj.bottom = rcObj.bottom + rcMargin.top;
		lpObjRc->bottom = lpObjRc->bottom + rcMargin.top;
	}
	if ((dwLockFlags & 4) == 4)
	{
		rcObj.right = rcObj.right - rcMargin.right;
	}
	else {
		lpObjRc->right = lpObjRc->right + rcMargin.right;
	}
	if ((dwLockFlags & 8) == 8)
	{
		rcObj.bottom = rcObj.bottom - rcMargin.bottom;
	}
	else {
		lpObjRc->bottom = lpObjRc->bottom + rcMargin.bottom;
	}
	if (dwOrgFlags == 15)
	{
		return;
	}
	rcObj.right = rcObj.right - rcObj.left;
	rcObj.bottom = rcObj.bottom - rcObj.top;
	if ((dwOrgFlags & 1) != 0)
	{
		rcObj.left = EOP_DEFAULT;
	}
	if ((dwOrgFlags & 2) != 0)
	{
		rcObj.top = EOP_DEFAULT;
	}
	if ((dwOrgFlags & 4) != 0)
	{
		rcObj.right = EOP_DEFAULT;
	}
	if ((dwOrgFlags & 8) != 0)
	{
		rcObj.bottom = EOP_DEFAULT;
	}
	Ex_ObjMove(hObj, rcObj.left, rcObj.top, rcObj.right, rcObj.bottom, false);
}

size_t __layout_linear_proc(layout_s* pLayput, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 2;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 2;
	}
	else if (nEvent == ELN_INITCHILDPROPS)
	{
		__set_int((void*)lParam, ELCP_LINEAR_SIZE * 4, -1);
	}
	else if (nEvent == ELN_CHECKCHILDPROPVALUE)
	{
		int nSize = HIWORD(wParam);
		if (nSize == ELN_CHECKCHILDPROPVALUE)
		{
			return (lParam< ELCP_LINEAR_ALGIN_FILL || lParam>ELCP_LINEAR_ALIGN_RIGHT_BOTTOM);
		}
	}
	else if (nEvent == ELN_UPDATE)
	{
		RECT rcClient{ 0 };
		if (pLayput->nBindType_ == HT_OBJECT)
		{
			Ex_ObjGetClientRect(wParam, &rcClient);
		}
		else {
			Ex_DUIGetClientRect(wParam, &rcClient);
		}
		void* pInfo = pLayput->lpLayoutInfo_;
		array_s* hArr = pLayput->hArrChildrenInfo_;
		int nDAlign = __get_int(pInfo, (ELP_LINEAR_DALIGN - 1) * 4);
		bool fVertical = __get_int(pInfo, (ELP_LINEAR_DIRECTION - 1) * 4) == ELP_DIRECTION_V;
		rcClient.left = rcClient.left + __get_int(pInfo, ELP_PADDING_LEFT * 4);
		rcClient.top = rcClient.top + __get_int(pInfo, ELP_PADDING_TOP * 4);
		rcClient.right = rcClient.right + __get_int(pInfo, ELP_PADDING_RIGHT * 4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfo, ELP_PADDING_BOTTOM * 4);
		SIZE szClient;
		szClient.cx = rcClient.right - rcClient.left;
		szClient.cy = rcClient.bottom - rcClient.top;
		RECT rcObj;
		rcObj.left = rcClient.left;
		rcObj.top = rcClient.top;
		std::vector<RECT> arrRect;
		std::vector<int> arrOrg;
		if (nDAlign != 0 && Array_GetCount(hArr) > 0)
		{
			arrRect.resize(Array_GetCount(hArr));
			arrOrg.resize(Array_GetCount(hArr));
		}
		for (int i = 0; i < Array_GetCount(hArr); i++)
		{
			int orgFlags = 0;
			void* pInfo = (void*)Array_GetMember(hArr, i);
			EXHANDLE hObj = __get_int(pInfo, 0);
			if (hObj == 0) continue;
			int nSize = __get_int(pInfo, ELCP_LINEAR_SIZE * 4);
			RECT rcTmp{ 0 };
			Ex_ObjGetRect(hObj, &rcTmp);
			int w = rcTmp.right - rcTmp.left;
			int h = rcTmp.bottom - rcTmp.top;
			if (nSize < 0)
			{
				if (fVertical)
				{
					nSize = h;
				}
				else {
					nSize = w;
				}
			}
			int nFill = __get_int(pInfo, ELCP_LINEAR_ALIGN * 4);
			orgFlags = 0;
			if (fVertical)
			{
				if (nFill == ELCP_LINEAR_ALIGN_LEFT_TOP)
				{
					rcObj.left = rcClient.left;
					rcObj.right = rcObj.left + w;
				}
				else if (nFill == ELCP_LINEAR_ALIGN_CENTER)
				{
					rcObj.left = rcClient.left + (szClient.cx - w) / 2;
					rcObj.right = rcObj.left + w;
					orgFlags = 4;
				}
				else if (nFill == ELCP_LINEAR_ALIGN_RIGHT_BOTTOM)
				{
					rcObj.right = rcClient.right;
					rcObj.left = rcClient.right - w;
				}
				else {
					rcObj.left = rcClient.left;
					rcObj.right = rcClient.right;
				}
				rcObj.bottom = rcObj.top + nSize;
			}
			else {
				if (nFill == ELCP_LINEAR_ALIGN_LEFT_TOP)
				{
					rcObj.top = rcClient.top;
					rcObj.bottom = rcObj.top + rcObj.bottom - rcTmp.top;
				}
				else if (nFill == ELCP_LINEAR_ALIGN_CENTER)
				{
					rcObj.top = rcClient.top + (szClient.cy - h) / 2;
					rcObj.bottom = rcObj.top + h;
					orgFlags = 8;
				}
				else if (nFill == ELCP_LINEAR_ALIGN_RIGHT_BOTTOM)
				{
					rcObj.bottom = rcClient.bottom;
					rcObj.top = rcClient.bottom - (rcTmp.bottom - rcTmp.top);
				}
				else {
					rcObj.top = rcClient.top;
					rcObj.bottom = rcClient.bottom;
				}
				rcObj.right = rcObj.left + nSize;
			}
			if (nDAlign == 0)
			{
				_layout_move_margin(hObj, &rcObj, (void*)((size_t)pInfo - 16), fVertical ? 5 : 10, orgFlags);
			}
			else {
				rcObj.left = rcObj.left + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
				rcObj.top = rcObj.top + __get_int(pInfo, ELCP_MARGIN_TOP * 4);
				rcObj.right = rcObj.right + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
				rcObj.bottom = rcObj.bottom + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
				arrRect[i] = rcObj;
				arrOrg[i] = orgFlags;
			}
			if (fVertical)
			{
				rcObj.top = rcObj.bottom;
			}
			else {
				rcObj.left = rcObj.right;
			}
		}
		if (Array_GetCount(hArr) > 0 && arrRect.size() > 0)
		{
			void* pInfo = pLayput->lpLayoutInfo_;
			int nDAlign = __get_int(pInfo, (ELP_LINEAR_DALIGN - 1) * 4);
			int w = 0;
			int h = 0;
			if (fVertical)
			{
				int nSize = arrRect[arrRect.size() - 1].bottom - arrRect[0].top;
				h = 5;
				if (nDAlign == 2)//bottom
				{
					w = rcClient.bottom - nSize - arrRect[0].top;
				}
				else if (nDAlign == 1)//CENTER
				{
					w = rcClient.top + (rcClient.bottom - rcClient.top - nSize) / 2 - arrRect[0].top;
				}
				else {
					w = 0;
				}
			}
			else {
				int nSize = arrRect[arrRect.size() - 1].right - arrRect[0].left;
				h = 10;
				if (nDAlign == 2)//right
				{
					w = rcClient.right - nSize - arrRect[0].left;
				}
				else if (nDAlign == 1)//center
				{
					w = rcClient.left + (rcClient.right - rcClient.top - nSize) / 2 - arrRect[0].left;
				}
				else {
					w = 0;
				}
			}
			for (int i = 0; i > Array_GetCount(hArr); i++)
			{
				void* pInfo = (void*)Array_GetMember(hArr, i);
				RECT rcObj = arrRect[i];
				if (fVertical)
				{
					OffsetRect(&rcObj, 0, w);
				}
				else {
					OffsetRect(&rcObj, w, 0);
				}
				_layout_move_margin(__get_int(pInfo, 0), &rcObj, (void*)((size_t)pInfo - 16), 15, arrOrg[i]);
			}
		}
	}
	return 0;
}

size_t __layout_flow_proc(layout_s* pLayout, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 1;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 2;
	}
	else if (nEvent == ELN_INITCHILDPROPS)
	{
		__set_int((void*)lParam, ELCP_LINEAR_SIZE * 4, -1);
	}
	else if (nEvent == ELN_UPDATE)
	{
		RECT rcClient{ 0 };
		if (pLayout->nBindType_ == HT_OBJECT)
		{
			Ex_ObjGetClientRect(wParam, &rcClient);
		}
		else {
			Ex_DUIGetClientRect(wParam, &rcClient);
		}
		void* pInfoa = pLayout->lpLayoutInfo_;
		array_s* hArr = pLayout->hArrChildrenInfo_;
		bool fVertical = __get_int(pInfoa, (ELP_LINEAR_DIRECTION - 1) * 4) == ELP_DIRECTION_V;
		rcClient.left = rcClient.left + __get_int(pInfoa, ELP_PADDING_LEFT * 4);
		rcClient.top = rcClient.top + __get_int(pInfoa, ELP_PADDING_TOP * 4);
		rcClient.right = rcClient.right + __get_int(pInfoa, ELP_PADDING_RIGHT * 4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfoa, ELP_PADDING_BOTTOM * 4);
		RECT rcObj{ 0 };
		rcObj.left = rcClient.left;
		rcObj.top = rcClient.top;

		int nMaxSize = 0;
		for (int i = 0; i < Array_GetCount(hArr); i++)
		{
			void* pInfo = (void*)Array_GetMember(hArr, i);
			EXHANDLE hObj = (EXHANDLE)__get_int(pInfo, 0);
			if (hObj == 0) continue;
			int nSize = __get_int(pInfo, ELCP_LINEAR_SIZE * 4);
			RECT rcTmp{ 0 };
			Ex_ObjGetRect(hObj, &rcTmp);
			if (nSize < 0)
			{
				if (fVertical)
				{
					nSize = rcTmp.bottom - rcTmp.top;
				}
				else {
					nSize = rcTmp.right - rcTmp.left;
				}
			}
			if (fVertical)
			{
				rcObj.right = rcObj.left + rcTmp.right - rcTmp.left;
				rcObj.bottom = rcObj.top + nSize;
				if (rcObj.bottom > rcClient.bottom || __get_int(pInfo, ELCP_FLOW_NEW_LINE * 4) != 0)
				{
					rcObj.top = rcClient.top;
					rcObj.bottom = rcObj.top + nSize;
					rcObj.left = rcObj.left + nMaxSize;
					rcObj.right = rcObj.left + rcTmp.right - rcTmp.left;
					nMaxSize = 0;
				}
			}
			else {
				rcObj.bottom = rcObj.top + rcTmp.bottom - rcTmp.top;
				rcObj.right = rcObj.left + nSize;
				if (rcObj.right > rcClient.right || __get_int(pInfo, ELCP_FLOW_NEW_LINE * 4) != 0)
				{
					rcObj.left = rcClient.left;
					rcObj.right = rcObj.left + nSize;
					rcObj.top = rcObj.top + nMaxSize;
					rcObj.bottom = rcObj.top + rcTmp.bottom - rcTmp.top;
					nMaxSize = 0;
				}
			}
			_layout_move_margin(hObj, &rcObj, (void*)((size_t)pInfo - 16), 0, 0);
			if (fVertical)
			{
				if (rcObj.right - rcObj.left > nMaxSize)
				{
					nMaxSize = rcObj.right - rcObj.left;
				}
				rcObj.top = rcObj.bottom;
			}
			else {
				if (rcObj.bottom - rcObj.top > nMaxSize)
				{
					nMaxSize = rcObj.bottom - rcObj.top;
				}
				rcObj.left = rcObj.right;
			}
		}

	}
	return 0;
}

size_t __layout_page_proc(layout_s* pLayout, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 1;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 1;
	}
	else if (nEvent == ELN_UPDATE)
	{
		RECT rcClient{ 0 };
		if (pLayout->nBindType_ == HT_OBJECT)
		{
			Ex_ObjGetClientRect(wParam, &rcClient);
		}
		else {
			Ex_DUIGetClientRect(wParam, &rcClient);
		}
		void* pInfoa = pLayout->lpLayoutInfo_;
		array_s* hArr = pLayout->hArrChildrenInfo_;
		bool fVertical = __get_int(pInfoa, (ELP_LINEAR_DIRECTION - 1) * 4) == ELP_DIRECTION_V;
		rcClient.left = rcClient.left + __get_int(pInfoa, ELP_PADDING_LEFT * (size_t)4);
		rcClient.top = rcClient.top + __get_int(pInfoa, ELP_PADDING_TOP * (size_t)4);
		rcClient.right = rcClient.right + __get_int(pInfoa, ELP_PADDING_RIGHT * (size_t)4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfoa, ELP_PADDING_BOTTOM * (size_t)4);
		int nCurrentPage = __get_int(pInfoa, (ELP_PAGE_CURRENT - 1) * (size_t)4);
		for (int i = 0; i < Array_GetCount(hArr); i++)
		{
			void* pInfo = (void*)Array_GetMember(hArr, i);
			EXHANDLE hObj = (EXHANDLE)__get_int(pInfo, 0);
			if (hObj == 0) continue;
			if (i == nCurrentPage)//疑问
			{
				if (__get_int(pInfo, ELCP_PAGE_FILL * 4) != 0)
				{
					_layout_move_margin(hObj, &rcClient, (void*)((size_t)pInfo - 16), 15, 0);
				}
			}
			Ex_ObjShow(hObj, i == nCurrentPage);
		}
	}
	return 0;
}

size_t __layout_table_proc(layout_s* pLayout, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 2;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 5;
	}
	else if (nEvent == ELN_INITPROPS)
	{
		__set((void*)lParam, (ELP_TABLE_ARRAY_ROW - 1) * sizeof(size_t), (size_t)Array_Create(0));
		__set((void*)lParam, (ELP_TABLE_ARRAY_CELL - 1) * sizeof(size_t), (size_t)Array_Create(0));
	}
	else if (nEvent == ELN_INITCHILDPROPS)
	{
		__set_int((void*)lParam, ELCP_TABLE_ROW_SPAN * 4, 1);
		__set_int((void*)lParam, ELCP_TABLE_CELL_SPAN * 4, 1);
	}
	else if (nEvent == ELN_UNINITPROPS)
	{
		Array_Destroy((array_s*)__get((void*)lParam, (ELP_TABLE_ARRAY_ROW - 1) * sizeof(size_t)));
		Array_Destroy((array_s*)__get((void*)lParam, (ELP_TABLE_ARRAY_CELL - 1) * sizeof(size_t)));
	}
	else if (nEvent == ELN_UPDATE)
	{
		RECT rcClient{ 0 };
		if (pLayout->nBindType_ == HT_OBJECT)
		{
			Ex_ObjGetClientRect(wParam, &rcClient);
		}
		else {
			Ex_DUIGetClientRect(wParam, &rcClient);
		}
		void* pInfoa = pLayout->lpLayoutInfo_;
		array_s* hArr = pLayout->hArrChildrenInfo_;
		array_s* hArrRows = (array_s*)__get(pInfoa, (ELP_TABLE_ARRAY_ROW - 1) * sizeof(size_t));
		array_s* hArrCells = (array_s*)__get(pInfoa, (ELP_TABLE_ARRAY_CELL - 1) * sizeof(size_t));

		rcClient.left = rcClient.left + __get_int(pInfoa, ELP_PADDING_LEFT * (size_t)4);
		rcClient.top = rcClient.top + __get_int(pInfoa, ELP_PADDING_TOP * (size_t)4);
		rcClient.right = rcClient.right + __get_int(pInfoa, ELP_PADDING_RIGHT * (size_t)4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfoa, ELP_PADDING_BOTTOM * (size_t)4);
		std::vector < std::vector<RECT>> aRects;
		RECT rcTmp{ 0 };
		if (Array_GetCount(hArrRows) > 0 && Array_GetCount(hArrCells) > 0)
		{
			aRects.resize(Array_GetCount(hArrRows));
			for (int index = 0; index < aRects.size(); index++)
			{
				aRects[index].resize(Array_GetCount(hArrCells));
			}
			for (int i = 0; i < Array_GetCount(hArrRows); i++)
			{
				rcTmp.left = rcClient.left;
				rcTmp.bottom = Array_GetMember(hArrRows, i);
				if (rcTmp.bottom < 0)
				{
					rcTmp.bottom = (rcClient.bottom - rcClient.top) * abs(rcTmp.bottom) / 100;
				}
				for (int j = 0; j < Array_GetCount(hArrCells); j++)
				{
					aRects[i][j].left = rcTmp.left;
					aRects[i][j].top = rcTmp.top;
					rcTmp.right = Array_GetMember(hArrCells, j);
					if (rcTmp.right < 0)
					{
						rcTmp.right = (rcClient.right - rcClient.left) * abs(rcTmp.right) / 100;
					}
					aRects[i][j].right = aRects[i][j].left + rcTmp.right;
					aRects[i][j].bottom = aRects[i][j].top + rcTmp.bottom;
					rcTmp.left = rcTmp.left + rcTmp.right;
				}
				rcTmp.top = rcTmp.top + rcTmp.bottom;
			}
		}
		else {
			Ex_SetLastError(ERROR_EX_LAYOUT_INVALID);
			return -1;
		}
		for (int i = 0; i < Array_GetCount(hArr); i++)
		{
			void* pInfo = (void*)Array_GetMember(hArr, i);
			EXHANDLE hObj = __get_int(pInfo, 0);
			if (hObj == 0) continue;
			rcTmp.left = __get_int(pInfo, ELCP_TABLE_CELL * 4);
			rcTmp.top = __get_int(pInfo, ELCP_TABLE_ROW * 4);
			rcTmp.right = __get_int(pInfo, ELCP_TABLE_CELL_SPAN * 4) - 1;
			if (rcTmp.right <= 0)
			{
				rcTmp.right = rcTmp.left;
			}
			else {
				rcTmp.right = rcTmp.left + rcTmp.right;
			}
			rcTmp.bottom = __get_int(pInfo, ELCP_TABLE_ROW_SPAN * 4) - 1;
			if (rcTmp.bottom <= 0)
			{
				rcTmp.bottom = rcTmp.top;
			}
			else {
				rcTmp.bottom = rcTmp.top + rcTmp.bottom;
			}
			if (rcTmp.left <= 0)
			{
				rcTmp.left = 0;
			}
			if (rcTmp.top <= 0)
			{
				rcTmp.top = 0;
			}
			if (rcTmp.left > Array_GetCount(hArrCells))
			{
				rcTmp.left = Array_GetCount(hArrCells) - 1;
			}
			if (rcTmp.top > Array_GetCount(hArrRows))
			{
				rcTmp.top = Array_GetCount(hArrRows) - 1;
			}
			if (rcTmp.right < rcTmp.left)
			{
				rcTmp.right = rcTmp.left;
			}
			if (rcTmp.bottom < rcTmp.top)
			{
				rcTmp.bottom = rcTmp.top;
			}
			if (rcTmp.right > Array_GetCount(hArrCells))
			{
				rcTmp.right = Array_GetCount(hArrCells) - 1;
			}
			if (rcTmp.bottom > Array_GetCount(hArrRows))
			{
				rcTmp.bottom = Array_GetCount(hArrRows) - 1;
			}
			rcClient.left = aRects[rcTmp.top][rcTmp.left].left;
			rcClient.top = aRects[rcTmp.top][rcTmp.left].top;
			rcClient.right = aRects[rcTmp.bottom][rcTmp.right].right;
			rcClient.bottom = aRects[rcTmp.bottom][rcTmp.right].bottom;
			_layout_move_margin(hObj, &rcClient, (void*)((size_t)pInfo - 16), 15, 0);
		}
	}
	return 0;
}

size_t __layout_relative_proc(layout_s* pLayout, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 0;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 10;
	}
	else if (nEvent == ELN_CHECKCHILDPROPVALUE)
	{
		if (Ex_ObjIsValidate(lParam) && LOWORD(wParam) > 0)
		{
			array_s* pChildrenInfo = pLayout->hArrChildrenInfo_;
			void* pInfo = (void*)Array_GetMember(pChildrenInfo, LOWORD(wParam));
			EXHANDLE hObj = __get_int(pInfo, 0);
			int dwPropID = HIWORD(wParam);
			void* pInfoOther = _layout_get_child(pLayout, lParam);
			bool nRet = false;
			if (hObj != 0 && dwPropID > 0 && pInfoOther != 0)
			{
				if (dwPropID == ELCP_RELATIVE_LEFT_OF)
				{
					nRet = __get_int(pInfoOther, ELCP_RELATIVE_RIGHT_OF * 4) != hObj;
				}
				else if (dwPropID == ELCP_RELATIVE_RIGHT_OF)
				{
					nRet = __get_int(pInfoOther, ELCP_RELATIVE_RIGHT_OF * 4) != hObj;
				}
				else if (dwPropID == ELCP_RELATIVE_TOP_OF)
				{
					nRet = __get_int(pInfoOther, ELCP_RELATIVE_TOP_OF * 4) != hObj;
				}
				else if (dwPropID == ELCP_RELATIVE_BOTTOM_OF)
				{
					nRet = __get_int(pInfoOther, ELCP_RELATIVE_BOTTOM_OF * 4) != hObj;
				}
				else {
					nRet = __get_int(pInfoOther, dwPropID * 4) != hObj;
				}
			}
			return !nRet;
		}
	}
	else if (nEvent == ELN_UPDATE)
	{
		_layout_relative_update(pLayout, pLayout->lpLayoutInfo_, pLayout->hArrChildrenInfo_, lParam);
	}
	return 0;
}

void _layout_relative_update(layout_s* pLayout, void* pLayoutInfo, array_s* hArrObjs, size_t lParam)
{
	int Prime = GetNearestPrime(Array_GetCount(hArrObjs));
	hashtable_s* hHashPosInfos = HashTable_Create(Prime, &pfnDefaultFreeData);
	EXHANDLE hObjParent = pLayout->hBind_;
	RECT rcClient{ 0 };
	if (pLayout->nBindType_ == HT_OBJECT)
	{
		Ex_ObjGetClientRect(hObjParent, &rcClient);
	}
	else {
		Ex_DUIGetClientRect(hObjParent, &rcClient);
	}
	rcClient.left = rcClient.left + __get_int(pLayoutInfo, ELP_PADDING_LEFT * (size_t)4);
	rcClient.top = rcClient.top + __get_int(pLayoutInfo, ELP_PADDING_TOP * (size_t)4);
	rcClient.right = rcClient.right + __get_int(pLayoutInfo, ELP_PADDING_RIGHT * (size_t)4);
	rcClient.bottom = rcClient.bottom + __get_int(pLayoutInfo, ELP_PADDING_BOTTOM * (size_t)4);
	for (int i = 0; i < Array_GetCount(hArrObjs); i++)
	{
		void* pInfo = (void*)Array_GetMember(hArrObjs, i);
		EXHANDLE hObj = (EXHANDLE)__get_int(pInfo, 0);
		if (hObj == 0) continue;
		RECT rcObj{ 0 };
		Ex_ObjGetRect(hObj, &rcObj);
		void* pPosInfo = Ex_MemAlloc(4 * 3 * 4 + sizeof(size_t) + 4);//多一个放pInfo和orgFlags
		//[是否确定/类型/句柄或坐标]*4个边界,确定以后一定是坐标
		bool fNoPosInfoH = true;
		for (int j = 1; j <= 10; j += 2)
		{
			if (__get_int(pInfo, j * 4) != 0)
			{
				fNoPosInfoH = false;
				break;
			}
		}
		bool fNoPosInfoV = true;
		for (int j = 2; j <= 10; j += 2)
		{
			if (__get_int(pInfo, j * 4) != 0)
			{
				fNoPosInfoV = false;
				break;
			}
		}
		if (fNoPosInfoH)
		{
			__set_int(pPosInfo, 0, 1);
			__set_int(pPosInfo, 4, 0);
			__set_int(pPosInfo, 8, rcObj.left + __get_int(pInfo, ELCP_MARGIN_LEFT * 4));
			__set_int((void*)((size_t)pPosInfo + 24), 0, 1);
			__set_int((void*)((size_t)pPosInfo + 24), 4, 0);
			__set_int((void*)((size_t)pPosInfo + 24), 8, rcObj.right + __get_int(pInfo, ELCP_MARGIN_LEFT * 4) + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4));
		}
		if (fNoPosInfoV)
		{
			__set_int((void*)((size_t)pPosInfo + 12), 0, 1);
			__set_int((void*)((size_t)pPosInfo + 12), 4, 0);
			__set_int((void*)((size_t)pPosInfo + 12), 8, rcObj.top + __get_int(pInfo, ELCP_MARGIN_TOP * 4));
			__set_int((void*)((size_t)pPosInfo + 36), 0, 1);
			__set_int((void*)((size_t)pPosInfo + 36), 4, 0);
			__set_int((void*)((size_t)pPosInfo + 36), 8, rcObj.bottom + __get_int(pInfo, ELCP_MARGIN_TOP * 4) + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4));
		}
		//rcObj暂时按左顶宽高处理
		rcObj.right = rcObj.right - rcObj.left;
		rcObj.bottom = rcObj.bottom - rcObj.top;
		int orgFlags = 0;
		//预处理边界定位信息
		std::vector<size_t> Infos(3);
		if (fNoPosInfoH == false)
		{
			Infos[0] = 1;
			if (__get_int(pInfo, ELCP_RELATIVE_LEFT_ALIGN_OF * 4) != 0)
			{
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_LEFT_ALIGN_OF * 4);
				Infos[1] = ELCP_RELATIVE_LEFT_ALIGN_OF;
				EXHANDLE hEXDUI = 0;
				if (dwValue == -1)//相对父
				{
					Infos[2] = rcClient.left + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
				}
				else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.right;
				}
			}
			else if (__get_int(pInfo, ELCP_RELATIVE_CENTER_PARENT_H * 4) != 0)
			{
				Infos[1] = ELCP_RELATIVE_CENTER_PARENT_H;
				Infos[2] = rcClient.left + (rcClient.right - rcClient.left - rcObj.right) / 2 + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
				orgFlags = orgFlags | 4;
			}
			else {
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_RIGHT_OF * 4);
				EXHANDLE hEXDUI = 0;
				if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[1] = ELCP_RELATIVE_RIGHT_OF;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.right;
				}
			}
			RtlMoveMemory(pPosInfo, Infos.data(), 12);
			//右边界处理
			Infos[0] = 1;
			if (__get_int(pInfo, ELCP_RELATIVE_RIGHT_ALIGN_OF * 4) != 0)
			{
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_RIGHT_ALIGN_OF * 4);
				Infos[1] = ELCP_RELATIVE_RIGHT_ALIGN_OF;
				EXHANDLE hEXDUI = 0;
				if (dwValue == -1)//相对父
				{
					Infos[2] = rcClient.right + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
				}
				else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.right;
				}
			}
			else if (__get_int(pInfo, ELCP_RELATIVE_CENTER_PARENT_H * 4) != 0)
			{
				Infos[1] = ELCP_RELATIVE_CENTER_PARENT_H;
				Infos[2] = rcClient.left + (rcClient.right - rcClient.left + rcObj.right) / 2 + __get_int(pInfo, ELCP_MARGIN_LEFT * 4) + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
				orgFlags = orgFlags | 4;
			}
			else {
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_LEFT_OF * 4);
				EXHANDLE hEXDUI = 0;
				if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[1] = ELCP_RELATIVE_LEFT_OF;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.right;
				}
			}
			RtlMoveMemory((void*)((size_t)pPosInfo + 2 * 12), Infos.data(), 12);
		}
		if(fNoPosInfoV == false)
		{
			//上边界处理
			Infos[0] = 1;
			if (__get_int(pInfo, ELCP_RELATIVE_TOP_ALIGN_OF * 4) != 0)
			{
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_TOP_ALIGN_OF * 4);
				Infos[1] = ELCP_RELATIVE_TOP_ALIGN_OF;
				EXHANDLE hEXDUI = 0;
				if (dwValue == -1)//相对父
				{
					Infos[2] = rcClient.top;
				}
				else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.bottom;
				}
			}
			else if (__get_int(pInfo, ELCP_RELATIVE_CENTER_PARENT_V * 4) != 0)
			{
				Infos[1] = ELCP_RELATIVE_CENTER_PARENT_V;
				Infos[2] = rcClient.top + (rcClient.bottom - rcClient.top - rcObj.bottom) / 2 + __get_int(pInfo, ELCP_MARGIN_TOP * 4);
				orgFlags = orgFlags | 8;
			}
			else {
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_BOTTOM_OF * 4);
				EXHANDLE hEXDUI = 0;
				if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[1] = ELCP_RELATIVE_BOTTOM_OF;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.bottom;
				}
			}
			RtlMoveMemory((void*)((size_t)pPosInfo+1*12), Infos.data(), 12);
			//下边界处理
			Infos[0] = 1;
			if (__get_int(pInfo, ELCP_RELATIVE_BOTTOM_ALIGN_OF * 4) != 0)
			{
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_BOTTOM_ALIGN_OF * 4);
				Infos[1] = ELCP_RELATIVE_BOTTOM_ALIGN_OF;
				EXHANDLE hEXDUI = 0;
				if (dwValue == -1)//相对父
				{
					Infos[2] = rcClient.bottom+__get_int(pInfo, ELCP_MARGIN_TOP*4)- __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
				}
				else if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.bottom;
				}
			}
			else if (__get_int(pInfo, ELCP_RELATIVE_CENTER_PARENT_V * 4) != 0)
			{
				Infos[1] = ELCP_RELATIVE_CENTER_PARENT_V;
				Infos[2] = rcClient.top + (rcClient.bottom - rcClient.top + rcObj.bottom) / 2 + __get_int(pInfo, ELCP_MARGIN_TOP * 4) + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
				orgFlags = orgFlags | 8;
			}
			else {
				int dwValue = __get_int(pInfo, ELCP_RELATIVE_TOP_OF * 4);
				EXHANDLE hEXDUI = 0;
				if (dwValue != 0 && (hObjParent == Ex_ObjGetParentEx(dwValue, &hEXDUI)) || hObjParent == hEXDUI)//同层组件
				{
					Infos[0] = 0;
					Infos[1] = ELCP_RELATIVE_TOP_OF;
					Infos[2] = dwValue;
				}
				else {
					Infos[0] = 0;
					Infos[1] = 0;
					Infos[2] = rcObj.bottom;
				}
			}
			RtlMoveMemory((void*)((size_t)pPosInfo + 3 * 12), Infos.data(), 12);
		}
		__set(pPosInfo, 48, (size_t)pInfo);
		__set_int(pPosInfo, 48 + sizeof(size_t), orgFlags);
		HashTable_Set(hHashPosInfos, hObj, (size_t)pPosInfo);
	}
	std::vector<size_t> Infos(3);
	for (int index = 0; index < 5; index++)
	{
		int cNoLockObj = HashTable_GetCounts(hHashPosInfos);
		for (int i = 0; i < Array_GetCount(hArrObjs); i++)
		{
			void* pInfo=(void*)Array_GetMember(hArrObjs, i);
			EXHANDLE hObj = __get_int(pInfo, 0);
			void* pPosInfo = nullptr;
			HashTable_Get(hHashPosInfos, hObj,(size_t*) &pPosInfo);
			if (pPosInfo != 0)
			{
				//找能确定的点
				if(__get_int(pPosInfo, 0) == 0)
				{
					RtlMoveMemory(Infos.data(), pPosInfo, 12);
					if (Infos[1] == ELCP_RELATIVE_RIGHT_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 2 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 2 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == ELCP_RELATIVE_LEFT_ALIGN_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 0 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 0 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == 0)
					{
						if (__get_int(pPosInfo, 2 * 12) != 0)//已经锁定了
						{
							Infos[2] = __get_int(pPosInfo, 2 * 12 + 8)-Infos[2] - __get_int(pInfo, ELCP_MARGIN_RIGHT * 4) - __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
							Infos[0] = 1;
						}
					}
					RtlMoveMemory(pPosInfo, Infos.data(), 12);
				}
				if (__get_int((void*)((size_t)pPosInfo + 12), 0) == 0)
				{
					RtlMoveMemory(Infos.data(), (void*)((size_t)pPosInfo + 12), 12);
					if (Infos[1] == ELCP_RELATIVE_BOTTOM_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 3 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 3* 12 + 8) + __get_int(pInfo, ELCP_MARGIN_TOP * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == ELCP_RELATIVE_TOP_ALIGN_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 1* 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 1 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_TOP * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == 0)
					{
						if (__get_int(pPosInfo, 3 * 12) != 0)//已经锁定了
						{
							Infos[2] = __get_int(pPosInfo, 3 * 12 + 8) - Infos[2] - __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4) - __get_int(pInfo, ELCP_MARGIN_TOP * 4);
							Infos[0] = 1;
						}
					}
					RtlMoveMemory((void*)((size_t)pPosInfo + 12), Infos.data(), 12);
				}
				if (__get_int((void*)((size_t)pPosInfo + 12*2), 0) == 0)
				{
					RtlMoveMemory(Infos.data(), (void*)((size_t)pPosInfo + 12*2), 12);
					if (Infos[1] == ELCP_RELATIVE_LEFT_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 0 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 0 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == ELCP_RELATIVE_RIGHT_ALIGN_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 2 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 2 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == 0)
					{
						if (__get_int(pPosInfo, 0 * 12) != 0)//已经锁定了
						{
							Infos[2] = __get_int(pPosInfo, 0 * 12 + 8) +Infos[2] + __get_int(pInfo, ELCP_MARGIN_LEFT * 4) + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
							Infos[0] = 1;
						}
					}
					RtlMoveMemory((void*)((size_t)pPosInfo + 2*12), Infos.data(), 12);
				}
				if (__get_int((void*)((size_t)pPosInfo + 12 * 3), 0) == 0)
				{
					RtlMoveMemory(Infos.data(), (void*)((size_t)pPosInfo + 12 * 3), 12);
					if (Infos[1] == ELCP_RELATIVE_TOP_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 1 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 1* 12 + 8) + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == ELCP_RELATIVE_BOTTOM_ALIGN_OF)
					{
						void* pInfoOther = nullptr;
						if (HashTable_Get(hHashPosInfos, Infos[2], (size_t*)&pInfoOther))
						{
							if (__get_int(pInfoOther, 3 * 12) != 0)//已经锁定了
							{
								Infos[2] = __get_int(pInfoOther, 3 * 12 + 8) + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
								Infos[0] = 1;
							}
						}
					}
					else if (Infos[1] == 0)
					{
						if (__get_int(pPosInfo, 1 * 12) != 0)//已经锁定了
						{
							Infos[2] = __get_int(pPosInfo, 1* 12 + 8) + Infos[2] + __get_int(pInfo, ELCP_MARGIN_TOP * 4) + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
							Infos[0] = 1;
						}
					}
					RtlMoveMemory((void*)((size_t)pPosInfo + 3 * 12), Infos.data(), 12);
				}
				if (__get_int(pInfo, 0) != 0 && __get_int(pInfo, 12) != 0 && __get_int(pInfo, 24) != 0 && __get_int(pInfo, 36) != 0)//已经确定整个组件了
				{
					cNoLockObj = cNoLockObj - 1;
				}
			}
			else{
				cNoLockObj = cNoLockObj - 1;
			}
		}
		if (cNoLockObj <= 0)
		{
			break;
		}
	}

	std::vector<size_t> pInfos;
	std::vector<size_t> hObjs;
	HashTable_GetAllKeysAndValues(hHashPosInfos, hObjs, pInfos);
	for (int i = 0; i < hObjs.size(); i++)
	{
		EXHANDLE hObj= hObjs[i];
		void* pPosInfo = (void*)pInfos[i];
		void* pInfo = (void*)__get(pPosInfo, 48);
		int orgFlags = __get_int(pPosInfo, 48 + sizeof(size_t));
		RECT rcObj{ 0 };
		Ex_ObjGetRect(hObj, &rcObj);
		RECT rcTmp{ 0 };
		if (__get_int((void*)((size_t)pPosInfo + 0 * 12), 0) == 0)
		{
			rcTmp.left = rcObj.left;
		}
		else {
			rcTmp.left = __get_int((void*)((size_t)pPosInfo + 0 * 12), 8);
		}
		if (__get_int((void*)((size_t)pPosInfo + 1 * 12), 0) == 0)
		{
			rcTmp.top = rcObj.top;
		}
		else {
			rcTmp.top = __get_int((void*)((size_t)pPosInfo + 1 * 12), 8);
		}
		if (__get_int((void*)((size_t)pPosInfo + 2 * 12), 0) == 0)
		{
			rcTmp.right = rcTmp.left + rcObj.right - rcObj.left;
		}
		else {
			rcTmp.right = __get_int((void*)((size_t)pPosInfo + 2 * 12), 8);
		}
		if (__get_int((void*)((size_t)pPosInfo + 3 * 12), 0) == 0)
		{
			rcTmp.bottom = rcTmp.top + rcObj.bottom - rcObj.top;
		}
		else {
			rcTmp.bottom = __get_int((void*)((size_t)pPosInfo + 3 * 12), 8);
		}
		_layout_move_margin(hObj, &rcTmp, (void*)((size_t)pInfo - 16), 15, orgFlags);
	}
	HashTable_Destroy(hHashPosInfos);
}

size_t __layout_absolute_proc(layout_s* pLayout, int nEvent, size_t wParam, size_t lParam)
{
	if (nEvent == ELN_GETPROPSCOUNT)
	{
		return 0;
	}
	else if (nEvent == ELN_GETCHILDPROPCOUNT)
	{
		return 16;
	}
	else if (nEvent == ELN_UPDATE)
	{
		RECT rcClient{ 0 };
		if (pLayout->nBindType_ == HT_OBJECT)
		{
			Ex_ObjGetClientRect(wParam, &rcClient);
		}
		else {
			Ex_DUIGetClientRect(wParam, &rcClient);
		}
		void* pInfoa = pLayout->lpLayoutInfo_;
		array_s* hArr = pLayout->hArrChildrenInfo_;
		rcClient.left = rcClient.left + __get_int(pInfoa, ELP_PADDING_LEFT * (size_t)4);
		rcClient.top = rcClient.top + __get_int(pInfoa, ELP_PADDING_TOP * (size_t)4);
		rcClient.right = rcClient.right + __get_int(pInfoa, ELP_PADDING_RIGHT * (size_t)4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfoa, ELP_PADDING_BOTTOM * (size_t)4);
		SIZE szClient{ 0 };
		szClient.cx = rcClient.right - rcClient.left;
		szClient.cy = rcClient.bottom - rcClient.top;
		for (int i = 0; i < Array_GetCount(hArr); i++)
		{
			void* pInfo = (void*)Array_GetMember(hArr, i);
			EXHANDLE hObj = __get_int(pInfo, 0);
			if (hObj == 0) continue;
			RECT rcTmp{ 0 };
			Ex_ObjGetRect(hObj, &rcTmp);
			int ary1 = 0;
			int ary2 = 0;
			int ary3 = 0;
			int ary4 = 0;
			int nType = __get_int(pInfo, ELCP_ABSOLUTE_WIDTH_TYPE * 4);
			int tmp= __get_int(pInfo, ELCP_ABSOLUTE_WIDTH * 4);
			SIZE szObj{ 0 };
			if (nType == ELCP_ABSOLUTE_TYPE_PS)
			{
				szObj.cx = tmp / 100 * szClient.cx;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_PX)
			{
				szObj.cx = tmp;
			}
			else {
				szObj.cx = rcTmp.right - rcTmp.left;
			}
			nType = __get_int(pInfo, ELCP_ABSOLUTE_HEIGHT_TYPE * 4);
			tmp = __get_int(pInfo, ELCP_ABSOLUTE_HEIGHT * 4);
			if (nType == ELCP_ABSOLUTE_TYPE_PS)
			{
				szObj.cy = tmp / 100 * szClient.cy;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_PX)
			{
				szObj.cy = tmp;
			}
			else {
				szObj.cy = rcTmp.bottom - rcTmp.top;
			}

			RECT rcObj{ 0 };
			for (int index = 0; index < 5; index++)
			{
				if (ary1 == 0)
				{
					ary1 = 1;
					nType= __get_int(pInfo, ELCP_ABSOLUTE_LEFT_TYPE * 4);
					tmp= __get_int(pInfo, ELCP_ABSOLUTE_LEFT * 4);
					if (nType == ELCP_ABSOLUTE_TYPE_PS)
					{
						rcObj.left = rcClient.left + tmp / 100 + szClient.cx;
					}
					else if (nType == ELCP_ABSOLUTE_TYPE_PX)
					{
						rcObj.left = rcClient.left + tmp;
					}
					else if (ary3 == 1)
					{
						rcObj.left = rcObj.right - szObj.cx;
					}
					else {
						ary1 = 0;
					}
				}
				if (ary2 == 0)
				{
					ary2 = 1;
					nType = __get_int(pInfo, ELCP_ABSOLUTE_TOP_TYPE * 4);
					tmp = __get_int(pInfo, ELCP_ABSOLUTE_TOP * 4);
					if (nType == ELCP_ABSOLUTE_TYPE_PS)
					{
						rcObj.top = rcClient.top + tmp / 100 + szClient.cy;
					}
					else if (nType == ELCP_ABSOLUTE_TYPE_PX)
					{
						rcObj.top = rcClient.top + tmp;
					}
					else if (ary4 == 1)
					{
						rcObj.top = rcObj.bottom - szObj.cy;
					}
					else {
						ary2 = 0;
					}
				}
				if (ary3 == 0)
				{
					ary3 = 1;
					nType = __get_int(pInfo, ELCP_ABSOLUTE_RIGHT_TYPE * 4);
					tmp = __get_int(pInfo, ELCP_ABSOLUTE_RIGHT * 4);
					if (nType == ELCP_ABSOLUTE_TYPE_PS)
					{
						rcObj.right = rcClient.right - tmp / 100 + szClient.cx;
					}
					else if (nType == ELCP_ABSOLUTE_TYPE_PX)
					{
						rcObj.right = rcClient.right - tmp;
					}
					else if (ary1 == 1)
					{
						rcObj.right = rcObj.left + szObj.cx;
					}
					else {
						ary3 = 0;
					}
				}
				if (ary4 == 0)
				{
					ary4 = 1;
					nType = __get_int(pInfo, ELCP_ABSOLUTE_BOTTOM_TYPE * 4);
					tmp = __get_int(pInfo, ELCP_ABSOLUTE_BOTTOM * 4);
					if (nType == ELCP_ABSOLUTE_TYPE_PS)
					{
						rcObj.bottom = rcClient.bottom - tmp / 100 + szClient.cy;
					}
					else if (nType == ELCP_ABSOLUTE_TYPE_PX)
					{
						rcObj.bottom = rcClient.bottom -tmp;
					}
					else if (ary2 == 1)
					{
						rcObj.bottom = rcObj.top + szObj.cy;
					}
					else {
						ary4 = 0;
					}
				}
				if (ary1 == 1 && ary2 == 1 && ary3 == 1 && ary4 == 1)
				{
					break;
				}
			}
			if (ary1 == 0)
			{
				rcObj.left = rcTmp.left;
			}
			if (ary2 == 0)
			{
				rcObj.top = rcTmp.top;
			}
			if (ary3 == 0)
			{
				rcObj.right = rcTmp.right;
			}
			if (ary4 == 0)
			{
				rcObj.bottom = rcTmp.bottom;
			}
			nType = __get_int(pInfo, ELCP_ABSOLUTE_OFFSET_H_TYPE * 4);
			tmp = __get_int(pInfo, ELCP_ABSOLUTE_OFFSET_H * 4);
			if (nType == ELCP_ABSOLUTE_TYPE_PS)
			{
				rcObj.left = rcObj.left + tmp / 100 * szClient.cx;
				rcObj.right = rcObj.right + tmp / 100 * szClient.cx;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_PX)
			{
				rcObj.left = rcObj.left + tmp ;
				rcObj.right = rcObj.right + tmp ;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_OBJPS)
			{
				rcObj.left = rcObj.left + tmp / 100 * szObj.cx;
				rcObj.right = rcObj.right + tmp / 100 * szObj.cx;
			}

			nType = __get_int(pInfo, ELCP_ABSOLUTE_OFFSET_V_TYPE * 4);
			tmp = __get_int(pInfo, ELCP_ABSOLUTE_OFFSET_V * 4);
			if (nType == ELCP_ABSOLUTE_TYPE_PS)
			{
				rcObj.top = rcObj.top + tmp / 100 * szClient.cy;
				rcObj.bottom = rcObj.bottom + tmp / 100 * szClient.cy;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_PX)
			{
				rcObj.top = rcObj.top + tmp;
				rcObj.bottom = rcObj.bottom + tmp;
			}
			else if (nType == ELCP_ABSOLUTE_TYPE_OBJPS)
			{
				rcObj.top = rcObj.top + tmp / 100 * szObj.cy;
				rcObj.bottom = rcObj.bottom + tmp / 100 * szObj.cy;
			}
			_layout_move_margin(hObj, &rcObj, (void*)((size_t)pInfo - 16), 15,0);
		}
	}
	else if (nEvent == ELN_CHECKCHILDPROPVALUE)
	{
		array_s* pChildrenInfo=pLayout->hArrChildrenInfo_;
		void* pInfo=(void*)Array_GetMember(pChildrenInfo, LOWORD(wParam));
		int nType = HIWORD(wParam);
		if (nType % 2 == 1 && nType >= ELCP_ABSOLUTE_LEFT && nType <= ELCP_ABSOLUTE_OFFSET_V)
		{
			if (__get_int(pInfo, (nType + 1) * 4) == ELCP_ABSOLUTE_TYPE_UNKNOWN)
			{
				__set_int(pInfo, (nType + 1) * 4, ELCP_ABSOLUTE_TYPE_PX);
			}
		}
		
	}
	return 0;
}

void _layout_init()
{
	_layout_register(ELT_LINEAR, &__layout_linear_proc);
	_layout_register(ELT_FLOW, &__layout_flow_proc);
	_layout_register(ELT_PAGE, &__layout_page_proc);
	_layout_register(ELT_TABLE, &__layout_table_proc);
	_layout_register(ELT_RELATIVE, &__layout_relative_proc);
	_layout_register(ELT_ABSOLUTE, &__layout_absolute_proc);
}