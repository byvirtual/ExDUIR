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
		void* pInfo=pLayput->lpLayoutInfo_;
		array_s* hArr=pLayput->hArrChildrenInfo_;
		int nDAlign = __get_int(pInfo, (ELP_LINEAR_DALIGN - 1) * 4);
		bool fVertical = __get_int(pInfo, (ELP_LINEAR_DIRECTION - 1) * 4) == ELP_DIRECTION_V;
		rcClient.left = rcClient.left + __get_int(pInfo, ELP_PADDING_LEFT * 4);
		rcClient.top = rcClient.top + __get_int(pInfo, ELP_PADDING_TOP * 4);
		rcClient.right = rcClient.right + __get_int(pInfo, ELP_PADDING_RIGHT * 4);
		rcClient.bottom = rcClient.bottom + __get_int(pInfo, ELP_PADDING_BOTTOM* 4);
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
			int orgFlags = 0;
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
					rcObj.bottom = rcObj.top + rcObj.bottom-rcTmp.top;
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
					rcObj.top = rcClient.bottom - (rcTmp.bottom-rcTmp.top);
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
				rcObj.left - rcObj.left + __get_int(pInfo, ELCP_MARGIN_LEFT * 4);
				rcObj.top - rcObj.top + __get_int(pInfo, ELCP_MARGIN_TOP * 4);
				rcObj.right - rcObj.right + __get_int(pInfo, ELCP_MARGIN_RIGHT * 4);
				rcObj.bottom - rcObj.bottom + __get_int(pInfo, ELCP_MARGIN_BOTTOM * 4);
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
			void* pInfo=pLayput->lpLayoutInfo_;
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
				void* pInfo=(void*)Array_GetMember(hArr, i);
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