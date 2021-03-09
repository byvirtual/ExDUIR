#pragma once
#include "help_ex.h"
#include "HashTable_ex.h"
#include "Array_ex.h"
#include "Object_ex.h"
#include "Wnd_ex.h"

typedef int(*LayoutThreePROC)(void*, int, size_t, size_t);
typedef int(*LayoutTwoPROC)(int, size_t, size_t);

//ע��,���и���������(PADDING����)��__get��ʱ��Ҫ-1��*4
//ELP_ Ϊ����������, ELCP_ Ϊ�Ӳ�������
//#define ELN_GETPROPSCOUNT 1 //��ȡ���ָ����Ը���
//#define ELN_GETCHILDPROPCOUNT 2 //��ȡ���������Ը���
//#define ELN_INITPROPS 3 //��ʼ���������б�
//#define ELN_UNINITPROPS 4 //�ͷŸ������б�
//#define ELN_INITCHILDPROPS 5 //��ʼ���������б�
//#define ELN_UNINITCHILDPROPS 6 //�ͷ��������б�
//#define ELN_CHECKPROPVALUE 7 //�������ֵ�Ƿ���ȷ,wParamΪpropID��lParamΪֵ
//#define ELN_CHECKCHILDPROPVALUE 8 //���������ֵ�Ƿ���ȷ,wParam��λΪnIndex����λΪpropID��lParamΪֵ
//#define ELN_FILL_XML_PROPS 9 //��XML���Ա���䵽������Ϣ��
//#define ELN_FILL_XML_CHILD_PROPS 10 //��XML���Ա���䵽��������Ϣ��
//#define ELN_UPDATE 15 //���²���
//
//#define ELT_NULL 0 //��������_��
//#define ELT_LINEAR 1 //��������_����
//#define ELT_FLOW 2 //��������_��ʽ
//#define ELT_PAGE 3 //��������_ҳ��
//#define ELT_TABLE 4 //��������_���
//#define ELT_RELATIVE 5 //��������_���
//#define ELT_ABSOLUTE 6 //��������_����
//
//#define ELP_PADDING_LEFT -1 //��������_ͨ��_�ڼ��_��
//#define ELP_PADDING_TOP -2 //��������_ͨ��_�ڼ��_��
//#define ELP_PADDING_RIGHT -3 //��������_ͨ��_�ڼ��_��
//#define ELP_PADDING_BOTTOM -4 //��������_ͨ��_�ڼ��_��
//
//#define ELCP_MARGIN_LEFT -1 //��������_ͨ��_����_��
//#define ELCP_MARGIN_TOP -2 //��������_ͨ��_����_��
//#define ELCP_MARGIN_RIGHT -3 //��������_ͨ��_����_��
//#define ELCP_MARGIN_BOTTOM -4 //��������_ͨ��_����_��
//
//#define ELP_LINEAR_DIRECTION 1 //��������:����
//#define ELCP_LINEAR_SIZE 1 //����������:�ߴ�[-1��δ��дΪ�����ǰ�ߴ�]
//#define ELCP_LINEAR_ALIGN 2 //����������:����һ������Ķ��뷽ʽ
//
//#define ELP_LINEAR_DALIGN 2 //��������:���ַ�����뷽ʽ
//#define ELP_LINEAR_DALIGN_LEFT_TOP 0
//#define ELP_LINEAR_DALIGN_CENTER 1
//#define ELP_LINEAR_DALIGN_RIGHT_BOTTOM 2
//
//#define ELCP_LINEAR_ALGIN_FILL 0
//#define ELCP_LINEAR_ALIGN_LEFT_TOP 1
//#define ELCP_LINEAR_ALIGN_CENTER 2
//#define ELCP_LINEAR_ALIGN_RIGHT_BOTTOM 3
//
//#define ELP_DIRECTION_H 0 //����:ˮƽ
//#define ELP_DIRECTION_V 1 //����:��ֱ
//
//#define ELP_FLOW_DIRECTION 1 //��������:����
//#define ELCP_FLOW_SIZE 1 //����������:�ߴ�[-1��δ��дΪ�����ǰ�ߴ�]
//#define ELCP_FLOW_NEW_LINE 2 //����������:���ǿ�ƻ���
//
//#define ELP_PAGE_CURRENT 1 //��������:��ǰ��ʾҳ������
//#define ELCP_PAGE_FILL 1 //����������:�Ƿ������������
//
#define ELP_TABLE_ARRAY_ROW 1 //�ڲ���������:�и�����
#define ELP_TABLE_ARRAY_CELL 2 //�ڲ���������:�п�����
//
//#define ELCP_TABLE_ROW 1 //��������_���_������
//#define ELCP_TABLE_CELL 2 //��������_���_������
//#define ELCP_TABLE_ROW_SPAN 3 //��������_���_������
//#define ELCP_TABLE_CELL_SPAN 4 //��������_���_������
//#define ELCP_TABLE_FILL 5 //��������_���_�Ƿ�����
//
//#define ELCP_RELATIVE_LEFT_OF 1 //��������_���_�����(���)
//#define ELCP_RELATIVE_TOP_OF 2 //��������_���_֮����(���)
//#define ELCP_RELATIVE_RIGHT_OF 3 //��������_���_�Ҳ���(���)
//#define ELCP_RELATIVE_BOTTOM_OF 4 //��������_���_֮����(���)
//#define ELCP_RELATIVE_LEFT_ALIGN_OF 5 //��������_���_�������(���)
//#define ELCP_RELATIVE_TOP_ALIGN_OF 6 //��������_���_��������(���)
//#define ELCP_RELATIVE_RIGHT_ALIGN_OF 7 //��������_���_�Ҷ�����(���)
//#define ELCP_RELATIVE_BOTTOM_ALIGN_OF 8 //��������_���_�׶�����(���)
//#define ELCP_RELATIVE_CENTER_PARENT_H 9 //��������_���_ˮƽ�����ڸ�
//#define ELCP_RELATIVE_CENTER_PARENT_V 10 //��������_���_��ֱ�����ڸ�
//
//#define ELCP_ABSOLUTE_LEFT 1 //��������_����_���
//#define ELCP_ABSOLUTE_LEFT_TYPE 2 //��������_����_�������
//#define ELCP_ABSOLUTE_TOP 3 //��������_����_����
//#define ELCP_ABSOLUTE_TOP_TYPE 4 //��������_����_��������
//#define ELCP_ABSOLUTE_RIGHT 5 //��������_����_�Ҳ�
//#define ELCP_ABSOLUTE_RIGHT_TYPE 6 //��������_����_�Ҳ�����
//#define ELCP_ABSOLUTE_BOTTOM 7 //��������_����_�ײ�
//#define ELCP_ABSOLUTE_BOTTOM_TYPE 8 //��������_����_�ײ�����
//#define ELCP_ABSOLUTE_WIDTH 9 //��������_����_���(���ȼ������Ҳ�)
//#define ELCP_ABSOLUTE_WIDTH_TYPE 10 //��������_����_�������
//#define ELCP_ABSOLUTE_HEIGHT 11 //��������_����_�߶ȣ����ȼ����ڵײ���
//#define ELCP_ABSOLUTE_HEIGHT_TYPE 12 //��������_����_�߶�����
//#define ELCP_ABSOLUTE_OFFSET_H 13 //��������_����_ˮƽƫ����
//#define ELCP_ABSOLUTE_OFFSET_H_TYPE 14 //��������_����_ˮƽƫ��������
//#define ELCP_ABSOLUTE_OFFSET_V 15 //��������_����_��ֱƫ����
//#define ELCP_ABSOLUTE_OFFSET_V_TYPE 16 //��������_����_��ֱƫ��������
//
//#define ELCP_ABSOLUTE_TYPE_UNKNOWN 0 //��������_����_����_δ֪(δ���û򱣳ֲ���)
//#define ELCP_ABSOLUTE_TYPE_PX 1 //��������_����_����_����
//#define ELCP_ABSOLUTE_TYPE_PS 2 //��������_����_����_�ٷֱ�
//#define ELCP_ABSOLUTE_TYPE_OBJPS 3 //��������_����_����_����ߴ�ٷֱȣ���OFFSET����

