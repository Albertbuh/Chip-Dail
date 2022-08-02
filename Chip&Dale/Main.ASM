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
	stdcall Chip.Contact, [x_pos], [y_pos], [box_x], [box_y]
	stdcall Chip.Draw, [x_pos], [y_pos]
	cmp	[boxUpped], True
	je	.keypress
	stdcall Box.Draw, [box_x], [box_y]

.keypress:
	stdcall Keyboard.KeyPressInt16
.Esc:
	cmp	ax, KEY_ESCAPE
	je	.end

.Space:
	cmp	ax, KEY_SPACE
	jne	.upbox
	stdcall Phys.Jump
	stdcall Phys.Falling

.upbox: ; press Q
       stdcall PressQ

.moveKeys:
	stdcall Chip.Move

	jmp	.Start


.end:
	ret









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

proc PressQ
	cmp	ax, KEY_Q
	jne	.end

	cmp	[prev_key], KEY_D
	jne	@F
	stdcall Chip.PushBox, boxForward
@@:
	cmp	[prev_key], KEY_A
	jne	.checkLift
	stdcall Chip.PushBox, boxBack
.checkLift:
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, 0000_1111b
	jz     .end
	stdcall Chip.GetBox
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
x_pos	dw 150
y_pos	dw 150-H
box_x	dw 200
box_y	dw 149
boxUpped  db   False
contactFlag db	 0000_0000b
prev_key    dw	 ?
     ;	 include 'classes\Box\Box.du'

