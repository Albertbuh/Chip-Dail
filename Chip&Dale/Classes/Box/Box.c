proc    Box.Create uses di cx dx,\  ;drawing all boxes
        boxes

        mov     cx, box_col
        mov     di, [boxes]
@@:
	mov	dx, [di]
	cmp	dx, 0
	je	.skip
        stdcall Box.Draw, [di], [di+2]
.skip:
        add     di, 4
        loop    @B


        ret
endp   

proc    Box.Draw uses es di ax cx dx,\   ;drawing a single box 
        x,y

        
	
	stdcall	bmp_read, BoxImg, [x], [y]
.end:
        ret
endp    
  
proc	drawrect,\
	x, y
	mov     ax, box_color
        mov     cx, box_y
        mov     di, [y]
        imul    di, 320
        add     di, [x]
.draw:
        push    cx
        mov     cx, box_x
        rep     stosb
        pop     cx
        sub     di, 320+box_x
        loop    .draw
	ret 
endp
   
proc    Box.CanBeLifted

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