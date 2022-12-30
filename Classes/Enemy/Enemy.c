proc  Enemy.DrawEnemy
	cmp		[EnemyAlive], True
	jne @f
	invoke	SelectObject, [hMemDC], [hbmpEnemyMask]
	invoke	BitBlt, [hdc], [EnemyXY], [EnemyXY+4], [EnemyW_dd], [EnemyH_dd],[hMemDC],[Enemy_pictX],0,SRCAND
	invoke	SelectObject, [hMemDC], [hbmpEnemy]
	invoke	StretchBlt, [hdc], [EnemyXY], [EnemyXY+4], [EnemyW_dd], [EnemyH_dd],[hMemDC],[Enemy_pictX],0,pictEnemy_W, pictEnemy_H,SRCPAINT
	jmp  .end
@@:
	invoke	SelectObject, [hMemDC], [hbmpEnemyMask]
	
	xor		eax,eax
	sub		eax, [EnemyH_dd]
	invoke	BitBlt, [hdc], [EnemyXY], [EnemyXY+4], [EnemyW_dd], eax,[hMemDC],[Enemy_pictX],0,SRCAND
	invoke	SelectObject, [hMemDC], [hbmpEnemy]
	xor		eax,eax
	sub		eax, [EnemyH_dd]
	invoke	StretchBlt, [hdc], [EnemyXY], [EnemyXY+4], [EnemyW_dd], eax,[hMemDC],[Enemy_pictX],0,pictEnemy_W, pictEnemy_H,SRCPAINT
.end:
	ret
endp	




proc	Enemy.setStartPos uses eax edi,\
	enemyXY, x_prsnt, y_ref
	 
	 mov	edi, [enemyXY]
	 fild	[right_wall_dd]
	 fmul	[x_prsnt]
	 fistp	dword[edi]
	 ;sub	 dword[edi], EnemyW


	 mov	eax, [y_floor_dd]
	 sub	eax, [y_ref]
	 mov	[edi+4], eax



	ret
endp  

proc Enemy.setKD

		
		cmp [second_scene], True
		je	.next
		stdcall Screen.setObjHeightInBoxes, 0.45
		mov	edx, [box_y]
		mul	edx
		add	eax, [EnemyH_dd]
		stdcall Enemy.setStartPos, EnemyXY, 1.05, eax
		jmp		.end
.next:
	
		mov	[EnemyAlive], True
	mov	eax, [right_wall_dd]
	mov	[EnemyXY], eax

	mov	eax, [y_floor_dd]
	sub	eax, [EnemyH_dd]
	mov	[EnemyXY+4], eax

	mov	[EnemySpeed], 22

.end:
		 invoke KillTimer, [global_hwnd], COOLDOWNTIMER
	ret
endp


proc	Enemy.DieAction uses esi edi eax

	mov		edi, EnemyXY
	mov		eax, [x_pos]
	cmp		[edi], eax
	jb		@f
	add		dword[edi], 80
	jmp		.end
@@:	
	sub		dword[edi], 80
.end:
	sub		dword[edi+4], 60

	
	ret
endp



proc	Enemy.ChipContact
	stdcall ObjectsContact, [x_pos], [y_pos], [W],[H],\
					 EnemyXY,1, [EnemyW_dd], [EnemyH_dd]
	cmp		 [contactFlag], 0
	jne		Chip.getHit
.finish:
	
	mov	[contactFlag], 0 
	ret
endp

proc	Enemy.FlyBoxContact uses edi eax
	locals
	    const_10 dd 10
	    temp dd 0
	endl
	cmp	[BoxFly], True
	jne	.end
	mov	edi, [uppedBoxAdr]
	mov eax, [EnemyXY]
	fild	 [EnemyW_dd]
	fidiv	 [const_10]
	fistp	 [temp]
	add eax, [temp]
	stdcall Phys.Contact, dword[edi],dword[edi+4], [box_x], [box_y],\
					 eax, [EnemyXY+4],[EnemyW_dd], [EnemyH_dd]
	cmp		 [contactFlag], 0
	je		 .end
	mov	[EnemyAlive], False
	
	;add [points+2], 1
	 stdcall AddPoints, points,points_len-1, 3
	;clear flying box state
	stdcall Box.clUppedBox
.end:
     
	mov	[contactFlag], 0 
	ret
endp	

proc	Enemy.BoxContact  uses edx ecx
	locals
		highspeed=physSpeed*3
		lowspeed=physSpeed
	endl

	;use chip function but for Enemy Object
	mov  edx, [EnemyXY+4]
	stdcall ObjectsContact, [EnemyXY], edx, [EnemyW_dd], [EnemyH_dd],\
					 Boxes_coordinates, box_col, [box_x],[box_y]
	mov				 cl, [contactFlag]
	test				 cl, availS
	jnz				 .skip
	test				cl, availA
	jnz				.skip
	mov				eax, [y_floor_dd]
	sub				eax, [EnemyH_dd]
	cmp				 [EnemyXY+4], eax
	ja				 .skip
	add	edx, highspeed
	jmp	.end
.skip:
	test	cl, availA
	jz	.end
	sub	edx, lowspeed

.end:
	mov	eax, [y_floor_dd]
	sub	eax, [EnemyH_dd]
	cmp	edx, eax
	jbe	 @f
	mov	 eax, [y_floor_dd]
	sub	 eax, [EnemyH_dd]
	mov	edx, eax
@@:
	mov	[EnemyXY+4], edx
	mov	[contactFlag], 0
	ret
endp  




proc	EnemyModel  uses eax ebx ecx edx edi
push	word[contactFlag]

cmp  [EnemyAlive], True
je	.model
		stdcall Enemy.DieAction
		jmp .end
.model:
mov [contactFlag], 0
cmp	[text_flag], True
jne @f
mov eax, [EnemySpeed]
sub [EnemyXY], eax
@@:
;falling check
stdcall Enemy.BoxContact
@@:
mov	eax, 10
sub	eax, [EnemyW_dd]
cmp [EnemyXY], eax
jg	@f
mov	eax, [right_wall_dd]
mov	[EnemyXY], eax
@@:

.contact :

stdcall Enemy.FlyBoxContact
stdcall Enemy.ChipContact

.end :
pop	word[contactFlag]
ret
endp

