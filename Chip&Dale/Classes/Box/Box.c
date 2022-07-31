proc    Box.Draw uses es di,\
        x,y


        push    $A000
        pop     es

        mov     ax, box_color
        mov     cx, box_a
        mov     di, [y]
        imul    di, 320
        add     di, [x]

@@:
        push    cx
        mov     cx, box_a
        rep     stosb

        pop     cx
        sub	di, 320+box_a
        loop    @B
        ret
endp
     