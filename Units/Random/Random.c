Random.Initialize:
	invoke	GetTickCount
	mov	[Random.dPrevNumber], eax
	ret

proc	Random.Get uses ecx edx,\
	dMin:DWORD,dMax:DWORD

	
	mov	   eax, [Random.dPrevNumber]
	mov		ecx, 29
	mul		ecx
	add		eax, 47

	mov		[Random.dPrevNumber], eax

	mov		ecx, [dMax]
	mov		edx, [dMin]
	sub		ecx, edx
	inc		ecx
	xor		edx, edx
	div		ecx

	mov		eax, edx
	mov		edx, [dMin]
	add		eax, edx
	ret
endp