struct layout_s
{
	int nType_; //��������
	void* lpfnProc_;//���ֻص�����
	EXHANDLE hBind_;//�󶨵ĸ����(������������������)/HOBJ/HEXDUI
	int nBindType_;//�����������,#HT_OBJECT��#HL_DUI
	void* lpLayoutInfo_;//������Ĳ�����Ϣ
	int cbInfoLen_; //��������Գ���(���ݲ�ͬ�������Ͳ�ͬ)
	void* hArrChildrenInfo_; //������������������
	bool fUpdateable_; //�Ƿ������ָ���
	EXHANDLE hLayout_; //�����־��
};


bool _layout_register(int nType, void* lpfnLayoutProc);
bool _layout_unregister(int nType);
void _layout_free_info(void* hArr, int nIndex, void* pvItem, int nType);
EXHANDLE _layout_create(int nType, EXHANDLE hObjBind);
EXHANDLE _layout_get_parent_layout(EXHANDLE hObj);
bool _layout_destory(EXHANDLE hLayout);
bool _layout_enum_find_obj(void* hArr, int nIndex, void* pvItem, int nType, void* pvParam);
void* _layout_get_child(void* pLayout, EXHANDLE hObj);
bool _layout_update(EXHANDLE hLayout);
int _layout_gettype(EXHANDLE hLayout);
bool _layout_enableupdate(EXHANDLE hLayout, bool fUpdateable);
size_t _layout_notify(EXHANDLE hLayout, int nEvent, void* wParam, void* lParam);
bool _layout_table_setinfo(EXHANDLE hLayout, void* aRowHeight, int cRows, void* aCellWidth, int cCells);
bool _layout_setchildprop(EXHANDLE hLayout, EXHANDLE hObj, int dwPropID, size_t pvValue);
bool _layout_getchildprop(EXHANDLE hLayout, EXHANDLE hObj, int dwPropID, size_t* pvValue);
bool _layout_setprop(EXHANDLE hLayout, int dwPropID, size_t pvValue);
size_t _layout_getprop(EXHANDLE hLayout, int dwPropID);
bool _layout_absolute_setedge(EXHANDLE hLayout, EXHANDLE hObjChild, int dwEdge, int dwType, size_t nValue);
