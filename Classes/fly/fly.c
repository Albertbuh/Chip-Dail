proc  FlyEnemy.DrawFlyEnemy

	cmp [FlyEnemyAlive], True
	jne @f
	invoke	SelectObject, [hMemDC], [hbmpFlyEnemy]
	invoke	StretchBlt, [hdc], [FlyEnemyXY], [FlyEnemyXY+4], [FlyEnemyW_dd], [FlyEnemyH_dd],[hMemDC],[FlyEnemy_pictX], 0,pictFly_W,pictFly_H, SRCCOPY
	jmp	.drawBall
@@:
	invoke	SelectObject, [hMemDC], [hbmpFlyEnemy]
	xor	eax,eax
	sub	eax, [FlyEnemyH_dd]
	invoke	StretchBlt, [hdc], [FlyEnemyXY], [FlyEnemyXY+4], [FlyEnemyW_dd], eax,[hMemDC],[FlyEnemy_pictX], 0,pictFly_W,pictFly_H, SRCCOPY

.drawBall:
	invoke	SelectObject, [hMemDC], [hbmpFlyBallMask]
	invoke	StretchBlt, [hdc], [FlyBallXY], [FlyBallXY+4], [FlyBallW_dd], [FlyBallH_dd], [hMemDC], 0,0, pictBall_W, pictBall_H,SRCAND
	invoke	SelectObject, [hMemDC], [hbmpFlyBall]
	invoke	StretchBlt, [hdc], [FlyBallXY], [FlyBallXY+4], [FlyBallW_dd], [FlyBallH_dd], [hMemDC], 0,0, pictBall_W, pictBall_H, SRCPAINT
	ret
endp   



proc	FlyEnemy.ChipContact
	stdcall ObjectsContact, [x_pos], [y_pos], [W],[H],\
					 FlyEnemyXY,1, [FlyEnemyW_dd],[FlyEnemyH_dd]
	cmp		 [contactFlag], 0
	jne		Chip.getHit
.finish:
	
	mov	[contactFlag], 0 
	ret
endp

proc	FlyEnemy.FlyBoxContact uses edi eax
	local		      const_10 dd 10
	local		      temp dd 0
	cmp	[BoxFly], True
	jne	.end
	mov	edi, [uppedBoxAdr]
	mov eax, [FlyEnemyXY]
	fild	 [FlyEnemyW_dd]
	fidiv	 [const_10]
	fistp	 [temp]
	add eax, [temp]
	stdcall Phys.Contact, dword[edi],dword[edi+4], [box_x],[box_y],\
					 eax, [FlyEnemyXY+4], [FlyEnemyW_dd], [FlyEnemyH_dd]
	cmp		 [contactFlag], 0
	je		 .end
	mov	[FlyEnemyAlive], False
	stdcall AddPoints, points,points_len-1, 5
	;clear flying box state
	stdcall Box.clUppedBox
.end:
	
	mov	[contactFlag], 0 
	ret
endp	

proc  FlyBall.Contact
	stdcall ObjectsContact, [x_pos], [y_pos], [W],[H],\
					 FlyBallXY,1, [FlyBallW_dd],[FlyBallH_dd]
	cmp		 [contactFlag], 0
	jne		Chip.getHit
.finish:
	
	mov	[contactFlag], 0 
	ret
endp



proc FlyBall.setPos uses eax edi
     local	const_2 dd 2
     local	temp	dd 0
	mov		eax, [FlyEnemyXY]
	mov		[FlyBallXY], eax

	mov		eax, [FlyEnemyXY+4]
	fild		[FlyEnemyH_dd]
	fidiv		[const_2]
	fistp		[temp]
	add		eax,[temp]
	mov		[FlyBallXY+4], eax

	stdcall FlyBall.setSpeed, FlyBallSpeedX, FlyBallSpeedY
	ret
endp

proc  FlyBall.move uses eax, \
x_speed, y_speed

