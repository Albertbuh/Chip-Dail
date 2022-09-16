format PE GUI 4.0
entry  start

include 	'win32a.inc'

section '.data' data readable writeable

class	db	'FASMWIN32',0
title	db	'Window',0
classb db 'BUTTON',0
classe db 'EDIT',0
classs db 'STATIC',0

textb1 db 'Copy',0
textb2 db 'Clear',0
textg db 'Image',0
texts db 'Text',0
textc db 'Clear all',0
errtxt db 'Error',0
hwnd dd ?
hwnde dd ?
hwnds dd ?
hwndc dd ?
text rb 100

form	db	'получено файлов: %d',0

wc	WNDCLASS       0, WindowProc, 0,0,0,0,0,COLOR_BTNFACE+1,0,class

msg	MSG

section '.code' code readable executable

start:

invoke	GetModuleHandle,0
mov	[wc.hInstance], eax
invoke	LoadIcon, 0,IDI_QUESTION
mov	[wc.hIcon], eax
invoke	LoadCursor, 0,IDC_ARROW
mov	[wc.hCursor], eax
invoke RegisterClass, wc
cmp    eax, 0
je     error

invoke	CreateWindowEx, WS_EX_ACCEPTFILES,class,title,WS_VISIBLE+WS_SYSMENU, 128,128,256,192,0,0,[wc.hInstance],0
cmp	eax, 0
je	error
mov	[hwnd], eax
msg_loop:
invoke	GetMessage, msg,0,0,0
cmp	eax, 0
je	end_loop
invoke	IsDialogMessage, [hwnd],msg
cmp	eax, 0
jne	msg_loop
invoke	TranslateMessage, msg
invoke	DispatchMessage, msg
jmp	msg_loop

error:
invoke	MessageBox, 0,errtxt, 0,MB_ICONERROR+MB_OK

end_loop:
invoke	ExitProcess, [msg.wParam]

proc	WindowProc hwnd,wmsg,wparam,Iparam
push	ebx esi edi
cmp	[wmsg], WM_CREATE
je	.wmcreate
cmp	[wmsg], WM_COMMAND
je	.wmcommand
cmp	[wmsg], WM_DROPFILES
je	.drop
cmp	[wmsg], WM_DESTROY
je	.wmdestroy
.defwndproc:
invoke	DefWindowProc, [hwnd],[wmsg],[wparam],[Iparam]
jmp	.finish
.wmcreate:
invoke	CreateWindowEx, 0,classb, textg, WS_VISIBLE+WS_CHILD+BS_GROUPBOX,5,5,240,150,[hwnd],1000, [wc.hInstance],0
invoke	CreateWindowEx, 0,classb, textb1, WS_VISIBLE+WS_CHILD+BS_PUSHBUTTON+WS_TABSTOP,20,100,100,40,[hwnd],1001,[wc.hInstance],0
invoke	CreateWindowEx, 0,classb, textb2, WS_VISIBLE+WS_CHILD+BS_PUSHBUTTON+WS_TABSTOP,130,100,100,40,[hwnd],1002,[wc.hInstance],0
invoke	CreateWindowEx, 0,classe, texts, WS_VISIBLE+WS_CHILD+WS_BORDER+WS_TABSTOP+ES_AUTOHSCROLL,10,25,230,20,[hwnd],1003,[wc.hInstance],0
mov	[hwnde], eax
invoke	CreateWindowEx, 0,classe,0,WS_VISIBLE+WS_CHILD+WS_BORDER+WS_TABSTOP+ES_AUTOHSCROLL+ES_READONLY,10,50,230,20,[hwnd],1004,[wc.hInstance],0
mov	[hwnds], eax
invoke	CreateWindowEx, 0,classb,textc,WS_VISIBLE+WS_CHILD+BS_AUTOCHECKBOX+WS_TABSTOP,130,75,110,20,[hwnd],1005,[wc.hInstance],0
mov	[hwndc], eax
invoke	CreateWindowEx, 0,classs,textg,WS_VISIBLE+WS_CHILD,100,145,43,15,[hwnd],1006,[wc.hInstance],0
invoke	SetFocus,[hwnde]
jmp	.finish

.wmcommand:
cmp	[wparam],1001
je	.but1
cmp	[wparam],1002
je	.but2
jmp	.finish

.but1:
invoke	SendMessage, [hwnde],WM_GETTEXT,100,text
invoke	SendMessage, [hwnds], WM_SETTEXT,0,text
jmp	.finish
.but2:
invoke SendMessage,[hwnds],WM_SETTEXT,0,0
invoke SendMessage,[hwndc],BM_GETCHECK,0,0
cmp    eax, BST_CHECKED

jne    .finish
invoke SendMessage, [hwnde], WM_SETTEXT,0,0
invoke SendMessage, [hwndc], BM_SETCHECK,BST_UNCHECKED,0
jmp    .finish

;opening file through dropping it into form with cursor
.drop:
invoke	DragQueryFile, [wparam], -1,text,100
invoke	wsprintf,  text,form,eax
invoke	SendMessage, [hwnds], WM_SETTEXT,0,text
invoke	DragQueryFile, [wparam],0,text,100
invoke	SendMessage, [hwnde], WM_SETTEXT,0,text
jmp	.finish

.wmdestroy:
invoke	PostQuitMessage, 0
mov	eax, 0


.finish:
pop	edi esi ebx
ret
endp



section '.idata' import data readable writeable

library kernel32,'KERNEL32.DLL',\
	user32,'USER32.DLL',\
	shell32, 'SHELL32.DLL'

include 'api\kernel32.inc'
include 'api\user32.inc'
include 'api\shell32.inc'