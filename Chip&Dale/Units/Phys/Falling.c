proc    Phys.Falling
        locals
                fallSpeed =  15
                wOldSec dw      ?
                RepSec  db      5
        endl
.down:	
        mov     ah, $2C
        int     21h
        movzx   ax, dl
        div     [RepSec]
        movzx   dx, al
        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        stdcall Chip.Contact, [x_pos], [y_pos],[box_x], [box_y]
        stdcall	Chip.Move.Down, fallSpeed ; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
	je	.slowdown

        stdcall Screen.bkgClear
        stdcall Chip.Draw, [x_pos], [y_pos]
        stdcall Box.Draw, [box_x], [box_y]
        stdcall FlyMove
.dontmove:	
        jmp .down

;small distance between ground and legs

.slowdown:
	mov	cl, [contactFlag]
	test	cl, availS
	jnz	.staybox
	
	mov	ax, [y_pos]
	add	ax, H
	sub	ax, y_floor
	cmp	ax, 0
	jbe	.end
	sub	[y_pos], ax
        

.staybox:
	mov	ax, [box_y]
	sub	ax, box_a-1 
	sub	ax, [y_pos]
	sub	ax, H
	cmp	ax, fallSpeed
	ja	.end
	add	[y_pos], ax

.end:
        ret
endp


