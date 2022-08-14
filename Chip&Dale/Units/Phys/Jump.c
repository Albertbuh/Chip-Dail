proc	Phys.Jump uses ax dx
	locals
		jpSpeed = physSpeed
		jpLim	= 60  ; Limit of jump
		jpLimY	dw ?
		wOldSec dw ?
		RepSec	db 5
	endl
	mov	ax, [y_pos]
	sub	ax, jpLim
	mov	[jpLimY], ax   ; jpLimY <- obj pos + Limit of jump

.moveup:
	mov	ah, $2C
	int	21h
	movzx	ax, dl
	div	[RepSec]
	movzx	  dx, al


	cmp	[wOldSec], dx
	je	.dontmove

	mov	[wOldSec], dx
	sub	[y_pos], jpSpeed
	jbe	.end
	stdcall Screen.bkgClear
	stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall Box.Create, Boxes_coordinates

	
	stdcall FlyMove
.dontmove:
	mov	ax, [jpLimY]
	cmp	[y_pos], ax
	jae	.moveup
	jmp	.end



.end:
	ret
endp	  