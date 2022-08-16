proc	Chip.Draw  uses es di ax cx,\
	x, y
	
	push	$A000
	pop	es
	mov	ax, color
	;set up coordinates
	mov	di, [y]
	imul	di, 320
	add	di, [x]

	;set object Height
	mov	cl, [shiftState]
	cmp	cl, True
	mov	cx, H/2
	je	.draw
	mov	cx, H ; shift not pressed

	;draw	model
.draw:
	push	cx
	mov	cx, W
	rep	stosb
	sub	di, 320+W
	pop	cx
	loop	.draw		 

	cmp	[boxUpped], False
	je	.end
	stdcall Chip.GetBox, [uppedBoxAdr]
	

.end:

	ret
endp 


proc	Chip.PushBox,\
	dir

	cmp	[boxUpped], True
	jne	.end
	and	[boxUpped], False
	stdcall Box.Shoot, [dir], [uppedBoxAdr]
	mov	di, [uppedBoxAdr]
	mov	word[di], ZERO
	mov	word[di+2], ZERO
	mov	[uppedBoxID], 0

.end:
	ret
endp 
;boxes <- coordinates of a single box on the field
proc	Chip.GetBox  uses si di ax dx,\
	boxes
	; in shift state you can't take box
	mov	cl, [shiftState]
	cmp	cl, True
	je	.end

	;set pos
	xor	     ax,ax
	xor	     dx,dx
	mov	ax, [x_pos]
	add	ax, W/4
	mov	dx, [y_pos]
	sub	dx, H
	mov	di, [boxes]
	mov	[di], ax
	mov	[di+2], dx

	;save adress of upped box
	mov	[uppedBoxAdr], di

.end:
	ret
endp
      






proc	Chip.KeyMove  uses ax dx
	local const_two = 2

	stdcall Chip.ContactWithObjects, [x_pos], [y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a

.D:
	cmp	ax, KEY_D
	jne	 .S ;check D
	mov	[prev_key], ax 	
	stdcall Chip.Move.Right, speed

.S:
	cmp	ax, KEY_S
	jne	.A ; check S
	cmp	[boxUpped], True
	je	.A
	mov	cl, [shiftState]	
	xor	cl, True
	mov	[shiftState], cl
	;stdcall Chip.Move.Down, speed
.A:
	cmp	ax, KEY_A
	jne	.W  ;Check A
	mov	[prev_key], ax 
	stdcall Chip.Move.Left, speed
.W:
	cmp	ax, KEY_W
	jne	.end  ;check w :
	stdcall Chip.PushBox, boxUp
	
.end:
	
	ret
endp 


; get position of main object, object's pointer
; and the number of objects
proc	Chip.ContactWithObjects uses si di,\
	x,y, h,w,\
	objects, num_of_obj, obj_h, obj_w

	mov	di, [objects]
	mov	cx, [num_of_obj]
	mov	[contactFlag], 0
.checkObj:
	cmp	[uppedBoxAdr], di
	je	.skip
	stdcall Phys.Contact, [x],[y], [h],[w],\
			      [di], [di+2],[obj_h], [obj_w]
.skip:
	add	di, 4
	loop	.checkObj
.end:
	ret
endp  

