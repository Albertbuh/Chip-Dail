proc	Screen.DrawBKG 
	invoke	SelectObject, [hBkgDC], [hbmpBKG]
	mov	eax, 0
	mov	edx, [y_floor_dd]
.drawbkg:
	push	eax edx
	invoke BitBlt, [hdc],eax,edx,50,50,[hBkgDC],0,0,SRCCOPY
	pop    edx eax
	add    eax,50
	cmp    eax, [SCREEN_WIDTH]
	jbe    .drawbkg
	add    edx, 50
	xor    eax,eax
	cmp    edx, [SCREEN_HEIGHT]
	jbe    .drawbkg
	
	mov    [bkgPainted], True
	ret
endp



proc Screen.subFloor,\
	floor, h
	mov eax, [floor]
	sub eax, [h]
	ret
endp

proc Screen.addFloor,\
	floor, h
	mov eax, [floor]
	add eax, [h]
	ret
endp

proc	Screen.setFloor,\
	screenH
	locals
	      persent	dd 0.8
	      const_100 dd 100
	      const_5	dd 5
	       temp dd 0
	endl
	fild	[screenH]
	fmul	[persent]
	fistp	[temp]

	mov	eax, [temp]
	dec	eax
	ret
endp



proc Screen.subRightWall,\
	right, w
	mov eax, [right]
	sub eax, [w]
	ret
endp

proc Screen.addRightWall,\
	floor, w
	mov eax, [right]
	add eax, [w]
	ret
endp

proc	Screen.setRightWall,\
	screenW
	local	persent dd 0.99
	local	temp dd 0

	fild	[screenW]
	fmul	[persent]
	fistp	[temp]

	mov	eax, [temp]
	ret
endp
      







proc	Screen.countHeightInBoxes

	locals
	    temp     dd 0
	endl
	fild	[y_floor_dd]
	fidiv	[box_y]
	fistp	[temp]
	mov	eax, [temp]
	ret
endp

proc	Screen.setObjHeightInBoxes,\
	percents

	local temp dd ?

	fild	[HeightInBoxes]
	fmul	[percents]
	fistp	[temp]
	mov	eax, [temp];;return rejection on Box_col

	ret
endp

proc	Screen.setZoom	uses eax edx
	locals
		zoom_inc dd 2.5
		zoom_incInt dd ?
		const_1000 dd 1000
		const_100  dd 100
		temp	 dd 0
	endl

	mov	eax, [right_wall_dd]
	xor	edx, edx
	div	[const_100]
	xor	edx, edx
	mul	[const_100]
	mov	[temp], eax
	fild	[temp]
	fidiv	[const_1000]
	fstp	[zoom_inc]

.count:
	fild	[box_x]
	fild	[box_y]
	fmul	[zoom_inc]
	fistp	[box_y]
	fmul	[zoom_inc]
	fistp	[box_x]

	stdcall toLock, box_x
	stdcall toLock, box_y

	fild	[W]
	fmul	[zoom_inc]
	fistp	[W]

	stdcall toLock, W

	fild	[H]
	fmul	[zoom_inc]
	fistp	[H]
	stdcall toLock, H


	fild	[EnemyW_dd]
	fmul	[zoom_inc]
	fistp	[EnemyW_dd]
	stdcall toLock, EnemyW_dd
	fild	[EnemyH_dd]
	fmul	[zoom_inc]
	fistp	[EnemyH_dd]
	stdcall toLock, EnemyH_dd

	fild	[FlyEnemyW_dd]
	fmul	[zoom_inc]
	fistp	[FlyEnemyW_dd]
	stdcall toLock, FlyEnemyW_dd

	fild	[FlyEnemyH_dd]
	fmul	[zoom_inc]
	fistp	[FlyEnemyH_dd]
	stdcall toLock, FlyEnemyH_dd


	fild	[FlyBallW_dd]
	fmul	[zoom_inc]
	fistp	[FlyBallW_dd]
	stdcall toLock, FlyBallW_dd

	fild	[FlyBallH_dd]
	fmul	[zoom_inc]
	fistp	[FlyBallH_dd]
	stdcall toLock, FlyBallH_dd


	ret
endp

