#pragma once

// 调试模式
#define EXGF_DEBUG	1
// 启用DPI缩放
#define EXGF_DPI_ENABLE	2
// 画布_不抗锯齿
#define EXGF_RENDER_CANVAS_ALIAS	64
// 使用GDI/GDI+渲染
#define EXGF_RENDER_METHOD_GDI	128
// 使用D2D渲染
#define EXGF_RENDER_METHOD_D2D	256
// 使用支持GDI交互的D2D渲染
#define EXGF_RENDER_METHOD_D2D_GDI_COMPATIBLE	768
// 文本渲染_使用ClearType抗锯齿
#define EXGF_TEXT_CLEARTYPE	1024
// 文本渲染_抗锯齿
#define EXGF_TEXT_ANTIALIAS	2048
// 图像渲染_抗锯齿
#define EXGF_IMAGE_ANTIALIAS	4096
// 禁用动画效果
#define EXGF_OBJECT_DISABLEANIMATION	65536
// 显示组件边界
#define EXGF_OBJECT_SHOWRECTBORDER	131072
// 显示组件位置
#define EXGF_OBJECT_SHOWPOSTION	262144
// 允许JS全局对象访问文件
#define EXGF_JS_FILE	524288
// 允许JS全局对象访问内存
#define EXGF_JS_MEMORY	1048576
// 允许JS全局对象Ex_MemAlloc
#define EXGF_JS_MEMORY_ALLOC	2097152
// 允许JS全局对象创建进程、允许程序、加载DLL
#define EXGF_JS_PROCESS	4194304
// 允许JS全局对象访问所有资源
#define EXGF_JS_ALL	7864320
// 渲染所有菜单
#define EXGF_MENU_ALL	8388608
// 暂无注释
#define EXR_DEFAULT	0
// 暂无注释
#define EXR_STRING	1
// 暂无注释
#define EXR_LAYOUT	2
// 不显示菜单阴影
#define EMNF_NOSHADOW	-2147483648
// 显示窗口图标
#define EMBF_WINDOWICON	-2147483648
// 消息框居父窗口中间
#define EMBF_CENTEWINDOW	1073741824
// 显示倒计时
#define EMBF_SHOWTIMEOUT	536870912
// 窗体风格_关闭按钮
#define EWS_BUTTON_CLOSE	1
// 窗体风格_最大化按钮
#define EWS_BUTTON_MAX	2
// 窗体风格_最小化按钮
#define EWS_BUTTON_MIN	4
// 窗体风格_菜单按钮
#define EWS_BUTTON_MENU	8
// 窗体风格_皮肤按钮
#define EWS_BUTTON_SKIN	16
// 窗体风格_设置按钮
#define EWS_BUTTON_SETTING	32
// 窗体风格_帮助按钮
#define EWS_BUTTON_HELP	64
// 窗体风格_图标
#define EWS_HASICON	128
// 窗体风格_标题
#define EWS_TITLE	256
//窗口风格_信息框 内部使用
#define EWS_MESSAGEBOX 2147483648
//窗口风格_模糊背景图片
#define EWS_BLUR 524288
//窗口风格_菜单 内部使用
#define EWS_MENU 1073741824
// 窗体风格_全屏模式.设置该标记窗口最大化时,任务栏将被遮挡.
#define EWS_FULLSCREEN	512
// 窗体风格_允许调整尺寸
#define EWS_SIZEABLE	1024
// 窗体风格_允许随意移动
#define EWS_MOVEABLE	2048
// 窗体风格_不显示窗口阴影
#define EWS_NOSHADOW	4096
// 窗体风格_不继承父窗口背景数据
#define EWS_NOINHERITBKG	8192
// 窗体风格_不显示TAB焦点边框
#define EWS_NOTABBORDER	16384
// 窗体风格_ESC关闭窗口
#define EWS_ESCEXIT	32768
// 窗体风格_主窗口(拥有该风格时,当窗口被关闭,会调用PostQuitMessage()退出消息循环)
#define EWS_MAINWINDOW	65536
// 窗体风格_窗口居中(如果有父窗口,则在父窗口中间,否则为屏幕中间)
#define EWS_CENTERWINDOW	131072
// 窗体风格_标题栏取消置顶
#define EWS_NOCAPTIONTOPMOST	262144
// 窗体风格_弹出式窗口
#define EWS_POPUPWINDOW	524288
// 背景模糊
#define EWL_BLUR	-2
//主题包句柄
#define EWL_HTHEME -3
// 窗口消息过程
#define EWL_MSGPROC	-4
// 窗口透明度
#define EWL_ALPHA	-5
//窗口句柄
#define EWL_HWND -6
// 自定义参数
#define EWL_LPARAM	-7
// 窗口边框圆角度
#define EWL_RADIUS	-11
// 边框颜色
#define EWL_CRBORDER	-30
// 背景颜色
#define EWL_CRBKG	-31
// 最小高度
#define EWL_MINHEIGHT	-33
// 最小宽度
#define EWL_MINWIDTH	-34
// 焦点组件组件
#define EWL_OBJFOCUS	-53
// 标题栏组件句柄
#define EWL_OBJCAPTION	-54
// XML属性分发回调(wParam->atomPropName,lParam->lpwzPropValue>)
#define WM_EX_XML_PROPDISPATCH	-1
// JS脚本分发回调(wParam->atomPropName,lParam->dispIdMember>)
#define WM_EX_JS_DISPATCH	-2
// 左键单击组件
#define WM_EX_LCLICK	-3
// 右键单击组件
#define WM_EX_RCLICK	-4
// 中键单击组件
#define WM_EX_MCLICK	-5
// 弹出式窗口已经初始化完毕
#define WM_EX_INITPOPUP	-6
// 弹出式窗口即将销毁 (wParam=0:即将销毁.wParam=1:是否可销毁,返回1则取消销毁)
#define WM_EX_EXITPOPUP	-7
// 发给控件用这个,窗口是EMT_EASING转NM_EASING
#define WM_EX_EASING	-8
// 控件接收到拖放(lParam为EX_DROPINFO结构体,若处理后应当返回 DROPEFFECT_开头的常量)
#define WM_EX_DROP	-9
// 控件绘制中事件,(wParam为绘制进度,lParam为EX_PAINTSTRUCT指针)
#define WM_EX_PAINTING	-10
// 当控件开始绘制后
#define EPP_BEGIN	0
// 当控件绘制背景后
#define EPP_BKG	1
// 当控件绘制边框后
#define EPP_BORDER	2
// 当控件自定义绘制后
#define EPP_CUSTOMDRAW	3
// 当控件绘制结束后
#define EPP_END	4
// 需要与GDI交互
#define ECF_D2D_GDI_COMPATIBLE	1
// 组件数值_透明度
#define EOL_ALPHA	0
// 组件数值_模糊系数
#define EOL_BLUR	1
// 组件数值_光标句柄
#define EOL_CURSOR	2
// 组件数值_扩展风格
#define EOL_EXSTYLE	3
// 组件数值_画布句柄
#define EOL_HCANVAS	4
// 组件数值_字体句柄
#define EOL_HFONT	5
// 组件数值_组件ID
#define EOL_ID	6
// 组件数值_自定义参数
#define EOL_LPARAM	7
// 组件数值_标题内容指针
#define EOL_LPWZTITLE	8
// 组件数值_节点ID
#define EOL_NODEID	9
// 组件数值_父句柄
#define EOL_OBJPARENT	10
// 组件数值_组件回调
#define EOL_OBJPROC	11
// 组件数值_控件数据
#define EOL_OWNER	12
// 组件数值_组件状态
#define EOL_STATE	13
// 组件数值_基本风格
#define EOL_STYLE	14
// 组件数值_文本格式
#define EOL_TEXTFORMAT	15
// 组件数值_用户数据
#define EOL_USERDATA	16

