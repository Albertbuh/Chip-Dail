	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'

	org 100h

Start:
	stdcall Screen.SetMode,  $0013

@@:
	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Keyboard.KeyPress ; ax <- key
	stdcall Chip.Move

	cmp	ax, KEY_ESCAPE
	je	.end
	loop	@B

.end:
	ret




	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'

x_pos	dw 160
y_pos	dw 100



