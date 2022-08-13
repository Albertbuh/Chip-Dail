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

	cmp	[uppedBox], False
	je	.end
	stdcall Chip.GetBox, [uppedBoxPos]
	

.end:

	ret
endp 

 
;boxes <- coordinates of a single box on the field
proc    Chip.GetBox  uses si di ax dx,\
        boxes
        ; in shift state you can't take box
        mov     cl, [shiftState]
        cmp     cl, True
        je      .end

        ;set pos
        xor          ax,ax
        xor          dx,dx
        mov     ax, [x_pos]
        add     ax, W/4
        mov     dx, [y_pos]
        sub     dx, H
        mov     di, [boxes]
        mov     [di], ax
        mov     [di+2], dx

        ;save adress of upped box
        mov     [uppedBoxPos], di

.end:
        ret
endp
      


proc	Chip.PushBox,\
	dir

	cmp	[boxUpped], True
	jne	.end
	and	[boxUpped], False
	stdcall Box.Shoot, [dir]
.end:
	ret
endp 




proc	Chip.KeyMove  uses ax dx
	local const_two = 2

	stdcall	Chip.ContactWithObjects, [x_pos],[y_pos], H,W,\
				Boxes_coordinates, box_col, box_a,box_a

.D:
	cmp	ax, KEY_D
	jne	 .S ;check D
	mov	[prev_key], ax 
	stdcall Chip.Move.Right, speed
.S:
	cmp	ax, KEY_S
	jne	.A ; check S
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
	;stdcall Chip.PushBox, boxUp
.end:
	ret
endp 


;with a single obj
proc    Phys.Contact uses ax cx dx,\
        x,y,height, weight,\
        obj_x, obj_y, obj_h, obj_w

        local sSpeed = physSpeed
        xor   cx, cx
.D:
        mov     ax, [x]
        add     ax, [weight]
        mov     dx, [obj_x]
        cmp     ax, dx
        jne     .S


        mov     ax, [y]
        sub     ax, [height] ;upper bound of hero
        mov     dx, [obj_y]
        cmp     ax, dx
        ja      .S
        add     ax, H ; lower bound of hero
        sub     dx, [obj_h]; upper bound of box
        cmp     ax, dx
        jb     .S

        add     cl, availD
.S:
        mov     ax, [y]
        add     ax, [height]
        mov     dx, [obj_y]
        cmp     ax, dx
        ja      .A
        sub     dx, [obj_h]  ; ax <- obj down; dx <- box front
        inc     dx
        sub     dx, sSpeed
        cmp     ax, dx
        jb      .A


        mov     ax, [x]
        mov     dx, [obj_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax ;x < box_x
        cmp     dx,  [weight]
        jae     .A
        jmp     .notavail
@@:
        sub     ax, dx
        cmp     ax, [obj_w]
        jae      .A

.notavail:
        add     cl, availS
.A:
        mov     ax, [x]
        mov     dx, [obj_x]
        add     dx, [obj_w]
        cmp     ax, dx
        jne     .W


        mov     ax, [y]
        sub     ax, [height] ;upper bound of hero
        mov     dx, [obj_y]
        cmp     ax, dx
        ja      .S
        add     ax, [height] ; lower bound of hero
        sub     dx, [obj_h]; upper bound of box
        cmp     ax, dx
        jb     .S

        add     cl, availA
.W:
        mov     ax, [y]
        mov     dx, [obj_y]
        cmp     ax, dx
        jne     .end

        mov     ax, [x]
        mov     dx, [obj_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax
        cmp     dx,  [weight]
        jb      .end
@@:
        sub     ax, dx
        cmp     ax, [obj_w]
        jb      .end

       ; add     cl, availW
.end:
        mov     [contactFlag], cl
        ret
endp
      
