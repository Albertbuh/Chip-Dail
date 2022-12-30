proc	Chip.Move.Left uses eax ecx ebx edx,\
	speed

	mov	eax, [x_pos]
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availA
	jnz	.end

	mov	edx, eax
	cmp	edx, left_wall
	jbe	.end ; check left_wall
	sub	eax,  [speed]
	mov	[x_pos], eax

.end:
	ret
endp

proc	Chip.Move.Right uses eax ecx ebx edx,\
	speed
	
	
	mov	eax, [x_pos]

	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availD
	jnz	.end

	
	add	eax, [speed]
	mov	[x_pos], eax
.end:
	ret
endp

proc	Chip.Move.DownCarry uses ecx ebx edx,\
	speed
	
	
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availS
	jnz	.dontmove

	mov	edx, [y_pos]
	add	edx, [H]
	add	edx, [speed]
	mov	eax, [y_floor_dd]
	dec	eax
	cmp	edx, eax
	jge	.dontmove ; check floor

	;ADD SPEED TO Y COORDINATES
	stdcall Chip.Move.Down, [speed]
	jmp	.end
.dontmove:
	 mov	eax, 0
	
.end:
	ret
endp	

proc Chip.Move.Down uses edx,\
			speed
		mov	edx, [speed]
		add [y_pos], edx
	ret
endp