proc	Falling uses eax edx ecx
local	fallSpeed =  physSpeed * 2
	local	slowdownBorder=60

	mov	edx, [y_pos]
	add edx, fallSpeed
	stdcall ObjectsContact, [x_pos], edx, [W],[H],\
					Boxes_coordinates, box_col,[box_x],[box_y]
	mov	cl, [contactFlag]	
	test	cl, availS
	jz	@f
	mov		[FallingFlag], False		
@@:
	stdcall Chip.Move.DownCarry, fallSpeed ; return cx with STOPFALLING value if obj on ground

	cmp	eax, 0
	jne	 .skip
	mov[FallingFlag], False

.skip:
	ret
endp




		
proc correctHeightLevels uses edx ecx
	local correctionSpeed=physSpeed

	mov	ecx, [H]
	add	ecx, correctionSpeed
	stdcall ObjectsContact, [x_pos], [y_pos], [W], ecx, \
					Boxes_coordinates, box_col,[box_x],10

	xor		ecx,ecx
	mov		cl, [contactFlag]
	test	cl, availS
	jz		.floorcorrection	
	
	mov		eax, [y_pos]
	add		eax, 10
	stdcall ObjectsContact, [x_pos], eax, [W], [H], \
				Boxes_coordinates, box_col, [box_x], 10
	mov		cl, [contactFlag]
	test	cl, availS
	jnz		.endCorrection
		stdcall Chip.Move.Down, 10
	jmp		.end


.floorcorrection:
	mov	eax, [H]
	inc	eax
	stdcall Screen.subFloor, [y_floor_dd], eax;H+1
	cmp		[y_pos], eax  ;y_floor-H-1
	je		.endCorrection
	stdcall  Chip.Move.Down, correctionSpeed

	mov	eax, [H]
	stdcall Screen.subFloor, [y_floor_dd], eax;H+1
	cmp		[y_pos],eax
	jb		.end
	stdcall setToFloor		
.endCorrection:
	mov [OnGround], True
.end:
	ret
endp 


proc setToFloor
	mov		eax, [y_floor_dd]
	sub		eax, [H]
	dec		eax
	mov		[y_pos], eax
	ret
endp


proc DieFalling
local	fallSpeed =  physSpeed * 2
	local	slowdownBorder=60	
	stdcall Chip.Move.Down, physSpeed*2/3 ; return cx with STOPFALLING value if obj on ground
.skip:
	ret
endp
