#include "Easing_ex.h"

void _easing_curve_free(void* pCurveInfo)
{
	Ex_MemFree(pCurveInfo);
}

void _easing_calc_curve(double nProgress, int nStart, int nStop, double* nCurrent, size_t param)
{
	int dwType = ((easinghead_s*)param)->type_;
	int nCount= ((easinghead_s*)param)->node_count_;
	std::vector<std::vector<float>> nodes;
	if (dwType == 2)
	{
		nodes.resize(nCount + 4);
		for (int i = 0; i < nCount + 4; i++)
		{
			nodes[i].resize(2);
		}
		nodes[nCount + 2][0] = 100;
		nodes[nCount + 3][0] = 100;
		nodes[nCount + 2][1] = ((easinghead_s*)param)->dv_;
		nodes[nCount + 3][1] = ((easinghead_s*)param)->dv_;
		RtlMoveMemory(nodes.data() + 16, (void*)(param + offsetof(easinghead_s, node_start_)), nCount * sizeof(easinghead_s));
	}
	else {
		nodes.resize(nCount + 2);
		for (int i = 0; i < nCount + 2; i++)
		{
			nodes[i].resize(2);
		}
		nodes[nCount + 1][0] = 100;
		nodes[nCount + 1][1] = ((easinghead_s*)param)->dv_;
		RtlMoveMemory(nodes.data() + 8, (void*)(param + offsetof(easinghead_s, node_start_)), nCount * sizeof(easinghead_s));
	}
	double nPst = 0;
	if (dwType == 1)
	{
		_easing_calc_bezier(nProgress, &nPst, nodes, nCount + 2, (void*)param);
	}
	else if (dwType == 2)
	{
		_easing_calc_Bspline(nProgress, &nPst, nodes, nCount + 4, (void*)param);
	}
	else {
		_easing_calc_line(nProgress, &nPst, nodes, nCount + 2, (void*)param);
	}
	*nCurrent = (nStop - nStart) * nPst / 100 + nStart;
}

bool _easing_setstate(easing_s* pEasing, int  nState)
{
	bool ret = false;
	if (pEasing != 0 && !IsBadWritePtr(pEasing, sizeof(easing_s)))
	{
		pEasing->nState_ = nState;
		SetEvent(pEasing->hEventPause_);
		ret = true;
	}
	return ret;
}

int _easing_getstate(easing_s* pEasing)
{
	int ret = 0;
	if (pEasing != 0 && !IsBadWritePtr(pEasing, sizeof(easing_s)))
	{
		ret = pEasing->nState_;
	}
	return ret;
}

