proc    Keyboard.KeyPressInt21
        mov     ax, $0C08
        int     21h
        movzx   cx, al
        test    al,al
        jnz     @F
        mov     ah, $08
        int     21h
        mov     ch, al
@@:
        mov     ax,cx
        ret
endp

proc	Keyboard.KeyPressInt16
	xor	ax,ax
	int	16h
	xor	al,al
	movzx	ax, ah
	ret
endp   