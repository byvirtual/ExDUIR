#include "Canvas_ex.h"

bool _canvas_destroy(ExHandle hCanvas)
{
	int nError = 0;
	void* pCanvas = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* bmp = _cv_dx_bmp(pCanvas);
		((ID2D1Bitmap1*)bmp)->Release();
		Ex_MemFree(pCanvas);
		_handle_destroy(hCanvas, &nError);
	}
	return nError == 0;
}

void _canvas_recreate(void* pCanvas, int width, int height, int* nError)
{

	if (width == ((canvas_s*)pCanvas)->width_ && height == ((canvas_s*)pCanvas)->height_) return;
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;
	((canvas_s*)pCanvas)->width_ = width;
	((canvas_s*)pCanvas)->height_ = height;
	void* pWnd = ((canvas_s*)pCanvas)->pWnd_;
	// TODO: gdi support

	bool fGDI = __query(pCanvas, offsetof(canvas_s, dwFlags_), CVF_GDI_COMPATIBLE) or Flag_Query(EXGF_RENDER_METHOD_D2D_GDI_COMPATIBLE);
	void* pBitmap = _dx_createbitmap(((wnd_s*)pWnd)->dx_context_, width, height, fGDI, nError);

	if (pBitmap != 0)
	{
		void* oldBitmap = ((canvas_s*)pCanvas)->pBitmap_;
		if (oldBitmap != 0)
		{
			((ID2D1Bitmap1*)oldBitmap)->Release();
		}
		((canvas_s*)pCanvas)->pBitmap_ = pBitmap;
	}
}