void _easing_progress(easing_s* pEasing)
{
	int uType = pEasing->dwType_;
	void* lpCalcProc = pEasing->lpfnEsaing_;
	void* pEasingContext = pEasing->lpEasingContext_;
	EXHANDLE pContext = pEasing->pContext_;
	int nFrameCount = pEasing->nFrameCount_;
	int nInterval = pEasing->nInterval_;
	int nFrameStep = nInterval + pEasing->nTotal_;
	int nStart = pEasing->nStart_;
	int nStop = pEasing->nStop_;
	size_t param1 = pEasing->param1_;
	size_t param2 = pEasing->param2_;
	size_t param3 = pEasing->param3_;
	size_t param4 = pEasing->param4_;
	int nMode = LOWORD(pEasing->dwMode_);
	int nTimes = HIWORD(pEasing->dwMode_);
	if (nTimes <= 0) nTimes = 1;
	EX_EASINGINFO EasingInfo;
	double nProcess = 0;
	double nCurrent = 0;
	void* pProgress;
	void* pCurrent;
	if ((nMode & 缓动模式_分发消息) == 缓动模式_分发消息)
	{
		EasingInfo.p1 = param1;
		EasingInfo.p2 = param2;
		EasingInfo.p3 = param3;
		EasingInfo.p4 = param4;
		EasingInfo.pEasing = pEasing;
		EasingInfo.pEasingContext = pEasingContext;
	}
	else {
		pProgress = &nProcess;
		pCurrent = &nCurrent;
	}
	bool fDesc = true;
	int nProcessTime = 0;
	while (nTimes > 0 || (nMode & 缓动模式_循环) == 缓动模式_循环)
	{
		if ((nMode & 缓动模式_来回) == 缓动模式_来回)
		{
			fDesc = !fDesc;
		}
		else if ((nMode & 缓动模式_逆序) == 缓动模式_逆序)
		{
			fDesc = true;
		}
		else {
			fDesc = false;
		}
		nProcessTime = timeGetTime();
		int i = 1;
		bool fStop = false;
		while (i <= nFrameCount)
		{
			Ex_Sleep((nInterval - (timeGetTime() - nProcessTime)) * 1000);
			if(pEasing->nState_== EES_PAUSE)
			{
				WaitForSingleObject(pEasing->hEventPause_, INFINITE);//停住最大程度节省CPU
				nProcessTime = timeGetTime();//如果没停住,则延时一段时间节省CPU
				continue;
			}
			nProcess = i * nFrameStep;
			if (!_easing_calc(lpCalcProc, uType, pEasingContext, fDesc ? nStop : nStart, fDesc ? nStart : nStop, nProcess, &nCurrent))
			{
				nProcess = 1;
				pEasing->nState_ = 1;
			}
			if ((nMode & 缓动模式_分发消息) == 缓动模式_分发消息)
			{
				EasingInfo.nCurrent = nCurrent;
				EasingInfo.nProgress = nProcess;
				EasingInfo.nTimesSurplus = nTimes - 1;
				void* ptr = 0;
				int nError = 0;
				if (_handle_validate(pContext, HT_DUI, (void**)&ptr, &nError))
				{
					fStop=SendMessageW(((wnd_s*)ptr)->hWnd_, g_Li.dwMessage, (WPARAM)&EasingInfo, MAKELONG(EMT_EASING, 0)) != 0;
				}
				if (_handle_validate(pContext, HT_OBJECT, (void**)&ptr, &nError))
				{
					fStop = Ex_ObjSendMessage(pContext, WM_EX_EASING, (WPARAM)pEasing, (size_t)&EasingInfo) != 0;
				}
			}
			else {
				fStop = ((EasingPROC2)pContext)(pEasing, nProcess, nCurrent, pEasingContext, nTimes - 1, param1, param2, param3, param4)!=0;
			}
			if (fStop || pEasing->nState_ == EES_STOP)
			{
				nTimes = 0;
				nMode = (nMode & ~缓动模式_循环);
				break;
			}
			i = i + 1;
			nProcessTime = timeGetTime();
		}
		nTimes = nTimes - 1;
	}
	if ((nMode & 缓动模式_释放曲线) == 缓动模式_释放曲线 && uType == 缓动类型_曲线)
	{
		_easing_curve_free(pEasingContext);
	}
	CloseHandle(pEasing->hEventPause_);
	Ex_MemFree(pEasing);
}

int _easing_calc(void* lpEasingProc, int nType, void* pEasingContext, int nStart, int nStop, double nProgress, double* nCurrent)
{
	if (lpEasingProc != 0)
	{
		((EasingPROC)lpEasingProc)(nProgress, nStart, nStop, nCurrent, pEasingContext);
	}
	else {
		_easing_calc_Linear(nProgress, nStart, nStop, nCurrent, pEasingContext);
	}
	return 1;
}

void _easing_calc_line(double nProgress, double* nCurrent, std::vector<std::vector<float>> aNodes, int nCount, void* param)
{
	double nPst = nProgress * 100;
	double n = 0;
	for (int i = 0; i < 2; i++)
	{
		n = nPst - aNodes[i][0];
		n = n / (aNodes[i + 1][0] - aNodes[i][0]);
		if (nPst <= aNodes[i + 1][0])
		{
			*nCurrent = aNodes[i][1] + (aNodes[i + 1][1] - aNodes[i][1]) * n;
			break;
		}
	}
}

