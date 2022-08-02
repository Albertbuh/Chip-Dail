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

 proc    Chip.Contact uses ax cx dx,\
        x,y, box_x, box_y

        xor     cx,cx
.availD:
        mov     ax, [x]
        add     ax, W
        mov     dx, [box_x]
        cmp     ax, dx
        jne     .availS

        mov     ax, [y]
        mov     dx, [box_y]
        cmp     ax, dx
        ja      .availS
        sub     dx, ax
        cmp     dx, H+box_a-1
        jae    .availS

        add     cl, 0000_0001b
.availS:
        mov     ax, [y]
        add     ax, H
        mov     dx, [box_y]
        add     dx, box_a
        cmp     ax, dx
        jne     .availA

        mov     ax, [x]
        mov     dx, [box_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax
        cmp     dx,  W
        jb      .availA
@@:
        sub     ax, dx
        cmp     ax, box_a
        jb      .availA

        add     cl, 0000_0010b
.availA:
        mov     ax, [x]
        mov     dx, [box_x]
        add     dx, box_a
        cmp     ax, dx
        jne     .availW


        mov     ax, [y]
        mov     dx, [box_y]
        cmp     ax, dx
        ja      .availW
        sub     dx, ax
        cmp     dx, H+box_a-1
        jae    .availW

        add     cl, 0000_0100b
.availW:
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

        add     cl, 0000_1000b
.end:
        mov     [contactFlag], cl
        ret
endp   


proc    Chip.Move  uses ax dx

        xor     cx,cx
        mov     cl, [contactFlag]
.D:
        cmp     ax, KEY_D
        jne      .S ;check D
        stdcall Chip.Move.Right, speed
.S:
        cmp     ax, KEY_S
        jne     .A ; check S
        stdcall Chip.Move.Down
.A:
        cmp     ax, KEY_A
        jne     .W  ;Check A
        stdcall Chip.Move.Left, speed
.W:
        cmp     ax, KEY_W
        jne     .end  ;check w :
        stdcall Chip.PushBox, boxUp
.end:
        ret
endp 
      
