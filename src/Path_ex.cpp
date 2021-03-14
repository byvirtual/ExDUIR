#include "Path_ex.h"


void _path_destroy_dx(path_s* pPath) {
	if (pPath->pObj_) {
		pPath->pObj_->Release();
	}
	if (pPath->pGeometry_) {
		pPath->pGeometry_->Release();
	}
	RtlZeroMemory(pPath, sizeof(path_s));
}

int _path_destroy(EXHANDLE hPath)
{
	path_s* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		_path_destroy_dx(pPath);
		Ex_MemFree(pPath);
		_handle_destroy(hPath, &nError);
	}
	return nError;
}

int _path_reset(EXHANDLE hPath)
{
	path_s* pPath = nullptr;
	int nError = 0;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		_path_destroy_dx(pPath);
		ID2D1PathGeometry* pObj = nullptr;
		nError = ((ID2D1Factory*)g_Ri.pD2Dfactory)->CreatePathGeometry(&pObj);
		if (nError == 0)
		{
			pPath->pGeometry_ = pObj;
		}
	}
	return nError;
}

int _path_create(int dwFlags, EXHANDLE* hPath)
{
	int nError = 0;
	path_s* pPath = (path_s*)Ex_MemAlloc(sizeof(path_s));
	if (pPath != 0)
	{
		*hPath = _handle_create(HT_PATH, pPath, &nError);
		if (*hPath != 0)
		{
			nError = _path_reset(*hPath);
			pPath->dwFlags_ = dwFlags;
			if (nError != 0)
			{
				_path_destroy(*hPath);
				hPath = 0;
			}
		}
	}
	else {
		nError = ERROR_EX_MEMORY_ALLOC;
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_getbounds(EXHANDLE hPath, void* lpBounds)
{
	int nError = 0;
	if (IsBadWritePtr(lpBounds, 16))
	{
		nError = ERROR_EX_MEMORY_BADPTR;
	}
	else {
		path_s* pPath = nullptr;
		if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
		{
			nError = pPath->pGeometry_->GetBounds(NULL, (D2D1_RECT_F*)lpBounds);
		}
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_open(EXHANDLE hPath)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		if ((pPath->dwFlags_ & epf_bOpened) == epf_bOpened)
		{
			_path_reset(hPath);
		}
		ID2D1GeometrySink* pSink = nullptr;
		nError = pPath->pGeometry_->Open(&pSink);
		if (nError == 0)
		{
			pPath->pObj_ = pSink;
			pPath->dwFlags_ = pPath->dwFlags_ | epf_bOpened;
		}
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_close(EXHANDLE hPath)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		if ((pPath->dwFlags_ & epf_bOpened) == epf_bOpened)
		{
			ID2D1GeometrySink* pSink = pPath->pObj_;
			pPath->pObj_ = NULL;
			pSink->Close();
			pSink->Release();
		}
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_beginfigure(EXHANDLE hPath)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		pPath->pObj_->BeginFigure({ 0,0 }, D2D1_FIGURE_BEGIN_FILLED);
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_endfigure(EXHANDLE hPath, bool fCloseFigure)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		pPath->pObj_->EndFigure(fCloseFigure == true ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
	}
	Ex_SetLastError(nError);
	return nError;
}

bool _path_hittest(EXHANDLE hPath, float x, float y)
{
	int nError = 0;
	path_s* pPath = nullptr;
	BOOL ret = false;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		void* pGeometry = pPath->pGeometry_;
		((ID2D1PathGeometry*)pGeometry)->FillContainsPoint({ x,y }, NULL, 0, &ret);
	}
	Ex_SetLastError(nError);
	return ret;
}

int _path_addline(EXHANDLE hPath, float x1, float y1, float x2, float y2)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		if (!((pPath->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			if (g_Li.DpiX > 1)
			{
				x1 = x1 * g_Li.DpiX;
				y1 = y1 * g_Li.DpiX;
				x2 = x2 * g_Li.DpiX;
				y2 = y2 * g_Li.DpiX;
			}
		}
		pPath->pObj_->AddLine({ x1,y1 });
		pPath->pObj_->AddLine({ x2,y2 });
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_addarc(EXHANDLE hPath, float x1, float y1, float x2, float y2, float radiusX, float radiusY, bool fClockwise)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		if (!((pPath->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			if (g_Li.DpiX > 1)
			{
				x1 = x1 * g_Li.DpiX;
				y1 = y1 * g_Li.DpiX;
				x2 = x2 * g_Li.DpiX;
				y2 = y2 * g_Li.DpiX;
			}
		}
		pPath->pObj_->AddLine({ x1,y1 });
		D2D1_ARC_SEGMENT arc = {};
		arc.point.x = x2;
		arc.point.y = y2;
		arc.size.width = radiusX;
		arc.size.height = radiusY;
		arc.sweepDirection = (fClockwise == true ? D2D1_SWEEP_DIRECTION_CLOCKWISE : D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE);
		pPath->pObj_->AddArc(arc);
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_addrect(EXHANDLE hPath, float left, float top, float right, float bottom)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		if (!((pPath->dwFlags_ & EPF_DISABLESCALE) == EPF_DISABLESCALE))
		{
			if (g_Li.DpiX > 1)
			{
				left = left * g_Li.DpiX;
				top = top * g_Li.DpiX;
				right = right * g_Li.DpiX;
				bottom = bottom * g_Li.DpiX;
			}
		}
		ID2D1GeometrySink* pSink = pPath->pObj_;
		pSink->AddLine({ left,top });
		pSink->AddLine({ right,top });
		pSink->AddLine({ right,bottom });
		pSink->AddLine({ left,bottom });
		pSink->AddLine({ left,top });
	}
	Ex_SetLastError(nError);
	return nError;
}

int _path_addroundedrect(EXHANDLE hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight)
{
	int nError = 0;
	path_s* pPath = nullptr;
	if (_handle_validate(hPath, HT_PATH, (void**)&pPath, &nError))
	{
		void* pObj = pPath->pObj_;
		if (radiusTopLeft == 0)//左上->右上
		{
			_path_addline(hPath, left, top, right - radiusTopRight, top);
		}
		else {
			_path_addarc(hPath, left, top + radiusTopLeft, left + radiusTopLeft, top, radiusTopLeft, radiusTopLeft, true);
		}

		if (radiusTopRight == 0)//右上-右下
		{
			_path_addline(hPath, right, top, right, bottom - radiusBottomRight);
		}
		else {
			_path_addarc(hPath, right - radiusTopRight, top, right, top + radiusTopRight, radiusTopRight, radiusTopRight, true);
		}

		if (radiusBottomRight == 0)//右下-左下
		{
			_path_addline(hPath, right, bottom, left + radiusBottomLeft, bottom);
		}
		else {
			_path_addarc(hPath, right, bottom - radiusBottomRight, right - radiusBottomRight, bottom, radiusBottomRight, radiusBottomRight, true);
		}

		if (radiusBottomLeft == 0)//左下-左上
		{
			_path_addline(hPath, left, bottom, left, top + radiusTopLeft);
		}
		else {
			_path_addarc(hPath, left + radiusBottomLeft, bottom, left, bottom - radiusBottomLeft, radiusBottomLeft, radiusBottomLeft, true);
			_path_addline(hPath, left, bottom - radiusBottomLeft, left, top + radiusTopLeft);
		}
	}
	Ex_SetLastError(nError);
	return nError;
}