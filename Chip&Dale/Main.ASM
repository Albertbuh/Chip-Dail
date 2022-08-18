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
	stdcall KeyModel
	cmp	ax, QUIT
	jne	.cycle
	ret
endp

ABOBA:
	ret

proc	BoxShoot
	cmp	[BoxFly], True
	jne	.end
	cmp	[boxDir], KEY_W
	jne	.pressD
	stdcall Box.MoveUp, [uppedBoxAdr]
	cmp	dx,  y_floor
	jbe	.end
.pressD:
	cmp	[boxDir], KEY_D
	jne	.pressA
	stdcall Box.MoveForward, [uppedBoxAdr]
.pressA:
	cmp	[boxDir], KEY_A
	jne	@F
	stdcall Box.MoveBack, [uppedBoxAdr]
@@:
	cmp	dx, right_wall
	jb	.end
	mov	di, [uppedBoxAdr]
	mov	word[di], ZERO
	mov	word[di+2], ZERO
	mov	[uppedBoxID], 0
	mov	[BoxFly], False
.end:
	ret
endp
proc	Jump
	locals
		jpSpeed = physSpeed
		jpLim	= 60  ; Limit of jump

	endl
	cmp	[JumpFlag], True
	jne	.end
	cmp	[jpLimY], 0
	jne	.skipLimCount
	mov	ax, [y_pos]
	sub	ax, jpLim
	mov	[jpLimY], ax  ; jpLimY <- obj pos + Limit of jump

.skipLimCount:
	sub	[y_pos], jpSpeed
	jbe	.end
	mov	ax, [prev_key]
	stdcall Chip.KeyMove
.skip:
	mov	ax, [jpLimY]
	cmp	[y_pos], ax
	jae	.end
	mov	[JumpFlag], False
	and	[jpLimY], 0
	mov	[FallingFlag], True

.end:
	ret
endp

proc	Falling
	local	fallSpeed = physSpeed


	mov	dx, [y_pos]
	stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
				Boxes_coordinates, box_col, box_a+fallSpeed,box_a
	mov	cl, [contactFlag]
	test	cl, availS
	jnz	NotFall

	add	dx, fallSpeed
	stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
				Boxes_coordinates, box_col, box_a, box_a


	stdcall Chip.Move.Down, fallSpeed ; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
       ; jne	 .skip
	je	 NotFall

	mov	ax, [prev_key]
	stdcall Chip.KeyMove
.skip:

	ret
endp
NotFall:
	 mov	  [FallingFlag], False
	ret

proc SlowDown
.slowdown:
	mov	cl, [contactFlag]
	cmp	cl, 0
	jne	.slowObj
	mov	ax, y_floor-1
	sub	ax, [y_pos]
	add	[y_pos], ax
	jmp	.end
.slowObj:

	stdcall Chip.ContactWithObjects, [x_pos], [y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a

	stdcall Chip.Move.Down, 5; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
	jne	.slowObj
.end:
	ret
endp



proc	CheckLift  uses di cx
     stdcall Chip.FirstPickContact, [x_pos],[y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a
	mov	[uppedBoxID], box_col
	sub	[uppedBoxID], cl
	; check if we can up box
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availA+availD
	jz     .end

	;find the box we can up
	mov	di, Boxes_coordinates
	mov	cl, [uppedBoxID]
@@:
	add	di, 4
	loop	@B
	stdcall Chip.GetBox, di
	mov	[boxUpped], True
.end:
	ret
endp






	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\MainHeroes\Chip.Move.c'
	include 'classes\Box\Box.c'
	include 'classes\Box\BoxShoot.c'
      ;  include 'units\Phys\Falling.c'
      ;  include 'units\Phys\Jump.c'
	include 'units\Phys\Contact.c'
	include    'classes\KeyPress.c'
x_pos	dw 10
y_pos	dw 149

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

JumpFlag	db	False
jpLimY		dw 0
FallingFlag	db	False
boxUpped	db	False
uppedBoxID	db	0
uppedBoxAdr dw		0
contactFlag db		0000_0000b
prev_key    dw		?
shiftState  db		False

BoxFly	db	False
boxDir	dw	?
     ;	 include 'classes\Box\Box.du'

