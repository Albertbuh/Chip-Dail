proc    Falling
        local   fallSpeed = physSpeed


        mov     dx, [y_pos]
        stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
                                Boxes_coordinates, box_col, box_a+fallSpeed,box_a
        mov     cl, [contactFlag]
        test    cl, availS
        jnz     NotFall

        add     dx, fallSpeed
        stdcall Chip.ContactWithObjects, [x_pos], dx, H,W,\
                                Boxes_coordinates, box_col, box_a, box_a


        stdcall Chip.Move.Down, fallSpeed ; return cx with STOPFALLING value if obj on ground
        cmp     cx, STOPFALLING
       ; jne     .skip
        je       NotFall

        mov     ax, [prev_key]
        stdcall Chip.KeyMove
.skip:

        ret
endp
NotFall:
         mov      [FallingFlag], False
        ret

proc SlowDown
.slowdown:
        mov     cl, [contactFlag]
        cmp     cl, 0
        jne     .slowObj
        mov     ax, y_floor-1
        sub     ax, [y_pos]
        add     [y_pos], ax
        jmp     .end
.slowObj:

        stdcall Chip.ContactWithObjects, [x_pos], [y_pos], H,W,\
                                Boxes_coordinates, box_col, box_a,box_a

        stdcall Chip.Move.Down, 5; return cx with STOPFALLING value if obj on ground
        cmp     cx, STOPFALLING
        jne     .slowObj
.end:
        ret
endp  