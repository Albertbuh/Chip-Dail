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

        ret
endp 

proc    Chip.Move  uses ax dx

.D:
        cmp     ax, KEY_D
        jne      .S ;check D
        mov      dx, [x_pos]
        add      dx, W
        cmp      dx, right_wall
        jae      .S  ;check rWall
        add     [x_pos], speed
.S:
        cmp     ax, KEY_S
        jne     .A  ; check S
        mov     dx, [y_pos]
        add     dx, H
        cmp     dx, y_floor
        ja      .A    ; check floor

        add     [y_pos], speed
.A:
        cmp     ax, KEY_A
        jne     .W  ;Check A
        mov     dx, [x_pos]
        cmp     dx, left_wall
        jbe     .W ; check left_wall
        sub     [x_pos], speed
.W:
        cmp     ax, KEY_W
        jne     .end  ;check w
        mov     dx, [y_pos]
        cmp     dx, seil
        jbe     .end  ; check seil touching
        sub     [y_pos], speed
.end:
        ret
endp 