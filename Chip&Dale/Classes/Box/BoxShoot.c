proc	Box.Shoot uses si di,\
	dir, boxAdr ; 0-up, 1-forward, 2-back; boxAdr - adr of upped box X coordinate

	
	cmp [dir], boxUp
	jne @F
	stdcall Box.ShootUp, [boxAdr]
@@:
	cmp [dir], boxForward
	jne @F
	stdcall Box.ShootForward, [boxAdr]
@@:
	cmp [dir], boxBack
	jne .end
	stdcall Box.ShootBack, [boxAdr]
.end:
	ret
endp

proc	Box.ShootForward uses si di,\
	boxAdr
	locals
		shootSpeed = 15
		wOldSec    dw ?
	endl
.shoot:
	mov	ah, $2c
	int	21h
	movzx	  dx, dl
	cmp	[wOldSec], dx
	je	.dontmove
	mov	[wOldSec], dx

	stdcall	Box.MoveForward, [boxAdr]

	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos] 
	stdcall  Box.Create, Boxes_coordinates
       
	cmp	dx, right_wall
	jae	.end
	and	[prev_key], 0
	stdcall	KeyModel
	cmp	ax, QUIT
	je	.end
.dontmove:

	cmp	dx, right_wall
	jb	.shoot

.end:
	ret
endp
	


proc	Box.ShootUp uses si di,\
	boxAdr
	locals
		shootSpeed = 15
		wOldSec    dw ?
	endl
.shoot:
	mov	ah, $2c
	int	21h
	movzx	  dx, dl
	cmp	[wOldSec], dx
	je	.dontmove
	mov	[wOldSec], dx

	mov	di, [boxAdr]
	add	di, 2
	mov	dx, [di]
	sub	dx, shootSpeed
	mov	[di], dx

	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos] 
	stdcall  Box.Create, Boxes_coordinates
       
	cmp	dx, seil
	jb	.end
	and	[prev_key], 0
	stdcall	KeyModel
	cmp	ax, QUIT
	je	.end
.dontmove:

	cmp	dx, seil
	jae	.shoot

.end:
	ret
endp

proc	Box.ShootBack ,\
	boxAdr
	locals
		shootSpeed = 15
		wOldSec    dw ?
	endl
.shoot:
	mov	ah, $2c
	int	21h
	movzx	  dx, dl
	cmp	[wOldSec], dx
	je	.dontmove

	mov	[wOldSec], dx

	stdcall	Box.MoveBack, [boxAdr]


.draw:
	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos]
	stdcall  Box.Create, Boxes_coordinates

	cmp	dx, right_wall
	jae	.end
	and	[prev_key], 0
	stdcall	KeyModel
	cmp	ax, QUIT
	je	.end
.dontmove:
	cmp	dx, right_wall
	jb	 .shoot
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

