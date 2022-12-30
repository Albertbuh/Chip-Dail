proc	ChipMoveTimer
	cmp	[pict_x], 120
	ja	@f
	add	[pict_x], pictW
	jmp	.end
@@:
	mov	[pict_x], 0
.end:
	ret
endp


proc	SetChipGoAnimation
	cmp [boxUpped], True
	jne		@f
	mov		[ChipState], ChipBoxGo
	jmp   .finish
@@:
	mov    [ChipState], ChipGo
.finish:
	ret
endp


proc	SetChipStayAnimation
	cmp [boxUpped], True
	jne		@f
	mov		[ChipState], ChipBoxStay
	jmp   .finish
@@:	
	mov		[ChipState], ChipStay

.finish:
	ret
endp
proc	CheckChipJumpAnimation
	cmp    [OnGround], True
	je	   .finish
	mov    [ChipState], ChipJump
.finish:
	ret
endp 

proc	CheckChipBoxPushAnimation  uses eax edx edi

	cmp	[BoxFly], True
	jne	.finish
	cmp	[boxDir], 'W'
	je	.YCHECK
;CHECK X FLY
	mov	eax, [x_pos]
	mov	edi, [uppedBoxAdr]
	mov	edx, [edi]

	cmp	eax, edx
	jb	@f
	;x_pos > flybox X
	xchg   eax, edx
@@:
	;x_pos < flybox X
	sub	edx, eax
	cmp	edx, 150
	ja	.finish
	mov	[ChipState], ChipBoxPush
	jmp	.finish
;Y FLY
.YCHECK:
	mov	eax, [y_pos]
	mov	edi, [uppedBoxAdr]
	mov	edx, [edi+4]
	;y_pos > flybox Y
	sub	eax, edx
	cmp	eax, 200
	ja	.finish
	mov	[ChipState], ChipBoxPush
.finish:
	ret
endp 

proc   DogAnimTimer

	cmp	[Enemy_pictX], 0
	ja	@f
	mov	eax, pictEnemy_W
	add	[Enemy_pictX], eax
	jmp	.end
@@:
	mov	[Enemy_pictX], 0
.end:
	ret
endp


proc   FlyAnimTimer

	cmp	[FlyEnemy_pictX], pictFly_W *2
	ja	@f
	add	[FlyEnemy_pictX], pictFly_W
	jmp	.end
@@:
	mov	[FlyEnemy_pictX], 0
.end:
	ret
endp



proc	BossAttackPhaseTimer uses edi eax		

	stdcall Cheese.setImg
	mov	[CheeseFlag], True
	mov	eax, [hbmpBossAtt]
	mov	[BossImg], eax

	
	ret
endp