void _easing_calc_bezier(double nProgress, double* nCurrent, std::vector<std::vector<float>> aNodes, int nCount, void* param)
{
	if (nCount <= 0) return;
	std::vector<std::vector<float>> tmpTVs;
	tmpTVs.resize(nCount);
	for (int index =0;index<nCount;index++)
	{
		tmpTVs[index].resize(2);
	}

	for (int i = 1; i < nCount ; i++)
	{
		for (int j = 0; j < nCount - i; j++)
		{
			if (i == 1)
			{
				tmpTVs[j][0] = aNodes[j][0] * (1 - nProgress) + aNodes[j + 1][0] * nProgress;
				tmpTVs[j][2] = aNodes[j][2] * (1 - nProgress) + aNodes[j + 1][2] * nProgress;
			}
			else {
				tmpTVs[j][0] = tmpTVs[j][0] * (1 - nProgress) + tmpTVs[j + 1][0] * nProgress;
				tmpTVs[j][2] = tmpTVs[j][2] * (1 - nProgress) + tmpTVs[j + 1][2] * nProgress;
			}
		}
	}
	*nCurrent = tmpTVs[0][1];
}

void _easing_calc_Bspline(double nProgress,double* nCurrent, std::vector<std::vector<float>> aNodes,int nCount, void* param)
{
	double nPst = nProgress * 100;
	double n = 0;
	double t = 0;
	double a1, a2, a3;
	for (int i = 0; i < 2; i++)
	{
		n = nPst - aNodes[i][0];
		n = n / (aNodes[i + 1][0] - aNodes[i][0]);
		if (nPst <= aNodes[i + 1][0])
		{
			t = n * n;
			a1 = (t - 2 * n + 1) / 2;
			a2 = (2 * n - 2 * t + 1) / 2;
			a3 = t / 2;
			*nCurrent = abs(a1 * aNodes[i][1] + a2 * aNodes[i + 1][1] + a3 * aNodes[i + 2][1]);
			break;
		}
	}
}

void _easing_calc_Linear(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent =(nStop - nStart)* nProgress + nStart;
}

void _easing_calc_InQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent = (nStop - nStart) * nProgress * nProgress + nStart;
}

void _easing_calc_OutQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent = -(nStop - nStart) * nProgress * (nProgress - 2) + nStart;
}

void _easing_calc_InOutQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ts = nProgress * 2;
	if (_ts < 1)
	{
		*nCurrent =(nStop - nStart)/ 2 * _ts *_ts + nStart;
		return;
	}
	_ts = _ts - 1;
	*nCurrent = -(nStop - nStart) / 2 *(_ts *(_ts - 2) - 1) + nStart;
}

void _easing_calc_InCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent =(nStop - nStart)* nProgress * nProgress * nProgress+ nStart;
}

void _easing_calc_OutCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress - 1;
	*nCurrent =(nStop - nStart) *(_st * _st * _st + 1) + nStart;
}

void _easing_calc_InOutCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress *2;
	double _ed1 = nStop - nStart;
	if (_st < 1)
	{
		*nCurrent = _ed1 / 2 * _st * _st * _st + nStart;
		return;
	}
	_st = _st - 2;
	*nCurrent = _ed1 / 2 * (_st * _st * _st + 2) + nStart;
}

void _easing_calc_InQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent =(nStop - nStart) * nProgress* nProgress* nProgress* nProgress + nStart;
}

void _easing_calc_OutQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress - 1;
	*nCurrent = -(nStop - nStart) *(_st * _st * _st * _st - 1) + nStart;
}

void _easing_calc_InOutQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress * 2;
	double _ed1 = nStop - nStart;
	if (_st < 1)
	{
		*nCurrent = _ed1 / 2 * _st * _st * _st * _st + nStart;
		return;
	}
	_st = _st - 2;
	*nCurrent = -_ed1 / 2 * (_st * _st * _st * _st - 2) + nStart;
}

void _easing_calc_InQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent =(nStop - nStart) * nProgress * nProgress * nProgress * nProgress * nProgress + nStart;
}

void _easing_calc_OutQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress - 1;
	double _ed1 = nStop - nStart;
	*nCurrent = _ed1 *(_st * _st * _st * _st * _st + 1) + nStart;
}

void _easing_calc_InOutQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress *2;
	double _ed1 = nStop - nStart;
	if (_st < 1)
	{
		*nCurrent = _ed1 / 2 * _st * _st * _st * _st * _st + nStart;
		return;
	}
	_st = _st - 2;
	*nCurrent =_ed1 / 2 *(_st * _st * _st * _st * _st + 2) +nStart;
}

