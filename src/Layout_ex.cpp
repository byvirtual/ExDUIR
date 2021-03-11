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