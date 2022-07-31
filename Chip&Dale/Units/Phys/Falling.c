proc    Phys.Falling
        locals
                fallSpeed = physSpeed
                wOldSec dw      ?
        endl
.down:
        mov     ah, $2C
        int     21h
        movzx   dx, dl

        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        add     [y_pos], fallSpeed
        stdcall Screen.bkgClear
        stdcall Chip.Draw, [x_pos], [y_pos]
.dontmove:
        mov     ax, [y_pos]
        add     ax,  H
        cmp     ax, y_floor-fallSpeed
        jb .down
;small distance between ground and legs
        mov     ax, y_floor
        sub     ax, [y_pos]
        sub     ax, H
        cmp     ax, fallSpeed
        ja     .SkipDrop
        mov     [fallSpeed], ax
        add     [y_pos], ax
.SkipDrop:
        ret
endp 