bool _canvas_resize(ExHandle hCanvas, int width, int height)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{

		_canvas_recreate(pCanvas, width, height, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _canvas_init(int* nError)
{
	bool bDX = false;

	//����GdiplusDLL();

#if defined(_M_IX86)
	char iid[16] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#elif defined(_M_AMD64)
	char iid[32] = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#endif
	//GdiplusStartup(&g_Ri.hToken, iid, NULL);
	//GdipCreateMatrix(&g_Ri.pMatrix);
	*nError = CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, &g_Ri.pWICFactory);
	if (*nError == 0)
	{
		bDX = _dx_init(nError);
	}
	if (!bDX)
	{
		_dx_uninit();
		Flag_Del(EXGF_RENDER_METHOD_D2D);
	}
	g_Li.pfnUpdateLayeredWindowIndirect = (UpdateLayeredWindowIndirectPROC)GetProcAddr(L"user32.dll", "UpdateLayeredWindowIndirect");
	nError = 0;
}

void _canvas_uninit()
{
	_dx_uninit();
	((IWICImagingFactory*)g_Ri.pWICFactory)->Release();
	//GdipDeleteMatrix(g_Ri.pMatrix);
	//GdiplusShutdown(g_Ri.hToken);
}

void* _cv_dx_bmp(void* pCanvas)
{

	return ((canvas_s*)pCanvas)->pBitmap_;
}

void* _cv_context(void* pCanvas)
{
	return ((canvas_s*)pCanvas)->pContext_;
}

void* _cv_dx_gdiinterop(void* pCanvas)
{
	return ((canvas_s*)pCanvas)->pGdiInterop_;
}

void* _canvas_getcontext(ExHandle hCanvas, int nType)
{
	void* pCanvas = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if (nType == CVC_DX_D2DCONTEXT)
		{
			ret = _cv_context(pCanvas);
		}
		else if (nType == CVC_DX_D2DBITMAP)
		{
			ret = _cv_dx_bmp(pCanvas);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _canvas_begindraw(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pWnd = ((canvas_s*)pCanvas)->pWnd_;
		void* pContext = ((wnd_s*)pWnd)->dx_context_;
		((canvas_s*)pCanvas)->pContext_ = pContext;
		((canvas_s*)pCanvas)->pGdiInterop_ = ((wnd_s*)pWnd)->dx_gdiinterop_;
		if (((wnd_s*)pWnd)->dx_counts_ == 0)
		{
			_dx_begindraw(pContext);
		}
		InterlockedExchangeAdd((long*)&(((wnd_s*)pWnd)->dx_counts_), 1);

		_dx_settarget(pContext, _cv_dx_bmp(pCanvas));
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_enddraw(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pWnd = ((canvas_s*)pCanvas)->pWnd_;
		void* pContext = ((wnd_s*)pWnd)->dx_context_;

		_dx_settarget(pContext, 0);

		if (InterlockedExchangeAdd((long*)&((wnd_s *)pWnd)->dx_counts_, -1) == 1)
		{
			_dx_enddraw(pContext);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_clear(ExHandle hCanvas, int Color)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{

		_dx_clear(_cv_context(pCanvas), Color);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawline(ExHandle hCanvas, void* hBrush, float X1, float Y1, float X2, float Y2, float strokeWidth, void* strokeStyle)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		strokeWidth = Ex_Scale(strokeWidth);
		void* pContext = _cv_context(pCanvas);
		D2D1_POINT_2F point1 = { X1,Y1 };
		D2D1_POINT_2F point2 = { X2,Y2 };
		((ID2D1DeviceContext*)pContext)->DrawLine(point1, point2, (ID2D1Brush*)hBrush, strokeWidth, (ID2D1StrokeStyle*)strokeStyle);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawrect(ExHandle hCanvas, void* hBrush, float left, float top, float right, float bottom, float strokeWidth, void* strokeStyle)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		strokeWidth = Ex_Scale(strokeWidth);
		void* pContext = _cv_context(pCanvas);
		D2D1_RECT_F rect = { left,top,right,bottom };
		((ID2D1DeviceContext*)pContext)->DrawRectangle(rect, (ID2D1Brush*)hBrush, strokeWidth, (ID2D1StrokeStyle*)strokeStyle);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_fillrect(ExHandle hCanvas, void* hBrush, float left, float top, float right, float bottom)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_RECT_F rect = { left,top,right,bottom };
		((ID2D1DeviceContext*)pContext)->FillRectangle(rect, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawroundedrect(ExHandle hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY, float strokeWidth, void* strokeStyle)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		strokeWidth = Ex_Scale(strokeWidth);
		void* pContext = _cv_context(pCanvas);
		D2D1_ROUNDED_RECT rect = {};
		rect.rect = { left, top, right, bottom };
		rect.radiusX = radiusX;
		rect.radiusY = radiusY;
		((ID2D1DeviceContext*)pContext)->DrawRoundedRectangle(rect, (ID2D1Brush*)hBrush, strokeWidth, (ID2D1StrokeStyle*)strokeStyle);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_fillroundedrect(ExHandle hCanvas, void* hBrush, float left, float top, float right, float bottom, float radiusX, float radiusY)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_ROUNDED_RECT rect = {};
		rect.rect = { left, top, right, bottom };
		rect.radiusX = radiusX;
		rect.radiusY = radiusY;
		((ID2D1DeviceContext*)pContext)->FillRoundedRectangle(rect, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawpath(ExHandle hCanvas, ExHandle hPath, void* hBrush, float strokeWidth, void* strokeStyle)
{
	void* pCanvas = nullptr;
	void* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
		{
			void* pContext = _cv_context(pCanvas);
			((ID2D1DeviceContext*)pContext)->DrawGeometry((ID2D1Geometry*)((path_s*)pPath)->pGeometry_, (ID2D1Brush*)hBrush, strokeWidth, (ID2D1StrokeStyle*)strokeStyle);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_fillpath(ExHandle hCanvas, ExHandle hPath, void* hBrush)
{
	void* pCanvas = nullptr;
	void* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if (_handle_validate(hPath, HT_PATH, &pPath, &nError))
		{
			void* pContext = _cv_context(pCanvas);
			((ID2D1DeviceContext*)pContext)->FillGeometry((ID2D1Geometry*)((path_s*)pPath)->pGeometry_, (ID2D1Brush*)hBrush, 0);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawellipse(ExHandle hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY, float strokeWidth, void* strokeStyle)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		strokeWidth = Ex_Scale(strokeWidth);
		void* pContext = _cv_context(pCanvas);
		D2D1_ELLIPSE ELLIPSE = {};
		ELLIPSE.point = { x,y };
		ELLIPSE.radiusX = radiusX;
		ELLIPSE.radiusY = radiusY;
		((ID2D1DeviceContext*)pContext)->DrawEllipse(ELLIPSE, (ID2D1Brush*)hBrush, strokeWidth, (ID2D1StrokeStyle*)strokeStyle);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_fillellipse(ExHandle hCanvas, void* hBrush, float x, float y, float radiusX, float radiusY)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_ELLIPSE ELLIPSE = {};
		ELLIPSE.point = { x,y };
		ELLIPSE.radiusX = radiusX;
		ELLIPSE.radiusY = radiusY;
		((ID2D1DeviceContext*)pContext)->FillEllipse(ELLIPSE, (ID2D1Brush*)hBrush);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_fillregion(ExHandle hCanvas, void* hRgn, void* hBrush)
{
	int nError = 0;
	if (hRgn != 0)
	{
		void* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
		{
			void* pContext = _cv_context(pCanvas);
			((ID2D1DeviceContext*)pContext)->FillGeometry((ID2D1Geometry*)hRgn, (ID2D1Brush*)hBrush, 0);
		}
	}
	else {
		nError = ERROR_EX_INVALID_OBJECT;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawimagerectrect(ExHandle hCanvas, ExHandle hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha)
{
	void* pCanvas = nullptr;
	void* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if (_handle_validate(hImage, HT_IMAGE, &pImage, &nError))
		{
			void* pObj = ((img_s*)pImage)->pObj_;
			void* pContext = _cv_context(pCanvas);
			ID2D1Bitmap* pBitmap = nullptr;
			nError = ((ID2D1DeviceContext*)pContext)->CreateBitmapFromWicBitmap((IWICBitmapSource*)pObj, &pBitmap);
			if (pBitmap != 0)
			{
				D2D1_RECT_F rect = { dstLeft,dstTop,dstRight,dstBottom };
				D2D1_RECT_F srect = { srcLeft,srcTop,srcRight,srcBottom };
				((ID2D1DeviceContext*)pContext)->DrawBitmap(pBitmap, rect, alpha / 255.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srect);
				((ID2D1Bitmap*)pBitmap)->Release();
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawimagerect(ExHandle hCanvas, ExHandle hImage, float Left, float Top, float Right, float Bottom, int alpha)
{
	float w, h;
	bool ret = false;
	if (_img_getsize(hImage, &w, &h))
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Right, Bottom, 0, 0, w, h, alpha);
	}
	return ret;
}


bool _canvas_drawimage(ExHandle hCanvas, ExHandle hImage, float Left, float Top, int alpha)
{
	float w, h;
	bool ret = false;
	if (_img_getsize(hImage, &w, &h))
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, Left, Top, Left + w, Top + h, 0, 0, w, h, alpha);
	}
	return ret;
}

bool _canvas_drawimagefromgrid(ExHandle hCanvas, ExHandle hImage, float dstLeft, float dstTop, float dstRight, float dstBottom,
	float srcLeft, float srcTop, float srcRight, float srcBottom, float gridPaddingLeft, float gridPaddingTop, float gridPaddingRight,
	float gridPaddingBottom, int dwFlags, int dwAlpha)
{
	float pl, pt, pr, pb;
	pl = gridPaddingLeft;
	pt = gridPaddingTop;
	pr = gridPaddingRight;
	pb = gridPaddingBottom;
	if ((dwFlags & BIF_DISABLESCALE) == 0)
	{
		pl = Ex_Scale(pl);
		pt = Ex_Scale(pt);
		pr = Ex_Scale(pr);
		pb = Ex_Scale(pb);
	}
	bool ret = false;
	//��-�м�
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop + pt, dstRight, dstBottom - pb, srcRight - gridPaddingRight, srcTop + gridPaddingTop, srcRight, srcBottom - gridPaddingBottom, dwAlpha);
	//����
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstBottom - pb, dstRight, dstBottom, srcRight - gridPaddingRight, srcBottom - gridPaddingBottom, srcRight, srcBottom, dwAlpha);
	// ��-�м�
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstBottom - pb, dstRight - pr, dstBottom, srcLeft + gridPaddingLeft, srcBottom - gridPaddingBottom, srcRight - gridPaddingRight, srcBottom, dwAlpha);
	// ����
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstBottom - pb, dstLeft + pl, dstBottom, srcLeft, srcBottom - gridPaddingBottom, srcLeft + gridPaddingLeft, srcBottom, dwAlpha);
	//��-�м�
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstTop + pt, dstLeft + pl, dstBottom - pb, srcLeft, srcTop + gridPaddingTop, srcLeft + gridPaddingLeft, srcBottom - gridPaddingBottom, dwAlpha);
	if ((dwFlags & BIF_GRID_EXCLUSION_CENTER) == 0)//�м�
	{
		ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop + pt, dstRight - pr, dstBottom - pb, srcLeft + gridPaddingLeft, srcTop + gridPaddingTop, srcRight - gridPaddingRight, srcBottom - gridPaddingBottom, dwAlpha);
	}
	//����
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft, dstTop, dstLeft + pl, dstTop + pt, srcLeft, srcTop, srcLeft + gridPaddingLeft, srcTop + gridPaddingTop, dwAlpha);
	//����
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstRight - pr, dstTop, dstRight, dstTop + pt, srcRight - gridPaddingRight, srcTop, srcRight, srcTop + gridPaddingTop, dwAlpha);
	// ��-�м�
	ret = _canvas_drawimagerectrect(hCanvas, hImage, dstLeft + pl, dstTop, dstRight - pr, dstTop + pt, srcLeft + gridPaddingLeft, srcTop, srcRight - gridPaddingRight, srcTop + gridPaddingTop, dwAlpha);
	return ret;
}


bool _canvas_drawimagefrombkgimg_ex(ExHandle hCanvas, ExHandle hImage, int x, int y, int dwRepeat, void* lpGrid, int dwFlags, int dwAlpha, void* lpRcSrc, void* lpRCFDst)
{
	void* pCanvas = nullptr;
	int nError = 0;
	D2D1_RECT_F rcfDst;
	RECT rcSrc, rcGrid;
	int w, h;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if (hImage != 0)
		{
			if (lpRCFDst == 0)
			{
				rcfDst.right = ((canvas_s*)pCanvas)->width_;
				rcfDst.bottom = ((canvas_s*)pCanvas)->height_;
			}
			else {
				RtlMoveMemory(&rcfDst, lpRCFDst, sizeof(D2D1_RECT_F));
			}
			if (lpRcSrc == 0)
			{
				_img_getsize(hImage, &rcSrc.right, &rcSrc.bottom);
			}
			else {
				RtlMoveMemory(&rcSrc, lpRcSrc, sizeof(RECT));
			}
			if (dwRepeat != BIR_NO_REPEAT)
			{
				if ((dwFlags & BIF_DISABLESCALE) == 0)
				{
					x = Ex_Scale(x);
					y = Ex_Scale(y);
				}
				rcfDst.left = rcfDst.left + x;
				rcfDst.top = rcfDst.top + y;
			}
			if (lpGrid != 0)
			{
				RtlMoveMemory(&rcGrid, lpGrid, sizeof(RECT));
				_canvas_drawimagefromgrid(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom,
					rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, rcGrid.left, rcGrid.top, rcGrid.right, rcGrid.bottom, dwFlags, dwAlpha);
			}
			else {
				if (dwRepeat == BIR_DEFALUT)
				{
					_canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom,
						rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, dwAlpha);
				}
				else if (dwRepeat == BIR_NO_REPEAT)
				{
					w = rcSrc.right - rcSrc.left;
					h = rcSrc.bottom - rcSrc.top;
					if ((dwFlags & BIF_DISABLESCALE) == 0)
					{
						w = Ex_Scale(w);
						h = Ex_Scale(h);
					}
					if ((dwFlags & BIF_POSITION_X_PERCENT) == 0)
					{
						if ((dwFlags & BIF_DISABLESCALE) == 0) {
							x = Ex_Scale(x);
						}
						rcfDst.left = rcfDst.left + x;
					}
					else {
						rcfDst.left = rcfDst.left + (rcfDst.right - rcfDst.left - w) / 2;
					}
					if ((dwFlags & BIF_POSITION_Y_PERCENT) == 0)
					{
						if ((dwFlags & BIF_DISABLESCALE) == 0) {
							y = Ex_Scale(y);
						}
						rcfDst.top = rcfDst.top + y;
					}
					else {
						rcfDst.top = rcfDst.top + (rcfDst.bottom - rcfDst.top - h) / 2;
					}
					_canvas_drawimagerectrect(hCanvas, hImage, rcfDst.left, rcfDst.top, rcfDst.left + w, rcfDst.top + h, rcSrc.left, rcSrc.top, rcSrc.right, rcSrc.bottom, dwAlpha);
				}
				else {
					w = rcSrc.right - rcSrc.left;
					h = rcSrc.bottom - rcSrc.top;
					ExHandle tmpImg = _img_copyrect(hImage, rcSrc.left, rcSrc.top, w, h);
					if ((dwFlags & BIF_DISABLESCALE) == 0)
					{
						w = Ex_Scale(w);
						h = Ex_Scale(h);
					}
					ExHandle hScale = _img_scale(tmpImg, w, h);
					_img_destroy(tmpImg);
					if (hScale != 0)
					{
						void* hBrush = _brush_createfromimg(hScale);
						if (dwRepeat == BIR_REPEAT_X)
						{
							rcfDst.bottom = rcfDst.top + h;
						}
						else if (dwRepeat == BIR_REPEAT_Y)
						{
							rcfDst.right = rcfDst.left + w;
						}
						if (hBrush != 0)
						{
							_canvas_fillrect(hCanvas, hBrush, rcfDst.left, rcfDst.top, rcfDst.right, rcfDst.bottom);
							_brush_destroy(hBrush);
						}
						_img_destroy(hScale);
					}
				}
			}
		}
	}
	return nError == 0;
}

bool _canvas_drawimagefrombkgimg(ExHandle hCanvas, void* lpBkgImg)
{
	bool ret = false;
	if (lpBkgImg != 0)
	{
		ret = _canvas_drawimagefrombkgimg_ex(hCanvas, ((bkgimg_s*)lpBkgImg)->hImage_, ((bkgimg_s*)lpBkgImg)->x_,
			((bkgimg_s*)lpBkgImg)->y_, ((bkgimg_s*)lpBkgImg)->dwRepeat_, ((bkgimg_s*)lpBkgImg)->lpGrid_,
			((bkgimg_s*)lpBkgImg)->dwFlags_, ((bkgimg_s*)lpBkgImg)->dwAlpha_, NULL, NULL);
	}
	return ret;
}

bool _canvas_cliprect(ExHandle hCanvas, int left, int top, int right, int bottom)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		/*if (__query(pCanvas, offsetof(canvas_s, dwFlags_), CVF_CLIPED))
		{

			_dx_resetclip(_cv_context(pCanvas));
		}*/
		if ((((canvas_s*)pCanvas)->dwFlags_ & CVF_CLIPED) == CVF_CLIPED)
		{
			_dx_resetclip(_cv_context(pCanvas));
		}
		__add(pCanvas, offsetof(canvas_s, dwFlags_), CVF_CLIPED);
		((canvas_s*)pCanvas)->c_left_ = left;
		((canvas_s*)pCanvas)->c_top_ = top;
		((canvas_s*)pCanvas)->c_right_ = right;
		((canvas_s*)pCanvas)->c_bottom_ = bottom;
		_dx_cliprect(_cv_context(pCanvas), ((canvas_s*)pCanvas)->c_left_, ((canvas_s*)pCanvas)->c_top_, ((canvas_s*)pCanvas)->c_right_, ((canvas_s*)pCanvas)->c_bottom_);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_resetclip(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		if ((((canvas_s*)pCanvas)->dwFlags_ & CVF_CLIPED) == CVF_CLIPED)
		{
			_dx_resetclip(_cv_context(pCanvas));
			__del(pCanvas, offsetof(canvas_s, dwFlags_), CVF_CLIPED);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_flush(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		_dx_flush(_cv_context(pCanvas));
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_bitblt(ExHandle hCanvas, ExHandle sCanvas, int dstLeft, int dstTop, int dstRight, int dstBottom, int srcLeft, int srcTop)
{
	void* phCanvas = nullptr;
	void* psCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &phCanvas, &nError))
	{
		if (_handle_validate(sCanvas, HT_CANVAS, &psCanvas, &nError))
		{
			_dx_flush(_cv_context(phCanvas));

			_dx_bmp_copyfrom(&((canvas_s*)phCanvas)->pBitmap_, _cv_dx_bmp(psCanvas), dstLeft, dstTop, srcLeft, srcTop, srcLeft + dstRight - dstLeft, srcTop + dstBottom - dstTop);

		}

	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_alphablend(ExHandle hCanvas, ExHandle sCanvas, float dstLeft, float dstTop, float dstRight, float dstBottom, float srcLeft, float srcTop, float srcRight, float srcBottom, int alpha)
{
	void* phCanvas = nullptr;
	void* psCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &phCanvas, &nError))
	{
		if (_handle_validate(sCanvas, HT_CANVAS, &psCanvas, &nError))
		{

			_dx_drawbitmaprectrect(_cv_context(phCanvas), _cv_dx_bmp(psCanvas), dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, srcRight, srcBottom, alpha);
		}

	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_getsize(ExHandle hCanvas, int* width, int* height)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		*width = ((canvas_s*)pCanvas)->width_;
		*height = ((canvas_s*)pCanvas)->height_;
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_calctextsize_ex(void* pCanvas, void* pFont, LPCWSTR lpwzText, int dwLen, int dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, void* lpWidth, void* lpHeight, void** ppLayout, int* nError)
{
	void* pObj = ((font_s*)pFont)->pObj_;
	if (layoutWidth < 0) layoutWidth = 0;
	if (layoutHeight < 0) layoutHeight = 0;
	int nPreFix;
	auto lpwzTextFix = prefixstring(lpwzText, dwDTFormat, &nPreFix);
	float iWidth, iHeight;
	*nError = ((IDWriteFactory*)g_Ri.pDWriteFactory)->CreateTextLayout((WCHAR*)(lpwzTextFix == 0 ? lpwzText : lpwzTextFix), dwLen, (IDWriteTextFormat*)pObj, layoutWidth, layoutHeight, (IDWriteTextLayout**)ppLayout);
	void * pLayout = *ppLayout;
	if (*nError == 0)
	{
		auto byte = ((font_s*)pFont)->font_.lfUnderline;
		if (byte != 0)
		{
			DWRITE_TEXT_RANGE range = { 0,dwLen };
			((IDWriteTextLayout*)pLayout)->SetUnderline(byte, range);
		}
		byte = ((font_s*)pFont)->font_.lfStrikeOut;
		if (byte != 0)
		{
			DWRITE_TEXT_RANGE range = { 0,dwLen };
			((IDWriteTextLayout*)pLayout)->SetStrikethrough(byte, range);
		}
		((IDWriteTextLayout*)pLayout)->SetWordWrapping((DWRITE_WORD_WRAPPING)((dwDTFormat & DT_SINGLELINE) != DWRITE_WORD_WRAPPING_NO_WRAP ? 1 : DWRITE_WORD_WRAPPING_WRAP));
		if ((dwDTFormat & DT_PATH_ELLIPSIS) != 0 || (dwDTFormat & DT_WORD_ELLIPSIS) != 0)
		{
			IDWriteInlineObject* pEllipsis = nullptr;
			*nError = ((IDWriteFactory*)g_Ri.pDWriteFactory)->CreateEllipsisTrimmingSign((IDWriteTextFormat*)pLayout, &pEllipsis);
			if (*nError == 0)
			{
				DWRITE_TRIMMING tmp1;
				tmp1.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
				((IDWriteTextLayout*)pLayout)->SetTrimming(&tmp1, pEllipsis);
				pEllipsis->Release();
			}
		}
		DWRITE_TEXT_METRICS Metrics = { 0 };
		((IDWriteTextLayout*)pLayout)->GetMetrics(&Metrics);
		iWidth = Metrics.width;
		iHeight = Metrics.height;
		DWRITE_TEXT_ALIGNMENT ALIGNMENT;
		if ((dwDTFormat & DT_CENTER) != 0)
		{
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_CENTER;
		}
		else if ((dwDTFormat & DT_RIGHT) != 0)
		{
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_TRAILING;
		}
		else {
			ALIGNMENT = DWRITE_TEXT_ALIGNMENT_LEADING;
		}
		((IDWriteTextLayout*)pLayout)->SetTextAlignment(ALIGNMENT);
		DWRITE_PARAGRAPH_ALIGNMENT PALIGNMENT;
		if ((dwDTFormat & DT_VCENTER) != 0)
		{
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
		}
		else if ((dwDTFormat & DT_BOTTOM) != 0)
		{
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_FAR;
		}
		else {
			PALIGNMENT = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
		}
		((IDWriteTextLayout*)pLayout)->SetParagraphAlignment(PALIGNMENT);
		if (nPreFix != 0)
		{
			DWRITE_TEXT_RANGE rangea = { nPreFix / 2,1 };
			((IDWriteTextLayout*)pLayout)->SetUnderline(true, rangea);
		}
	}
	if (lpwzTextFix != 0)
	{
		Ex_MemFree(lpwzTextFix);
	}
	if (lpWidth != 0)
	{
		__set_float(lpWidth, 0, iWidth);
		__set_float(lpWidth, 0, iHeight);
	}
	return *nError == 0;
}

bool _canvas_calctextsize(ExHandle hCanvas, void* hFont, LPCWSTR lpwzText, int dwLen, int dwDTFormat, LPARAM lParam, float layoutWidth, float layoutHeight, void* lpWidth, void* lpHeight)
{
	int nError = 0;
	if (dwLen = -1)
	{
		dwLen = lstrlenW(lpwzText);
	}
	if (dwLen > 0)
	{
		void* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
		{
			size_t pFont = 0;
			HashTable_Get(g_Li.hTableFont, (size_t)hFont, &pFont);
			if (pFont != 0)
			{
				void* pLayout = nullptr;
				_canvas_calctextsize_ex(pCanvas, (void*)pFont, lpwzText, dwLen, dwDTFormat, lParam, layoutWidth, layoutHeight, lpWidth, lpHeight, &pLayout, &nError);
				if (pLayout != 0)
				{
					((IDWriteTextLayout*)pLayout)->Release();
				}
			}

		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

void _canvas_dx_drawtext_buffer(void* pCanvas, void* pLayout, int crText, float left, float top, int iGlowsize, int crShadow)
{
	void* hBrush = _brush_create(crText);
	if (hBrush != 0)
	{
		void* pContext = _cv_context(pCanvas);
		D2D1_POINT_2F point = { left,top };
		((ID2D1DeviceContext*)pContext)->DrawTextLayout(point, (IDWriteTextLayout*)pLayout, (ID2D1Brush*)hBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
		_brush_destroy(hBrush);
	}
	((IDWriteTextLayout*)pLayout)->Release();
}

bool _canvas_drawtextex(ExHandle hCanvas, void* hFont, int crText, LPCWSTR lpwzText, int dwLen, int dwDTFormat, float left, float top, float right, float bottom, int iGlowsize, int crShadom, LPARAM lParam, void* prclayout)
{
	if (dwLen == -1)
	{
		dwLen = lstrlenW(lpwzText);
	}
	void* pCanvas = nullptr;
	int nError = 0;
	if (dwLen > 0)
	{
		if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
		{
			size_t pFont = 0;
			if (HashTable_Get(g_Li.hTableFont, (size_t)hFont, &pFont))
			{
				if (bottom > top && right > left)
				{
					float iWidth, iHeight;
					void* pLayout = nullptr;
					if (_canvas_calctextsize_ex(pCanvas, (void*)pFont, lpwzText, dwLen, dwDTFormat, lParam, right - left, bottom - top, &iWidth, &iHeight, &pLayout, &nError))
					{
						_canvas_dx_drawtext_buffer(pCanvas, pLayout, crText, left, top, iGlowsize, crShadom);
					}
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_drawtext(ExHandle hCanvas, void* hFont, int crText, LPCWSTR lpwzText, int dwLen, int dwDTFormat, float left, float top, float right, float bottom)
{
	return _canvas_drawtextex(hCanvas, hFont, crText, lpwzText, dwLen, dwDTFormat, left, top, right, bottom, 0, 0, NULL, NULL);
}

bool _canvas_blur(ExHandle hCanvas, float fDeviation, void* lprc)
{
	int nError = 0;
	if (fDeviation > 0)
	{
		void* pCanvas = nullptr;
		if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
		{
			_dx_blur(_cv_context(pCanvas), ((canvas_s*)pCanvas)->pBitmap_, fDeviation, lprc, &nError);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _canvas_rotate_hue(ExHandle hCanvas, float fAngle)
{
	int nError = 0;
	void* pCanvas = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		_dx_rotate_hue(_cv_context(pCanvas), ((canvas_s*)pCanvas)->pBitmap_, fAngle, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

ExHandle _canvas_createfrompwnd(void* pWnd, int width, int height, int dwFlags, int* nError)
{
	void* pCanvas = Ex_MemAlloc(sizeof(canvas_s));
	ExHandle hCanvas = 0;
	if (pCanvas != 0)
	{

		hCanvas = _handle_create(HT_CANVAS, pCanvas, nError);

		if (hCanvas != 0)
		{

			((canvas_s*)pCanvas)->dwFlags_ = dwFlags;
			((canvas_s*)pCanvas)->pWnd_ = pWnd;

			_canvas_recreate(pCanvas, width, height, nError);

		}
	}
	else {
		*nError = ERROR_EX_MEMORY_ALLOC;
	}
	if (*nError != 0)
	{

		if (pCanvas != 0)
		{
			Ex_MemFree(pCanvas);
		}
		if (hCanvas != 0)
		{
			_handle_destroy(hCanvas, nError);
		}
	}
	return hCanvas;
}

void* _canvas_getdc_ex(void* pCanvas, int* nError)
{
	void* pWnd = ((canvas_s*)pCanvas)->pWnd_;
	void* hDC = nullptr;
	if (((wnd_s*)pWnd)->dx_counts_ > 0)
	{
		void* pGdiInterop = _cv_dx_gdiinterop(pCanvas);
		*nError = ((ID2D1GdiInteropRenderTarget*)pGdiInterop)->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, (HDC*)&hDC);
	}
	return hDC;
}

void* _canvas_getdc(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		ret = _canvas_getdc_ex(pCanvas, &nError);
	}
	Ex_SetLastError(nError);
	return ret;
}

void _canvas_releasedc_ex(void* pCanvas, int* nError)
{
	void* pWnd = ((canvas_s*)pCanvas)->pWnd_;
	if (((wnd_s*)pWnd)->dx_counts_ > 0)
	{
		void* pgdiinterop = _cv_dx_gdiinterop(pCanvas);
		*nError = ((ID2D1GdiInteropRenderTarget*)pgdiinterop)->ReleaseDC(0);
	}
}

bool _canvas_releasedc(ExHandle hCanvas)
{
	void* pCanvas = nullptr;
	int nError = 0;
	if (_handle_validate(hCanvas, HT_CANVAS, &pCanvas, &nError))
	{
		_canvas_releasedc_ex(pCanvas, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}