// 状态_正常
#define STATE_NORMAL	0
// 状态_禁止
#define STATE_DISABLE	1
// 状态_选择
#define STATE_SELECT	2
// 状态_焦点
#define STATE_FOCUS	4
// 状态_按下
#define STATE_DOWN	8
// 状态_选中
#define STATE_CHECKED	16
// 状态_半选中
#define STATE_HALFSELECT	32
// 状态_只读
#define STATE_READONLY	64
// 状态_点燃
#define STATE_HOVER	128
// 状态_默认
#define STATE_DEFAULT	256
// 状态_子项目_可视
#define STATE_SUBITEM_VISIABLE	512
// 状态_子项目_隐藏
#define STATE_SUBITEM_HIDDEN	1024
// 状态_繁忙中
#define STATE_BUSY	2048
// 状态_滚动中
#define STATE_ROLLING	8192
// 状态_动画中
#define STATE_ANIMATING	16384
// 状态_隐藏
#define STATE_HIDDEN	32768
// 状态_允许修改尺寸
#define STATE_ALLOWSIZE	131072
// 状态_允许拖动
#define STATE_ALLOWDRAG	262144
// 状态_允许焦点
#define STATE_ALLOWFOCUS	1048576
// 状态_允许选择
#define STATE_ALLOWSELECT	2097152
// 状态_超链接_点燃
#define STATE_HYPERLINK_HOVER	4194304
// 状态_超链接_已访问
#define STATE_HYPERLINK_VISITED	8388608
// 状态_允许多选
#define STATE_ALLOWMULTIPLE	16777216
// 状态_密码模式
#define STATE_PASSWORD	536870912
// 组件风格_滚动条不可用时显示禁止状态
#define EOS_DISABLENOSCROLL	33554432
// 组件风格_可调整尺寸
#define EOS_SIZEBOX	67108864
// 组件风格_禁止
#define EOS_DISABLED	134217728
// 组件风格_可视
#define EOS_VISIBLE	268435456
// 组件风格_边框
#define EOS_BORDER	536870912
// 组件风格_垂直滚动条
#define EOS_VSCROLL	1073741824
// 组件风格_水平滚动条
#define EOS_HSCROLL	-2147483648
// 组件风格_扩展_自适应尺寸
#define EOS_EX_AUTOSIZE	4194304
// 组件风格_扩展_鼠标穿透
#define EOS_EX_TRANSPARENT	8388608
// 组件风格_扩展_背景模糊
#define EOS_EX_BLUR 16777216
// 组件风格_扩展_允许拖拽
#define EOS_EX_DRAGDROP	33554432
// 组件风格_扩展_接收文件拖放
#define EOS_EX_ACCEPTFILES	67108864
// 组件风格_扩展_允许焦点
#define EOS_EX_FOCUSABLE	134217728
// 组件风格_扩展_允许TAB焦点
#define EOS_EX_TABSTOP	268435456
// 组件风格_扩展_总在最前
#define EOS_EX_TOPMOST	536870912
// 组件风格_扩展_背景混合
#define EOS_EX_COMPOSITED	1073741824
// 组件风格_扩展_自定义绘制
#define EOS_EX_CUSTOMDRAW	-2147483648
// 暂无注释
#define SB_HORZ	0
// 暂无注释
#define SB_VERT	1
// 暂无注释
#define SB_CTL	2
// 暂无注释
#define SB_BOTH	3
// 按钮风格_复选按钮
#define EBS_CHECKBUTTON	1
// 按钮风格_单选按钮
#define EBS_RADIOBUTTON	2
// 按钮风格_文本偏移
#define EBS_TEXTOFFSET	4
// 按钮风格_图标在右
#define EBS_ICONRIGHT	8
// 滚动条风格_水平滚动条
#define ESS_HORIZONTALSCROLL	0
// 滚动条风格_垂直滚动条
#define ESS_VERTICALSCROLL	1
// 滚动条风格_左顶对齐
#define ESS_LEFTTOPALIGN	2
// 滚动条风格_右底对齐
#define ESS_RIGHTBOTTOMALIGN	4
// 滚动条风格_控制按钮
#define ESS_CONTROLBUTTON	8
// 编辑框风格_允许拖拽
#define EES_DISABLEDRAG	1
// 编辑框风格_密码输入
#define EES_USEPASSWORD	2
// 编辑框风格_显示选择文本
#define EES_HIDESELECTION	4
// 编辑框风格_丰富文本
#define EES_RICHTEXT	8
// 编辑框风格_允许鸣叫
#define EES_ALLOWBEEP	16
// 编辑框风格_只读
#define EES_READONLY	32
// 编辑框风格_回车换行
#define EES_NEWLINE	64
// 编辑框风格_数值输入
#define EES_NUMERICINPUT	128
// 编辑框风格_自动选择字符
#define EES_AUTOWORDSEL	256
// 编辑框风格_禁用右键默认菜单
#define EES_DISABLEMENU	512
// 编辑框风格_解析URL
#define EES_PARSEURL	1024
// 编辑框风格_允许TAB字符
#define EES_ALLOWTAB	2048
// 编辑框风格_总是显示提示文本
#define EES_SHOWTIPSALWAYS	4096
// 编辑框风格_隐藏插入符
#define EES_HIDDENCARET	8192
// 设置提示文本(wParam:提示文本颜色,lParam:文本指针)
#define EM_SETCUEBANNER	5377
// 加载RTF文件(wParam:数据长度,lParam:数据指针)
#define EM_LOAD_RTF	6001
// 列表风格_纵向列表 
#define ELS_VERTICALLIST	0
// 列表风格_横向列表
#define ELS_HORIZONTALLIST	1
// 列表风格_允许多选 
#define ELS_ALLOWMULTIPLE	8
// 列表风格_表项跟踪 
#define ELS_ITEMTRACKING	16
// 列表风格_始终显示选择项
#define ELS_SHOWSELALWAYS	32
// 未命中
#define LVHT_NOWHERE	1
// 命中表项
#define LVHT_ONITEM	14
// 现行选中项被改变
#define LVN_ITEMCHANGED	-101
// 热点跟踪
#define LVN_HOTTRACK	-121
// 命中测试
#define LVM_HITTEST	4114
// 清空项目
#define LVM_DELETEALLITEMS	4015
// 删除项目
#define LVM_DELETEITEM	4104
// 保证显示项目
#define LVM_ENSUREVISIBLE	4115
// 取可视范围内的项目数量
#define LVM_GETCOUNTPERPAGE	4136
// 取项目总数
#define LVM_GETITEMCOUNT	4100
// 取指定项目的矩形范围
#define LVM_GETITEMRECT	4110
// 取被选择项目数
#define LVM_GETSELECTEDCOUNT	4146
// 取现行选择项目
#define LVM_GETSELECTIONMARK	4162
// 置现行选择项目
#define LVM_SETSELECTIONMARK	4163
// 取在可视范围中第一个项目的索引
#define LVM_GETTOPINDEX	4135
// 插入项目
#define LVM_INSERTITEM	4103
// 重画项目
#define LVM_REDRAWITEMS	4117
// 设置列表项目总数(wParam:表项条数,lParma:刷新风格)
#define LVM_SETITEMCOUNT	4143
// 取项目状态
#define LVM_GETITEMSTATE	4140
// 置项目状态
#define LVM_SETITEMSTATE	4139
// 取鼠标指针指向项目的索引
#define LVM_GETHOTITEM	4157
// 立即申请重算表项尺寸
#define LVM_CALCITEMSIZE	5150
// 返回值将作为列表项控件
#define TLVM_ITEM_CREATE	10010
// 暂无注释
#define TLVM_ITEM_CREATED	10011
// 暂无注释
#define TLVM_ITEM_DESTROY	10012
// wParam:nIndex,lParam:hObjItem
#define TLVM_ITEM_FILL	10013
// wParam:cbSize,lParam:pTemplate
#define TLVM_SETTEMPLATE	10020
// wParam:表项索引,返回表项容器句柄(不在可视区返回0)
#define TLVM_GETITEMOBJ	10021
// 需要与GDI交互
#define CVF_GDI_COMPATIBLE	1
// ID2D1DeviceContext
#define CVC_DX_D2DCONTEXT	1
// ID2D1Bitmap
#define CVC_DX_D2DBITMAP	2
// Grahpics*
#define CVC_GDIP_GRAPHICS	1
// 混合模式-覆盖
#define CV_COMPOSITE_MODE_SRCOVER	0
// 混合模式-拷贝
#define CV_COMPOSITE_MODE_SRCCOPY	1
// 背景颜色
#define COLOR_EX_BACKGROUND	0
// 边框颜色
#define COLOR_EX_BORDER	1
// 文本颜色.正常
#define COLOR_EX_TEXT_NORMAL	2
// 文本颜色.点燃
#define COLOR_EX_TEXT_HOVER	3
// 文本颜色.按下
#define COLOR_EX_TEXT_DOWN	4
// 文本颜色.焦点
#define COLOR_EX_TEXT_FOCUS	5
// 文本颜色.选中
#define COLOR_EX_TEXT_CHECKED	6
// 文本颜色.选择
#define COLOR_EX_TEXT_SELECT	7
// 文本颜色.热点
#define COLOR_EX_TEXT_HOT	8
// 文本颜色.已访问
#define COLOR_EX_TEXT_VISTED	9
// 文本颜色.阴影
#define COLOR_EX_TEXT_SHADOW	10
// 编辑框.光标原色
#define COLOR_EX_EDIT_CARET	30
// 编辑框.提示文本颜色
#define COLOR_EX_EDIT_BANNER	31
// 播放动画
#define BIF_PLAYIMAGE	1
// 禁用缩放
#define BIF_DISABLESCALE	2
// 九宫矩形-排除中间区域
#define BIF_GRID_EXCLUSION_CENTER	4
// X使用百分比单位
#define BIF_POSITION_Y_PERCENT	8
// Y使用百分比单位
#define BIF_POSITION_X_PERCENT	16
// 默认(缩放)
#define BIR_DEFALUT	0
// 平铺不重复
#define BIR_NO_REPEAT	1
// 水平、垂直重复平铺
#define BIR_REPEAT	2
// 水平重复平铺
#define BIR_REPEAT_X	3
// 垂直重复平铺
#define BIR_REPEAT_Y	4
// 暂无注释
#define EPF_DISABLESCALE	1
// 并集.采用两个区域的并集来合并这两个区域
#define RGN_COMBINE_UNION	0
// 交集.采用两个区域的交集来合并这两个区域
#define RGN_COMBINE_INTERSECT	1
// 异或.采用两个区域的并集，且去除重叠区域
#define RGN_COMBINE_XOR	2
// 排除.从第一个区域中排除第二个区域
#define RGN_COMBINE_EXCLUDE	3
// 单位:像素
#define UNIT_PIXEL	0
// 单位:百分比
#define UNIT_PERCENT	1
// 事件_创建
#define NM_CREATE	-99
// 事件_销毁
#define NM_DESTROY	-98
// 事件_计算尺寸
#define NM_CALCSIZE	-97
// 事件_控件移动
#define NM_MOVE	-96
// 事件_尺寸被改变
#define NM_SIZE	-95
// 事件_禁止状态被改变
#define NM_ENABLE	-94
// 事件_可视状态被改变
#define NM_SHOW	-93
// 事件_左键被放开
#define NM_LUP	-92
// 事件_离开组件
#define NM_LEAVE	-91
// 事件_时钟
#define NM_TIMER	-90
// 事件_选中
#define NM_CHECK	-89
// 事件_托盘图标
#define NM_TRAYICON	-88
// 事件_对话框初始化完毕
#define NM_INTDLG	-87
// 事件_缓动
#define NM_EASING	-86
// 事件_左键被单击
#define NM_CLICK	-2
// 事件_左键被双击
#define NM_DBLCLK	-3
// 事件_回车键被按下
#define NM_RETURN	-4
// 事件_右键被单击
#define NM_RCLICK	-5
// 事件_右键被双击
#define NM_RDBLCLK	-6
// 事件_设置焦点
#define NM_SETFOCUS	-7
// 事件_失去焦点
#define NM_KILLFOCUS	-8
// 事件_自定义绘制
#define NM_CUSTOMDRAW	-12
// 事件_进入组件
#define NM_HOVER	-13
// 事件_点击测试
#define NM_NCHITTEST	-14
// 事件_按下某键
#define NM_KEYDOWN	-15
// 事件_取消鼠标捕获
#define NM_RELEASEDCAPTURE	-16
// 事件_字符输入
#define NM_CHAR	-18
// 事件_提示框即将弹出
#define NM_TOOLTIPSCREATED	-19
// 事件_左键被按下
#define NM_LDOWN	-20
// 事件_右键被按下
#define NM_RDOWN	-21
// 事件_字体被改变
#define NM_FONTCHANGED	-23
// 获取布局父属性个数
#define ELN_GETPROPSCOUNT	1
// 获取布局子属性个数
#define ELN_GETCHILDPROPCOUNT	2
// 初始化父属性列表
#define ELN_INITPROPS	3
// 释放父属性列表
#define ELN_UNINITPROPS	4
// 初始化子属性列表
#define ELN_INITCHILDPROPS	5
// 释放子属性列表
#define ELN_UNINITCHILDPROPS	6
// 检查属性值是否正确,wParam为propID，lParam为值
#define ELN_CHECKPROPVALUE	7
// 检查子属性值是否正确,wParam低位为nIndex，高位为propID，lParam为值
#define ELN_CHECKCHILDPROPVALUE	8
// 从XML属性表填充到布局信息中
#define ELN_FILL_XML_PROPS	9
// 从XML属性表填充到父布局信息中
#define ELN_FILL_XML_CHILD_PROPS	10
// 更新布局
#define ELN_UPDATE	15
// 布局类型_无
#define ELT_NULL	0
// 布局类型_线性
#define ELT_LINEAR	1
// 布局类型_流式
#define ELT_FLOW	2
// 布局类型_页面
#define ELT_PAGE	3
// 布局类型_表格
#define ELT_TABLE	4
// 布局类型_相对
#define ELT_RELATIVE	5
// 布局类型_绝对
#define ELT_ABSOLUTE	6
// 布局属性_通用_内间距_左
#define ELP_PADDING_LEFT	-1
// 布局属性_通用_内间距_顶
#define ELP_PADDING_TOP	-2
// 布局属性_通用_内间距_右
#define ELP_PADDING_RIGHT	-3
// 布局属性_通用_内间距_底
#define ELP_PADDING_BOTTOM	-4
// 布局属性_通用_外间距_左
#define ELCP_MARGIN_LEFT	-1
// 布局属性_通用_外间距_顶
#define ELCP_MARGIN_TOP	-2
// 布局属性_通用_外间距_右
#define ELCP_MARGIN_RIGHT	-3
// 布局属性_通用_外间距_底
#define ELCP_MARGIN_BOTTOM	-4
// 布局属性:方向
#define ELP_LINEAR_DIRECTION	1
// 布局子属性:尺寸[-1或未填写为组件当前尺寸]
#define ELCP_LINEAR_SIZE	1
// 布局紫属性:另外一个方向的对齐方式
#define ELCP_LINEAR_ALIGN	2
// 布局属性:布局方向对齐方式
#define ELP_LINEAR_DALIGN	2
// 暂无注释
#define ELP_LINEAR_DALIGN_LEFT_TOP	0
// 暂无注释
#define ELP_LINEAR_DALIGN_CENTER	1
// 暂无注释
#define ELP_LINEAR_DALIGN_RIGHT_BOTTOM	2
// 暂无注释
#define ELCP_LINEAR_ALGIN_FILL	0
// 暂无注释
#define ELCP_LINEAR_ALIGN_LEFT_TOP	1
// 暂无注释
#define ELCP_LINEAR_ALIGN_CENTER	2
// 暂无注释
#define ELCP_LINEAR_ALIGN_RIGHT_BOTTOM	3
// 方向:水平
#define ELP_DIRECTION_H	0
// 方向:垂直
#define ELP_DIRECTION_V	1
// 布局属性:方向
#define ELP_FLOW_DIRECTION	1
// 布局子属性:尺寸[-1或未填写为组件当前尺寸]
#define ELCP_FLOW_SIZE	1
// 布局子属性:组件强制换行
#define ELCP_FLOW_NEW_LINE	2
// 布局属性:当前显示页面索引
#define ELP_PAGE_CURRENT	1
// 布局子属性:是否填充整个布局
#define ELCP_PAGE_FILL	1
// 布局属性_表格_所在行
#define ELCP_TABLE_ROW	1
// 布局属性_表格_所在列
#define ELCP_TABLE_CELL	2
// 布局属性_表格_跨行数
#define ELCP_TABLE_ROW_SPAN	3
// 布局属性_表格_跨列数
#define ELCP_TABLE_CELL_SPAN	4
// 布局属性_表格_是否填满
#define ELCP_TABLE_FILL	5
// 布局属性_相对_左侧于(组件)
#define ELCP_RELATIVE_LEFT_OF	1
// 布局属性_相对_之上于(组件)
#define ELCP_RELATIVE_TOP_OF	2
// 布局属性_相对_右侧于(组件)
#define ELCP_RELATIVE_RIGHT_OF	3
// 布局属性_相对_之下于(组件)
#define ELCP_RELATIVE_BOTTOM_OF	4
// 布局属性_相对_左对齐于(组件)
#define ELCP_RELATIVE_LEFT_ALIGN_OF	5
// 布局属性_相对_顶对齐于(组件)
#define ELCP_RELATIVE_TOP_ALIGN_OF	6
// 布局属性_相对_右对齐于(组件)
#define ELCP_RELATIVE_RIGHT_ALIGN_OF	7
// 布局属性_相对_底对齐于(组件)
#define ELCP_RELATIVE_BOTTOM_ALIGN_OF	8
// 布局属性_相对_水平居中于父
#define ELCP_RELATIVE_CENTER_PARENT_H	9
// 布局属性_相对_垂直居中于父
#define ELCP_RELATIVE_CENTER_PARENT_V	10
// 布局属性_绝对_左侧
#define ELCP_ABSOLUTE_LEFT	1
// 布局属性_绝对_左侧类型
#define ELCP_ABSOLUTE_LEFT_TYPE	2
// 布局属性_绝对_顶部
#define ELCP_ABSOLUTE_TOP	3
// 布局属性_绝对_顶部类型
#define ELCP_ABSOLUTE_TOP_TYPE	4
// 布局属性_绝对_右侧
#define ELCP_ABSOLUTE_RIGHT	5
// 布局属性_绝对_右侧类型
#define ELCP_ABSOLUTE_RIGHT_TYPE	6
// 布局属性_绝对_底部
#define ELCP_ABSOLUTE_BOTTOM	7
// 布局属性_绝对_底部类型
#define ELCP_ABSOLUTE_BOTTOM_TYPE	8
// 布局属性_绝对_宽度（优先级低于右侧）
#define ELCP_ABSOLUTE_WIDTH	9
// 布局属性_绝对_宽度类型
#define ELCP_ABSOLUTE_WIDTH_TYPE	10
// 布局属性_绝对_高度（优先级低于底部）
#define ELCP_ABSOLUTE_HEIGHT	11
// 布局属性_绝对_高度类型
#define ELCP_ABSOLUTE_HEIGHT_TYPE	12
// 布局属性_绝对_水平偏移量
#define ELCP_ABSOLUTE_OFFSET_H	13
// 布局属性_绝对_水平偏移量类型
#define ELCP_ABSOLUTE_OFFSET_H_TYPE	14
// 布局属性_绝对_垂直偏移量
#define ELCP_ABSOLUTE_OFFSET_V	15
// 布局属性_绝对_垂直偏移量类型
#define ELCP_ABSOLUTE_OFFSET_V_TYPE	16
// 布局属性_绝对_类型_未知(未设置或保持不变)
#define ELCP_ABSOLUTE_TYPE_UNKNOWN	0
// 布局属性_绝对_类型_像素
#define ELCP_ABSOLUTE_TYPE_PX	1
// 布局属性_绝对_类型_百分比
#define ELCP_ABSOLUTE_TYPE_PS	2
// 布局属性_绝对_类型_组件尺寸百分比，仅OFFSET可用
#define ELCP_ABSOLUTE_TYPE_OBJPS	3
// 组件位置默认值
#define EOP_DEFAULT	-2147483648
// 组合框风格_允许编辑
#define ECS_ALLOWEDIT	1
// 组合框事件_列表项被改变
#define CBN_SELCHANGE	1
// 组合框事件_编辑内容被改变
#define CBN_EDITCHANGE	5
// 组合框事件_即将弹出列表
#define CBN_DROPDOWN	7
// 组合框事件_即将关闭列表
#define CBN_CLOSEUP	8
// 组合框事件_弹出下拉列表
#define CBN_POPUPLISTWINDOW	2001
// 缓动类型_线性
#define ET_Linear	1
// 缓动类型_圆线性插值
#define ET_Clerp	2
// 缓动类型_弹性
#define ET_Spring	3
// 缓动类型_冲击
#define ET_Punch	4
// 缓动类型_二次方_In
#define ET_InQuad	5
// 缓动类型_二次方_Out
#define ET_OutQuad	6
// 缓动类型_二次方_InOut
#define ET_InOutQuad	7
// 缓动类型_三次方_In
#define ET_InCubic	8
// 缓动类型_三次方_Out
#define ET_OutCubic	9
// 缓动类型_三次方_InOut
#define ET_InOutCubic	10
// 缓动类型_四次方_In
#define ET_InQuart	11
// 缓动类型_四次方_Out
#define ET_OutQuart	12
// 缓动类型_四次方_InOut
#define ET_InOutQuart	13
// 缓动类型_五次方_In
#define ET_InQuint	14
// 缓动类型_五次方_Out
#define ET_OutQuint	15
// 缓动类型_五次方_InOut
#define ET_InOutQuint	16
// 缓动类型_正弦曲线_In
#define ET_InSine	17
// 缓动类型_正弦曲线_Out
#define ET_OutSine	18
// 缓动类型_正弦曲线_InOut
#define ET_InOutSine	19
// 缓动类型_指数曲线_In
#define ET_InExpo	20
// 缓动类型_指数曲线_Out
#define ET_OutExpo	21
// 缓动类型_指数曲线_InOut
#define ET_InOutExpo	22
// 缓动类型_圆曲线_In
#define ET_InCirc	23
// 缓动类型_圆曲线_Out
#define ET_OutCirc	24
// 缓动类型_圆曲线_InOut
#define ET_InOutCirc	25
// 缓动类型_反弹_In
#define ET_InBounce	26
// 缓动类型_反弹_Out
#define ET_OutBounce	27
// 缓动类型_反弹_InOuts
#define ET_InOutBounce	28
// 缓动类型_三次方溢出_In
#define ET_InBack	29
// 缓动类型_三次方溢出_Out
#define ET_OutBack	30
// 缓动类型_三次方溢出_InOut
#define ET_InOutBack	31
// 缓动类型_正弦曲线指数衰减_In
#define ET_InElastic	32
// 缓动类型_正弦曲线指数衰减_Out
#define ET_OutElastic	33
// 缓动类型_正弦曲线指数衰减_InOut
#define ET_InOutElastic	34
// 缓动类型_自定义 pEasingContext为自定义回调函数(nProcess,nStart,nStop,nCurrent*,pEasingContext)
#define ET_CUSTOM	50
// 缓动类型_曲线 pEasingContext为pCurveInfo(_easing_load_curve)
#define ET_CURVE	51
// 缓动模式_单次
#define ES_SINGLE	1
// 缓动模式_循环,注意自行停止
#define ES_CYCLE		2
// 缓动模式_多次,高位则为次数
#define ES_MANYTIMES		4
// 缓动模式_顺序
#define ES_ORDER		8
// 缓动模式_逆序
#define ES_REVERSE		16
// 缓动模式_来回
#define ES_BACKANDFORTH		32
// 缓动模式_调用函数 pContext为回调函数,bool isStop Cbk(pEasingProgress,double nProgress,double nCurrent,pEasingContext,nTimesSurplus,Param1,Param2,Param3,Param4)
#define ES_CALLFUNCTION	0
// 缓动模式_分发消息 pContext为hObj或hExDUI, wParam:pEasing,lParam:lpEasingInfo,result:isStop
#define ES_DISPATCHNOTIFY	128
// 缓动模式_使用线程 使用线程处理,否则在UI线程处理(过程中会阻塞输入)
#define ES_THREAD	256
// 缓动模式_释放曲线 当使用曲线类型时生效,在结束时会自动释放曲线指针
#define ES_RELEASECURVE	512
// 播放
#define EES_PLAY	0
// 暂停
#define EES_PAUSE	1
// 停止
#define EES_STOP	2
// 暂无注释
#define sizeof_ex_nmhdr	20
// 暂无注释
#define sizeof_ex_easing	44
// 暂无注释
#define sizeof_ex_jsdispatchinfo	20
// 暂无注释
#define sizeof_ex_customdraw	40
// 暂无注释
#define sizeof_ex_paintstruct	72
// 暂无注释
#define sizeof_ex_dropinfo	16
// 暂无注释
#define TVI_FIRST	-65535
// 暂无注释
#define TVI_LAST	-65534
// 暂无注释
#define TVI_ROOT	-65536
// 暂无注释
#define TVI_SORT	-65533
// 删除节点及所有子孙(lParam为节点句柄,0或TVI_ROOT为删除所有)
#define TVM_DELETEITEM	4353
// 保证显示(lParam为显示的节点句柄)
#define TVM_ENSUREVISIBLE	4372
// 展开收缩(wParam为是否展开,lParam为设置的节点句柄)
#define TVM_EXPAND	4354
// 取节点数
#define TVM_GETCOUNT	4357
// 取留白宽度
#define TVM_GETINDENT	4358
// 取节点信息(wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TVM_GETITEMW	4414
// 取节点矩形(wParam为节点句柄,lParam为 EX_RECT 指针,注意该节点必须处于可见范围,否则消息无法获取并返回0)
#define TVM_GETITEMRECT	4356
// 取相关节点(wParam为 TVGN_ 开头的常量,lParam为节点句柄)
#define TVM_GETNEXTITEM	4362
// 取展开可视节点个数
#define TVM_GETVISIBLECOUNT	4368
// 命中测试(wParam低位为x高位为y[相对控件],lParam为 返回#TVHT_开头常量 的指针,消息返回值为命中的节点句柄)
#define TVM_HITTEST	4369
// 插入节点(lParam为 EX_TREEVIEW_ITEMINFO 指针，tzText为Unicode)
#define TVM_INSERTITEMW	4402
// 置选中项(lParam为选中的节点句柄)
#define TVM_SELECTITEM	4363
// 设置留白宽度(wParam为留白宽度)
#define TVM_SETINDENT	4359
// 设置节点标题(wParam为节点句柄,lParam为 EX_TREEVIEW_ITEMINFO 指针)
#define TVM_SETITEMW	4415
// 更新树形框
#define TVM_UPDATE	4499
// 设置行高(lParam为新行高)
#define TVM_SETITEMHEIGHT	5091
// 获取行高
#define TVM_GETITEMHEIGHT	5092
// 从索引获取节点句柄(wParam为索引,节点必须可见否则返回0)
#define TVM_GETNODEFROMINDEX	5093
// 设置节点标题(wParam为节点句柄,lParam为 文本指针,Unicode)
#define TVM_SETITEMTEXTW	14414
// 获取节点标题(wParam为节点句柄,返回值为标题Unicode字符串,不要自行释放)
#define TVM_GETITEMTEXTW	14415
// 获取根节点
#define TVGN_ROOT	0
// 获取下一个节点
#define TVGN_NEXT	1
// 获取上一个节点
#define TVGN_PREVIOUS	2
// 获取父节点
#define TVGN_PARENT	3
// 获取子节点
#define TVGN_CHILD	4
// 获取下一个可见节点
#define TVGN_NEXTVISIBLE	6
// 没有命中
#define TVHT_NOWHERE	1
// 命中图标
#define TVHT_ONITEMICON	2
// 命中标题
#define TVHT_ONITEMLABEL	4
// 命中留白
#define TVHT_ONITEMINDENT	8
// 命中加减框
#define TVHT_ONITEMSTATEICON	64
// 删除节点
#define TVN_DELETEITEM	391
// 节点展开
#define TVN_ITEMEXPANDED	394
// 节点展开中
#define TVN_ITEMEXPANDING	395
// 绘制节点
#define TVN_DRAWITEM	3099
// 暂无注释
#define sizeof_ex_treeview_insertinfo	40
// 暂无注释
#define sizeof_ex_treeview_iteminfo	28
// 报表框_绘制横线
#define ERLS_DRAWHORIZONTALLINE	256
// 报表框_绘制竖线
#define ERLS_DRAWVERTICALLINE	512
// 报表框_无表头
#define ERLS_NOHEAD	1024
// 表头风格_可点击
#define ERLV_CS_CLICKABLE	1
// 表头风格_锁定宽度
#define ERLV_CS_LOCKWIDTH	2
// 表头风格_可排序(前提是得可点击)
#define ERLV_CS_SORTABLE	4
// 获取表象(lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_GETITEM	4101
// 设置表象(lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_SETITEM	4102
// 获取表象文本(wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_GETITEMTEXT	4141
// 设置表象文本(wParam若不为0则为表项索引,lParam为EX_REPORTLIST_ITEMINFO指针)
#define LVM_SETITEMTEXT	4142
// 插入列(wParm为是否立即更新,lParam为EX_REPORTLIST_COLUMNINFO指针)
#define LVM_INSERTCOLUMN	4123
// 删除列(wParm为是否立即更新,lParam为列索引)
#define LVM_DELETECOLUMN	4124
// 排序(lParam为EX_REPORTLIST_SORTINFO指针)
#define LVM_SORTITEMS	4144
// 更新列表框
#define LVM_UPDATE	4138
// 删除所有列
#define LVM_DELETEALLCOLUMN	4900
// 获取列数
#define LVM_GETCOLUMNCOUNT	4901
// 获取列信息(wParam为列索引,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LVM_GETCOLUMN	4902
// 设置列信息(wParam低位为列索引,高位为是否立即刷新,lParam为 EX_REPORTLIST_COLUMNINFO 指针)
#define LVM_SETCOLUMN	4903
// 设置列标题(wParam低位为列索引,高位为是否立即刷新,lParam为 宽文本指针)
#define LVM_SETCOLUMNTEXT	4904
// 获取列标题(wParam为列索引,lParam为 宽文本指针)
#define LVM_GETCOLUMNTEXT	4905
// 获取列宽
#define LVM_GETCOLUMNWIDTH	4906
// 设置列宽(wParam为列索引,lParam为 列宽)
#define LVM_SETCOLUMNWIDTH	4907
// 设置表项高度
#define LVM_SETITEMHEIGHT	4908
// 获取表项高度(lParam为新行高)
#define LVM_GETITEMHEIGHT	4909
// 获取图片组
#define LVM_GETIMAGELIST	4098
// 设置图片组(wParam为是否更新表项宽高,lParam为图片组句柄)
#define LVM_SETIMAGELIST	4099
// 表头被单击
#define RLVN_COLUMNCLICK	97000
// 绘制表行
#define RLVN_DRAW_TR	97001
// 绘制表项
#define RLVN_DRAW_TD	97002
// 检查框点击事件
#define RLVN_CHECK	97003
// 检查框点击消息
#define RLVM_CHECK	99001
// 设置检查框状态
#define RLVM_SETCHECK	99002
// 获取检查框状态
#define RLVM_GETCHECK	99003
// 表项带检查框
#define ERLV_RS_CHECKBOX	1
// 检查框为选中状态
#define ERLV_RS_CHECKBOX_CHECK	2
// 暂无注释
#define sizeof_ex_reportlist_columninfo	24
// 暂无注释
#define sizeof_ex_reportlist_rowinfo	16
// 暂无注释
#define sizeof_ex_reportlist_iteminfo	28
// 暂无注释
#define sizeof_ex_reportlist_sortinfo	20
// 设置图片组(wParam为是否更新表项宽高,lParam为图片组句柄)
#define TVM_SETIMAGELIST	4361
// 获取图片组
#define TVM_GETIMAGELIST	4360
// 树形框风格_显示加减号
#define ETS_SHOWADDANDSUB	64
// 树形框风格_显示连接线
#define ETS_SHOWCABLE	128
// 显示
#define SW_SHOW	5
// 隐藏
#define SW_HIDE	0
// 最大化
#define SW_SHOWMAXIMIZED	3
// 最小化
#define SW_SHOWMINIMIZED	2
// 不激活
#define SW_SHOWNOACTIVATE	4
// 子控件
#define GW_CHILD	5
// 上一个兄弟
#define GW_HWNDPREV	3
// 下一个兄弟
#define GW_HWNDNEXT	2
// 暂无注释
#define IDYES	6
// 暂无注释
#define IDNO	7
// 暂无注释
#define IDCANCEL	2
// 暂无注释
#define IDOK	1
// 暂无注释
#define IDCLOSE	8
// 暂无注释
//#define NULL	0
// 设置进度条范围
#define PBM_SETRANGE	1025
// 设置进度条位置
#define PBM_SETPOS	1026
// 获取进度条位置
#define PBM_GETPOS	1032
// 获取进度条范围
#define PBM_GETRANGE	1031
// 主题包头
#define EPDF_THEME	254
// 资源包头
#define EPDF_FILES	255
// 让菜单按钮弹出菜单(wParam:菜单组,lParam:菜单句柄)
#define MBM_POPUP	103001
// 菜单弹出事件(wParam:菜单组,lParam:菜单句柄)
#define MBN_POPUP	102401
// 位图
#define IMAGE_BITMAP	0
// 图标
#define IMAGE_ICON	1
//字体风格_普通
#define EFS_DEFAULT	0
//字体风格_加粗
#define EFS_BLOD	1
//字体风格_倾斜
#define EFS_SLANT 2
//字体风格_加粗倾斜
#define EFS_BLODANDSLANT 3
//字体风格_下划线
#define EFS_UNDERLINE 4
//字体风格_删除线
#define EFS_STRICKOUT 5

