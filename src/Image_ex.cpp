#include "Image_ex.h"


bool _img_destroy(EXHANDLE hImg)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		if (pObj)
		{
			((IWICBitmap*)pObj)->Release();
		}
		
		void* pWicDecoder = pImage->pWicDecoder_;
		if (pWicDecoder)
		{
			((IWICBitmapDecoder*)pWicDecoder)->Release();
		}
		Ex_MemFree(pImage);
		_handle_destroy(hImg, &nError);
	}
	return nError == 0;
}

void _wic_drawframe(img_s* pImg, void* pFrame, int* nError)
{
	if (pImg->nMaxFrames_ > 1)
	{
		void* rtp = Ex_MemAlloc(sizeof(D2D1_RENDER_TARGET_PROPERTIES));

		ID2D1RenderTarget* rt = nullptr;
		((ID2D1Factory1*)g_Ri.pD2Dfactory)->CreateWicBitmapRenderTarget((IWICBitmap*)pImg->pObj_, (D2D1_RENDER_TARGET_PROPERTIES*)rtp, &rt);
		if (rt != 0)
		{
			_dx_begindraw(rt);
			ID2D1Bitmap* pBitmap = nullptr;
			rt->CreateBitmapFromWicBitmap((IWICBitmapSource*)pFrame, NULL, &pBitmap);
			if (pBitmap != 0)
			{
				rt->DrawBitmap(pBitmap, NULL, 1, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, NULL);
				pBitmap->Release();
			}
			_dx_enddraw(rt);
			rt->Release();
		}
		Ex_MemFree(rtp);
	}
}

void* _wic_convert(void* pBitmap, bool bFreeOld, int* nError)
{
	//void* pBitmapConvert = pBitmap;
	void* pBitmapConvert = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateFormatConverter(&pConverter);
	if (*nError == 0)
	{
		*nError = pConverter->Initialize((IWICBitmapSource*)pBitmap, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0, WICBitmapPaletteTypeCustom);
		if (*nError == 0)
		{
			*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSource(pConverter, WICBitmapCacheOnDemand, (IWICBitmap**)&pBitmapConvert);
			if (*nError == 0 && bFreeOld)
			{
				((IWICBitmap*)pBitmap)->Release();
			}
		}
		pConverter->Release();
	}
	return  pBitmapConvert;
}

void* _wic_selectactiveframe(void* pDecoder, int nIndex, int* nError)
{
	void* ret = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrame(nIndex, &pFrame);
	if (*nError == 0)
	{
		ret = _wic_convert(pFrame, true, nError);
	}
	return ret;
}