proc	toLock uses edi eax edx,\
		    val
	mov	edi, [val]
.mainloop:
	xor	ax,ax
	mov	al, byte[edi]
	mov	dl, 5
	div	dl
	test	ah, ah
	jz	.end
	inc	dword[edi]
	jmp	.mainloop
.end:
	ret
endp	 


proc	Screen.PrintText uses edi,\
			 text, text_len, txtRect, type

	cmp [transition_flag], True
	je  .end
	invoke SelectObject, [hdc], [hfont]
	invoke SetBkMode, [hdc], TRANSPARENT
	invoke SetTextColor, [hdc], 0xFFFFFF
	invoke DrawText, [hdc], [text],[text_len],[txtRect], [type]
	invoke SetBkMode, [hdc], OPAQUE

	invoke	 lstrlenA, [text]
	cmp	[text_flag], True
	jne	@f
	mov	[text_len], eax
@@:
	cmp	 [text_len], eax
	jae	 @f
	inc    [text_len]
	jmp	.end
@@:
	mov	[text_flag], True
	mov	edi, [txtRect]
	sub	dword[edi+4], 5
.end:
	mov  eax, [text_len]
	ret
endp	


proc	Screen.DrawPoints uses eax edx edi
	invoke SelectObject, [hdc], [hfont]
	invoke SetBkMode, [hdc], TRANSPARENT
	invoke SetTextColor, [hdc], 0xFFFFFF

	invoke lstrlenA, points
	invoke DrawText, [hdc], points,eax, pointsRect, DT_CENTER
	invoke SetBkMode, [hdc], OPAQUE

	invoke	SelectObject, [hMemDC], [hbmpPoints]
			
	invoke	BitBlt, [hdc], [pointsRect.left], [pointsRect.top],54, 54, [hMemDC], 0,0, SRCCOPY
    ret
endp

proc AddPoints uses edi eax,\
	points, ind, addP

	xor eax, eax
	mov eax, [addP]
	mov edi, [points]
	cmp dword[edi], '999'
	je .skip
@@:
	mov ecx, [ind]
	add edi, ecx
	mov dh, [edi]
	add [edi], al
.a:
	cmp byte[edi], '9'
	jbe	.skip
	;our value is addP-(10-cur)
	mov dl, ':'
	sub dl, dh ;10-val
	sub al, dl
	add al, '0'
	mov byte[edi], al
	mov eax, [addP]
	dec edi
	add byte[edi], 1
	loop .a

.skip:
	ret
endp































proc	drawTransition uses edi esi eax edx ecx
	invoke CreateSolidBrush, $0
	invoke SelectObject, [hdc], eax
	invoke Ellipse, [hdc], [transition_rect.left],[transition_rect.top],[transition_rect.right],[transition_rect.bottom]
      ;  invoke FillRect, [hdc], transition_rect, eax
	cmp	[transition_inc], True
	jne	@f
	mov	eax, physSpeed*3
	jmp	.trans_set
@@:
	mov	eax, -physSpeed*3
.trans_set:
	sub    [transition_rect.left],eax
	sub    [transition_rect.top],eax
	add    [transition_rect.right],eax
	add    [transition_rect.bottom],eax

	mov	eax, [transition_rect.right]
	sub	eax, [transition_rect.left]
	cmp	eax, 0
	jl	.stop

	stdcall CountTransR
	cmp    [transition_rect.left], eax
	jg     .end
	cmp    [transition_inc], False
	je     .stop
	mov    [transition_inc], False
	jmp    .end
.stop:
	and    [transition_flag], False
.end:
	ret
endp

proc	CountTransR
	locals
	    temp dd 0
	    const_2 dd 2
	endl
	fild	[SCREEN_WIDTH]
	fimul	[SCREEN_WIDTH]
	fild	[SCREEN_HEIGHT]
	fimul	[SCREEN_HEIGHT]
	fadd	[SCREEN_WIDTH]
	fsqrt
	fidiv	[const_2]
	fistp	[temp]	  ;st0<-SCREEN_WIDTH^2
	mov	eax, [temp]
	fsqrt
	fidiv	[const_2]
	fistp	[temp]
	sub	eax, [temp]

	ret
endp   

