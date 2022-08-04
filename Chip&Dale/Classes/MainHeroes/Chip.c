proc    Chip.Draw  uses es di,\
	x, y
	
        push    $A000
        pop     es	

        mov     ax, color
        mov     cx, H
        mov 	di, [y]
	imul	di, 320
	add	di, [x]
	
@@:
        push    cx
        mov     cx, W
        rep     stosb

        pop     cx
        add     di, 320-W
        loop    @B


	cmp	[boxUpped], False
	je	.end
	stdcall	Chip.GetBox
	stdcall	Box.Draw, [box_x], [box_y]
	

.end:
        ret
endp 

 proc    Chip.Contact uses ax cx dx,\
        x,y,\
        box_x, box_y

        local sSpeed = physSpeed
        xor     cx,cx
.D:
        mov     ax, [x]
        add     ax, W
        mov     dx, [box_x]
        cmp     ax, dx
        jne     .S

        mov     ax, [y]
        mov     dx, [box_y]
        cmp     ax, dx
        ja      .S
        sub     dx, ax
        cmp     dx, H+box_a-1
        jae    .S

        add     cl, availD
.S:
        mov     ax, [y]
        add     ax, H
        mov     dx, [box_y]
        cmp     ax, dx
        ja      .A
        sub     dx, box_a-1 ; ax <- obj down; dx <- box front
        sub     dx, sSpeed
        cmp     ax, dx
        jb      .A


        mov     ax, [x]
        mov     dx, [box_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax ;x < box_x
        cmp     dx,  W
        jae     .A
        jmp     .notavail
@@:
        sub     ax, dx
        cmp     ax, box_a
        jae      .A

.notavail:
        add     cl, availS
.A:
        mov     ax, [x]
        mov     dx, [box_x]
        add     dx, box_a
        cmp     ax, dx
        jne     .W


        mov     ax, [y]
        mov     dx, [box_y]
        cmp     ax, dx
        ja      .W
        sub     dx, ax
        cmp     dx, H+box_a-1
        jae    .W

        add     cl, availA
.W:
        mov     ax, [y]
        mov     dx, [box_y]
        cmp     ax, dx
        jne     .end

        mov     ax, [x]
        mov     dx, [box_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax
        cmp     dx,  W
        jb      .end
@@:
        sub     ax, dx
        cmp     ax, box_a
        jb      .end

        add     cl, availW
.end:
        mov     [contactFlag], cl
        ret
endp 
 

proc    Chip.GetBox  uses ax dx

        xor          ax,ax
        xor          dx,dx
        mov     ax, [x_pos]
        add     ax, W/4
        mov     dx, [y_pos]
        sub     dx, 1

        mov     [box_x], ax
        mov     [box_y], dx
        mov     [boxUpped], True
.end:
        ret
endp

proc    Chip.PushBox,\
        dir

	cmp	[boxUpped], True
	jne	.end
        and     [boxUpped], False
        stdcall Box.Shoot, [dir]
.end:
        ret
endp 




proc    Chip.KeyMove  uses ax dx

 	stdcall Chip.Contact, [x_pos], [y_pos], [box_x], [box_y]

.D:
        cmp     ax, KEY_D
        jne      .S ;check D
	mov     [prev_key], ax 
        stdcall Chip.Move.Right, speed
.S:
        cmp     ax, KEY_S
        jne     .A ; check S
        stdcall Chip.Move.Down, speed
.A:
        cmp     ax, KEY_A
        jne     .W  ;Check A
	mov     [prev_key], ax 
        stdcall Chip.Move.Left, speed
.W:
        cmp     ax, KEY_W
        jne     .end  ;check w :
        stdcall Chip.PushBox, boxUp
.end:
        ret
endp 
      
