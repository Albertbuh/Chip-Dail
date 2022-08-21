
proc    BoxShoot
        cmp     [BoxFly], True
        jne     .end
        cmp     [boxDir], KEY_W
        jne     .pressD
        stdcall Box.MoveUp, [uppedBoxAdr]
        cmp     dx,  y_floor
        jbe     .end
.pressD:
        cmp     [boxDir], KEY_D
        jne     .pressA
        stdcall Box.MoveForward, [uppedBoxAdr]
.pressA:
        cmp     [boxDir], KEY_A
        jne     @F
        stdcall Box.MoveBack, [uppedBoxAdr]
@@:
        cmp     dx, right_wall
        jb      .end
        mov     di, [uppedBoxAdr]
        mov     word[di], ZERO
        mov     word[di+2], ZERO
        mov     [uppedBoxID], 0
        mov     [BoxFly], False
.end:
        ret
endp 

proc	Box.MoveForward uses di,\
	boxAdr
	mov	di, [boxAdr]
	mov	dx, [di]
	add	dx, shootSpeed
	mov	[di], dx
	ret
endp

proc	Box.MoveBack uses di,\
	boxAdr
	mov	di, [boxAdr]
	mov	dx, [di]
	sub	dx, shootSpeed
	mov	[di], dx
	ret
endp

proc	Box.MoveUp uses di,\
	boxAdr
	mov	di, [boxAdr]
	add	di, 2
	mov	dx, [di]
	sub	dx, shootSpeed
	mov	[di], dx
	ret
endp

