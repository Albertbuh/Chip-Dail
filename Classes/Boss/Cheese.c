proc	Cheese.DrawBoxesWithCheese uses edi ecx
	invoke	SelectObject, [hMemDC], [hbmpBox]
	mov	edi, Boxes_coordinates
	mov	ecx, box_col - 4
.drawboxes:
	push	ecx
	invoke StretchBlt, [hdc], [edi], [edi + 4], [box_x], [box_y], [hMemDC], 0, 0, pictBox_W, pictBox_H, SRCCOPY
	add    edi, 8
	pop    ecx
	loop   .drawboxes

	stdcall 	Cheese.DrawCheese, CheeseBoxStartAdr, cheeseAmount
.end:
	ret
endp

proc	Cheese.DrawCheese uses edi ebx ecx eax,\
	cheeseAdr, cheese_amount
	local	   const8 dd 8
	local	   temp   dd 0

	mov		edi, [cheeseAdr]
	mov		ecx, [cheese_amount]
	mov		ebx, hCheeseImg
.drawloop:
	push ecx

	invoke	SelectObject, [hMemDC], [hbmpCheeseMask]
	mov eax, [edi]

       ; sub eax, CheeseW / 8
	fild	 [CheeseW]
	fidiv	 [const8]
	fistp	 [temp]
	sub	 eax, [temp]

	invoke	BitBlt, [hdc], eax, [edi+4], [CheeseW], [CheeseH],[hMemDC],0,0,SRCAND
	;invoke StretchBlt, [hdc], [edi], [edi + 4], [box_x], [box_y], [hMemDC], 0, 0,CheeseW,CheeseH, SRCAND
	add		edi, 8
	add		ebx, 4
	pop		ecx
	loop	.drawloop

	mov		edi, [cheeseAdr]
	mov		ecx, [cheese_amount]
	mov		ebx, hCheeseImg
.drawloop2:
	push ecx
	invoke	SelectObject, [hMemDC], [ebx] ;ebx <- hCheeseImg
	mov eax, [edi]
	sub eax,[temp]; CheeseW/8
	invoke	BitBlt, [hdc], eax, [edi +4], [CheeseW], [CheeseH],[hMemDC],0,0,SRCPAINT
	;invoke StretchBlt, [hdc], [edi], [edi + 4], [box_x], [box_y], [hMemDC], 0, 0, CheeseW, CheeseH, SRCPAINT

	add		edi, 8
	add		ebx, 4
	pop		ecx
	loop	.drawloop2

	ret
endp


 


proc Cheese.setAction uses edi ecx

	mov edi, CheeseType
	mov ecx, [curCheeseID]

.setID:
	add edi, 4
	loop .setID
.set:
	sub edi, 4; SUB 4 EXTRA BITS
	cmp dword[edi], STANDART
	jne @f
	stdcall Cheese.ChipContact
	jmp .finish
@@:
	cmp dword[edi], RAGE
	jne @f
	stdcall Cheese.RageContact
	jmp .finish
@@:
	cmp dword[edi], BLACK
	jne @f
	stdcall Cheese.ChipContact
	jmp	.finish
@@:
.finish:
	ret
endp




proc	Cheese.setType uses edi ecx

	mov edi, CheeseType
	mov ecx, [curCheeseID]

.setID:
	add edi, 4
	loop .setID
	sub edi, 4
.set:
    stdcall  Random.Get, 1,10
    cmp     al, 3
    jae   @f
    mov dword[edi], STANDART;1-6
	jmp		.end
 @@: 
	cmp	al, 7
	jae	@f
	mov dword[edi], RAGE ; 7-8
	jmp .end
@@:
	;al > 9
	mov dword[edi], BLACK;9-10

.end:
	ret
endp


proc	Cheese.setStartPos uses eax
	locals
	   const_4 dd 4
	   temp dd 0
	endl
	mov	[CheeseFlag], False
	mov	[CheeseCanBeUpped], False
	;set start loc of cheese
	stdcall    Screen.subRightWall, [right_wall_dd], [BossW_dd]
	mov	[CheeseXY], eax
	fild	[BossH_dd]
	fidiv	[const_4]
	fistp	[temp]

	stdcall Screen.subFloor, [y_floor_dd], [temp]
	mov	[CheeseXY+4], eax
	ret
endp

proc	Cheese.setSpeed
	stdcall 	  Random.Get, 1,10
	cmp		  ax, 3
	jb		  .slow
	stdcall 	  Random.Get, 25,50
	jmp		  .end
.slow:
	stdcall 	  Random.Get, 5,27
.end:
	ret
endp  

	

proc	Cheese.setImg uses eax ecx edi 
	mov edi, CheeseType
	mov ecx, [curCheeseID]

.setID:
	add edi, 4
	loop .setID	
	sub edi, 4; SUB 4 EXTRA BITS

.set:
	cmp dword[edi], STANDART
	jne @f
	mov	eax, [hbmpCheese]
	jmp .startsetting
@@:
	cmp dword[edi], RAGE
	jne @f
	mov eax, [hbmpRedCheese]
	jmp .startsetting
@@:
	cmp dword[edi], BLACK
	jne @f
	mov eax, [hbmpBlackCheese]
	jmp .startsetting
@@:
.startsetting:
	mov edi, hCheeseImg
	mov ecx, [curCheeseID]
.setID2:
	add edi, 4
	loop .setID2
	sub edi, 4
.set2:
	mov	[edi], eax ;hCheeseImg <- needed img of cheese
	ret
endp


proc	Cheese.RageContact
	push	word[contactFlag]
	stdcall  ObjectsContact, [x_pos], [y_pos], [W],[H],\
					 CheeseXY,1,  [CheeseW], [CheeseH]
	cmp		 [contactFlag], 0
	je		 .finish
	cmp		 [CheeseCanBeUpped], False
	jne		 .up
	cmp	[getRecentHit], False
	jne	.finish
	dec[HP]
	stdcall 	Chip.getHit
.up:
.finish:
	
	pop	word[contactFlag]
    ret
endp

proc	Cheese.ChipContact uses eax edx
	push	word[contactFlag]
	mov	eax, [W]
	add	eax, 5
	mov	edx, [H]
	add	edx, 10
	stdcall ObjectsContact, [x_pos], [y_pos], eax,edx,\
					 CheeseXY,1,  [CheeseW], [CheeseH]
	cmp		 [contactFlag], 0
	je		 .finish
	cmp		 [CheeseCanBeUpped], False
	jne		 .up
	stdcall 	Chip.getHit
.up:
.finish:
	
	pop	word[contactFlag]
	ret
endp
	 

proc	SetCheeseToBoxes uses eax edi esi,\
	cheese, box ; Adreses to obj position num for example cheesyXY, Boxes_coordinates+8

	mov	edi, [box] ;edi<-Boxes_coordinates+8
	cmp	[uppedBoxAdr], edi
	je	.skip
	mov	esi, [cheese] ; esi<-CheeseXY
	mov	eax, [esi] ;eax<-CheeseXPOS
	mov	[edi], eax ;boxX <- CheeseX
	mov	eax, [esi+4]
	mov	[edi+4], eax
.skip:
	ret
endp	

