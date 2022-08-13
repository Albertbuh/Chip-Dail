proc	Phys.Falling
	locals
		fallSpeed =  5
		wOldSec dw	?
		RepSec	db	5
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

	stdcall Chip.ContactWithObjects, [x_pos],[y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a


	stdcall Chip.Move.Down, fallSpeed ; return cx with STOPFALLING value if obj on ground
	cmp	cx, STOPFALLING
	je	.slowdown

	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Box.Create, Boxes_coordinates
	stdcall FlyMove
.dontmove:	
	jmp .down
	ret
;small distance between ground and legs

.slowdown:
     ;	 mov	 cl, [contactFlag]
     ;	 test	 cl, availS
     ;	 jnz	 .staybox
	;
         ;mov	 ax, [y_pos]
      	 ;sub	 ax, y_floor
      	; cmp	 ax, 0
     	; jbe	 .end
     	; sub	 [y_pos], ax
	

.staybox:
     ;	 mov	 ax, [box_y]
     ;	 sub	ax, box_a-1
     ;	 sub	 ax, [y_pos]
     ;	 cmp	 ax, fallSpeed
     ;	 ja	 .end
     ;	 add	 [y_pos], ax

.end:
	ret
endp


