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
	mov	bx, ax

.A:

	mov	di, ChipRight
	mov	[ChipImgAdr], di
	stdcall ViewUpdate
	stdcall Timer

.end:

	stdcall Screen.SetMode, bx
       ; mov	 ah, $4C
       ; int	 10h
	ret


proc	Timer  uses  bx
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

	stdcall ViewUpdate

	stdcall KeyModel
	cmp	ax, QUIT
	je	.end
.next:
	stdcall BoxShoot


	stdcall Jump
       ; stdcall ViewUpdate
	cmp	[FallingFlag], True
	jne	.skip
	stdcall Falling
       ; stdcall  ViewUpdate
	cmp	[FallingFlag], False
	jne	 .skip

	stdcall SlowDown
	stdcall ViewUpdate



.skip:

	stdcall KeyModel
	cmp	ax, QUIT
	jne	.cycle
.end:
	ret
endp

ABOBA:

.skip:
	ret

proc	ViewUpdate   uses es di ax cx bx dx



	mov	ax, cs
	add	ax, 1000h
	mov	es, ax

	stdcall Screen.bkgClear
	stdcall Box.Create, Boxes_coordinates
	stdcall Chip.Draw, [x_pos], [y_pos]

	Call	Buf

	ret
endp

Buf:
	push	ds es
	mov	ax,es
	mov	ds,ax
	push	$A000
	pop	es
	mov	cx, 320*200
	xor	si, si
	xor	di, di
	rep	movsw
	pop	es ds
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
	include 'BMP.c'


;chip values
x_pos	dw 10
y_pos	dw 149
contactFlag db		0000_0000b
shiftState  db		False



;phys values
JumpFlag	db	False
jpLimY		dw 0
FallingFlag	db	False

;key value
prev_key    dw		?

;box values
aboba = 109
Boxes_coordinates   dw	150,149-box_y,\
			150,149,\
			150+box_x, 149,\
			150+box_x*2, 149,\
			310-box_x*4,aboba,\
			310-box_x*3,aboba,\
			310-box_x*2,aboba,\
			310-box_x*1,aboba,\
			310-box_x*4,aboba-60,\
			310-box_x*3,aboba-60,\
			310-box_x*2,aboba-60,\
			310-box_x*1,aboba -60
box_col=($-Boxes_coordinates)/4
	include 'classes\Box\Box.du'
	include 'BMP.du'

const_8 db 8
const_4 db 4
BoxImg	db	"C:Box.bmp",0
ChipImgAdr  dw	    ?
ChipRight	db "C:Chip1.bmp",0
ChipLeft	db "C:Chip2.bmp",0




buffer	db	320*200 dup(?)

