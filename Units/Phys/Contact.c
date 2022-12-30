;with a single obj
proc	Phys.Contact uses eax ecx edx,\
	x,y,weight, height,\
	obj_x, obj_y, obj_w, obj_h

	local sSpeed = physSpeed

	xor   cx, cx
	mov   cl, [contactFlag]
.D:
	mov	eax, [x]
	add	eax, [weight];right bound of hero
	mov	edx, [obj_x];left bound of obj
	cmp	eax, edx
	jb     .S
	add	edx, [obj_w];right bound of obj
	cmp	eax, edx
	ja	.S

	mov	eax, [y]
       ; sub	 eax, [height] ;upper bound of hero
	mov	edx, [obj_y]  ;lower bound of box
	add	edx, [obj_h]
	cmp	eax,edx
	jae	.S
	add	eax, [height] ; lower bound of hero
	sub	edx, [obj_h]; upper bound of box
	cmp	eax, edx
	jbe	.S

	test	cl, availD
	jnz	.S
	add	cl, availD
.S:
	mov eax, [y_floor_dd]
	sub eax, [y]
	cmp eax, [height]
	jl .A

	mov	eax, [y]
	add	eax, [height]
	mov	edx, [obj_y]
	cmp	eax, edx
	jb	.A
	sub	eax, [height]
	add	edx, [obj_h]
	cmp	eax, edx
	ja	.A

	mov	eax, [x]
	add	eax, [weight]  ;right bound of heros
	sub eax, physSpeed+5
	mov	edx, [obj_x] ;left bound of box
	cmp	eax,edx
	jbe	 .A
	sub	eax, [weight] ;left bound of hero
	add	edx, [obj_w]  ;right bound of box
	sub	edx, physSpeed*2
	cmp	eax,edx
	jae	 .A
	jmp	.notavail


.notavail:
	test	cl, availS
	jnz	.A
	add	cl, availS
.A:
	mov	eax, [x]
	mov	edx, [obj_x]
	cmp	eax, edx
	jb	.W
	add	edx, [obj_w]
	cmp	eax, edx
	ja     .W

    mov     eax, [y]
	mov	edx, [obj_y] 
	add	edx, [obj_h] ; lower bound of object
	cmp	eax,edx
	jae	.W
	add	eax, [height] ; lower bound of hero
	sub	edx, [obj_h]; upper bound of object
	cmp	eax, edx
	jbe	.W

	test	cl, availA
	jnz	.W
	add	cl, availA
.W:
     ;	 mov	 ax, [y]
     ;	 mov	 dx, [obj_y]
     ;	 cmp	 ax, dx
     ;	 jne	 .end

     ;	 mov	 ax, [x]
     ;	 mov	 dx, [obj_x]
     ;;   cmp	  ax,dx
     ;	 jae	 @F
     ;	 sub	 dx, ax
     ;;   cmp	  dx,  [weight]
     ;	 jb	 .end
@@:
      ;  sub	 ax, dx
       ; cmp	 ax, [obj_w]
       ; jb	 .end

       ; test	 cl, availW
       ; jnz	 .end
       ; add	 cl, availW
.end:

	mov	[contactFlag], cl
	ret
endp   