void _easing_calc_InSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = -_ed1* cos(nProgress / 1 * 3.1415926/ 2) + _ed1 + nStart;
}

void _easing_calc_OutSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = -_ed1 * sin(nProgress / 1 * 3.1415926 / 2)  + nStart;
}

void _easing_calc_InOutSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = -_ed1 /2 *(cos(3.1415926 * nProgress / 1) - 1) + nStart;
}

void _easing_calc_InExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = _ed1 *pow(2, 10 *(nProgress / 1 - 1)) + nStart;
}

void _easing_calc_OutExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = _ed1 *(-pow(2, -10 * nProgress / 1) + 1) + nStart;
}

void _easing_calc_InOutExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress * 2;
	double _ed1 = nStop - nStart;
	if (_st < 1)
	{
		*nCurrent = _ed1 / 2 * pow(2, 10 *(_st - 1)) + nStart;
		return;
	}
	_st = _st - 1;
	*nCurrent = _ed1 / 2 *(-pow(2, -10 * _st) + 2) + nStart;
}

void _easing_calc_InCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed1 = nStop - nStart;
	*nCurrent = -_ed1 *(sqrt(1 - nProgress * nProgress) - 1) + nStart;
}

void _easing_calc_OutCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress - 1;
	double _ed1 = nStop - nStart;
	*nCurrent = _ed1 * sqrt(1 - _st * _st) + nStart;
}

void _easing_calc_InOutCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = nProgress *2;
	double _ed1 = nStop - nStart;
	if (_st < 1)
	{
		*nCurrent = -_ed1 / 2 *(sqrt(1 - _st * _st) - 1) + nStart;
		return;
	}
	_st = _st - 2;
	*nCurrent = _ed1 / 2 *(sqrt(1 - _st * _st) + 1) + nStart;
}

void _easing_calc_InBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent = _easing_calc_getInBounce(nStart, nStop, nProgress);
}

void _easing_calc_OutBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	*nCurrent = _easing_calc_getOutBounce(nStart, nStop, nProgress);
}

void _easing_calc_InOutBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed = nStop - nStart;
	if (nProgress < 0.5)
	{
		*nCurrent = _easing_calc_getInBounce(0, _ed, nProgress * 2) * 0.5 + nStart;
	}
	else {
		*nCurrent = _easing_calc_getOutBounce(0, _ed, nProgress * 2 - 1) * 0.5 + _ed * 0.5 + nStart;
	}
}

double _easing_calc_getOutBounce(double nStart, double nStop, double nProgress)
{
	double _ed = nStop - nStart;
	double _st = nProgress;
	if (_st < 1 / 2.75)
	{
		return _ed * 7.5625 * _st * _st + nStart;
	}
	if (_st < 2 / 2.75)
	{
		_st = _st - 1.5 / 2.75;
		return _ed * (7.5625 * _st * _st + 0.75) + nStart;
	}
	if (_st < 2.5 / 2.75)
	{
		_st = _st - 2.25 / 2.75;
		return _ed *(7.5625 * _st * _st + 0.9375) + nStart;
	}
	_st = _st - 2.625 / 2.75;
	return _ed *(7.5625 * _st * _st + 0.984375) + nStart;
}

double _easing_calc_getInBounce(double nStart, double nStop, double nProgress)
{
	double _ed = nStop - nStart;
	return _ed - _easing_calc_getOutBounce(0, _ed, 1 - nProgress) + nStart;
}

void _easing_calc_InBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed = nStop - nStart;
	double _s = 1.70158;
	*nCurrent = _ed * nProgress * nProgress * ((_s +1) * nProgress - _s) + nStart;
}

void _easing_calc_OutBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed = nStop - nStart;
	double _s = 1.70158;
	double _st = nProgress - 1;
	*nCurrent = _ed *(_st * _st * ((_s + 1) * _st + _s) + 1) + nStart;
}

void _easing_calc_InOutBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed = nStop - nStart;
	double _s = 1.70158;
	double _st = nProgress *2;
	if (_st < 1)
	{
		_s = _s * 1.525;
		*nCurrent = _ed / 2 * _st * _st * ((_s + 1) * _st - _s) + nStart;
		return;
	}
	_st = _st - 2;
	_s = _s * 1.525;
	*nCurrent = _ed / 2 *(_st * _st * ((_s + 1) * _st + _s) +2) + nStart;
}

