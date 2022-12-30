proc	Boss.BossDraw
local finaltext db ?
.finaltext :
	cmp[BossHP], 0
	jg @f	
	stdcall Screen.PrintText, cutscene_text, [cutscene_len], cutsceneRect, DT_LEFT
	mov[cutscene_len], eax
	mov eax, [hbmpBoss]
	mov[BossImg], eax
	jmp		.bossstart
@@:
	cmp		[HP], 0
	jg .bossstart
	stdcall Screen.PrintText, cutscene_text2, [cutscene_len], cutsceneRect, DT_LEFT 
	mov [cutscene_len], eax

.bossstart:
	invoke	SelectObject, [hMemDC], [BossImg]
	invoke BitBlt, [hdc], [BossXY], [BossXY + 4], [BossW_dd], [BossH_dd], [hMemDC], 0, 0, SRCPAINT

	stdcall Screen.PrintText, bosstext, [text_len],bosstxtRect, DT_LEFT
	mov [text_len], eax		
	invoke	 lstrlenA, bosstext
	cmp		[text_len], eax
	jb		.YUMMY
	or		[BossStart], True
.YUMMY: 
	cmp		[flagYUMMY], TRUE
	jne		.end
	invoke	 lstrlenA, textYUMMY
	stdcall Screen.PrintText, textYUMMY, eax,rectYUMMY, DT_LEFT	
	
	cmp [rectYUMMY.bottom], seil
	jge		.end
	mov[flagYUMMY], FALSE
	invoke SetRect, rectYUMMY, 900, 200, 1100, 400




.end:
	ret
endp




proc	Boss.FlyBoxContact uses edi eax
	local	temp dd 0
	local	const_4 dd 4
	push	word[contactFlag]

	mov	edi, [uppedBoxAdr]
	mov eax, [BossXY]
	fild	 [BossW_dd]
	fidiv	 [const_4]
	fistp	 [temp]
	add eax, [temp]

	mov	[contactFlag], 0
	stdcall Phys.Contact, dword[edi],dword[edi+4], [box_x], [box_y],\
					 eax, [BossXY+4], [BossW_dd], [BossH_dd]
	cmp		 [contactFlag], 0
	je		 .end

	cmp [uppedBoxAdr], CheeseBoxStartAdr
	jae @f
;JUST STANDART BOX
	cmp [CheeseFlag], True
	je  .clearBox
	dec [BossHP]
	
	stdcall AddPoints, points, points_len - 1, 5
	jmp .clearBox
@@:
;CHEESE SHOT
	stdcall Boss.HitByCheese
	

.clearBox:
	;clear flying box state
	stdcall Box.clUppedBox
.end:
	pop	word[contactFlag]
	ret
endp



	   
proc	Boss.ChipContact
	push	word[contactFlag]
	stdcall ObjectsContact, [x_pos], [y_pos], [W],[H],\
					 BossXY,1, [BossW_dd], [BossH_dd]
	cmp		 [contactFlag], 0
	je	.finish
	stdcall 	 Chip.getHit
	sub	[x_pos],200
.finish:
	
	pop	word[contactFlag]
	ret
endp 








proc	BossModel uses edi esi eax,\
					   hwnd
	push	word[contactFlag]


	cmp	[BossStart], True
	jne		.end
	stdcall Boss.ChipContact
	cmp	[BoxFly], True
	jne	@f
	stdcall Boss.FlyBoxContact
@@:
	stdcall Cheese.setAction

	cmp	[CheeseFlag], True
	jne	.end
	cmp	[CheeseSpeed], 0
	jne	 .shoot
	
	mov	 [CheeseCanBeUpped], True
;SET NEW CHEESE VALUES

	inc [curCheeseID]
	cmp  [curCheeseID], cheeseAmount
	jbe	@f
	mov [curCheeseID], 1
@@:
	stdcall Cheese.setType
	stdcall Cheese.setStartPos
	stdcall Cheese.setSpeed
	mov	[CheeseSpeed], al

	invoke	 KillTimer, [hwnd], BOSSTIMER
	cmp		[BossHP], 4
	jb		.fast
	cmp		[BossHP], 7
	jae		.extrafast
	invoke	SetTimer, [hwnd], BOSSTIMER, ms*10/7, BossAttackPhaseTimer
	jmp		.cheeseset
.fast:
	cmp		[BossHP], 1
	je		.extrafast
	invoke	SetTimer, [hwnd], BOSSTIMER, ms, BossAttackPhaseTimer
	jmp		.cheeseset
.extrafast:
	invoke	SetTimer, [hwnd], BOSSTIMER, ms*4/7, BossAttackPhaseTimer

.cheeseset:
;SET NEW CHEESE ADRESS FOR SHOOT IN BOX BUFFER
	cmp	[CheeseBoxAdr], CheeseBoxStartAdr+(cheeseAmount-1)*8
	jae	.resetAdr
	add	[CheeseBoxAdr], 8
	jmp	.checkAdrCorrection
.resetAdr:
	mov	[CheeseBoxAdr], CheeseBoxStartAdr

.checkAdrCorrection:

	mov	eax, [uppedBoxAdr]
	cmp	[CheeseBoxAdr], eax
	jne .loadBossImg

	add	[CheeseBoxAdr], 8
	inc [curCheeseID]
	cmp  [curCheeseID], cheeseAmount
	jbe	@f
	mov [curCheeseID], 1
@@:
	

.loadBossImg:

	mov edi, [CheeseBoxAdr]
	mov dword[edi], CART
	mov dword[edi + 4], CART

	mov	 eax, [hbmpBoss]
	mov	 [BossImg], eax
	jmp	 .end

.shoot:

	stdcall  SetCheeseToBoxes, CheeseXY, [CheeseBoxAdr];adresses of need box
	stdcall Boss.Shoot

.end:
	pop	word[contactFlag]
	ret
endp






proc	Boss.Shoot uses ecx eax

	xor	ecx,ecx 
	mov	cl, [CheeseSpeed]
	dec	cl
	stdcall Screen.subFloor, [y_floor_dd], [CheeseH]
	cmp	[CheeseXY+4], eax
	jae	.skipY

	xor	eax,eax
	stdcall 	  Random.Get, 1,9
	add	[CheeseXY+4], eax
	inc	cl
.skipY:
	sub	[CheeseXY], ecx
	mov	[CheeseSpeed], cl
.skipX:
	ret
endp  
     


proc	Boss.HitByCheese uses edi ecx eax
	local adrSize=8

	mov edi, CheeseType
	mov eax, [uppedBoxAdr]
	sub eax, CheeseBoxStartAdr
	mov ecx, adrSize
	div ecx

	mov ecx, eax
	inc ecx
.setID:
	add edi, 4
	loop .setID
	sub edi, 4

;STANDART CHEESE SHOT
	cmp	 dword[edi], STANDART
	jne	 @f
	cmp	[CheeseFlag], True
	je		.end
	dec		[BossHP]
	stdcall AddPoints, points, points_len - 1, 3
	jmp		.end
@@:
	cmp	dword[edi], RAGE
	jne	@f
	inc	[BossHP]
	or [flagYUMMY], TRUE	
	 stdcall AddPoints, points,points_len-1, 10
	jmp	.end
@@:
	cmp	dword[edi], BLACK
	jne	@f
	dec	[BossHP]
	cmp	[CheeseFlag], True
	jne		 .skipdopdec
	dec		[HP]	
.skipdopdec:
	stdcall  Chip.getHit	
	stdcall AddPoints, points,points_len-1, 7
@@:
.end:

	
	ret
endp  