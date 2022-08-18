proc	KeyModel	;uses dx

	;  mov     ax, $0305
      ;  mov     bx, $1f03
      ;  int     16h
        mov     ah, $01
        int     16h
        jz      .end
        xor     ax,ax
        int     16h
        movzx   ax, ah 

	mov	dx, [prev_key]
	cmp	ax, KEY_SPACE
	je	.pressSpace
	mov	[prev_key], ax
.KeyCheck:
.pressESC:
	cmp	[prev_key], KEY_ESCAPE
	jne	.pressQ
	mov	ax, QUIT
.pressQ:
	cmp	[prev_key], KEY_Q
	jne	.pressA
	mov	[prev_key], dx
	cmp	[boxUpped], False
	jne	.shot
	cmp	[BoxFly], True
	je	.pressA
	stdcall	CheckLift	
	jmp	.end
.shot:
	mov	[BoxFly], True
	and	[boxUpped], False
	mov	[boxDir], dx

.pressA:
	cmp	[prev_key], KEY_A
	jne	.pressD
	stdcall	Chip.KeyMove
	cmp	[JumpFlag], True
	je	.pressD
	mov	[FallingFlag], True
.pressD:
	cmp	[prev_key], KEY_D
	jne	.pressS
	stdcall	Chip.KeyMove
	cmp	[JumpFlag], True
	je	.pressS
	mov	[FallingFlag], True	
.pressS:
	cmp	[prev_key], KEY_S
	jne	.pressSpace
	stdcall	Chip.KeyMove
.pressSpace:
	
	cmp     ax, KEY_SPACE
        jne     .pressW
        mov     [JumpFlag], True
        mov     [FallingFlag], False 
.pressW:
	cmp	ax, KEY_W
	jne	.end
	mov	[BoxFly], True
	and	[boxUpped], False
	mov	[boxDir], ax
.end:
	ret
endp