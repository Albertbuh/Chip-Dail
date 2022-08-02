proc    Phys.Falling
        locals
                fallSpeed = 5+physSpeed
                wOldSec dw      ?
		RepSec	db	3
        endl
.down:
        mov     ah, $2C
        int     21h
        movzx   ax, dl
	div	[RepSec]
	movzx	dx, al

        cmp     [wOldSec], dx
        je      .dontmove

        mov     [wOldSec], dx
        add     [y_pos], fallSpeed
        stdcall Screen.bkgClear
        stdcall Chip.Draw, [x_pos], [y_pos]
	stdcall	Box.Draw, [box_x], [box_y]
	stdcall	FlyMove
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
        add     [y_pos], ax
.SkipDrop:
        ret
endp 