#include "test_obj.h"
#include "resource.h"

EXHANDLE hExDui_button;

int CALLBACK button_clicked(EXHANDLE hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nID == 201)//通过组件ID判断按钮
	{
		Ex_ObjEnable(hObj, false);//禁用自身
	}
	else if (nID == 202)
	{
		Ex_ObjEnable(Ex_ObjGetFromID(hExDui_button, 201), true);//通过组件ID取按钮句柄，解除按钮1禁用
	}
	else if (nID == 203)
	{
		Ex_ObjSetText(hObj,L"自身文本被改动", false);//改动自身文本
	}
	else if (nID == 204)
	{
		auto text_length = Ex_ObjGetTextLength(Ex_ObjGetFromID(hExDui_button, 201));//取按钮1文本长度
		std::wstring str;
		str.resize(text_length * 2 + 2);
		Ex_ObjGetText(Ex_ObjGetFromID(hExDui_button, 201),str.c_str(), text_length * 2);
		Ex_ObjSetText(hObj,(L"按钮1文本:"+ str).c_str(), false);
	}
	return 0;
}


void test_button(HWND hWnd)
{
	auto hWnd_button = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试按钮", 0, 0, 400, 300, 0, 0);
	hExDui_button = Ex_DUIBindWindowEx(hWnd_button, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_button, EWL_CRBKG, ExRGBA(120, 120, 120, 255));
	std::vector<EXHANDLE> buttons;
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"禁用自身", -1, 10, 30, 120, 30, hExDui_button, 201, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"解除按钮1禁用", -1, 10, 70, 120, 30, hExDui_button, 202, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"改动自身文本", -1, 10, 110, 120, 30, hExDui_button, 203, DT_VCENTER | DT_CENTER, 0, 0, NULL));
	buttons.push_back(Ex_ObjCreateEx(-1, L"button", L"取按钮1文本", -1, 10, 150, 120, 30, hExDui_button, 204, DT_VCENTER | DT_CENTER, 0, 0, NULL));

	for (auto button : buttons)
	{
		Ex_ObjHandleEvent(button, NM_CLICK, button_clicked);
	}
	Ex_DUIShowWindow(hExDui_button, SW_NORMAL, 0, 0, 0);
}


void test_label(HWND hWnd)
{
	auto hWnd_label = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试标签", 0, 0, 400, 300, 0, 0);
	auto hExDui_label = Ex_DUIBindWindowEx(hWnd_label, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_label, EWL_CRBKG, ExRGBA(220, 220, 220, 255));
	auto label = Ex_ObjCreateEx(-1, L"static" , NULL, -1, 10, 30, 100, 30, hExDui_label, 0, DT_VCENTER, 0, 0, NULL);
	std::vector<char> imgdata;
	Ex_ReadFile(L".\\00000.jpg", &imgdata);
	Ex_ObjSetBackgroundImage(label, imgdata.data(), imgdata.size(), 0, 0, 0, 0, 0, 255, true);
	bkgimg_s bkg{ 0 };
	Ex_ObjGetBackgroundImage(label, &bkg);
	output(L"背景信息:", bkg.x_, bkg.y_,bkg.dwAlpha_,bkg.dwRepeat_,bkg.hImage_,bkg.curFrame_,bkg.maxFrame_);
	RECT rect;
	Ex_ObjGetRect(label, &rect);
	output(L"标签矩形:", rect.right, rect.bottom);
	Ex_DUIShowWindow(hExDui_label, SW_NORMAL, 0, 0, 0);
}

