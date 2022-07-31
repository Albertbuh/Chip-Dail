proc    Phys.Jump uses ax dx
        locals
                jpSpeed = physSpeed
                jpLim   = 60  ; Limit of jump
                jpLimY  dw ?
                wOldSec dw ?
        endl
        mov     ax, [y_pos]
        sub     ax, jpLim
        mov     [jpLimY], ax   ; jpLimY <- obj pos + Limit of jump

.moveup:
        mov     ah, $2C
        int     21h
        movzx   dx, dl

        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        sub     [y_pos], jpSpeed
        jbe     .end
        stdcall Screen.bkgClear
        stdcall Chip.Draw, [x_pos], [y_pos]
.dontmove:

        mov     ax, [jpLimY]
        cmp     [y_pos], ax
        jae     .moveup

.end:
        ret
endp 