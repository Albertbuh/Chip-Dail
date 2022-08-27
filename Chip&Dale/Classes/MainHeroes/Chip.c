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

.draw:
	;push	cx
	;mov	cx, W
	;rep	stosb
	;sub	di, 320+W
	;;pop	cx
	;loop	.draw			 

	
	stdcall bmp_read, [ChipImgAdr], [x_pos], [y_pos]
	cmp	[boxUpped], False
	je	.end
	stdcall Chip.GetBox, [uppedBoxAdr]
	

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
      






proc	Chip.KeyMove  uses ax dx bx
	local const_two = 2

	
	stdcall Chip.ContactWithObjects, [x_pos], [y_pos], H,W,\
				Boxes_coordinates, box_col, box_x,box_y

	
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
	xor	[shiftState], True
	xor	[boxUppedAvail], True
.A:
	cmp	ax, KEY_A
	jne	.end  ;Check A
	mov	[prev_key], ax 
	stdcall Chip.Move.Left, speed

.end:
	stdcall ViewUpdate
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

;return position of first object to pick
proc	Chip.FirstPickContact uses si di,\
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

	mov	dl, [contactFlag]
	cmp	dl, 0
	jne	.end
.skip:
	add	di, 4

	loop	.checkObj

.end:

	ret
endp 



proc	CheckLift  uses di cx
     stdcall Chip.FirstPickContact, [x_pos],[y_pos], H,W,\
				Boxes_coordinates, box_col, box_x,box_y
	mov	[uppedBoxID], box_col
	sub	[uppedBoxID], cl
	; check if we can up box
	xor	cx,cx
	mov	cl, [contactFlag]
	test	cl, availA+availD
	jz     .end

	;find the box we can up
	mov	di, Boxes_coordinates
	mov	cl, [uppedBoxID]
@@:
	add	di, 4
	loop	@B
	stdcall Chip.GetBox, di
	mov	[boxUpped], True
.end:
	ret
endp 