void _apng_drawframe(img_s* pImage, int nIndex)//未完成
{
	/*' 0 dwLen
	' 4 Type
	' 8 uint sequence_number 序列
	' 12 uint width 宽度
	' 16 uint height 高度
	' 20 uint x_offset 水平偏移
	' 24 uint y_offset 垂直偏移
	' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
	' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
	' 32 byte dispose_op 处理方式
	' 33 byte blend_op 混合模式*/

	if (nIndex<0 || nIndex>pImage->nMaxFrames_ - 1)
	{
		return;
	}
	void* pHeader = pImage->lpHeader_;
	void* pFrame = (void*)__get(pImage->lpFrames_, nIndex * sizeof(void*));
	void* pIDAT =(void*)( (size_t)pFrame + 26 + 12);
	int type = __get_int(pIDAT, 4);
	if (type == PNG_IDAT || type == PNG_fdAT)
	{
		char dispose = __get_char(pFrame , 32);
		char blend= __get_char(pFrame, 33);
		int x = __get_int(pFrame, 20);
		int y = __get_int(pFrame, 24);
		int dwHeader=  __get_int(pHeader, 0);
		int dwLen = dwHeader;
		void* pIDATNext = pIDAT;
		size_t lpStream = 0;
		while (type== PNG_fdAT || type== PNG_IDAT)
		{
			lpStream= (size_t)_apng_thunk_getlength(pIDATNext) + (size_t)12;
			dwLen = dwLen + lpStream + (type == PNG_fdAT ? -4 : 0);
			pIDATNext =(void*)((size_t) pIDATNext + lpStream);
			type = __get_int(pIDATNext, 4);
		}
		dwLen = dwLen + 12;
		void* hMem = GlobalAlloc(2, dwLen);
		if (hMem != 0)
		{
			if (CreateStreamOnHGlobal(hMem, 1, (LPSTREAM*)&lpStream) == 0)
			{
				void* pBuffer = GlobalLock(hMem);
				if (pBuffer != 0)
				{
					RtlMoveMemory(pBuffer,(void*)((size_t) pHeader + 4), dwHeader);
					__set_int(pBuffer, 16, __get_int(pFrame, 12));
					__set_int(pBuffer, 20, __get_int(pFrame, 16));
					__set_int(pBuffer, 29,_byteswap_ulong(Crc32_Addr((void*)((size_t)pBuffer+12),17)));
					void* pOffset =(void*)((size_t)pBuffer + dwHeader);
					pIDATNext = pIDAT;
					type = __get_int(pIDAT, 4);
					while (type == PNG_fdAT || type == PNG_IDAT)
					{
						int dwDat = _apng_thunk_getlength(pIDATNext);
						if (type == PNG_fdAT)
						{
							__set_int(pOffset, 0, _byteswap_ulong(dwDat - 4));
							__set_int(pOffset, 4, PNG_IDAT);
							RtlMoveMemory((void*)((size_t)pOffset + 8), (void*)((size_t)pIDATNext + (size_t)8 + (size_t)4), dwDat - (size_t)4);
							__set_int(pOffset, dwDat + (size_t)12 - (size_t)4, _byteswap_ulong(Crc32_Addr((void*)((size_t)pOffset + 4), dwDat)));
							pOffset =(void*) ((size_t)pOffset + dwDat + 12 - 4);
						}
						else {
							__set_int(pOffset, 0, __get_int(pIDATNext,4));
							RtlMoveMemory(pOffset, pIDATNext, dwDat + (size_t)12);
							pOffset = (void*)((size_t)pOffset + dwDat + 12 );
						}
						pIDATNext = (void*)((size_t)pIDATNext + dwDat + 12);
						type = __get_int(pIDATNext, 4);
					}
					__set_int(pBuffer, dwLen - 8, 1145980233);
					__set_int(pBuffer, dwLen - 4, -2107620690);

					GlobalUnlock(hMem);
					void* pObj = pImage->pObj_;
					_dx_drawframe_apng(pImage, pObj, x, y, __get_int(pFrame, 12), __get_int(pFrame, 16), dispose, blend, nIndex);
				}
				((LPSTREAM)lpStream)->Release();
			}
			GlobalFree(hMem);
		}
	}
}

bool _img_selectactiveframe(EXHANDLE hImg, int nIndex)
{
	img_s* pImg = nullptr;
	int nError = 0;
	if (_img_getframecount(hImg) > 1)
	{
		if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
		{
			if (__query(pImg, offsetof(img_s, dwFlags_), IMGF_APNG))
			{
				_apng_drawframe(pImg, nIndex);
			}
			else if (pImg->nMaxFrames_ > 1)
			{
				void* pFrame = _wic_selectactiveframe(pImg->pWicDecoder_, nIndex, &nError);
				if (pFrame != 0)
				{
					_wic_drawframe(pImg, pFrame, &nError);
					((IWICBitmapFrameDecode*)pFrame)->Release();
					pImg->nCurFrame_ = nIndex;
				}
			}
		}
		Ex_SetLastError(nError);
	}
	return nError == 0;
}

void* _wic_getpixel(void* pBitmap, int x, int y, int* nError)
{
	WICRect rcl = { x,y,1,1 };
	void* ret = nullptr;
	*nError=((IWICBitmap*)pBitmap)->CopyPixels(&rcl, 4, 4, (BYTE*)ret);
	return ret;
}

void* _img_getpixel(EXHANDLE hImg, int x, int y)
{
	int nError = 0;
	void* ret = nullptr;
	img_s* pImg = nullptr;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
	{
		void* pObj = pImg->pObj_;
		ret = _wic_getpixel(pObj, x, y, &nError);
	}
	Ex_SetLastError(nError);
	return ret;
}

