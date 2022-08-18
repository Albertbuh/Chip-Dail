proc	Phys.Falling
	locals
		fallSpeed =  physSpeed
		wOldSec dw	?
		RepSec	db	2
	endl
.down:	
	mov	ah, $2C
	int	21h
	movzx	ax, dl
	div	[RepSec]
	movzx	dx, al
	cmp	[wOldSec], dx
	je	.dontmove
	mov	[wOldSec], dx

	mov	dx, [y_pos]
	stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
				Boxes_coordinates, box_col, box_a+fallSpeed,box_a
	mov	cl, [contactFlag]
	test	cl, availS
	jnz	.slowdown

	add	dx, fallSpeed
	stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
				Boxes_coordinates, box_col, box_a, box_a


	stdcall Chip.Move.Down, fallSpeed ; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
	je	.slowdown


.draw:
	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Box.Create, Boxes_coordinates
	
	
	stdcall	KeyModel
	cmp	ax, QUIT
	je	.end
	
.dontmove:
	jmp .down
;small distance between ground and legs

.slowdown:
	mov	cl, [contactFlag]
	cmp	cl, 0
	jne	.slowObj
	mov	ax, y_floor-1
	sub	ax, [y_pos]
	add	[y_pos], ax

.slowObj:

	stdcall Chip.ContactWithObjects, [x_pos], [y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a

	stdcall Chip.Move.Down, 5; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
	jne	.slowObj
.end:
	ret
endp












