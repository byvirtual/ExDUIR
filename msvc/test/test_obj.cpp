#include "test_obj.h"

static EXHANDLE button1;
static EXHANDLE button2;

int button_clicked(EXHANDLE hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam)
{
	Ex_ObjEnable(hObj, false);
	auto ret=Ex_ObjSetText(button2, L"解除禁用", false);
	auto text_length = Ex_ObjGetTextLength(button2);
	output(L"改动后文本长度:", text_length);
	LPCWSTR lpString = (LPCWSTR)Ex_MemAlloc(text_length * 2 + 2);
	Ex_ObjGetText(button2, lpString, text_length * 2 + 2);
	output(L"改动后文本:", lpString);
	Ex_MemFree((void*)lpString);
	//也可以以下方式
	//std::wstring str;
	//str.resize(text_length * 2 + 2);
	//Ex_ObjGetText(button2, (void*)str.c_str(), text_length * 2);
	//output(L"文本", str);
	return 0;
}

int button_clicked2(EXHANDLE hObj, int nID, int nCode, WPARAM wParam, LPARAM lParam)
{
	//Ex_MessageBoxEx(hObj, L"解除禁用", L"提示", 0, 0, 0, 0, 0, 0);
	Ex_ObjEnable(button1, true);

	return 0;
}

void test_button(EXHANDLE hExDui)
{
	button1 = Ex_ObjCreateEx(-1, L"button", L"禁用", -1, 10, 130, 100, 30, hExDui, 0, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	Ex_ObjHandleEvent(button1, NM_CLICK, button_clicked);
	button2 = Ex_ObjCreateEx(-1, L"button", L"解除禁用测试前", -1, 10, 170, 100, 30, hExDui, 0, DT_VCENTER | DT_CENTER, 0, 0, NULL);
	
	
	
	
	
	//Ex_ObjHandleEvent(button2, NM_CLICK, button_clicked2);
}

void test_label(EXHANDLE hExDui)
{
	EXHANDLE label = Ex_ObjCreateEx(-1, L"static" , NULL, -1, 10, 90, 100, 30, hExDui, 0, DT_VCENTER, 0, 0, NULL);
	std::vector<char> imgdata;
	Ex_ReadFile(L".\\00000.jpg", &imgdata);
	Ex_ObjSetBackgroundImage(label, imgdata.data(), imgdata.size(), 0, 0, 0, 0, 0, 255, true);
	bkgimg_s bkg{ 0 };
	Ex_ObjGetBackgroundImage(label, &bkg);
	output(L"背景信息:", bkg.x_, bkg.y_,bkg.dwAlpha_,bkg.dwRepeat_,bkg.hImage_,bkg.curFrame_,bkg.maxFrame_);

}