void test_checkbutton(HWND hWnd)
{
	auto hWnd_checkbutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试单选框选择框", 0, 0, 400, 300, 0, 0);
	auto hExDui_checkbutton = Ex_DUIBindWindowEx(hWnd_checkbutton, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_checkbutton, EWL_CRBKG, ExRGBA(150, 150, 150, 255));

	auto checkbutton = Ex_ObjCreateEx(-1, L"checkbutton", L"选择框", -1, 10, 30, 60, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	auto radiobuttona = Ex_ObjCreateEx(-1, L"radiobutton", L"单选框1", -1, 10, 60, 60, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	auto radiobuttonb = Ex_ObjCreateEx(-1, L"radiobutton", L"单选框2", -1, 80, 60, 60, 20, hExDui_checkbutton, 0, DT_VCENTER, 0, 0, NULL);
	Ex_DUIShowWindow(hExDui_checkbutton, SW_NORMAL, 0, 0, 0);
}

void test_edit(HWND hWnd)
{
	auto hWnd_edit = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试编辑框", 0, 0, 400, 300, 0, 0);
	auto hExDui_edit = Ex_DUIBindWindowEx(hWnd_edit, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_edit, EWL_CRBKG, ExRGBA(150, 150, 150, 255));

	auto edit = Ex_ObjCreateEx(EOS_EX_FOCUSABLE | EOS_EX_COMPOSITED | EOS_EX_BLUR, L"edit", L"测试编辑框", EOS_VISIBLE | EES_HIDESELECTION, 10, 30, 100, 30, hExDui_edit, 0, DT_VCENTER, 0, 0, NULL);
	Ex_DUIShowWindow(hExDui_edit, SW_NORMAL, 0, 0, 0);
}

VOID test_menubutton(HWND hWnd)
{
	auto hWnd_menubutton = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试菜单按钮", 0, 0, 400, 300, 0, 0);
	auto hExDui_menubutton = Ex_DUIBindWindowEx(hWnd_menubutton, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_menubutton, EWL_CRBKG, ExRGBA(150, 150, 150, 255));

	EXHANDLE hObjMenuBar = Ex_ObjCreate(L"Page", 0, -1, 0, 30, 400, 22, hExDui_menubutton);
	if (hObjMenuBar != 0) {
		EXHANDLE hLayout = _layout_create(ELT_LINEAR, hObjMenuBar);
		HMENU hMenu = LoadMenu(GetModuleHandle(0), (LPWSTR)IDR_MENU1);
		if (hMenu) {
			for (int i = 0; i < GetMenuItemCount(hMenu); i++) {
				WCHAR wzText[256];
				GetMenuString(hMenu, i, wzText, 256, MF_BYPOSITION);
				EXHANDLE hObj = Ex_ObjCreateEx(-1, L"MenuButton", wzText, -1, 0, 0, 50, 22, hObjMenuBar, 0, -1, (size_t)GetSubMenu(hMenu, i), 0, 0);
				if (hObj) {
					Ex_ObjSetColor(hObj, COLOR_EX_BACKGROUND, 0, false);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_HOVER, ExRGBA(255, 255, 255, 50), false);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_DOWN, ExRGBA(255, 255, 255, 100), false);
					Ex_ObjSetColor(hObj, COLOR_EX_TEXT_NORMAL, ExRGBA(255, 255, 255, 255), false);
					_layout_addchild(hLayout, hObj);
				}
			}
		}
		Ex_ObjLayoutSet(hObjMenuBar, hLayout, true);
	}
	Ex_DUIShowWindow(hExDui_menubutton, SW_NORMAL, 0, 0, 0);
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
				output(L"改变高度:", __get_int((void*)ni.lParam, 4));
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
				output(L"改变选中ID:", ni.idFrom, L"新选中项:", ni.wParam, L"旧选中项:", ni.lParam);
			}
		}
	}

	return 0;
}

void test_listview(HWND hWnd)
{
	auto hWnd_listview = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试列表框", 0, 0, 400, 300, 0, 0);
	auto hExDui_listview = Ex_DUIBindWindowEx(hWnd_listview, 0, EWS_BUTTON_CLOSE | EWS_BUTTON_MIN | EWS_MOVEABLE | EWS_CENTERWINDOW | EWS_TITLE | EWS_SIZEABLE | EWS_HASICON, 0, 0);
	Ex_DUISetLong(hExDui_listview, EWL_CRBKG, ExRGBA(150, 150, 150, 255));

	auto listview = Ex_ObjCreateEx(EOS_EX_COMPOSITED | EOS_EX_BLUR, L"listview", NULL, EOS_VISIBLE  | ELS_VERTICALLIST | EOS_VSCROLL, 130, 30, 150, 200, hExDui_listview, 0, -1, 0, 0, list_proc);
	Ex_ObjSetColor(listview, COLOR_EX_BACKGROUND, ExRGBA(255, 255, 255, 150), true);
	Ex_ObjSendMessage(listview, LVM_SETITEMCOUNT, 100, 100);
	Ex_DUIShowWindow(hExDui_listview, SW_NORMAL, 0, 0, 0);
}