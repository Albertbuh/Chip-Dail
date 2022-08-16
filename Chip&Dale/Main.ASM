	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'
	include 'units\Phys\Phys.h'
	include 'classes\Box\Box.h'
True = 1
False = 0
ZERO = 0

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
	stdcall Phys.Jump

.upbox: ; press Q
       stdcall PressQ

.moveKeys:
	stdcall Chip.KeyMove
	stdcall Phys.Falling

	jmp	.Start


.end:
	ret
;return position of first object to pick
proc	Chip.FirstPickContact uses si di,\
	x,y, h,w,\
	objects, num_of_obj, obj_h, obj_w

	mov	di, [objects]
	mov	cx, [num_of_obj]
	mov	[contactFlag], 0
.checkObj:
	cmp	[uppedBoxAdr], di
	je	.skip
	stdcall Phys.Contact, [x],[y], [h],[w],\
			      [di], [di+2],[obj_h], [obj_w]

	mov	dl, [contactFlag]
	cmp	dl, 0
	jne	.end
.skip:
	add	di, 4

	loop	.checkObj

.end:

	ret
endp


proc	FlyMove  uses	 ax

	mov	ah, $01
	int	16h
	jz	.prev
	mov	ah, $00
	int	16h
	movzx	ax, ah
	mov	dx, [prev_key]	; save previous dir to kick box right
	mov	[prev_key], ax
.prev:
	cmp	[prev_key], KEY_Q
	jne	@F
;============ Check the direction of fly ==================
       ; stdcall     PushingBox
       and     [boxUpped], False
	mov	di, [uppedBoxAdr]
	mov	dx, [di]
	add	dx, 15
	mov	[di], dx
;=========================================================
@@:
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

; in DX we need to put our key code before calling procedure
proc	PushingBox
	cmp	dx, KEY_D
	jne	@F
	stdcall Chip.PushBox, boxForward

@@:
	cmp	dx, KEY_A
	jne	.end
	stdcall Chip.PushBox, boxBack
.end:
	ret
endp

proc	PressQ uses di
	cmp	ax, KEY_Q
	jne	.end

	cmp    [boxUpped], True
	jne	.checkLift
;case when we have box on head
	mov   dx, [prev_key]
	stdcall PushingBox
	jmp .end
;case when we don't
.checkLift:
	 stdcall CheckLift
.end:

	 ret
endp

proc	CheckLift
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

;with a single obj
proc	Phys.Contact uses ax cx dx,\
	x,y,height, weight,\
	obj_x, obj_y, obj_h, obj_w

	local sSpeed = physSpeed

	xor   cx, cx
	mov   cl, [contactFlag]
.D:
	mov	ax, [x]
	add	ax, [weight]
	mov	dx, [obj_x]
	cmp	ax, dx
	jb     .S
	add	dx, [obj_w]
	cmp	ax, dx
	ja	.S

	mov	ax, [y]
	sub	ax, [height] ;upper bound of hero
	mov	dx, [obj_y]
	cmp	ax, dx
	jae	.S
	add	ax, H ; lower bound of hero
	sub	dx, [obj_h]; upper bound of box
	cmp	ax, dx
	jbe	.S

	test	cl, availD
	jnz	.S
	add	cl, availD
.S:
	mov	ax, [y]
	mov	dx, [obj_y]
	cmp	ax, dx
	ja	.A
	sub	dx, [obj_h]
	cmp	ax, dx
	jb	.A
	;sub	dx, [obj_h]  ; ax <- obj down; dx <- box front
	;inc	dx
	;sub	dx, sSpeed
	;cmp	ax, dx
	;jb	.A


	mov	ax, [x]
	mov	dx, [obj_x]
	cmp	ax,dx
	jae	@F
	sub	dx, ax ;x < box_x
	cmp	dx,  [weight]
	jae	.A
	jmp	.notavail
@@:
	sub	ax, dx
	cmp	ax, [obj_w]
	jae	 .A

.notavail:
	test	cl, availS
	jnz	.A
	add	cl, availS
.A:
	mov	ax, [x]
	mov	dx, [obj_x]
	cmp	ax, dx
	jb	.W
	add	dx, [obj_w]
	cmp	ax, dx
	ja     .W



	mov	ax, [y]
	sub	ax, [height] ;upper bound of hero
	mov	dx, [obj_y]
	cmp	ax, dx
	jae	.W
	add	ax, [height] ; lower bound of hero
	sub	dx, [obj_h]; upper bound of box
	cmp	ax, dx
	jbe    .W

	test	cl, availA
	jnz	.W
	add	cl, availA
.W:
     ;	 mov	 ax, [y]
     ;	 mov	 dx, [obj_y]
     ;	 cmp	 ax, dx
     ;	 jne	 .end

     ;	 mov	 ax, [x]
     ;	 mov	 dx, [obj_x]
     ;;   cmp	  ax,dx
     ;	 jae	 @F
     ;	 sub	 dx, ax
     ;;   cmp	  dx,  [weight]
     ;	 jb	 .end
@@:
      ;  sub	 ax, dx
       ; cmp	 ax, [obj_w]
       ; jb	 .end

       ; test	 cl, availW
       ; jnz	 .end
       ; add	 cl, availW
.end:

	mov	[contactFlag], cl
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
      ;  include 'units\Phys\Contact.c'
x_pos	dw 160
y_pos	dw 139

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

boxUpped	db	False
uppedBoxID	db	0
uppedBoxAdr dw		0
contactFlag db		0000_0000b
prev_key    dw		?
shiftState  db		False
     ;	 include 'classes\Box\Box.du'

