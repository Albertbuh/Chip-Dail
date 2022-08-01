	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'
	include 'units\Phys\Phys.h'
	include 'classes\Box\Box.h'
True = 1
False = 0
	org 100h

EntrypPoint:
	stdcall Screen.SetMode,  $0013

.Start:
	stdcall Screen.bkgClear
	stdcall Box.CanBeLifted

	stdcall Chip.Draw, [x_pos], [y_pos]

	cmp	[boxUpped], True
	je	.keypress
	stdcall Box.Draw, [box_x], [box_y]

.keypress:
	stdcall Keyboard.KeyPress

.Esc:
	cmp	ax, KEY_ESCAPE
	je	.end
.Space:
	cmp	ax, KEY_SPACE
	jne	.upbox
	stdcall Phys.Jump
	stdcall Phys.Falling
.upbox:
	cmp	ax, KEY_Q
	jne	.moveKeys
	cmp	[canLift], 1
	jne	.moveKeys
	stdcall Chip.GetBox
.moveKeys:
	stdcall Chip.Move

	jmp	.Start

.end:
	ret





	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\Box\Box.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
x_pos	dw 160
y_pos	dw 150-H
box_x	dw 200
box_y	dw 149
canLift   db   False
boxUpped  db   False
     ;	 include 'classes\Box\Box.du'