bool _img_lock(EXHANDLE hImg, void* lpRectL, int flags, lockedbitmapdata_s* lpLockedBitmapData)//FLAGS 1读 2写 3读写
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pBitmap = pImage->pObj_;
		int swidth, sheight;
		int width, height;
		nError = ((IWICBitmapSource*)pBitmap)->GetSize((UINT*)&swidth, (UINT*)&sheight);
		if (nError == 0)
		{
			
			if (lpRectL == 0 || IsBadReadPtr(lpRectL, 16))
			{
				width = swidth;
				height = sheight;
			}
			else {
				width = __get_int(lpRectL, 8);
				height = __get_int(lpRectL, 12);
			}
			int stride = swidth * 4;
			lpLockedBitmapData->width_ = width;
			lpLockedBitmapData->height_ = height;
			lpLockedBitmapData->pixelformat_ = 2498570;
			IWICBitmapLock* pLock;
			nError = ((IWICBitmap*)pBitmap)->Lock((WICRect*)lpRectL, flags, &pLock);
			
			if (nError == 0)
			{
				int stride;
				nError = pLock->GetStride((UINT*)&stride);
				
				if (nError == 0)
				{
					lpLockedBitmapData->stride_ = stride;
					UINT dwlen=0;
					WICInProcPointer scan0 = nullptr;
					nError = pLock->GetDataPointer(&dwlen,&scan0);
					
					if (nError == 0)
					{
						
						lpLockedBitmapData->scan0_ = scan0;
						lpLockedBitmapData->dwlen_ = dwlen;
						lpLockedBitmapData->pLock_ = pLock;
					}
					else 
					{
						pLock->Release();
					}
				}
			}
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_unlock(EXHANDLE hImg, lockedbitmapdata_s* lpLockedBitmapData)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pLock = lpLockedBitmapData->pLock_;
		if (pLock != 0)
		{
			((IWICBitmapLock*)pLock)->Release();
		}
		_wic_drawframe(pImage, pImage->pObj_, &nError);
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_setpixel(EXHANDLE hImg, int x, int y, int color)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		lockedbitmapdata_s* pBitmapData = (lockedbitmapdata_s*)Ex_MemAlloc(sizeof(lockedbitmapdata_s));
		if (pBitmapData != 0)
		{
			RECT rect1 = { x,y,1,1 };
			if (_img_lock(hImg, &rect1, 3, pBitmapData))
			{
				void* scan0 = pBitmapData->scan0_;
				if (scan0 != 0)
				{
					*(int*)scan0 = color;
				}
				else {
					nError = ERROR_EX_MEMORY_BADPTR;
				}
				_img_unlock(hImg, pBitmapData);
			}
			Ex_MemFree(pBitmapData);
		}
		else {
			nError = ERROR_EX_MEMORY_ALLOC;
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

bool _img_getsize(EXHANDLE hImg, void* lpWidth, void* lpHeight)
{
	img_s* pImage = nullptr;
	int nError = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		int w, h;
		nError = ((IWICBitmap*)pObj)->GetSize((UINT*)&w, (UINT*)&h);
		if (lpWidth != 0)
		{
			
			__set_int(lpWidth, 0, w);
		}
		if (lpHeight != 0)
		{
			__set_int(lpHeight, 0, h);
		}
	}
	Ex_SetLastError(nError);
	return nError == 0;
}

int _img_width(EXHANDLE hImg)
{
	int width = 0;
	_img_getsize(hImg, &width, NULL);
	return width;
}

int _img_height(EXHANDLE hImg)
{
	int height = 0;
	_img_getsize(hImg, NULL, &height);
	return height;
}

EXHANDLE _img_init(void* pObj, int curframe, int frames, void* pDecoder, int* nError)
{
	img_s* pImg = (img_s*)Ex_MemAlloc(sizeof(img_s));
	EXHANDLE hImg = 0;
	if (pImg != 0)
	{
		pImg->pObj_ = pObj;
		pImg->nCurFrame_ = curframe;
		pImg->pWicDecoder_ = pDecoder;

		_wic_drawframe(pImg, pObj, nError);
		pImg->nMaxFrames_ = frames;
		hImg = _handle_create(HT_IMAGE, pImg, nError);
	
	}
	if (*nError != 0)
	{
		if (pImg != 0)
		{
			Ex_MemFree(pImg);
		}
	}
	return hImg;
}

EXHANDLE _wic_create(int width, int height, GUID pFormat, int* nError)
{
	EXHANDLE hImg = 0;
	void* pBitmap = nullptr;
	*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmap(width, height, pFormat, WICBitmapCacheOnDemand, (IWICBitmap**)&pBitmap);
	if (*nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, NULL, nError);
	}
	Ex_SetLastError(*nError);
	return hImg;
}

EXHANDLE _img_create(int width, int height)
{
	int nError = 0;
	EXHANDLE hImg = _wic_create(width, height, GUID_WICPixelFormat32bppPBGRA, &nError);
	Ex_SetLastError(nError);
	return hImg;
}

