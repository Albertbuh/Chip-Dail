	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'
	include 'units\Phys\Phys.h'
	include 'classes\Box\Box.h'
True = 1
False = 0
ZERO = 0
QUIT = $7023
	org 100h

EntrypPoint:
	stdcall Screen.SetMode,  $0013
.A:
	stdcall Timer

.end:
	ret


proc	Timer
	locals
	    wOldSec	dw	?
	    RepSec	dq     1.2
	    FloatSec	dw     ?
	endl
.cycle:
	mov	ah, $2C
	int	21h
	movzx	dx, dl
	mov	[FloatSec], dx
	fild	 [FloatSec]
	fdiv	 [RepSec]
	fistp	   [FloatSec]
	xor	  dx,dx
	mov	  dx, [FloatSec]
	cmp	[wOldSec], dx
	je	.skip
	mov	[wOldSec], dx

	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Box.Create, Boxes_coordinates


	Call	ABOBA
.skip:
	stdcall KeyModel
	cmp	ax, QUIT
	jne	.cycle
	ret
endp

ABOBA:
	stdcall KeyModel
.next:
	stdcall BoxShoot

	stdcall Jump
	cmp	[FallingFlag], True
	jne	.skip
	stdcall Falling
	cmp	[FallingFlag], False
	jne	 .skip
	stdcall SlowDown
.skip:
	ret
















	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\MainHeroes\Chip.Move.c'
	include 'classes\Box\Box.c'
	include 'classes\Box\BoxShoot.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
	include 'units\Phys\Contact.c'
	include 'units\Keyboard\KeyPress.c'
;chip values
x_pos	dw 10
y_pos	dw 149
contactFlag db		0000_0000b
shiftState  db		False


;phys values
JumpFlag	db	False
jpLimY		dw 0
FallingFlag	db	False

;Keyboard
prev_key    dw		?

;box values
aboba = 109
Boxes_coordinates   dw	150,149-box_a,\
			150,149,\
			160, 149,\
			170, 149,\
			310-box_a,aboba,\
			310-box_a*2,aboba,\
			310-box_a*3,aboba,\
			310-box_a*4,aboba,\
			310-box_a,aboba-60,\
			310-box_a*2,aboba-60,\
			310-box_a*3,aboba-60,\
			310-box_a*4,aboba -60
box_col=($-Boxes_coordinates)/4
	include 'classes\Box\Box.du'

