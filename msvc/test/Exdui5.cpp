﻿#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "help_ex.h"

#include "test_obj.h"

void 测试哈希表()
{
	auto aptr = LocalAlloc(64, sizeof(void*));
	hashtable_s* table = HashTable_Create(17, &pfnDefaultFreeData);
	auto aptr2 = LocalAlloc(64, sizeof(void*));
	HashTable_Set(table, 1, (size_t)aptr);
	HashTable_Set(table, 8, (size_t)aptr2);
	size_t ret = 0;
	std::cout << (size_t)aptr2 << std::endl;
	HashTable_Get(table, 8, &ret);
	std::cout << ret << std::endl;
	std::vector<size_t> arry_key;
	std::vector<size_t> arry_value ;
	HashTable_GetAllKeysAndValues(table, arry_key, arry_value);
	std::cout << arry_key[1] << std::endl;
	std::cout << arry_value[1] << std::endl;
	//std::cout << "11"<< std::endl;

	HashTable_Destroy(table);

}

void 数组遍历()
{
	std::vector<int> aryColorsAtom = { ATOM_BACKGROUND_COLOR, ATOM_COLOR_BACKGROUND, ATOM_BORDER_COLOR, ATOM_COLOR_BORDER, ATOM_COLOR, ATOM_COLOR_NORMAL, ATOM_COLOR_HOVER, ATOM_COLOR_DOWN, ATOM_COLOR_FOCUS, ATOM_COLOR_CHECKED, ATOM_COLOR_SELECTED, ATOM_COLOR_HOT, ATOM_COLOR_VISTED, ATOM_COLOR_SHADOW };

	std::for_each(aryColorsAtom.cbegin(), aryColorsAtom.cend(), [](const int& val)->void {std::cout << val << std::endl; });
	for (auto iter = aryColorsAtom.cbegin(); iter != aryColorsAtom.cend(); iter++)
	{
		//std::cout << (*iter) << std::endl;
	}
	auto result = std::find(aryColorsAtom.begin(), aryColorsAtom.end(), ATOM_COLOR_BORDER); //查找3

	if (result != aryColorsAtom.end()) //找到
	{
		auto index = std::distance(aryColorsAtom.begin(), result);
		std::cout << "Yes" << (int)index << std::endl;

	}
	else //没找到
	{
		std::cout << "No" << std::endl;
	}


	std::cout << aryColorsAtom.size() << std::endl;
}


bool 枚举数组(array_s* pArray, int nIndex, void* pvItem, int nType, size_t pvParam)
{
	std::cout << "句柄:" << pArray << std::endl;
	std::cout << "索引:" << nIndex << std::endl;
	std::cout << "项目句柄:" << pvItem << std::endl;
	std::cout << "类型:" << nType << std::endl;
	std::cout << "回调参数:" << pvParam << std::endl;
	return (size_t)pvItem == pvParam;
}

void 测试数组()
{
	array_s* aa = Array_Create(5);
	Array_AddMember(aa, 6);
	Array_SetMember(aa, 2, 3);
	std::cout << Array_GetMember(aa, 2) << std::endl;//3
	Array_DelMember(aa, 1);
	std::cout << Array_GetMember(aa, 1) << std::endl;//3删除成员后索引往前进1,原来索引是2
	Array_SetMember(aa, 1, 4);
	std::cout << Array_GetMember(aa, 1) << std::endl;//4修改成员
	std::cout << Array_GetCount(aa) << std::endl;//5
	Array_Redefine(aa, 6, false);
	std::cout << Array_GetCount(aa) << std::endl;//6
	Array_SetExtra(aa, 66);
	std::cout << Array_GetExtra(aa) << std::endl;//66
	Array_Emum(aa, &枚举数组, 4);
	Array_Destroy(aa);
}

void 测试句柄池()
{
	g_Li.hHandles = _handle_init();
	auto bb = Ex_MemAlloc(sizeof(img_s));
	int nError = 0;
	std::cout << "bb:" << bb << std::endl;
	auto cc = _handle_create(HT_IMAGE, bb, &nError);
	std::cout << "cc:" << cc << std::endl;
	std::cout << "nError:" << nError << std::endl;
	void* dd;

	std::cout << _handle_validate(cc, HT_IMAGE, &dd, &nError) << std::endl;
	std::cout << "nError:" << nError << std::endl;
	std::cout << "dd:" << dd << std::endl;
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	_handle_destroy(cc, &nError);
	_handle_uninit(g_Li.hHandles);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	Ex_MemFree(bb);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
}


void 测试RC4()
{
	unsigned char A[1000] = { 97,98,99,100,101,102 };
	unsigned char key[256] = { 97,98,99,100,101,102 };
	//unsigned char A[1000] = "abcdef";
	//unsigned char key[256] = "abcdef";
	RC4(A, 6, key, 6);
	for (int index = 0; index < 6; index++)
	{
		std::cout << "A= " << (int)A[index] << std::endl;
	}
	std::cout << "M = " << ToHexString(A, 6) << std::endl;
}


size_t msgProc(HWND, EXHANDLE handle, UINT, size_t, size_t, void*)
{
	return 0;
}

