#pragma once
#include "help_ex.h"
#include "Font_ex.h"
#include "Canvas_ex.h"
#include "Class_Static_SysLink_ex.h"
#include "Class_SysButton_Page_ex.h"
#include "Class_Scrollbar_ex.h"

void pfnDefaultFreeData(void* dwData);
void Ex_SetLastError(int nError);
BOOL Ex_Init(HINSTANCE hInstance, int dwGlobalFlags, HCURSOR hDefaultCursor, LPCWSTR lpszDefaultClassName,
	LPVOID lpDefaultTheme, int dwDefaultThemeLen, LPVOID lpDefaultI18N, int dwDefaultI18NLen);
void Ex_UnInit();
void _object_init();
float Ex_Scale(float n);
int Ex_Atom(LPCWSTR lptstring);
BOOL Ex_IsDxRender();
bool Flag_Query(int dwFlag);
void Flag_Add(int dwFlag);
void Flag_Del(int dwFlag);