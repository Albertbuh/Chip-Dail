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
	stdcall Chip.Draw, [x_pos], [y_pos]

	stdcall Box.Create, Boxes_coordinates


	stdcall Keyboard.KeyPressInt16
.Esc:
	cmp	ax, KEY_ESCAPE
	je	.end

.Space:
	cmp	ax, KEY_SPACE
	jne	.upbox
      ;  stdcall Phys.Jump

.upbox: ; press Q
       stdcall PressQ

.moveKeys:
	stdcall Chip.KeyMove

       ; stdcall Phys.Falling
	jmp	.Start


.end:
	ret

; get position of main object, object's pointer
;and the number of objects
proc	Chip.ContactWithObjects uses si di,\
	x,y, h,w,\
	objects, num_of_obj, obj_h, obj_w

	mov	di, [objects]
	mov	cx, [num_of_obj]
.checkObj:
	stdcall Phys.Contact, [x],[y], [h],[w],\
			      [di], [di+2],[obj_h], [obj_w]
	add	di, 4

	mov	dl, [contactFlag]
	cmp	dl, 0
	jne	.end
	loop	.checkObj

.end:

	ret
endp



























proc	FlyMove  uses	 ax

	mov	ah, $01
	int	16h
	jz	.prev
	cmp	ah, KEY_A
	jne	@F
	stdcall Chip.Move.Left, speed
@@:
	cmp	ah, KEY_D
	jne	.end
	stdcall Chip.Move.Right, speed
.prev:
	cmp	[prev_key], KEY_A
	jne	@F
	stdcall Chip.Move.Left, speed
@@:
	cmp	[prev_key], KEY_D
	jne	.end
	stdcall Chip.Move.Right, speed

.end:

	ret
endp

proc PressQ uses di
	cmp	ax, KEY_Q
	jne	.end

      ;  cmp	 [prev_key], KEY_D
      ;  jne	 @F
      ;  stdcall Chip.PushBox, boxForward
@@:
       ; cmp	 [prev_key], KEY_A
       ; jne	 .checkLift
      ;  stdcall Chip.PushBox, boxBack
.checkLift:
	stdcall Chip.ContactWithObjects, [x_pos],[y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a
	mov	[uppedBoxID], box_col
	sub	[uppedBoxID], cl
	; check if we can up box
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availA+availD
	jz     .end
	cmp    [uppedBox], True
	je     .end
	;find the box we can up
	mov	di, Boxes_coordinates
	mov	cl, [uppedBoxID]
@@:
	add	di, 4
	loop	@B
	stdcall Chip.GetBox, di
	mov	[uppedBox], True
.end:
	ret
endp



	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\MainHeroes\Chip.Move.c'
	include 'classes\Box\Box.c'
	include 'classes\Box\BoxShoot.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
x_pos	dw 100
y_pos	dw 149


Boxes_coordinates   dw	150,149,\
			50 ,149,\
			125,130,\
			75, 130
box_col=($-Boxes_coordinates)/4

uppedBox	db	False
uppedBoxID	db	0
uppedBoxPos dw		0
contactFlag db		0000_0000b
prev_key    dw		?
shiftState  db		False
     ;	 include 'classes\Box\Box.du'

