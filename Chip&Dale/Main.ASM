	include 'macro\proc16.inc'

	include 'classes\MainHeroes\Chip.h'
	include 'units\Keyboard\Keyboard.h'
	include 'units\Screen\Screen.h'
	include 'units\Phys\Phys.h'
	include 'classes\Box\Box.h'

	org 100h

EntrypPoint:
	stdcall Screen.SetMode,  $0013

.Start:
	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Chip.CanLiftBox
	stdcall Box.Draw, [box_x], [box_y]
	stdcall Keyboard.KeyPress ; ax <- key


	cmp	ax, KEY_SPACE
	jne	@F
	stdcall Phys.Jump
	stdcall Phys.Falling
@@:
	cmp	ax, KEY_Q
	jne	@F
	cmp	[boxUpped], 1
	jne	@F
	stdcall Chip.GetBox

@@:
	cmp	ax, KEY_ESCAPE
	je	.end
	stdcall Chip.Move


	loop	.Start

.end:
	ret


proc	Chip.GetBox  uses ax dx



	xor	     ax,ax
	xor	     dx,dx
	mov	ax, [x_pos]
	add	ax, W/4
	mov	dx, [y_pos]
	sub	dx, 1

	mov	[box_x], ax
	mov	[box_y], dx
.end:
	ret
endp

proc	Chip.CanLiftBox uses ax dx

	mov	[boxUpped], 0
.checkX:
	xor	ax,ax
	xor	dx,dx
	mov	ax,[x_pos]
	add	ax, W
	mov	dx, [box_x]
	sub    ax,dx
	cmp    ax, 0
	jne    .end
.checkY:
	xor	ax,ax
	xor	dx,dx
	mov	ax, [y_pos]
	add	ax, H-1
	mov	dx, [box_y]
	sub	ax,dx
	cmp	ax,0
	jne	.end
	inc    [boxUpped]
.end:
	ret
endp

	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\Box\Box.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
x_pos	dw 160
y_pos	dw 150-H
boxUpped   db	0
	include 'classes\Box\Box.du'

