;with a single obj
proc    Phys.Contact uses ax cx dx,\
        x,y,weight, height,\
        obj_x, obj_y, obj_w, obj_h

        local sSpeed = physSpeed

        xor   cx, cx
        mov   cl, [contactFlag]
.D:
        mov     ax, [x]
        add     ax, [weight]
        mov     dx, [obj_x]
        cmp     ax, dx
        jb     .S
        add     dx, [obj_w]
        cmp     ax, dx
        ja      .S

        mov     ax, [y]
        sub     ax, [height] ;upper bound of hero
        mov     dx, [obj_y]
        cmp     ax, dx
        jae     .S
        add     ax, H ; lower bound of hero
        sub     dx, [obj_h]; upper bound of box
        cmp     ax, dx
        jbe     .S

        test    cl, availD
        jnz     .S
        add     cl, availD
.S:
        mov     ax, [y]
        mov     dx, [obj_y]
        cmp     ax, dx
        ja      .A
        sub     dx, [obj_h]
        cmp     ax, dx
        jb      .A
        ;sub    dx, [obj_h]  ; ax <- obj down; dx <- box front
        ;inc    dx
        ;sub    dx, sSpeed
        ;cmp    ax, dx
        ;jb     .A


        mov     ax, [x]
        mov     dx, [obj_x]
        cmp     ax,dx
        jae     @F
        sub     dx, ax ;x < box_x
        cmp     dx,  [weight]
        jae     .A
        jmp     .notavail
@@:
        sub     ax, dx
        cmp     ax, [obj_w]
        jae      .A

.notavail:
        test    cl, availS
        jnz     .A
        add     cl, availS
.A:
        mov     ax, [x]
        mov     dx, [obj_x]
        cmp     ax, dx
        jb      .W
        add     dx, [obj_w]
        cmp     ax, dx
        ja     .W



        mov     ax, [y]
        sub     ax, [height] ;upper bound of hero
        mov     dx, [obj_y]
        cmp     ax, dx
        jae     .W
        add     ax, [height] ; lower bound of hero
        sub     dx, [obj_h]; upper bound of box
        cmp     ax, dx
        jbe    .W

        test    cl, availA
        jnz     .W
        add     cl, availA
.W:
     ;   mov     ax, [y]
     ;   mov     dx, [obj_y]
     ;   cmp     ax, dx
     ;   jne     .end

     ;   mov     ax, [x]
     ;   mov     dx, [obj_x]
     ;;   cmp     ax,dx
     ;   jae     @F
     ;   sub     dx, ax
     ;;   cmp     dx,  [weight]
     ;   jb      .end
@@:
      ;  sub     ax, dx
       ; cmp     ax, [obj_w]
       ; jb      .end

       ; test    cl, availW
       ; jnz     .end
       ; add     cl, availW
.end:

        mov     [contactFlag], cl
        ret
endp   