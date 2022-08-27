proc	Chip.Move.Left uses ax cx bx dx,\
	speed
	
	
	mov	ax, [x_pos]
	xor     cx,cx
        mov     cl, [contactFlag]
	test	cl, availA
	jnz	.end
	mov	dx, ax
	cmp	dx, left_wall
	jbe	.end ; check left_wall
	sub	ax,  [speed]
	mov	[x_pos], ax

.end:
	ret
endp

proc	Chip.Move.Right uses ax cx bx dx,\
	speed
	
	
	mov	ax, [x_pos]

	xor     cx,cx
        mov     cl, [contactFlag]
	test	cl, availD
	jnz	.end
	mov	 dx, ax
	add	 dx, W
	cmp	 dx, right_wall
	jae	 .end  ;check rWall
	add	ax, [speed]
	mov	[x_pos], ax
.end:
	ret
endp

proc	Chip.Move.Down uses ax bx dx,\
	speed
	
	
	xor     cx,cx
        mov     cl, [contactFlag]
	test	cl, availS
	jnz	.dontmove

	mov	dx, [y_pos]
	add	dx, [speed]
	cmp	dx, y_floor
	jae	.dontmove ; check floor

	mov	ax, [speed]
	add	[y_pos], ax
	jmp	.end
.dontmove:
	mov	cx, STOPFALLING
	
.end:
	ret
endp	