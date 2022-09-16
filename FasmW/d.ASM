format PE GUI 4.0
entry  start

include 'win32a.inc'
include 'encoding\WIN1251.inc'

IDR_ICON	=  17

section '.data' data readable writable

class	db	'FASMWIN32',0
title	db	'Окно',0
hwnd	dd	?



mb111	db	'Пункт 1-1-1',0
mb13	db	'Пункт 1-3',0
hmenu	dd	?
menuinfo	MENUITEMINFO	sizeof.MENUITEMINFO, MIIM_STATE

wc	WNDCLASS	0,WindowProc,0,0,0,0,0,COLOR_BTNFACE+1,0,class

msg	MSG

section '.code' code readable executable

start:
invoke	GetModuleHandle, 0
mov	[wc.hInstance], eax
invoke	LoadIcon, eax,IDR_ICON
mov	[wc.hIcon], eax
invoke	LoadCursor, [wc.hInstance],IDC_ARROW
mov	[wc.hCursor], eax
invoke	RegisterClass, wc
cmp	eax, 0
je	error

invoke	LoadMenu,[wc.hInstance],1
mov	[hmenu], eax
invoke	CreateWindowEx, 0,class, title, WS_VISIBLE+WS_OVERLAPPEDWINDOW,\
			CW_USEDEFAULT, CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,\
			0, eax, [wc.hInstance],0
cmp	eax,0
je	error
mov	[hwnd], eax
msg_loop:
invoke	GetMessage, msg,0,0,0
cmp	eax, 0
je	end_loop
invoke	IsDialogMessage, [hwnd], msg
cmp	eax,0
jne	msg_loop
invoke	TranslateMessage, msg
invoke	DispatchMessage, msg
jmp	msg_loop

error:
invoke	MessageBox, 0,0,0

end_loop:
invoke	ExitProcess, [msg.wParam]

proc	WindowProc	hwnd,wmsg,wparam,Iparam
push	ebx esi edi
cmp	[wmsg], WM_COMMAND
je	.wmcommand
cmp	[wmsg], WM_DESTROY
je	.wmdestroy
.defwndproc:
invoke	DefWindowProc, [hwnd],[wmsg],[wparam],[Iparam]
jmp	.finish
.wmcommand:
cmp	[wparam],111
je	.111
cmp	[wparam],13
je	.13
cmp	[wparam], 21
je	.21
jmp	.finish

.111:
invoke	MessageBox,0,mb111,title,MB_OK
jmp	.finish
.13:
invoke	MessageBox,0,mb13,title,MB_OK
.21:
invoke	GetMenuItemInfo, [hmenu],21,0,menuinfo
xor	[menuinfo.fState], MFS_CHECKED
invoke	SetMenuItemInfo, [hmenu], 21,0, menuinfo
cmp	eax,0
je	error
jmp	.finish
.wmdestroy:
invoke	PostQuitMessage,0
mov	eax,0

.finish:
pop  edi esi ebx
ret
endp

section '.idata' import data readable writable

library kernel32,'KERNEL32.DLL',\
	user32,'USER32.DLL',\
	  gdi,'GDI32.DLL'

include 'api\kernel32.inc'
include 'api\user32.inc'


section '.rsrc' resource data readable
directory	RT_MENU, menus,\
		RT_ICON, icons,\
		RT_GROUP_ICON,group_icons

resource	menus,\
		1, LANG_RUSSIAN+SUBLANG_DEFAULT,main_menu

resource	icons,\
		1,LANG_NEUTRAL,icon_data

resource	group_icons,\
		IDR_ICON,LANG_NEUTRAL,main_icon



menu main_menu
menuitem 'Меню 1',10,MFR_POPUP
menuitem 'Пункт 1-1',11,MFR_POPUP
menuitem 'Пункт 1-1-1',111,MFR_END
menuitem 'Пункт 1-2',12,MFT_STRING,MFS_GRAYED
menuseparator
menuitem 'Пункт 1-3',13,MFR_END,MFS_DEFAULT
menuitem 'Меню 2',20,MFR_POPUP
menuitem 'Пункт 2-1',21,MFR_END,MFS_CHECKED
menuitem 'Меню 3',30,MFR_END

icon		main_icon,icon_data,'minipad.ico'

