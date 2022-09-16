format PE GUI 4.0
entry start

include 'win32a.inc'

section '.data' data readable writeable

_class	db		'FASMWIN32', 0
_title	db		'Пустое Окно',0
_error	db		'Ошибка', 0

_btnClass	db	'BUTTON',0
_btnText	db	'Press me',0
_msTitle	db	'ABOBA',0
wc	WNDCLASS	0,WindowProc,0,0,0,0,0, COLOR_BTNFACE+10,0,_class

msg	MSG

section '.code' code readable executable

start:

invoke	GetModuleHandle,0  ;return module identificator to eax
mov	[wc.hInstance], eax
invoke	LoadIcon,0,IDI_QUESTION
mov	[wc.hIcon], eax
invoke	LoadCursor,0,IDC_ARROW
mov	[wc.hCursor], eax
invoke	RegisterClass, wc
cmp	eax,0
je	error

invoke	CreateWindowEx, 0,_class,_title,WS_VISIBLE+WS_DLGFRAME+WS_SYSMENU,128,128,256,192,0,0,[wc.hInstance],0
cmp	eax, 0
je	error
msg_loop:
invoke	GetMessage, msg,0,0,0
cmp	eax,0
je	end_loop
invoke	TranslateMessage,msg
invoke	DispatchMessage, msg
jmp	msg_loop

error:
invoke	MessageBox,0,_error,0,MB_ICONERROR+MB_OK

end_loop:
invoke	ExitProcess,[msg.wParam]

proc	WindowProc  hwnd,wmsg,wparam, Iparam
push	ebx esi edi
cmp	[wmsg], WM_CREATE
je	.wmcreate
cmp	[wmsg], WM_COMMAND
je	.wmcommand
cmp	[wmsg], WM_DESTROY
je	.wmdestroy
.defwndproc:
invoke	DefWindowProc,[hwnd],[wmsg],[wparam],[Iparam]
jmp	.finish
.wmcreate:
invoke	CreateWindowEx, 0,_btnClass, _btnText, WS_VISIBLE+WS_CHILD+BS_PUSHBUTTON,10,10,100,50,[hwnd],1001,[wc.hInstance],NULL
jmp	.finish
.wmcommand:
cmp	[wparam], 1001
jne	.finish
invoke	MessageBox, [hwnd], _btnText, _msTitle, 0
jmp	.finish
.wmdestroy:
invoke	PostQuitMessage,0
mov	eax, 0
.finish:
pop	edi esi ebx
ret
endp

section '.idata' import data readable writeable

library kernel32,'KERNEL32.DLL',\
user32,'USER32.DLL'

include 'api\kernel32.inc'
include 'api\user32.inc'