#define IRF_DEFAULT					0		//不做改变
#define IRF_FLIP_L					4		//水平翻转
#define IRF_FLIP_V					8		//垂直翻转
#define IRF_FLIP_LV					12		//水平翻转然后垂直翻转
#define IRF_ROTATE_90				1		//90度旋转
#define IRF_ROTATE_180				2		//180度旋转
#define IRF_ROTATE_270				3		//270度旋转
#define IRF_ROTATE_90_L_FLIP		5		//90度旋转然后水平翻转
#define IRF_ROTATE_180_L_FLIP		6		//180度旋转然后水平翻转
#define IRF_ROTATE_270_L_FLIP		7		//270度旋转然后水平翻转
#define IRF_ROTATE_90_V_FLIP		9		//90度旋转然后垂直翻转
#define IRF_ROTATE_180_V_FLIP		10		//180度旋转然后垂直翻转
#define IRF_ROTATE_270_V_FLIP		11		//270度旋转然后垂直翻转
#define IRF_ROTATE_90_LV_FLIP		13		//90度旋转然后水平翻转然后垂直翻转
#define IRF_ROTATE_180_LV_FLIP		14		//180度旋转然后水平翻转然后垂直翻转
#define IRF_ROTATE_270_LV_FLIP		15		//270度旋转然后水平翻转然后垂直翻转

	//颜色
