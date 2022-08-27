proc Screen.Clear uses es di,\
	bkgColor
        push    $A000
        pop     es

        mov     ax, $0013
        int     10h

        mov     ax, [bkgColor]
        mov     cx, 320*200
        xor     di,di
        rep     stosb
        ret
endp    

proc Screen.SetMode uses si di,\
     wModeInfo

     mov        ah, $0F
     int        10h
     mov        dl, al
     mov        dh, bh

     movzx      ax, byte [wModeInfo]
     int        10h
     mov        ah, $05
     mov        al, byte [wModeInfo + 1]
     int        10h

     mov        ax, dx
     ret
endp

proc    Screen.bkgClear uses es di ax
        push    $A000
        pop     es

        mov     cx, 320*150
        mov	ax, $0035
        xor     di, di
        rep     stosb

	mov	cx, 320*50
	mov	ax, $24
	mov	di, 320*150
	rep	stosb

        ret
endp  