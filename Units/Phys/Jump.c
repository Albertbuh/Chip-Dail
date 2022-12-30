
proc	Jump
	locals
		jpSpeed = physSpeed/5*30
		constant	dd 1.75
		jpLim	dd 500; Limit of jump
	endl

	cmp	[JumpFlag], True
	jne	.end
	cmp	[y_pos], seil
	jle	.clear
	cmp	[jpLimY], 0
	jne	.skipLimCount
	;JUMP LIM COUNT
	fild	[y_floor_dd]
	fdiv	[constant]
	fistp	[jpLim]
	mov	eax, [y_pos]
	sub	eax, [jpLim]
	mov	[jpLimY], eax  ; jpLimY <- obj pos + Limit of jump

.skipLimCount:
	sub	[y_pos], jpSpeed
.skip:
	mov	eax, [jpLimY]
	cmp	[y_pos], eax
	jg	.end
.clear:
	mov	[JumpFlag], False
	and	[jpLimY], 0
	mov	[FallingFlag], True

.end:
	ret
endp	