void _easing_calc_InElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	if (nProgress = 0)
	{
		*nCurrent = nStart;
		return;
	}
	if (nProgress = 1)
	{
		*nCurrent = nStop;
		return;
	}
	double _ed = nStop - nStart;
	double _d = 1;
	double _p = _d * 0.3;
	double _s = 0;
	double _a = 0;
	if (_a = 0 || _a < abs(_ed))
	{
		_a = _ed;
		_s = _p / 4;
	}
	else {
		_s = _p / (2 * 3.1415826) * _easing_calc_asin(_ed / _a);
	}
	double _st = nProgress - 1;
	*nCurrent = -(_a * pow(2, 10 * _st) * sin((_st * _d - _s) * 2 * 3.1415926 / _p)) + nStart;
}

void _easing_calc_OutElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	if (nProgress = 0)
	{
		*nCurrent = nStart;
		return;
	}
	if (nProgress = 1)
	{
		*nCurrent = nStop;
		return;
	}
	double _ed = nStop - nStart;
	double _d = 1;
	double _p = _d * 0.3;
	double _s = 0;
	double _a = 0;
	if (_a = 0 || _a < abs(_ed))
	{
		_a = _ed;
		_s = _p / 4;
	}
	else {
		_s = _p / (2 * 3.1415826) * _easing_calc_asin(_ed / _a);
	}
	double _st = nProgress - 1;
	*nCurrent = _a * pow(2, -10 * nProgress) * sin((nProgress * _d - _s) * 2 * 3.1415926 / _p) + nStop;
}

void _easing_calc_InOutElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	if (nProgress = 0)
	{
		*nCurrent = nStart;
		return;
	}
	double _st = nProgress * 2;
	if (_st = 2)
	{
		*nCurrent = nStop;
		return;
	}
	double _ed = nStop - nStart;
	double _d = 1;
	double _p = _d * 0.3;
	double _s = 0;
	double _a = 0;
	if (_a = 0 || _a < abs(_ed))
	{
		_a = _ed;
		_s = _p / 4;
	}
	else {
		_s = _p / (2 * 3.1415826) * _easing_calc_asin(_ed / _a);
	}
	if (_st < 1)
	{
		_st = _st - 1;
		*nCurrent = -0.5 * _a * pow(2, 10 * _st) * sin((_st * _d - _s) * 2 * 3.1415926 / _p) + nStart;
		return;
	}
	_st = _st - 1;
	*nCurrent = _a * pow(2, -10 * _st) * sin((_st * _d - _s) * 2 * 3.1415926 / _p) * 0.5 + nStop;
}

double _easing_calc_asin(double v)
{
	if (v < -1 || v>1)
	{
		return 0;
	}
	return atan(v / sqrt(1 - v * v));
}

void _easing_calc_Clerp(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _ed = nStop - nStart;
	double _min = 0;
	double _max = 360;
	double _half = 180;
	double _retval = 0;
	double _diff = 0;
	if (_ed < -_half)
	{
		_diff = (_max - nStart + nStop) * nProgress;
		_retval = nStart + _diff;
	}
	else if (_ed > _half)
	{
		_diff = -(_max - nStop + nStart) * nProgress;
		_retval = nStart + _diff;
	}
	else {
		_retval = nStart + _ed * nProgress;
	}
	*nCurrent = _retval;
}

void _easing_calc_Spring(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	double _st = (sin(nProgress * 3.1415826 * (0.2 + 2.5 * nProgress * nProgress * nProgress)) * pow(1 - nProgress, 2.2) + nProgress) * (1 + 1.2 * (1 - nProgress));
	*nCurrent = nStart + (nStop - nStart) * _st;
}

void _easing_calc_Punch(double nProgress, int nStart, int nStop, double* nCurrent, void* param)
{
	if (nProgress < 0) nProgress = 0;
	if (nProgress > 1) nProgress = 1;
	if (nProgress == 0)
	{
		*nCurrent = 0;
		return;
	}
	if (nProgress == 1)
	{
		*nCurrent = 0;
		return;
	}
	double _p = 0.3;
	double _s = _p / (2 * 3.1415826) * _easing_calc_asin(0);
	*nCurrent = nStop * pow(2, -10 * nProgress) * sin((nProgress - _s) * 2 * 3.1415926 / _p);
}

