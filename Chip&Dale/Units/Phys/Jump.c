
proc    Jump
        locals
                jpSpeed = physSpeed
                jpLim   = 60  ; Limit of jump

        endl
        cmp     [JumpFlag], True
        jne     .end
        cmp     [jpLimY], 0
        jne     .skipLimCount
        mov     ax, [y_pos]
        sub     ax, jpLim
        mov     [jpLimY], ax  ; jpLimY <- obj pos + Limit of jump

.skipLimCount:
        sub     [y_pos], jpSpeed
        jbe     .end
        mov     ax, [prev_key]
        stdcall Chip.KeyMove
.skip:
        mov     ax, [jpLimY]
        cmp     [y_pos], ax
        jae     .end
        mov     [JumpFlag], False
        and     [jpLimY], 0
        mov     [FallingFlag], True

.end:
        ret
endp    