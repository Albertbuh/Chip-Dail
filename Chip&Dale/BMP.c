    

proc    bmp_read,\
	fileAdr,\
	x,y
        stdcall OpenFile, [fileAdr]

        mov     [fileHandle], ax
        mov     bx, [fileHandle]
        stdcall ReadHeader



        cmp     [biBitCount], 16
        jae     .skip
        stdcall ReadPallete
        stdcall SavePallete
.skip:

        stdcall bmpRead, [x], [y]
	;close file
        mov     ah, $3e
        mov     bx, [fileHandle]
        int     21h
        ret
endp
proc    OpenFile,\
	fileAdr
        mov     ax, $3d00
        mov     dx, [fileAdr]
        int     21H
        jnc      .end
        mov     ah, $09
        mov     dx, Error_msg
        int     21h
        mov     ah, $08
        int     21h
        mov     ah, $4c
        int     21h
.end:
        ret
endp
proc    ReadHeader  uses ax dx cx bx di
        mov     ah, $3f
        mov     bx, [fileHandle]
        mov     cx, 54
        mov     dx, header
        int     21h

        ;save  needed fields
        mov     di, header
        mov     dx, [di+$0012] ;biWidth
        mov     [biWidth], dx
        mov     dx, [di+$0016] ;biHeight
        mov     [biHeight], dx
        mov     dl, [di+$001C] ;biBitCount
        mov     [biBitCount], dl

        ret
endp

proc    ReadPallete   uses ax bx cx dx
        mov   ah, $3f
        mov   cx, 256*4 ;1024
        mov   bx, [fileHandle]
        mov   dx, pallete
        int   21h
        ret
endp

proc    SavePallete   uses si di bx ax cx dx
         mov   si, pallete
        mov   cx, 256
        xor   bx,bx
.palLoop:
        mov   dx, 3c8h
        mov   al, bl
        out     dx, al

        mov     dx, 3c9h
        lodsb
        shr     al,2
        out     dx, al
        lodsb
        shr     al, 2
        out     dx, al
        lodsb
        shr     al, 2
        out     dx, al
        inc     bl
        loop    .palLoop
        ret
endp

proc    DateSizeCount uses ax dx
        ;DateSize counting
        mov     ax, [biWidth]
        mul     [biBitCount]
        add     ax, 31
        div     [const_8]
        movzx   ax, al
        div     [const_4]
        movzx   ax, al
        mul     [const_4]
        movzx   dx, al

        mov     [DateSize], dx
        ret
endp
proc    bmpRead  uses ax bx cx dx di si,\
	x, y
	
        Call  DateSizeCount
        mov      cx, [biHeight]
        mov	di, [y]
	imul	di, 320
	add	di, [x]
        stdcall  bmpDrawCycle
        ret
endp

proc    ReadLine32 uses ax cx dx
        mov     ah, 3fh
        mov     cx, [DateSize]
        mov     dx, scrLine
        int     21h
        ret
endp

proc    bmpDrawCycle uses cx ax dx si di bx

        mov     bx, [fileHandle]

.loop:
        ;Reading new image line from file
         stdcall ReadLine32
        ;si <- bmp data, cx <- width of img
        stdcall    DrawLine32
        stdcall ClearExtraP
        Call    NewLine
        loop    .loop

        ret
endp

proc    DrawLine32   uses si cx
        mov     si, scrLine
        mov     cx, [biWidth]
       ; dec     cx
.newP:
        stdcall ReadColor32

        movsd  ;read pixel 32 bits
        sub     di, 3
.end:
        loop    .newP
        ret
endp

proc    ReadColor32
        ;write num of clr in 3c8H
        mov     dx,  3c8h
        mov     al, [si]
        out     dx, al
        ;write in 3c9H clr proportions
        mov     dx, 3c9h
        add     si, 2
        ;DF=1
        std
        lodsb
        shr     al, 2
        out     dx, al   ;Red
        lodsb
        shr     al, 2
        out     dx, al   ;Green
        lodsb
        shr     al, 2
        out     dx, al   ;Blue
        inc    si
        cld ;DF=0
        ret
endp



NewLine:
        push bp
        mov  bp, sp

        sub  di, 320
        xor  dx,dx
        mov  dx, [biWidth]
        sub  di, dx
        pop     bp
        ret

proc    ClearExtraP uses di cx ax
        mov         al, bkgColor
        mov         cx, 3
        rep         stosb
        ret
endp

;fileAdr db      "C:Chip2.bmp",0
;fileHandle      dw ?
;header          dw 54 dup(0)
;pallete         dw 256*4  dup(0)
;scrLine         dw 320 dup(0)


;biBitCount       db     ?
;biWidth          dw     ?
;biHeight         dw     ?
;DateSize         dw     ?

;Error_msg   db      "Error, probllems with open file$"