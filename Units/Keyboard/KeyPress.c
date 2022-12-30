proc	KeyPress
	local  fsLim dd ?
	local  percent dd 0.2
	mov	[shiftState], False
.S:
	
	invoke	GetAsyncKeyState, VK_DOWN
	cmp		eax, 1
	ja		.sAction
	invoke	GetAsyncKeyState, 'S'
	cmp	eax, 1
	jbe	@f
	
.sAction:
	cmp	[OnGround], True
	jne	@f

	mov	   [shiftState], True	
	 
	mov eax, [y_pos]
	add eax, [H]
	mov edx, [y_floor_dd]
	sub edx, 200
	cmp eax, edx
	jae		@f
	mov		edx, [H]
	add		edx, 5
	add		[y_pos], edx

@@:
	cmp	[shiftState], False
	jne	.finish


	

.Q:
	invoke	GetAsyncKeyState, 'Q'
	test	 al, al
	jz	.W
	cmp	[boxUppedAvail], False
	je	.W
	cmp	[boxUpped], True
	je	.shot
	cmp	[BoxFly], True
	je	.W
	stdcall ObjectsContact, [x_pos], [y_pos], [W], [H], \
				Boxes_coordinates, box_col, [box_x], [box_y]
	stdcall Box.CheckLift
	jmp	.finish
.shot:
	mov	[BoxFly], True
	and	[boxUpped], False
	

.W:
	invoke	GetAsyncKeyState, 'W'
	cmp	eax, 1
	jbe	.D
    cmp [boxUpped], True
	jne	.D
	cmp	   [BoxFly], True
	je	   .D

	mov	   [boxDir], 'W'
	mov	[BoxFly], True
	and	[boxUpped], False

.D:
	invoke	GetAsyncKeyState, 'D'
	test	eax,eax
	jnz	@f
	invoke		GetAsyncKeyState, VK_RIGHT
	test	eax, eax
	jz	.A
@@:	
	
	stdcall ObjectsContact, [x_pos], [y_pos], [W], [H], \
				Boxes_coordinates, box_col, [box_x], [box_y]

	cmp		[boss_scene], True
	je		@f
	fild	[right_wall_dd]
	fmul	[percent]
	fistp	[fsLim]
	mov		eax, [x_pos]
	cmp		eax, [fsLim]
	jb		@f
	cmp		[text_flag], True
	jne		.skipspeed

@@:
	stdcall  Chip.Move.Right, speed
.skipspeed:
	mov	[DirRight], True
	stdcall SetChipGoAnimation
	cmp	   [BoxFly], True
	je	   .A
	mov	   [boxDir], 'D'

.A:
	invoke	GetAsyncKeyState, 'A'
	test	eax, eax
	jnz		@f
	invoke	GetAsyncKeyState, VK_LEFT
	test	eax,eax
	jz	.space
@@:
	
	stdcall ObjectsContact, [x_pos], [y_pos], [W], [H], \
				Boxes_coordinates, box_col, [box_x], [box_y]
	stdcall  Chip.Move.Left, speed	
	mov	[DirRight], False
	stdcall SetChipGoAnimation

	cmp	   [BoxFly], True
	je	   .space
	mov	   [boxDir], 'A'


.space:
	invoke	GetAsyncKeyState, VK_SPACE
	test	eax,eax
	jnz		@f
	invoke	GetAsyncKeyState, VK_UP
	test	eax, eax
	jz		.esc
@@:
	cmp	[JumpFlag], True
	je	.esc
	cmp	[OnGround], True
	jne	.esc

	mov	[JumpFlag], True
	mov	[FallingFlag], False
	mov	[OnGround], False



.esc:
	invoke	GetKeyState, VK_ESCAPE
	cmp	eax, 1
	jbe	.finish
	mov	eax,QUIT
 .finish:
	
	ret
endp	      