mov		eax, [x_speed]
sub[FlyBallXY], eax
mov		eax, [y_speed]
add[FlyBallXY + 4], eax
ret
endp

proc  FlyEnemy.DieAction

	mov		edi, FlyEnemyXY
	add		dword[edi], 60
	sub		dword[edi+4], 60
	ret
endp

proc FlyEnemy.setKD

		
	stdcall Screen.setObjHeightInBoxes, 0.99
	mov	ecx, [box_y]
	mul	ecx
	add eax, [FlyEnemyH_dd]
	stdcall Enemy.setStartPos, FlyEnemyXY, 0.80 ,eax
	stdcall Enemy.setStartPos, FlyBallXY, 0.80, eax 
	mov		[FlyEnemyMoveUp], False

.end:
		 invoke KillTimer, [global_hwnd], COOLDOWNTIMER2
	ret
endp

proc	FlyEnemyModel  uses eax ebx ecx edx edi
push	word[contactFlag]

cmp  [FlyEnemyAlive], True
je	.model
		stdcall FlyEnemy.DieAction
		jmp .end

.model:
mov [contactFlag], 0
mov		eax, [FlyEnemySpeed]
cmp		[FlyEnemyMoveUp], True
jne		@f
sub		[FlyEnemyXY+4], eax
jmp		.setflag
@@:
add		[FlyEnemyXY+4], eax
.setflag:
cmp		[FlyEnemyXY+4], seil
jg		@f
and		[FlyEnemyMoveUp], False
jmp		.ball_action
@@:

stdcall Screen.subFloor, [y_floor_dd], [FlyEnemyW_dd]
cmp		[FlyEnemyXY+4], eax
jbe		.ball_action
or		[FlyEnemyMoveUp], True

.ball_action:
	stdcall 	Screen.addFloor, [y_floor_dd], 100
	cmp		[FlyBallXY+4], eax
	jbe		@f
	stdcall 	FlyBall.setPos
	jmp		.contact
@@:
	cmp	    [FlyBallXY], left_wall
	jge		.contact
	stdcall 	FlyBall.setPos
	jmp		.contact
	
.contact :
stdcall FlyBall.Contact
stdcall FlyEnemy.FlyBoxContact
stdcall FlyEnemy.ChipContact

.end :
pop	word[contactFlag]
ret
endp



proc  FlyBallTimer
	cmp [transition_flag], False
	jne .end
	stdcall 	FlyBall.move,[FlyBallSpeedX], [FlyBallSpeedY]
.end:
	ret
endp

proc	FlyBall.setSpeed uses eax edx edi,\
		     xspeed_adr, yspeed_adr
	local	const_inch	  dd 100
	local	const_mul	  dd 2
	local	const_dop	  dd 3
	local	min = 5
	local	temp  dd 0

	mov	eax,	[FlyBallXY]
	sub	eax,	 [x_pos]
	fild	[W]
	fidiv	[const_mul]
	fistp	[temp]
	sub	eax,	 [temp]
	div	[const_inch]
	mul	[const_mul]
	mov	edi, [xspeed_adr]
	cmp	eax, 0
	jg	@f
       ; xor	 eax,eax
	stdcall  Random.Get, 4, 8
@@:
	mov	[edi], eax

.y_count:
	mov	edi, [yspeed_adr]
	mov	eax, [y_pos]
	fild	[H]
	fidiv	[const_mul]
	fistp	[temp]
	add	eax, [temp]
	sub	eax, [FlyBallXY+4]
	cmp	eax, 0
	jg	@f
       ; xor	 eax,eax
	stdcall  Random.Get, -1, 2
	mov	[edi], eax
	stdcall  Random.Get, 15, 20
	mov	[edi-4], eax
	jmp	.end
@@:
	div	[const_inch]
	mul	[const_mul]
	mov	[edi], eax

.end:
	ret
endp 
