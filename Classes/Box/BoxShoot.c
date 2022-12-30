
proc	BoxShoot

	cmp	[BoxFly], True
	jne	.end
	cmp	[boxDir], 'W'
	jne	.pressD
	stdcall Box.MoveUp, [uppedBoxAdr]
	cmp	edx,  seil
	jle	.clUpBox
	jmp .end
.pressD:
	cmp	[boxDir], 'D'
	jne	.pressA
	stdcall Box.MoveForward, [uppedBoxAdr]
	mov	eax, [right_wall_dd]
	cmp edx, eax
	jae .clUpBox
	jmp .end
.pressA:
	cmp	[boxDir], 'A'
	jne	.end
	stdcall Box.MoveBack, [uppedBoxAdr]
	cmp	edx, left_wall
	jge	.end
.clUpBox:
	stdcall Box.clUppedBox
.end:
	ret
endp 


proc	Box.MoveForward uses edi,\
	boxAdr
	mov	edi, [boxAdr]
	mov	edx, [edi]
	add	edx, shootSpeed
	mov	[edi], edx
	ret
endp

proc	Box.MoveBack uses edi,\
	boxAdr
	mov	edi, [boxAdr]
	mov	edx, [edi]
	sub	edx, shootSpeed
	mov	[edi], edx
	ret
endp

proc	Box.MoveUp uses edi,\
	boxAdr
	mov	edi, [boxAdr]
	add	edi, 4
	mov	edx, [edi]
	sub	edx, shootSpeed
	mov	[edi], edx
	ret
endp