int list_proc(HWND hWnd, EXHANDLE hObj, UINT uMsg, size_t wParam, size_t lParam, int* lpResult)
{
	if (uMsg == WM_NOTIFY)
	{
		EX_NMHDR ni{ 0 };
		RtlMoveMemory(&ni, (void*)lParam, sizeof(EX_NMHDR));
		if (hObj == ni.hObjFrom)
		{
			
			if (ni.nCode == NM_CALCSIZE)
			{
				output(L"改变高度:",__get_int((void*)ni.lParam, 4));
				__set_int((void*)ni.lParam, 4, 25);
				
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == NM_CUSTOMDRAW)
			{
				
				EX_CUSTOMDRAW cd{ 0 };
				RtlMoveMemory(&cd, (void*)ni.lParam, sizeof(EX_CUSTOMDRAW));
				if (cd.iItem > 0 && cd.iItem <= 100)
				{
					
					int crItemBkg = 0;
					if ((cd.dwState & STATE_SELECT) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 255);
					}
					else if ((cd.dwState & STATE_HOVER) != 0)
					{
						crItemBkg = ExRGB2ARGB(16777215, 150);
					}
					if (crItemBkg != 0)
					{
						void* hBrush = _brush_create(crItemBkg);
						_canvas_fillrect(cd.hCanvas, hBrush, cd.rcDraw.left, cd.rcDraw.top, cd.rcDraw.right, cd.rcDraw.bottom);
						_brush_destroy(hBrush);
					}
					_canvas_drawtext(cd.hCanvas, Ex_ObjGetFont(hObj), ExRGB2ARGB(0, 180), L"你好123", -1, DT_SINGLELINE | DT_VCENTER, cd.rcDraw.left + 10, cd.rcDraw.top, cd.rcDraw.right, cd.rcDraw.bottom);
				}
				*lpResult = 1;
				return 1;
			}
			else if (ni.nCode == LVN_ITEMCHANGED)
			{
				//wParam 新选中项,lParam 旧选中项
				output(L"改变选中ID:",  ni.idFrom,L"新选中项:", ni.wParam, L"旧选中项:", ni.lParam);
			}
		}
	}

	return 0;
}




void 测试窗口()
{
	std::vector<char> data;
	Ex_ReadFile(L".\\Default.ext", &data);
	Ex_Init(GetModuleHandleW(NULL), EXGF_RENDER_METHOD_D2D | EXGF_DPI_ENABLE, 0, 0, data.data(), data.size(), 0, 0);
	LPCWSTR class_wnd = L"Ex_DirectUI";
	Ex_WndRegisterClass(class_wnd, 0, 0, 0);
	LPCWSTR title = L"test";
	HWND hWnd = Ex_WndCreate(0, class_wnd, title, 0, 0, 800, 600, 0, 0);
	if (hWnd != 0)
	{
		size_t hExDui = Ex_DUIBindWindowEx(hWnd, 0, EWS_MAINWINDOW | EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_BUTTON_MAX | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_ESCEXIT | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, msgProc);
		//Ex_DUISetLong(hExDui, EWL_CRBKG, -100630528);//-97900239
		std::vector<char> imgdata;
		Ex_ReadFile(L".\\bkg.png", &imgdata);
		Ex_ObjSetBackgroundImage(hExDui, imgdata.data(), imgdata.size(), 0, 0, BIR_DEFALUT, 0, BIF_PLAYIMAGE, 255,true);
		test_button(hExDui);
		test_label(hExDui);
		//单选框选择框
		LPCWSTR class_checkbutton = L"checkbutton";
		EXHANDLE checkbutton = Ex_ObjCreateEx(-1, class_checkbutton, title, -1, 10, 30, 60, 20, hExDui, 0, DT_VCENTER, 0, 0, NULL);

		LPCWSTR class_radiobutton = L"radiobutton";
		EXHANDLE radiobuttona = Ex_ObjCreateEx(-1, class_radiobutton, title, -1, 10, 60, 60, 20, hExDui, 0, DT_VCENTER, 0, 0, NULL);
		EXHANDLE radiobuttonb = Ex_ObjCreateEx(-1, class_radiobutton, title, -1, 80, 60, 60, 20, hExDui, 0, DT_VCENTER, 0, 0, NULL);
		
		
		

		//编辑框
		LPCWSTR class_edit = L"edit";
		EXHANDLE edit = Ex_ObjCreateEx(EOS_EX_FOCUSABLE, class_edit, title, EOS_VISIBLE | EES_HIDESELECTION, 10, 210, 100, 30, hExDui, 0, DT_VCENTER, 0, 0, NULL);

		//列表框
		LPCWSTR class_list = L"listview";
		EXHANDLE listview = Ex_ObjCreateEx(-1, class_list, title, EOS_VISIBLE  | ELS_VERTICALLIST | EOS_VSCROLL, 130, 30, 150, 200, hExDui, 0, -1, 0, 0, &list_proc);
		Ex_ObjSetColor(listview, COLOR_EX_BACKGROUND, ExRGB2ARGB(16711680, 50), true);
		Ex_ObjSendMessage(listview, LVM_SETITEMCOUNT, 100, 100);

		
		//信息框
		Ex_DUIShowWindow(hExDui, 5, 0, 0, 0);
	}
	Ex_WndMsgLoop();
	Ex_UnInit();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPWSTR wzCmd, _In_ int nCmdShow)
{
	//测试句柄池();
	//数组遍历();
	//测试哈希表();
	//测试数组();
	//测试RC4();
	测试窗口();
	return 0;
}




