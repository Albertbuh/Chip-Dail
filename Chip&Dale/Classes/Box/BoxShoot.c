proc    Box.Shoot,\
        dir ; 0-up, 1-forward, 2-back
        cmp [dir], boxUp
        jne @F
        stdcall Box.ShootUp
@@:
        cmp [dir], boxForward
        jne @F
        stdcall Box.ShootForward
@@:
        cmp [dir], boxBack
        jne .end
        stdcall Box.ShootBack
.end:
        ret
endp
proc    Box.ShootForward
        locals
                shootSpeed = 15
                wOldSec    dw ?
        endl
.shoot:
        mov     ah, $2c
        int     21h
        movzx     dx, dl
        cmp     [wOldSec], dx
        je      .dontmove
        mov     [wOldSec], dx

        add     [box_x], shootSpeed

        stdcall  Screen.bkgClear
        stdcall  Chip.Draw, [x_pos], [y_pos]
        stdcall  Box.Draw, [box_x], [box_y]
.dontmove:

        cmp     [box_x],  right_wall
        jb      .shoot

.end:
        ret
endp

proc    Box.ShootUp
        locals
                shootSpeed = 15
                wOldSec    dw ?
        endl
.shoot:
        mov     ah, $2c
        int     21h
        movzx     dx, dl
        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        sub     [box_y], shootSpeed
        stdcall  Screen.bkgClear
        stdcall  Chip.Draw, [x_pos], [y_pos]
        stdcall  Box.Draw, [box_x], [box_y]
.dontmove:
        cmp     [box_y],  seil
        ja      .shoot
.end:
        ret
endp

proc    Box.ShootBack
        locals
                shootSpeed = 15
                wOldSec    dw ?
        endl
.shoot:
        mov     ah, $2c
        int     21h
        movzx     dx, dl
        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        cmp     [box_x], shootSpeed
        jb      @F
        sub     [box_x], shootSpeed
        jmp     .draw
@@:
        sub     [box_x], 5 ; min count

.draw:
        stdcall  Screen.bkgClear
        stdcall  Chip.Draw, [x_pos], [y_pos]
        stdcall  Box.Draw, [box_x], [box_y]
.dontmove:
        cmp     [box_x],  left_wall
        jae      .shoot
.end:
        ret
endp 