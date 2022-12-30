proc	Chip.DrawChip uses eax

	
	invoke	LoadBitmap, [wc.hInstance], [ChipState]
	mov	[hBitmap], eax
	invoke	SelectObject, [hMemDC], [hBitmap]
	;image loads from KeyPress according to player action
	cmp	[DirRight], True
	jne	.dirLeft
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W],[H],[hMemDC],[pict_x], 0, pictW, pictH, SRCCOPY
	jmp	.end
.dirLeft:
	mov		eax, [pict_x]
	add		eax, pictW-1
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W], [H], [hMemDC], eax, 0, -pictW, pictH, SRCCOPY
.end:	
	;check if we need to tie uppedbox with chip
	cmp	[boxUpped], False
	je	 .finish
	stdcall  Chip.GetBox, [uppedBoxAdr] 
.finish:      
	invoke	DeleteObject, [hBitmap]  
	ret
endp

proc	Chip.DrawHP

	invoke	SelectObject, [hMemDC], [hbmpHP]

	xor	eax,eax
	mov	eax, [HP]	
	mul	[heartSize]
	invoke BitBlt, [hdc],20,10, eax, [heartSize], [hMemDC], 0, 0, SRCCOPY
	ret
endp

proc Chip.DrawDie

	invoke	LoadBitmap, [wc.hInstance], ChipDieMask
	mov	[hBitmap], eax
	invoke	SelectObject, [hMemDC], [hBitmap]
	;image loads from KeyPress according to player action
	cmp	[DirRight], True
	jne	.dirLeftM
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W],[H],[hMemDC],[pict_x], 0, pictW, pictH, SRCAND
	jmp	.endM
.dirLeftM:
	mov		eax, [pict_x]
	add		eax, pictW-1
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W], [H], [hMemDC], eax, 0, -pictW, pictH, SRCAND
.endM:

	invoke	LoadBitmap, [wc.hInstance], [ChipState]
	mov	[hBitmap], eax
	invoke	SelectObject, [hMemDC], [hBitmap]
	;image loads from KeyPress according to player action
	cmp	[DirRight], True
	jne	.dirLeft
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W],[H],[hMemDC],[pict_x], 0, pictW, pictH, SRCPAINT
	jmp	.end
.dirLeft:
	mov		eax, [pict_x]
	add		eax, pictW-1
	invoke StretchBlt,[hdc],[x_pos],[y_pos],[W], [H], [hMemDC], eax, 0, -pictW, pictH, SRCPAINT
.end:	
	ret
endp





;boxes <- coordinates of a single box on the field
proc	Chip.GetBox  uses esi edi eax edx,\
	boxes
	locals
	    const_6	dd 6
	    const_2	dd 2
	    temp	dd 0
	endl
	; in shift state you can't take box
	mov	cl, [shiftState]
	cmp	cl, True
	je	.end

	;set pos
	xor	     eax,eax
	xor	     edx,edx
	mov	eax, [x_pos]
	fild	[W]
	fidiv	[const_6]
	fistp	[temp];H/6
	add	eax, [temp]


	mov	edx, [y_pos]
	fild	[H]
	fidiv	[const_2]
	fistp	[temp] ;H/2
	sub	edx, [box_y]
	mov	edi, [boxes]
	mov	[edi], eax
	mov	[edi+4], edx

	;save adress of upped box
	mov	[uppedBoxAdr], edi

.end:
	ret
endp
      





; get position of main object, object's pointer
; and the number of objects
proc	ObjectsContact uses esi edi,\
	x,y, w,h,\
	objects, num_of_obj, obj_w,obj_h

	mov	edi, [objects]
	xor	ecx,ecx
	mov	ecx, [num_of_obj]
	mov	[contactFlag], 0
.checkObj:
	cmp	[uppedBoxAdr], edi
	je	.skip
	stdcall Phys.Contact, [x],[y], [w],[h], \
			      [edi], [edi+4], [obj_w],[obj_h]
.skip:
	add	edi, 8
	loop	.checkObj
.end:
	ret
endp  

;return position of first object to pick
proc	FirstPickContact uses esi edi ecx,\
	x,y, h,w,\
	objects, num_of_obj, obj_w, obj_h

	mov	edi, [objects]
	xor	ecx,ecx
	mov	ecx, [num_of_obj]
	mov	[contactFlag], 0
	xor	 eax, eax
.checkObj:
	stdcall Phys.Contact, [x],[y], [w],[h],\
			      [edi], [edi+4],[obj_w], [obj_h]
	cmp [contactFlag], 0
	jne	.end
.skip:
	add	edi, 8
	inc eax ;new obj ID
	loop	.checkObj	
.end:
	ret
endp 





proc	Chip.getHit
	cmp	[getRecentHit], False
	jne	.end

	dec	[HP]

	cmp [HP], 0
	jle	.highkick
	sub		 [y_pos], 50
	sub		 [x_pos], 200
	cmp		 [x_pos], left_wall
	jg		 @f
	mov		 [x_pos], 25
	jmp		@f
.highkick:
	sub		 [y_pos], 150
	sub		 [x_pos], 200
	cmp		 [x_pos], left_wall
	jg		 @f
	mov		 [x_pos], 25
@@:

	mov	[getRecentHit], True
	invoke	KillTimer, [global_hwnd], CHIP_HIT_AVAIL_TIMER
	invoke	SetTimer, [global_hwnd], CHIP_HIT_AVAIL_TIMER, ms*3/2, NewHitTimer

.end:
	ret
		endp

proc	NewHitTimer
		; and [getRecentHit], False
	ret
endp	