EXHANDLE _img_createfrompngbits(void* lpmem)
{
	int nError = 0;
	EXHANDLE hImg = 0;
	int width = __get_int(lpmem, sizeof(int));
	int height = __get_int(lpmem, 2 * sizeof(int));
	int len = width * height * 4;
	void* pBitmapData = nullptr;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromMemory(width, height, GUID_WICPixelFormat32bppPBGRA, width * 4, len, (BYTE*)((size_t)lpmem + 3 * sizeof(int)), (IWICBitmap**)&pBitmapData);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapData, 0, 1, NULL, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}

void* _img_createfromstream_init(void* lpData, int dwLen, int* nError)
{
	if (dwLen > 0)
	{
		void* hMem = GlobalAlloc(GMEM_MOVEABLE, dwLen);
		if (hMem != 0)
		{
			LPSTREAM lpStream = nullptr;
			*nError = CreateStreamOnHGlobal(hMem, true, &lpStream);
			if (*nError == 0)
			{
				void* lpMem = GlobalLock(hMem);
				if (lpMem != 0)
				{
					RtlMoveMemory(lpMem, lpData, dwLen);
					GlobalUnlock(hMem);
					return lpStream;
				}
				else {
					*nError = GetLastError();
				}
			}
			GlobalFree(hMem);
		}
		else {
			*nError = GetLastError();
		}
	}
	else {
		*nError = ERROR_EX_BAD_LENGTH;
	}
	return NULL;
}


EXHANDLE _wic_init_from_decoder(void* pDecoder, int* nError)
{
	UINT pCount = 0;
	EXHANDLE ret = 0;
	*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrameCount(&pCount);
	if (*nError == 0)
	{
		void* pFrame = _wic_selectactiveframe(pDecoder, 0, nError);
		if (*nError == 0)
		{
			
			ret = _img_init(pFrame, 0, pCount, pDecoder, nError);
		}
	}
	return ret;
}

