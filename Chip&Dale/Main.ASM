	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'
	include 'units\Phys\Phys.h'

	org 100h

EntrypPoint:
	stdcall Screen.SetMode,  $0013

.Start:
	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Keyboard.KeyPress ; ax <- key


	cmp	ax, KEY_SPACE
	jne	@F
	stdcall Phys.Jump
	stdcall Phys.Falling
@@:
	cmp	ax, KEY_ESCAPE
	je	.end
	stdcall Chip.Move


	loop	.Start

.end:
	ret









	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
x_pos	dw 160
y_pos	dw 110

