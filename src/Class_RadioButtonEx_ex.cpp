#include "Class_RadioButtonEx_ex.h"

ClsPROC m_pfnRadioButtonProc;/*控件基类的消息回调函数*/

void _RadioButtonEx_register()
{
	EX_CLASSINFO	pClsInfoRadioButton;

	/* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

	 /* 1、获取父类控件信息*/
	WCHAR	oldwzCls[] = L"RadioButton";
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoRadioButton);

	/* 2、保存父类控件回调函数指针*/
	m_pfnRadioButtonProc = pClsInfoRadioButton.pfnClsProc;

	/* 3、注册新控件*/
	WCHAR	newwzCls[] = L"RadioButtonEx";
	Ex_ObjRegister(newwzCls, pClsInfoRadioButton.dwStyle, pClsInfoRadioButton.dwStyleEx, pClsInfoRadioButton.dwTextFormat, 0, pClsInfoRadioButton.hCursor, pClsInfoRadioButton.dwFlags, _RadioButtonEx_proc);
}


LRESULT CALLBACK _RadioButtonEx_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		/*创建时初始化控件属性*/
	case WM_CREATE:
	{
		Ex_ObjInitPropList(hObj, 4 + 1);
		
		Ex_ObjSetProp(hObj, 1, ExRGB2ARGB(16777215, 255));
		
		Ex_ObjSetProp(hObj, 2, ExARGB(0, 0, 0, 255));
		Ex_ObjSetProp(hObj, 3, ExARGB(0, 0, 0, 255));
		Ex_ObjSetProp(hObj, 4, ExARGB(0, 0, 0, 255));
	}
	/*销毁时释放资源*/
	case WM_DESTROY:
	{

	}
	case WM_PAINT:
	{
		return(_RadioButtonEx_paint(hObj));
	}
	case WM_MOUSEHOVER:
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, false, 0, true);
		break;
	}
	case  WM_MOUSELEAVE:
	{
		Ex_ObjSetUIState(hObj, STATE_HOVER, true, 0, true);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, false, 0, true);
		break;
	}
	case WM_LBUTTONUP:
	{
		Ex_ObjSetUIState(hObj, STATE_DOWN, true, 0, true);
		break;
	}
	case EOL_EX_PROPS:
	{
		EX_OBJ_PROPS* RadioButtonExprops = (EX_OBJ_PROPS*)lParam;
		Ex_ObjInitPropList(hObj, 4 + 1);
		Ex_ObjSetProp(hObj, 1, RadioButtonExprops->COLOR_EX_BACKGROUND_DOWNORCHECKED);
		Ex_ObjSetProp(hObj, 2, RadioButtonExprops->COLOR_EX_BORDER_NORMAL);
		Ex_ObjSetProp(hObj, 3, RadioButtonExprops->COLOR_EX_BORDER_HOVER);
		Ex_ObjSetProp(hObj, 4, RadioButtonExprops->COLOR_EX_BORDER_DOWNORCHECKED);
		break;
	}
	default:
		break;
	}
	return(Ex_ObjCallProc(m_pfnRadioButtonProc, hWnd, hObj, uMsg, wParam, lParam, 0));
}

int _RadioButtonEx_paint(HEXOBJ hObj)
{
	/*
	 * 定义局部变量
	 * 变量类型 变量名 = 赋值;
	 */
	EX_PAINTSTRUCT2 ps;
	RECT rcBlock = { 0 };

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		/*
		 * 定义局部变量
		 * 变量类型 变量名 = 赋值;
		 */

		HEXBRUSH hBrush = _brush_create(Ex_ObjGetProp(hObj, 2));
		EXARGB	crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL);
		if ((ps.dwState & STATE_HOVER) == STATE_HOVER)
		{
			crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_NORMAL /*COLOR_EX_TEXT_HOVER*/);
			_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, 3));
		}

		if ((Ex_ObjGetLong(hObj, EOL_STATE) & STATE_CHECKED) != 0)
		{
			_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, 4));
		}
		/* 计算文本尺寸 */
		float nTextWidth = NULL;
		float nTextHeight = NULL;
		_canvas_calctextsize(ps.hCanvas, Ex_ObjGetFont(hObj), (LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE), -1, ps.dwTextFormat, 0, (float)ps.width, (float)ps.height, &nTextWidth, &nTextHeight);


		/* 定义单选框圆角矩形 */
		rcBlock.left = ps.p_left + (long)Ex_Scale(2);
		rcBlock.top = (ps.height - (long)nTextHeight) / 2;
		rcBlock.right = rcBlock.left + (long)nTextHeight;
		rcBlock.bottom = (ps.height + (long)nTextHeight) / 2;
		/* 绘制边框[GDI模式下，圆角度需要缩减一像素] */
		_canvas_drawroundedrect(ps.hCanvas, hBrush, (float)rcBlock.left, (float)rcBlock.top, (float)rcBlock.right, (float)rcBlock.bottom, nTextHeight / 2 - 1, nTextHeight / 2 - 1, 1, D2D1_DASH_STYLE_SOLID);

		/* 定义选中色 */
		_brush_setcolor(hBrush, Ex_ObjGetProp(hObj, 1));

		if ((Ex_ObjGetLong(hObj, EOL_STATE) & STATE_CHECKED) != 0)
		{
			crText = Ex_ObjGetColor(hObj, COLOR_EX_TEXT_CHECKED);
			/* 把矩形往里缩3像素 */
			rcBlock.left = rcBlock.left + (long)Ex_Scale(3);
			rcBlock.top = rcBlock.top + (long)Ex_Scale(3);
			rcBlock.right = rcBlock.right - (long)Ex_Scale(3);
			rcBlock.bottom = rcBlock.bottom - (long)Ex_Scale(3);
			float nRadius = nTextHeight / 2 - Ex_Scale(3);
			_canvas_fillroundedrect(ps.hCanvas, hBrush, (float)rcBlock.left, (float)rcBlock.top, (float)rcBlock.right, (float)rcBlock.bottom, nRadius, nRadius);
		}

		/* 绘制组件文本 */
		_canvas_drawtext(ps.hCanvas,
			Ex_ObjGetFont(hObj),
			crText,
			(LPCWSTR)Ex_ObjGetLong(hObj, EOL_LPWZTITLE),
			-1,
			DT_LEFT | DT_VCENTER,
			(float)ps.t_left + nTextHeight + Ex_Scale(7),
			(float)ps.t_top,
			(float)ps.t_right,
			(float)ps.t_bottom);

		_brush_destroy(hBrush);
		Ex_ObjEndPaint(hObj, &ps);
	}
	return(0);

}