proc	Box.DrawBoxes	uses edi ecx
.startboxdraw:	
	invoke	SelectObject, [hMemDC], [hbmpBox]
	mov	edi, Boxes_coordinates
	mov	ecx, box_col
.drawboxes:
	push	ecx
	invoke StretchBlt, [hdc], [edi], [edi + 4], [box_x], [box_y], [hMemDC], 0, 0,pictBox_W,pictBox_H, SRCCOPY
	add    edi, 8
	pop    ecx
	loop  .drawboxes
.end:
	ret
endp

proc	Box.drawDestroyBox	uses edi eax edx
	locals 
		const_2 dd 2
		size_x dd 0
		size_y dd 0
	endl
	mov edi, boxDstrXY

	invoke SelectObject, [hMemDC], [hbmpBoxDestroyMask]
	fild [box_x]
	fild [box_y]
	fimul [const_2]
	fistp [size_y]
	fimul [const_2]
	fistp [size_x]
	invoke StretchBlt, [hdc], [edi], [edi + 4], [size_x], [size_y], [hMemDC], [pictBoxDstrX], 0,pictBox_W*2,pictBox_H*2, SRCAND

	invoke SelectObject, [hMemDC], [hbmpBoxDestroy]
	mov    eax, [box_x]
	mov    edx, [box_y]
	invoke StretchBlt, [hdc], [edi], [edi + 4], [size_x], [size_y], [hMemDC], [pictBoxDstrX], 0, pictBox_W * 2, pictBox_H * 2, SRCPAINT
	ret
endp

proc	Box.DestroyBoxAnimation
	cmp [destroyBox], True
	jne .end
	stdcall Box.drawDestroyBox
	cmp	[pictBoxDstrX],(pictBox_W*2) * 2
	jb     @f
	mov	[destroyBox], False
	mov	[pictBoxDstrX], 0
	jmp	.end
@@:
	add	[pictBoxDstrX], pictBox_W*2
.end:
	ret
endp   


proc Box.clUppedBox uses edi eax edx
      locals
		 const_2 dd 2
		 temp	 dd 0
      endl
	
	 cmp [uppedBoxAdr], 0
	 je	 .end	

     mov     edi, [uppedBoxAdr]
		
	;SET DSTR BOX COORDINATES
    mov eax, [edi]
    fild     [box_x]
    fidiv    [const_2]
    fistp    [temp]
    sub eax, [temp]
    mov [boxDstrXY],eax
    mov eax, [edi+4]


    fild     [box_y]
    fidiv    [const_2]
    fistp    [temp]
    sub eax, [temp]
    mov [boxDstrXY+4], eax
    or [destroyBox], True
			
	mov		al, [uppedBoxID]
	mov		[boxDstrID], al

	mov	dword[edi], CART
	mov	dword[edi+4], CART
	mov	[uppedBoxAdr], CART
	mov	[uppedBoxID], 0
	mov	[BoxFly], False
.end:
	ret 
endp

   
proc	Box.CanBeLifted

	mov	[canLift], False
.checkX:
	xor	eax,eax
	xor	edx,edx
	mov	eax,[x_pos]
	add	eax, W
	mov	edx, [box_x]
	sub    eax,edx
	cmp    eax, 0
	jne    .end
.checkY:
	xor	eax,eax
	xor	edx,edx
	mov	eax, [y_pos]
	add	eax, H-1
	mov	edx, [box_y]
	sub	eax,edx
	cmp	eax,0
	jne	.end
	mov    [canLift], True
.end:
	ret
endp

 proc	 Box.Relocation uses edi esi ecx eax, \
							oldXY, newXY

 xor ecx, ecx
 mov	       ecx, box_col_NS

 mov	       edi, [oldXY]
 mov	       esi, [newXY]
 ;add		esi, (box_col_NS - 1) * 8
.reloc:

	mov	eax, [esi]
	mov	[edi], eax
	mov	eax, [esi+4]
	mov	[edi+4], eax
	add	esi, 8
	add	edi, 8

	loop	.reloc
	ret
endp	

;var for coordinate saving
;start_x_prsnt - x coordinate of center box
;y_floor_rej - y in boxes from y_floor (2 boxes: y_floor-2*box_y)
;left/down/right - num of boxes  in left/down/right from center box
;
proc    Box.setStartCoordinates uses edi esi ecx eax edx,\
        boxXY, start_x_prsnt,y_floor_rej, left, down,right
        locals
                temp    dd 0
        endl
        mov   edi,  [boxXY]

;SET BASE BOX COORDINATES
        ;:Y BASE BOX
        mov    eax, [y_floor_dd]
        mov    ecx, [y_floor_rej]
@@:
        sub     eax, [box_y]
        loop    @B
        mov    [edi+4], eax
        ;;X BASE BOX
        fild   [right_wall_dd]
        fmul   [start_x_prsnt]
        fistp  dword[edi]


        mov     eax, [edi]; BASE X
        mov     edx, [edi+4] ;BASE Y
        push    eax edx
        add     edi, 8
;setLefTBox
        mov     ecx, [left]
        cmp     ecx, 0
        jle     .setDown
.setLeft:
        sub     eax, [box_x]
        mov     [edi], eax
        mov     [edi+4], edx
        add     edi, 8
        loop    .setLeft

;setDownBox
.setDown:
        pop     edx eax
        push    eax edx
        mov     ecx, [down]
        cmp     ecx, 0
        jle     .setRight

.downloop:
        add     edx, [box_y]
        mov     [edi], eax
        mov     [edi+4], edx
        add     edi, 8
        loop    .downloop
        pop     edx eax
;setRightBox
.setRight:
        pop     edx eax

        mov     ecx, [right]
        cmp     ecx, 0
        jle     .end
.rightloop:
        add     eax, [box_x]
        mov     [edi], eax
        mov     [edi+4], edx
        add     edi, 8
        loop    .rightloop

.end:
        ret
endp 


proc	Box.CheckLift  uses edi ecx

	xor	ecx,ecx
	mov	cl, [contactFlag]
	test	cl, availA+availD
	jz     .end

	stdcall FirstPickContact, [x_pos],[y_pos], [W],[H],\
				Boxes_coordinates, box_col, [box_x],[box_y]
	
	mov	[uppedBoxID], al
	;sub	[uppedBoxID], al ;from 0 to obj_col-1

	;find the box we can up
	mov	edi, Boxes_coordinates
	mov	cl, [uppedBoxID]
	cmp	cl, 0
	je	.skip
@@:	
	add	edi, 8
	loop	@B
.skip:
	stdcall Chip.GetBox, edi
	mov	[boxUpped], True
.end:
	ret
endp 

