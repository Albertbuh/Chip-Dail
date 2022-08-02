proc    Chip.Move.Left uses ax dx,\
        speed

        mov     ax, [x_pos]

        test    cl, 0000_0100b
        jnz     .end
        mov     dx, ax
        cmp     dx, left_wall
        jbe     .end ; check left_wall
        sub     ax,  [speed]
        mov     [x_pos], ax

.end:
        ret
endp

proc    Chip.Move.Right uses ax dx,\
        speed
        mov     ax, [x_pos]

        test    cl, 0000_0001b
        jnz     .end
        mov      dx, ax
        add      dx, W
        cmp      dx, right_wall
        jae      .end  ;check rWall
        add     ax, [speed]
        mov     [x_pos], ax
.end:
        ret
endp

proc    Chip.Move.Down

        mov     dx, [y_pos]
        add     dx, H
        cmp     dx, y_floor
        jae     .end    ; check floor

        add     [y_pos], speed
.end:
        ret
endp    