EXHANDLE _img_createfromstream(void* lpStream)
{
	int nError = 0;
	void* pDecoder = nullptr;
	EXHANDLE hImg = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromStream((IStream*)lpStream, NULL, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	if (hImg != 0)
	{
		_apng_int(hImg, lpStream);
	}
	Ex_SetLastError(nError);
	return hImg;
}

EXHANDLE _img_createfrommemory(void* lpData, int dwLen)
{
	int nError = 0;
	EXHANDLE hImg = 0;
	void* lpStream = _img_createfromstream_init(lpData, dwLen, &nError);
	if (nError == 0)
	{
		hImg = _img_createfromstream(lpStream);
		((IStream*)lpStream)->Release();
	}
	Ex_SetLastError(nError);
	return hImg;
}

EXHANDLE _img_createfromhicon(void* hIcon)
{
	int nError = 0;
	void* pBitmap = nullptr;
	EXHANDLE hImg = 0;
	((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromHICON((HICON)hIcon, (IWICBitmap**)&pBitmap);
	void* pBitmapConvert = _wic_convert(pBitmap, true, &nError);
	if (nError == 0)
	{
		hImg = _img_init(pBitmapConvert, 0, 1, 0, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}

EXHANDLE _img_createfromfile(LPCWSTR lpwzFilename)
{
	void* pDecoder = nullptr;
	EXHANDLE hImg = 0;
	int nError = 0;
	nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateDecoderFromFilename(lpwzFilename, NULL, 2147483648, WICDecodeMetadataCacheOnLoad, (IWICBitmapDecoder**)&pDecoder);
	if (nError == 0)
	{
		hImg = _wic_init_from_decoder(pDecoder, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}

EXHANDLE _img_copyrect(EXHANDLE hImg, int x, int y, int width, int height)
{
	img_s* pImage = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	if (_handle_validate(hImg, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmap* pIBitmap = nullptr;
		nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromSourceRect((IWICBitmapSource*)pObj, x, y, width, height, &pIBitmap);
		if (nError == 0)
		{
			ret = _img_init(pIBitmap, 0, 1, NULL, &nError);
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

EXHANDLE _img_copy(EXHANDLE hImg)
{
	int width, height;
	EXHANDLE ret = 0;
	if (_img_getsize(hImg, &width, &height))
	{
		ret = _img_copyrect(hImg, 0, 0, width, height);
	}
	return ret;
}

EXHANDLE _img_scale(EXHANDLE hImage, int dstWidth, int dstHeight)
{
	img_s* pImage = nullptr;
	int nError = 0;
	EXHANDLE ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pObj = pImage->pObj_;
		IWICBitmapScaler* pBitmapScaler;
		((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapScaler(&pBitmapScaler);
		if (pBitmapScaler != 0)
		{
			nError = pBitmapScaler->Initialize((IWICBitmapSource*)pObj, dstWidth, dstHeight, WICBitmapInterpolationModeLinear);
			if (nError == 0)
			{
				ret = _img_init(pBitmapScaler, 0, 1, NULL, &nError);
			}
			else {
				pBitmapScaler->Release();
			}
		}
	}
	Ex_SetLastError(nError);
	return ret;
}

void _wic_savetobin(void* pBitmap, void* lpBin, size_t* len, int* nError)
{
	LPSTREAM pStream = nullptr;
	*nError = CreateStreamOnHGlobal(NULL, false, &pStream);
	if (*nError == 0)
	{
		UINT width, height;
		*nError = ((IWICBitmap*)pBitmap)->GetSize(&width, &height);
		if (*nError == 0)
		{
			IWICStream* pIWICStream = nullptr;
			*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateStream(&pIWICStream);
			if (*nError == 0)
			{
				*nError = pIWICStream->InitializeFromIStream(pStream);
				if (*nError == 0)
				{
					IWICBitmapEncoder* pEncoder = nullptr;
					*nError = ((IWICImagingFactory*)g_Ri.pWICFactory)->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
					if (*nError == 0)
					{
						*nError = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
						if (*nError == 0)
						{
							IWICBitmapFrameEncode* pFrame = nullptr;
							*nError = pEncoder->CreateNewFrame(&pFrame, NULL);
							if (*nError == 0)
							{
								*nError = pFrame->Initialize(NULL);
								if (*nError == 0)
								{
									*nError = pFrame->SetSize(width, height);
									if (*nError == 0)
									{
										//SetPixelFormat,如果为PBGRA,导致d2d绘图alpha通道丢失
										WICPixelFormatGUID aa = GUID_WICPixelFormatDontCare;
										*nError = pFrame->SetPixelFormat(&aa);
										if (*nError == 0)
										{
											*nError = pFrame->WriteSource((IWICBitmapSource*)pBitmap, NULL);
											if (*nError == 0)
											{
												*nError = pFrame->Commit();
												if (*nError == 0)
												{
													*nError = pEncoder->Commit();
													if (*nError == 0)
													{
														void* hMem = nullptr;
														*nError = GetHGlobalFromStream(pStream, &hMem);
														if (*nError == 0)
														{
															void* pData = GlobalLock(hMem);
															if (pData != 0)
															{
																*len = GlobalSize(hMem);
																lpBin = realloc(lpBin, *len);
																RtlMoveMemory(lpBin, pData, *len);
																GlobalUnlock(hMem);
															}
															else {
																*nError = GetLastError();
															}
															GlobalFree(hMem);
														}
													}
												}
											}
										}
									}
								}
								pFrame->Release();
							}
						}
						pEncoder->Release();
					}
				}
				pIWICStream->Release();
			}
		}
		pStream->Release();
	}
}

size_t _img_savetomemory(EXHANDLE hImage, void* lpBuffer)
{
	img_s* pImage = nullptr;
	int nError = 0;
	size_t ret = 0;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		void* pBitmap = pImage->pObj_;
		void* buffer = Ex_MemAlloc(4);
		_wic_savetobin(pBitmap, buffer, &ret, &nError);
		if (!IsBadWritePtr(lpBuffer, ret))
		{
			RtlMoveMemory(lpBuffer, buffer, ret);
		}
		Ex_MemFree(buffer);
	}
	return ret;
}

bool _wic_getframedelay(void* pDecoder, void* lpDelay, int nCount, int* nError)
{
	bool fOK = false;
	if (pDecoder != 0)
	{
		for (int i = 0; i < nCount; i++)
		{
			fOK = false;
			IWICBitmapFrameDecode* pFrame = nullptr;
			*nError = ((IWICBitmapDecoder*)pDecoder)->GetFrame(i, &pFrame);
			if (*nError == 0)
			{
				IWICMetadataQueryReader* pReader = nullptr;
				*nError = pFrame->GetMetadataQueryReader(&pReader);
				if (*nError == 0)
				{
					PROPVARIANT pValue = {};
					*nError = pReader->GetMetadataByName(L"/grctlext/Delay", &pValue);
					if (*nError == 0)
					{
						auto nDelay = pValue.uiVal;
						if (nDelay == 0)
						{
							nDelay = 10;
						}
						__set_int(lpDelay, i * (size_t)4, nDelay);
						fOK = true;
					}
					pReader->Release();
				}
				pFrame->Release();
			}
			if (!fOK) break;
		}
	}
	else {
		*nError = ERROR_EX_MEMPOOL_ALLOC;
	}
	Ex_SetLastError(*nError);
	return fOK;
}

bool _img_getframedelay(EXHANDLE hImg, void* lpDelayAry, int nFrames)
{
	img_s* pImg = nullptr;
	int nError = 0;
	bool ret = false;
	if (nFrames > 1 && lpDelayAry != 0)
	{
		if (_handle_validate(hImg, HT_IMAGE, (void**)&pImg, &nError))
		{
			if (((pImg->dwFlags_ & IMGF_APNG) == IMGF_APNG))
			{
				ret = _apng_getframedelay(pImg, lpDelayAry, nFrames);
			}
			else {
				ret = _wic_getframedelay(pImg->pWicDecoder_, lpDelayAry, nFrames, &nError);
			}
		}
		Ex_SetLastError(nError);
	}
	return ret;
}

int _img_getframecount(EXHANDLE hImage)
{
	img_s* pImage = nullptr;
	int nError = 0;
	int ret = 1;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		ret = pImage->nMaxFrames_;
	}
	Ex_SetLastError(nError);
	return ret;
}

void* _img_getcontext(EXHANDLE hImage)
{
	img_s* pImage = nullptr;
	int nError = 0;
	void* ret = nullptr;
	if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
	{
		ret = pImage->pObj_;
	}
	return ret;
}

int _apng_thunk_getlength(void* lpMem)
{
	return _byteswap_ulong(__get_int(lpMem, 0));
}

bool _apng_thunk_getnext(void* lpMem, int* nPos, int dwThunkType, void** lpThunk, int* dwThunkLen)
{
	int i = *nPos;
	bool ret = false;
	int dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	while (dwThunkDataLen != 0)
	{
		if (__get_int(lpMem, i + (size_t)4) == dwThunkType)
		{
			ret = true;
		}
		i = i + dwThunkDataLen + 12;
		if (ret)
		{
			*lpThunk = (void*)((size_t)lpMem + i - dwThunkDataLen - 12);
			*dwThunkLen = dwThunkDataLen + 12;
			*nPos = i;
			break;
		}
		dwThunkDataLen = _apng_thunk_getlength((void*)((size_t)lpMem + i));
	}
	return ret;
}

void _apng_int(EXHANDLE hImage, void* lpStream)
{
	void* hGlobal = nullptr;
	int i = 0;
	
	if (GetHGlobalFromStream((LPSTREAM)lpStream, &hGlobal) == 0)
	{
		void* lpMem = GlobalLock(hGlobal);
		if (lpMem != 0)
		{
			if (__get_int(lpMem, 0) == PNG_HEADER)
			{
				img_s* pImage = nullptr;
				int nError = 0;
				if (_handle_validate(hImage, HT_IMAGE, (void**)&pImage, &nError))
				{
					int nPos = 8;
					void* pIHDR = nullptr;
					int dwIHDR = 0;
					
					if (_apng_thunk_getnext(lpMem, &nPos, PNG_IHDR, &pIHDR, &dwIHDR))
					{
						
						void* pPLTE = nullptr;
						int dwPLTE = 0;
						void* pTRNS = nullptr;
						int dwTRNS = 0;
						_apng_thunk_getnext(lpMem, &nPos, PNG_PLTE, &pPLTE, &dwPLTE);
						_apng_thunk_getnext(lpMem, &nPos, PNG_tRNS, &pTRNS, &dwTRNS);
						int dwLen = 4 + 8 + dwIHDR + dwPLTE + dwTRNS;
						void* pHeader = Ex_MemAlloc(dwLen);
						if (pHeader != 0)
						{
							
							__set_int(pHeader, 0, dwLen - 4);
							__set_int(pHeader, 4, PNG_HEADER);
							__set_int(pHeader, 8, 169478669);
							RtlMoveMemory((void*)((size_t)pHeader + 12), pIHDR, dwIHDR);
							dwLen = dwIHDR + 12;
							if (pPLTE != 0)
							{
								RtlMoveMemory((void*)((size_t)pHeader + dwLen), pPLTE, dwPLTE);
								dwLen = dwLen + dwPLTE;
							}
							if (pTRNS != 0)
							{
								RtlMoveMemory((void*)((size_t)pHeader + dwLen), pTRNS, dwTRNS);
							}
							nPos = 8;
							void* pThunk = nullptr;
							if (_apng_thunk_getnext(lpMem, &nPos, PNG_acTL, &pThunk, &dwLen))
							{
								int nCount = _apng_thunk_getlength((void*)((size_t)pThunk + 8));
								
								void* pFrames = Ex_MemAlloc(nCount * sizeof(void*));
								if (pFrames != 0)
								{
									
;									pImage->dwFlags_ = pImage->dwFlags_ | IMGF_APNG;
									pImage->nMaxFrames_ = nCount;
									pImage->lpFrames_ = pFrames;
									pImage->lpHeader_ = pHeader;
									while (_apng_thunk_getnext(lpMem, &nPos, PNG_fcTL, &pThunk, &dwLen))
									{
										/*' 0 dwLen
										' 4 Type
										' 8 uint sequence_number 序列
										' 12 uint width 宽度
										' 16 uint height 高度
										' 20 uint x_offset 水平偏移
										' 24 uint y_offset 垂直偏移
										' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
										' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
										' 32 byte dispose_op 处理方式
										' 33 byte blend_op 混合模式*/
										__set_int(pThunk, 20, _apng_thunk_getlength((void*)((size_t)pThunk + 20)));
										__set_int(pThunk, 24, _apng_thunk_getlength((void*)((size_t)pThunk + 24)));
										__set_int(pThunk, 28, _apng_thunk_getlength((void*)((size_t)pThunk + 28)));
										
										//TODO: x64
										__set_int(pFrames, i * sizeof(void*), (int)pThunk);
										i = i + 1;
									}
								}
							}
							else {
								Ex_MemFree(pHeader);
							}
						}
					}
				}
			}
			GlobalUnlock(hGlobal);
		}
	}
}

bool _apng_getframedelay(img_s* pImg, void* lpDelay, int nFrames)
{
	bool ret = false;
	void* lpFrames = pImg->lpFrames_;
	if (lpFrames != 0)
	{
		/*' 0 dwLen
		' 4 Type
		' 8 uint sequence_number 序列
		' 12 uint width 宽度
		' 16 uint height 高度
		' 20 uint x_offset 水平偏移
		' 24 uint y_offset 垂直偏移
		' 28 ushort delay_num 为这一帧显示时间的以秒为单位的分子
		' 30 ushort delay_den 为这一帧显示时间以秒为单位的分母
		' 32 byte dispose_op 处理方式
		' 33 byte blend_op 混合模式*/
		for (int i = 0; i < nFrames; i++)
		{
			auto index = i * sizeof(void*);
			short delay_num = __get_short((void*)__get(lpFrames, index), 28);
			__set_int(lpDelay, index, 200);
			delay_num = HIWORD(delay_num) / LOWORD(delay_num) * 100;
			if (delay_num == 0) delay_num = 10;
			__set_int(lpDelay, index, delay_num);
		}
		ret = true;
	}
	return ret;
}

EXHANDLE _img_createfromres(hashtable_s* hRes, int atomPath)
{
	void* lpFile = nullptr;
	size_t dwLen = 0;
	EXHANDLE ret = 0;
	if (Ex_ResGetFileFromAtom(hRes, atomPath, &lpFile, &dwLen))
	{
		ret = _img_createfrommemory(lpFile, dwLen);
	}
	return ret;
}

EXHANDLE _img_createfromhbitmap(void* hBitmap, void* hPalette, int fPreAlpha)
{
	IWICBitmap* pBitmap = nullptr;
	EXHANDLE hImg = 0;
	int nError=((IWICImagingFactory*)g_Ri.pWICFactory)->CreateBitmapFromHBITMAP((HBITMAP)hBitmap, (HPALETTE)hPalette, (WICBitmapAlphaChannelOption)fPreAlpha, &pBitmap);
	if (nError == 0)
	{
		hImg = _img_init(pBitmap, 0, 1, 0, &nError);
	}
	Ex_SetLastError(nError);
	return hImg;
}