//typedef UINT32	ARGB, RGB0;	//ARGB和RGB
typedef UINT8	CHANNEL;	//通道
//
typedef COLORREF EXARGB;		// 颜色
typedef UINT32 EXATOM;			// 原子号

typedef UINT32 EXHANDLE;		// 句柄
typedef EXHANDLE HEXDUI;		// 界面句柄
typedef EXHANDLE HEXLAYOUT;		// 布局句柄
typedef LPVOID HEXTHEME;		// 主题句柄
typedef EXHANDLE HEXOBJ;		// 控件句柄
typedef EXHANDLE HEXCANVAS;		// 画布句柄
typedef LPVOID HEXBRUSH;		// 刷子句柄
typedef EXHANDLE HEXIMAGE;		// 图片句柄
typedef LPVOID HEXFONT;		// 字体句柄
typedef LPVOID HEXEASING;		// 缓动句柄/指针
typedef EXHANDLE HEXPATH;		// 路径句柄
typedef LPVOID HEXMATRIX;		// 矩阵句柄


struct EX_NMHDR
{
	HEXOBJ	hObjFrom;			// 0	组件句柄
	INT		idFrom;				// 4	组件ID
	INT		nCode;				// 8	通知消息
	WPARAM	wParam;				// 12	无符号整数	::通常是一个与消息有关的常量值，也可能是窗口或控件的句柄
	LPARAM	lParam;				// 16	长整型		::通常是一个指向内存中数据的指针
};
struct RECTF
{
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
};
struct EX_CUSTOMDRAW {
	HEXCANVAS	hCanvas;
	HEXTHEME	hTheme;
	DWORD		dwState;
	DWORD		dwStyle;
	RECT		rcDraw;
	//int left;
	//int top;
	//int right;
	//int bottom;
	INT			iItem;
	INT			iItemParam;
};
struct EX_PAINTSTRUCT2
{
	HEXCANVAS hCanvas;		// 0	画布
	HEXTHEME hTheme;		// 4	主题
	DWORD dwStyle;			// 8	风格
	DWORD dwStyleEx;		// 12	扩展风格
	DWORD dwTextFormat;		// 16	文本格式
	DWORD dwState;			// 20	状态
	INT dwOwnerData;		// 24	所有数据
	UINT uWidth;			// 28	宽度
	UINT uHeight;			// 32	高度
	RECT rcPaint;			// 36	绘制矩形
	//int pLeft;			// 36	绘制矩形::左
	//int pTop;				// 40	绘制矩形::顶
	//int pRight;			// 44	绘制矩形::右
	//int pBottom;			// 48	绘制矩形::底
	RECT rcText;			// 52	文本矩形
	//int tLeft;			// 52	文本矩形::左
	//int tTop;				// 56	文本矩形::顶
	//int tRight;			// 60	文本矩形::右
	//int tBottom;			// 64	文本矩形::底
	INT dwReserved;			// 68	保留
};
struct EX_BITMAPDATA {
	UINT		Width;
	UINT		Height;
	INT			Stride;
	INT			PixelFormat;
	COLORREF* Scan0;
	INT			Reserved;
};
#pragma pack(1)
struct EX_EASINGINFO
{
	LPVOID		pEasing;			// 0  缓动指针
	DOUBLE		nProgress;			// 4  进度 0-1
	DOUBLE		nCurrent;			// 12 当前值
	LPVOID	pEasingContext;		// 20 缓动参数
	UINT		nTimesSurplus;		// 24 剩余数
	size_t		p1;					// 28 参数1
	size_t		p2;					// 32 参数2
	size_t		p3;					// 36 参数3
	size_t		p4;					// 40 参数4
};
#pragma pack()
struct EX_REPORTLIST_COLUMNINFO {
	LPCWSTR		wzText;				//表头标题
	UINT		nWidth;				//列宽度
	DWORD		dwStyle;			//表头风格 ERLV_CS_
	DWORD		dwTextFormat;		//列文本格式
	EXARGB		crText;				//列文本颜色
	UINT		nInsertIndex;		//插入位置,0为在最后
};
struct EX_REPORTLIST_ITEMINFO {
	UINT		iRow;				//所在行[IN / OUT]
	UINT		iCol;				//所在列[IN / OUT]
	DWORD		dwStyle;			//表行风格(同行共用)
	LPCWSTR		wzText;				//表项文本
	UINT		nImageIndex;		//表项图片(同行共用)
	LPARAM		lParam;				//表项参数(同行共用)
	DWORD		dwState;			//表项状态(同行共用)
};
struct EX_REPORTLIST_ROWINFO {
	UINT		nInsertIndex;		//插入位置,0为最后 
	DWORD		dwStyle;			//表行风格 ERLV_RS_ 
	LPARAM		lParam;				//参数 
	UINT		nImageIndex;		//图片组索引 
};
struct EX_REPORTLIST_SORTINFO {
	UINT		iCol;				//0为按row.lParam排序 
	UINT		nType;				//0:文本,1:整数 
	INT			lpfnCmp;			//result cmp(hObj,nIndex1,pvData1,nIndex2,pvData2,nIndexCol,nType,lParam) 
	BOOL		fDesc;				// 是否倒序
	LPARAM		lParam;				//
};
struct EX_TREEVIEW_INSERTINFO {
	INT itemParent;					// 父项句柄（0为根项）
	INT itemInsertAfter;			// 插入在此项之后（必须是同层）
	INT nID;						// ID
	LPCWSTR tzText;					// 表项文本ansi / unicode
	LPARAM lParam;					// 参数
	INT nImageIndex;				// 收缩时图片索引
	INT nImageIndexExpand;			// 展开时图片索引
	BOOL fExpand;					// 是否展开
	DWORD dwStyle;					// 风格
	BOOL fUpdateLater;				// 是否暂不更新(统一用TVM_UPDATE更新)
};
struct EX_TREEVIEW_NODEITEM
{
	INT nID;						// ID
	LPCWSTR wzText;					// 文本
	INT pvParam;					// 
	INT nImageIndex;				// 图片索引
	INT nImageIndexExpand;			// 展开图片索引
	BOOL fExpand;					// 是否展开
	DWORD dwStyle;					// 状态
	INT nDepth;						// 层次
	INT pNodeParent;				// 父节点
	INT pNodePrev;					// 上一个节点
	INT pNodeNext;					// 下一个节点
	INT pNodeChildFirst;			// 子项目优先
	INT nCountChild;				// 子项目

};
struct EX_BACKGROUNDIMAGEINFO {
	DWORD		dwFlags;				//标识
	HEXIMAGE	hImage;				//图片指针
	INT		    x;			//
	INT		    y;		//
	DWORD		dwRepeat;	//重复方式
	INT		    lpGrid;		//九宫矩形
	INT         lpDelay;
	DWORD         curFrame;
	DWORD         maxFrame;
	DWORD       dwAlpha;
};