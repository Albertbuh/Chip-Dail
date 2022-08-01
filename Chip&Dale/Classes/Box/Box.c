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
     
proc    Box.CanBeLifted uses ax dx

        mov     [canLift], False
.checkX:
        xor     ax,ax
        xor     dx,dx
        mov     ax,[x_pos]
        add     ax, W
        mov     dx, [box_x]
        sub    ax,dx
        cmp    ax, 0
        jne    .end
.checkY:
        xor     ax,ax
        xor     dx,dx
        mov     ax, [y_pos]
        add     ax, H-1
        mov     dx, [box_y]
        sub     ax,dx
        cmp     ax,0
        jne     .end
        mov    [canLift], True
.end:
        ret
endp 