easing_s* _easing_create(int dwType, void* pEasingContext, int dwMode,EXHANDLE pContext, int nTotalTime, int nInterval, int nState, int nStart, int nStop, size_t param1, size_t param2, size_t param3, size_t param4)
{
	easing_s* pEasing = nullptr;
	if (pContext != 0)
	{
		pEasing =(easing_s*)Ex_MemAlloc(sizeof(easing_s));
		int nError = 0;
		void* ptr;
		if ((dwMode & 缓动模式_分发消息) == 缓动模式_分发消息)
		{
			if (_handle_validate(pContext, HT_DUI, (void**)&ptr, &nError))
			{

			}
			else if (_handle_validate(pContext, HT_OBJECT, (void**)&ptr, &nError))
			{

			}
			else {
				Ex_MemFree(pEasing);
				Ex_SetLastError(ERROR_EX_INVALID_OBJECT);
				return 0;
			}
		}
		pEasing->pContext_ = pContext;
		void* lpProc = nullptr;
		std::vector<void*> lpCalcProcs = { &_easing_calc_Linear, &_easing_calc_Clerp, &_easing_calc_Spring, &_easing_calc_Punch, &_easing_calc_InQuad, &_easing_calc_OutQuad, &_easing_calc_InOutQuad, &_easing_calc_InCubic, &_easing_calc_OutCubic, &_easing_calc_InOutCubic, &_easing_calc_InQuart, &_easing_calc_OutQuart, &_easing_calc_InOutQuart, &_easing_calc_InQuint, &_easing_calc_OutQuint, &_easing_calc_InOutQuint, &_easing_calc_InSine, &_easing_calc_OutSine, &_easing_calc_InOutSine, &_easing_calc_InExpo, &_easing_calc_OutExpo, &_easing_calc_InOutExpo, &_easing_calc_InCirc, &_easing_calc_OutCirc, &_easing_calc_InOutCirc, &_easing_calc_InBounce, &_easing_calc_OutBounce, &_easing_calc_InOutBounce, &_easing_calc_InBack, &_easing_calc_OutBack, &_easing_calc_InOutBack, &_easing_calc_InElastic, &_easing_calc_OutElastic, &_easing_calc_InOutElastic };
		if (dwType >= 0 && dwType < lpCalcProcs.size())
		{
			lpProc = lpCalcProcs[dwType];
		}
		else if (dwType == 缓动类型_自定义)
		{
			lpProc = pEasingContext;
		}
		else if (dwType == 缓动类型_曲线)
		{
			lpProc = &_easing_calc_curve;
			if (pEasingContext == 0)
			{
				Ex_MemFree(pEasing);
				Ex_SetLastError(ERROR_EX_MEMORY_BADPTR);
				return 0;
			}
		}
		else {
			lpProc = &_easing_calc_Linear;
		}
		if (lpProc == nullptr)
		{
			Ex_MemFree(pEasing);
			Ex_SetLastError(ERROR_EX_MEMORY_BADPTR);
			return 0;
		}
		pEasing->dwType_ = dwType;
		pEasing->lpfnEsaing_ = lpProc;
		pEasing->lpEasingContext_ = pEasingContext;
		pEasing->dwMode_ = dwMode;

		pEasing->nCurFrame_ = 0;
		pEasing->nTotal_ = nTotalTime;
		pEasing->nInterval_ = nInterval;
		pEasing->nFrameCount_ = nTotalTime / nInterval;
		pEasing->nState_ = nState;
		pEasing->nStart_ = nStart;
		pEasing->nStop_ = nStop;
		pEasing->param1_ = param1;
		pEasing->param2_ = param2;
		pEasing->param3_ = param3;
		pEasing->param4_ = param4;
		pEasing->hEventPause_ = CreateEventW(0, false, true, 0);
		if ((dwMode & 缓动模式_使用线程) != 0)
		{
			Thread_Create((LPTHREAD_START_ROUTINE)&_easing_progress, pEasing);
		}
		else {
			_easing_progress(pEasing);
		}
	}
	return pEasing;
}