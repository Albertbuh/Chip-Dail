proc	Box.Shoot uses si di,\
	dir, x,y ; 0-up, 1-forward, 2-back

	
	cmp [dir], boxUp
	jne @F
	stdcall Box.ShootUp, [x],[y]
@@:
	cmp [dir], boxForward
	jne @F
        stdcall Box.ShootForward, [x],[y]
@@:
	cmp [dir], boxBack
	jne .end
	stdcall Box.ShootBack, [x],[y]
.end:
	ret
endp

proc	Box.ShootForward uses si di,\
	box_x, box_y
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

	mov	di, [box_x]
	mov	dx, [di]
	add	dx, shootSpeed
	mov	[di], dx

	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos] 
	stdcall  Box.Create, Boxes_coordinates
       
.dontmove:

	cmp	dx, right_wall
	jb	.shoot

.end:
	ret
endp

proc	Box.ShootUp ,\
	box_x, box_y
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

	mov	di, [box_y]
	mov	dx, [di]
	sub	dx, shootSpeed
	mov	[di], dx

	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos]
	stdcall  Box.Create, Boxes_coordinates
.dontmove:
	cmp	dx, seil
	ja	.shoot
.end:
	ret
endp

proc	Box.ShootBack ,\
	box_x,box_y
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

	mov	di, [box_x]
	mov	dx, [di]
	sub	dx, shootSpeed
	mov	[di],dx


.draw:
	stdcall  Screen.bkgClear
	stdcall  Chip.Draw, [x_pos], [y_pos]
	stdcall  Box.Create, Boxes_coordinates
.dontmove:
	cmp	dx, right_wall
	jb	 .shoot
.end:
	ret
endp 