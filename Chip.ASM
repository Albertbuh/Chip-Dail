;CHIP&DALE2 (NES) WINAPI REALIZATION
format PE GUI 4.0
entry start

include 'win32a.inc'


True = 1
False = 0
CART=-100 ;y_floor
QUIT = $7023
TIMERSPEED=25
ms=1000

MAINTIMER=0
BOSSTIMER=1
CHIPTIMER=3
ENEMYTIMER=7
FLYENEMYTIMER=8
FLYBALLTIMER=9
CHIP_HIT_AVAIL_TIMER=15
COOLDOWNTIMER=24
COOLDOWNTIMER2=25

  include 'classes\MainHeroes\Chip.h'
  include 'units\Keyboard\Keyboard.h'
  include 'units\Screen\Screen.h'
  include 'units\Phys\Phys.h'
  include 'classes\Box\Box.h'
  include 'classes\Enemy\Enemy.h'
  include 'classes\Boss\Boss.h'
  include 'classes\Boss\Cheese.h'
  include 'classes\Fly\Fly.h'



section '.code' code readable executable

  start:


	invoke	GetModuleHandle,0
	mov	[wc.hInstance],eax
	invoke	LoadIcon,0,ICONS
	mov	[wc.hIcon],eax

	invoke	LoadCursor,0,0
	mov	[wc.hCursor],eax
	invoke	ShowCursor, 0

	invoke	RegisterClass,wc
	test	eax,eax
	jz	error


	invoke	GetSystemMetrics, SM_CYFULLSCREEN
	mov	[SCREEN_HEIGHT], eax
	stdcall Screen.setFloor, eax
	mov	[y_floor_dd], eax

	invoke	GetSystemMetrics, SM_CXFULLSCREEN  ;SM_CXMAXIMIZED
	mov	[SCREEN_WIDTH], eax
	stdcall Screen.setRightWall, [SCREEN_WIDTH]
	mov	[right_wall_dd], eax

	stdcall Screen.setZoom

	stdcall Screen.countHeightInBoxes
	mov	[HeightInBoxes], eax

	;boxXY	base-box-pescent left_col down_col right_col

	invoke SetRect, rcClient, 0,0,[SCREEN_WIDTH],[SCREEN_HEIGHT]
	invoke AdjustWindowRect, rcClient,WS_VISIBLE+WS_OVERLAPPEDWINDOW, NULL
	xor eax, eax
	sub eax, [rcClient.left]
	xor edx, edx
	sub edx, [rcClient.top]
	invoke OffsetRect, rcClient, 0, edx ;-rc.left, -rc.top


	invoke	CreateWindowEx, 0, _class,_title, WS_VISIBLE+WS_SYSMENU,\
			       [rcClient.left],[rcClient.top],[rcClient.right],[rcClient.bottom],\
				NULL,NULL,[wc.hInstance],NULL
	test	eax,eax
	jz	error



	add	[SCREEN_WIDTH], 30
	mov eax, [SCREEN_WIDTH]
	invoke	SetRect, CLEARRECT, 0,20,eax, [y_floor_dd]
	mov eax, [SCREEN_WIDTH]
	sub eax, 200
	invoke	SetRect, pointsRect, eax,35,[SCREEN_WIDTH],200


  msg_loop:
	invoke	GetMessage,msg,NULL,0,0
	cmp	eax, 1
	jb	end_loop
	jne	msg_loop
	invoke	TranslateMessage,msg
	invoke	DispatchMessage,msg
	jmp	msg_loop

  error:
	invoke	MessageBox,NULL,_error,NULL,MB_ICONERROR+MB_OK

  end_loop:
	invoke	ExitProcess,[msg.wParam]


proc	DestroyWin
	invoke	DeleteDC, [hMemDC]
	invoke	DeleteObject, [hBitmap]
	invoke	DeleteObject, [hbmpBox]
	invoke	DeleteObject, [hbmpBoxDestroy]
	invoke	DeleteObject, [hbmpBoxDestroyMask]
	invoke	DeleteObject, [hbmpBKG]
	invoke	DeleteObject, [hbmpHP]
	invoke	DeleteObject, [hbmpCheese]
	invoke	DeleteObject, [hbmpCheeseMask]
	invoke	DeleteObject, [hbmpBoss]
	invoke	DeleteObject, [hbmpBossMask]
	invoke	DeleteObject, [hbmpBossAtt]
	invoke	DeleteObject, [hbmpBossHeal]
	invoke	DeleteObject, [hbmpEnemy]
	invoke	DeleteObject, [hbmpEnemyMask]
	invoke	DeleteObject, [hbmpFlyEnemy]
	invoke	DeleteObject, [hbmpFlyBall]
	invoke	DeleteObject, [hbmpFlyBallMask]
	invoke	DeleteObject, [hbmpRedCheese]
	invoke	DeleteObject, [hbmpBlackCheese]
	invoke	DeleteObject, [hbmpMenu]
	invoke	DeleteObject, [hbmpPoints]
	invoke	DeleteObject, [hbmpFinal]
	invoke	PostQuitMessage,0
	xor	eax,eax
	ret
endp

;=======  WINDOWPROC =======================
proc WindowProc uses ebx esi edi,\
		 hwnd,wmsg,wparam,lparam
		 locals
		       const2 dd 2
		       temp   dd 0
		 endl
	mov	 eax, [wmsg]
	cmp	 eax, WM_CREATE
	je	 .wmcreate
	cmp	 eax, WM_PAINT
	je	 .wmpaint
	cmp	 eax, WM_DESTROY
	je	 DestroyWin
	cmp	 eax, WM_KEYDOWN
	je	 .wm_keydown
	cmp	 eax, WM_SYSCOMMAND
	je	 .wm_syscommand
	;WM_DESTROY HAS BEEN LOCATED IN MAIN(0) TIMER
  .defwndproc:
	invoke	DefWindowProc,[hwnd],[wmsg],[wparam],[lparam]
	jmp	.finish

;===WM_CREATE===
  .wmcreate:

	 mov eax, [hwnd]
	 mov [global_hwnd], eax
;SET RANDOM
	stdcall Random.Initialize
;GET DC INFORMATION AND CREATE COMPATIBLE DC
	invoke	GetDC, [hwnd]
	mov	[hdc], eax
	invoke	CreateCompatibleDC, [hdc]
	mov	[hMemDC], eax
	invoke	CreateCompatibleDC, [hdc]
	mov	[hBkgDC], eax
;LOAD IMAGES
	push	ebx
	mov	ebx, [wc.hInstance]

	invoke LoadBitmap, ebx, Points
	mov    [hbmpPoints],eax
	;LOAD BOX IMAGES
	invoke	LoadBitmap, ebx, pictBox
	mov	[hbmpBox], eax
	invoke	LoadBitmap, ebx, pictBoxDestroy
	mov	[hbmpBoxDestroy], eax
	invoke	LoadBitmap, ebx, pictBoxDestroyMask
	mov	[hbmpBoxDestroyMask], eax
	;LOAD BKG IMAGES
	invoke	LoadBitmap, ebx, pictBKG
	mov	[hbmpBKG], eax
	;LOAD HP IMAGES
	invoke	LoadBitmap, ebx, HitPoints
	mov	[hbmpHP], eax
	;LOAD ENEMY IMAGES
	invoke	LoadBitmap, ebx, Dog
	mov	[hbmpEnemy], eax
	invoke	LoadBitmap, ebx, DogMask
	mov	[hbmpEnemyMask], eax
	invoke	LoadBitmap, ebx, Fly
	mov	[hbmpFlyEnemy], eax
	invoke	LoadBitmap, ebx, FlyBall
	mov	[hbmpFlyBall], eax
	invoke	LoadBitmap, ebx, FlyBallMask
	mov	[hbmpFlyBallMask], eax
	;BOSS IMAGES
	cmp   [right_wall_dd], 1790
	jbe   .smallmode
	invoke	LoadBitmap, ebx, Boss
	mov	[hbmpBoss], eax
	mov	[BossImg], eax ;SAVE BOSS PICT CURRENTLY
	invoke	LoadBitmap, ebx, BossAtt
	mov	[hbmpBossAtt], eax

	mov	[BossW_dd], 900
	mov	[BossH_dd], 650

	 ;BOSS CHEESE IMAGES
	invoke	LoadBitmap, ebx, BRedCheese
	mov  [hbmpRedCheese], eax
	invoke LoadBitmap, ebx, BBlackCheese
	mov [hbmpBlackCheese], eax
	invoke	 LoadBitmap, ebx, BossCheese
	mov	[hbmpCheese], eax
	invoke	LoadBitmap, ebx, BCheeseMask
	mov	[hbmpCheeseMask], eax
	mov	[CheeseW], 112
	mov	[CheeseH], 75
	jmp	@f
.smallmode:
	invoke	LoadBitmap, ebx, SmallBoss
	mov	[hbmpBoss], eax
	mov	[BossImg], eax ;SAVE BOSS PICT CURRENTLY
	invoke	LoadBitmap, ebx, SmallBossAtt
	mov	[hbmpBossAtt], eax

	mov	[BossW_dd], 450
	mov	[BossH_dd], 325

	 ;BOSS CHEESE IMAGES
	invoke	LoadBitmap, ebx, BSmallRedCheese
	mov  [hbmpRedCheese], eax
	invoke LoadBitmap, ebx, BSmallBlackCheese
	mov [hbmpBlackCheese], eax
	invoke	 LoadBitmap, ebx, BossSmallCheese
	mov	[hbmpCheese], eax
	invoke	LoadBitmap, ebx, BSmallCheeseMask
	mov	[hbmpCheeseMask], eax
	mov	[CheeseW], 75
	mov	[CheeseH], 50
@@:

	;MENU IMAGES
	invoke LoadBitmap, [wc.hInstance], Menu
	mov [hbmpMenu], eax
	invoke LoadBitmap, [wc.hInstance], Final
	mov [hbmpFinal], eax
	pop   ebx
;SET TIMERS
	invoke	SetTimer, [hwnd],MAINTIMER,TIMERSPEED,TimerProc
	invoke	SetTimer, [hwnd],CHIPTIMER, ms/5, ChipMoveTimer

	invoke	mciSendString, bkg_music,0,0,0

	invoke	CreateFont, 20,20,10,10,700,FALSE,FALSE,FALSE,ANSI_CHARSET,\
			    OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,\
			    DEFAULT_PITCH ,NULL
	mov		    [hfont], eax
	jmp	.finish

;===WM_PAINT===
.wmpaint:
	invoke BeginPaint, [hwnd],ps
	mov    [hdc], eax
	invoke	SaveDC, [hdc]

	stdcall Painting

	invoke	RestoreDC, [hdc], -1
	invoke EndPaint, [hwnd], ps
	jmp    .finish

;PRESSS ENTER TO START GAME
.wm_keydown:
.enter:
	cmp	[final_scene], True
	jne	@f
	cmp	[transition_flag], False
	jne	@f
	stdcall DestroyWin
@@:
	cmp	 [wparam], VK_ESCAPE
	jne	 @f
	stdcall  DestroyWin
@@:
	cmp	 [wparam], VK_RETURN
	jne	 .finish
	mov	 eax, 0
	cmp	 [menu_scene], True
	je	 .menuflag
	cmp	  [text_flag], True
	je	  .finish
	mov	  [text_flag], True
	jmp	  .finish
.menuflag:
	mov	[menu_scene], False
	mov	[first_scene], True
	stdcall Box.clUppedBox
	stdcall FirstScenePrepare
	invoke	InvalidateRect, [global_hwnd], 0, True
	jmp	.finish
;ALT OFF
.wm_syscommand:
	mov	eax, 0xFFF0
	mul	[wparam]
	cmp  eax,SC_KEYMENU
	jne  .finish
	mov  eax, 0
	jmp .finish
  .finish:
	ret
endp


;============ Painting =================
proc	Painting

	cmp	[menu_scene], True
	jne	.draw
	stdcall drawMenuScene
	jmp	.finishdraw


.draw:

@@:
     cmp  [final_scene], True
     jne  @f

     stdcall	drawFinalScene
     stdcall	SqTransition, 0, [SCREEN_WIDTH], physSpeed*3/2
     jmp   .finishdraw
@@:
      cmp	[transition_flag], True
      jne	@f
      stdcall	SqTransition,0,[y_floor_dd], physSpeed*3


@@:
      cmp [boss_scene], True
      jne  @f
      stdcall	drawBossScene
      jmp	.bkgdraw


@@:
      cmp [second_scene], True
      jne @f
      stdcall	drawSecondScene
      jmp	.bkgdraw

@@:
      stdcall drawFirstScene
      jmp     .bkgdraw
;DRAW BKG ON START
.bkgdraw:
	;bkg draw
       cmp    [bkgPainted], True
       je     .finishdraw
       stdcall Screen.DrawBKG

.finishdraw:

.end:
	ret
endp

;transition between scenes
proc	SqTransition uses eax,\
		y_top, y_bottom, speed


	mov	eax, [y_top]
	mov	[transition_rect.top],eax
	mov	eax, [y_bottom]
	mov	[transition_rect.bottom], eax


	mov	eax, [speed]
	add	[transition_rect.right], eax
	mov	eax, [right_wall_dd]
	add	eax, 300
	cmp	[transition_rect.right], eax
	jl	.end
	mov	[transition_flag], False
.end:
	 ret
endp

;============= END PAINTING =======================


;===========	TIMER PROC	=====================================
proc	TimerProc uses ebx esi edi,\
		  hwnd,wmsg,wparam,lparam

	cmp	[menu_scene], True
	je	.finish
	cmp	[transition_flag], True
	je	.skip
	cmp	[final_scene], True
	je	.finish
;Boss ACTION
	cmp	[boss_scene], True
	jne	@f
	cmp	[BossHP], 0
	jbe	   .finish_game
	stdcall BossModel, [hwnd]
	jmp	.continue
.finish_game:
	cmp	[cutsceneRect.top], seil-40
	jge	.continue
	cmp	[final_scene], True
	je	.continue
	mov	[transition_flag], True
	invoke	SetRect, transition_rect, 0,0,physSpeed*3 ,1
	mov	eax, [SCREEN_WIDTH]
	mov	[CLEARRECT.bottom], eax
	mov	[final_scene], True
	jmp	.skip

.continue:
	cmp	[text_flag], True
	mov	eax, ChipStay
	mov	[ChipState], eax
	jne	.skip
	jmp	.physChip
@@:
;ENEMY ACTION
       cmp	[EnemyXY+4], seil
       jle	@f
       stdcall		EnemyModel
       jmp		.flyenemyaction
@@:
       cmp	[EnemyAlive], True
       je	.flyenemyaction
       mov	[EnemyAlive], True
       invoke SetTimer, [hwnd], COOLDOWNTIMER, ms*5/2, Enemy.setKD

.flyenemyaction:
	cmp	[second_scene], True
	jne	.physChip

	mov	 eax, [right_wall_dd]
	add	 eax, 300
	cmp	[FlyEnemyXY], eax
	jge	@f
	stdcall FlyEnemyModel
	jmp	.physChip
@@:
       cmp	[FlyEnemyAlive], True
       je	.physChip
       mov	[FlyEnemyAlive], True
       invoke SetTimer, [hwnd], COOLDOWNTIMER2, ms*4, FlyEnemy.setKD
;CHIP ACTION
.physChip:
;DIE ACTION CHECK
	cmp	[HP], 0
	jg	.chipact
	stdcall DieFalling
	mov	eax, [SCREEN_WIDTH]
	mov	[CLEARRECT.bottom], eax
	mov	eax, [SCREEN_HEIGHT]
	add	eax, [H]
	add	eax, 200
	cmp	[y_pos], eax
	jle	@f

	stdcall BossSceneClear, [hwnd]
	mov	[menu_scene], True
@@:
	mov	[bkgPainted], False
	jmp	 .skip

.chipact:
	mov		 [FallingFlag], True
;SET START CHIP ANIMATION
	stdcall SetChipStayAnimation
;KEYPRESS
	stdcall KeyPress
	cmp  eax,QUIT
	je   .wmdestroy

;SET NECESSARY CHIP ANIMATION
	stdcall CheckChipJumpAnimation
	stdcall CheckChipBoxPushAnimation

;EXIT on the right side to new scene
.next:
	mov	eax, [SCREEN_WIDTH]
	cmp  [x_pos], eax
	jbe  .actions
	mov  [transition_flag], True
	invoke	SetRect, transition_rect, 0,0,physSpeed*3 ,1

	cmp  [second_scene], True
	jne  @f
	stdcall SecondSceneClear, [hwnd]
	stdcall BossScenePrepare,[hwnd],[wmsg],[wparam],[lparam]
	jmp	.finish
@@:
	stdcall FirstSceneClear, [hwnd]
	stdcall SecondScenePrepare,[hwnd],[wmsg],[wparam],[lparam]
	jmp	.finish
;CHIP MODEL
.actions:
	stdcall BoxShoot
	stdcall Jump
	cmp	[FallingFlag], True
	jne	.skip
	mov	[OnGround], False
	stdcall Falling
	cmp	[FallingFlag], False
	jne	 .skip
	cmp	 [JumpFlag], False
	jne	 .skip
	cmp	 [OnGround], False
	jne	 .skip
	stdcall correctHeightLevels
.skip:
;SEND MSG TO CLEAR SCREEN
      cmp [transition_flag], True
      jne @f
      invoke  InvalidateRect, [hwnd], transition_rect, True
      jmp     .invalidsent
@@:
      invoke  InvalidateRect, [hwnd], CLEARRECT, TRUE
.invalidsent:
	jmp	.finish


  .wmdestroy:
	stdcall DestroyWin

  .finish:
	ret
endp

;================ END TIMERPROC ==============================



;==========	  DRAW	       =============
proc drawFirstScene
;DRAW CHIP
.drawChip:
	stdcall   Screen.DrawPoints
       cmp     [HP], 0
       jg      @f
       mov     [ChipState], ChipJump
       stdcall Chip.DrawDie
       jmp     .boxdraw
@@:
	stdcall Chip.DrawChip
	stdcall Chip.DrawHP
;DRAW BOXES
.boxdraw:
	stdcall Box.DrawBoxes
	stdcall Box.DestroyBoxAnimation
.drawtext:
	stdcall Screen.PrintText, text, [text_len], txtRect, DT_LEFT
	mov [text_len], eax
;DRAW DOG ENEMY
.enemydraw:
	;cmp	 [EnemyAlive], True
	;jne	 .end
	cmp	 [EnemyXY+4], seil
	jle	 .end
	stdcall Enemy.DrawEnemy
.end:
	 ret
endp


proc	drawSecondScene
;DRAW CHIP
.drawChip:
       cmp     [HP], 0
       jg      @f
       mov     [ChipState], ChipJump
       stdcall Chip.DrawDie
       jmp     .drawhp
@@:
	stdcall Chip.DrawChip
.drawhp:
	stdcall Chip.DrawHP
;DRAW BOXES
.drawbox:
	stdcall Box.DrawBoxes
	stdcall Box.DestroyBoxAnimation

;DRAW DOG ENEMY
.drawenemy:
	cmp	 [EnemyXY+4], seil
	jle	 .drawfly
	stdcall Enemy.DrawEnemy
.drawfly:
	mov	 eax, [right_wall_dd]
	cmp [FlyEnemyXY], eax
	jge .end
	stdcall FlyEnemy.DrawFlyEnemy
.end:
	stdcall   Screen.DrawPoints

	ret
endp

proc drawBossScene
;DRAW CHIP
.drawChip:
	stdcall   Screen.DrawPoints
       cmp     [HP], 0
       jg      @f
       mov     [ChipState], ChipJump
       stdcall Chip.DrawDie
       jmp     .drawhp
@@:
	stdcall Chip.DrawChip
.drawhp:
	stdcall Chip.DrawHP
;DRAW BOSS
.boss:
      stdcall Boss.BossDraw
;DRAW BOXES
.boxdraw:
	stdcall Cheese.DrawBoxesWithCheese
	cmp	[boxDstrID], 12
	jae	.end
	stdcall Box.DestroyBoxAnimation
.end:
	ret
endp

proc	drawMenuScene
	invoke SelectObject, [hMemDC], [hbmpMenu]
	invoke StretchBlt, [hdc], 0, 0, [SCREEN_WIDTH], [SCREEN_HEIGHT], [hMemDC], 0, 0, 1450, 950, SRCPAINT

	ret
endp

proc	drawFinalScene
	invoke SelectObject, [hMemDC], [hbmpFinal]
	invoke StretchBlt, [hdc], 0, 0, [SCREEN_WIDTH], [SCREEN_HEIGHT], [hMemDC], 0, 0, 1450, 950, SRCPAINT

	ret
endp
;================== END DRAW +==================================





;======== PREPARE ==============
proc	BossScenePrepare uses eax edx esi edi,\
	hwnd,wmsg,wparam,lparam


;SET NEW SCENE -------------------------------------------------
	mov  [boss_scene], True
;SET CHIP
	mov  [x_pos], 25
	cmp	[boxUpped], True
	jne	@f
	mov	[ChipState], ChipBoxStay
	jmp	.set_cheese
@@:
	mov	[ChipState], ChipStay

.set_cheese:
;SET CHEESE DEFAULT SETTINGS
	stdcall 	  Cheese.setType
	stdcall 	  Cheese.setStartPos
	stdcall 	  Cheese.setSpeed
	mov		  [CheeseSpeed], al
.set_bosstimer:
;SET BOSS TIMER
	invoke	SetTimer, [hwnd], BOSSTIMER, ms*4, BossAttackPhaseTimer

;SET NEW ENEMY COORDINATES
	stdcall Enemy.setStartPos, BossXY,0.6,[BossH_dd]
;SET BOXES VALUES
	mov  [uppedBoxAdr], Boxes_coordinates+32*2
	mov  [uppedBoxID],  (box_col-4)-4
	stdcall       Box.Relocation, Boxes_coordinates, Boxes_coordinates_newScene
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates+8*2, 0.5, 2, 0,1,0

;SET TEXTRECT

	mov	 [text_len], 1
	and	 [text_flag], False

	mov	eax, [BossXY]
	mov	edx, [BossXY+4]
	sub	edx, 25
	add	eax, 125
	mov	 [bosstxtRect.right], eax
	mov	 [bosstxtRect.top], edx
	sub	 eax, 600
	add	 edx, 100
	mov	 [bosstxtRect.left],eax
	mov	 [bosstxtRect.bottom], edx


	mov	 [cutscene_len], 1
	mov	eax, [BossXY]
	mov	edx, [BossXY+4]
	sub	edx, 25
	add	eax, 250
	mov	 [cutsceneRect.right], eax
	mov	 [cutsceneRect.top], edx
	sub	 eax, 800
	add	 edx, 100
	mov	 [cutsceneRect.left],eax
	mov	 [cutsceneRect.bottom], edx



	ret
endp

proc	BossSceneClear uses eax,\
			hwnd
	mov    [boss_scene], False
	invoke KillTimer, [hwnd], BOSSTIMER
	mov    [BossHP], 5
	mov    eax, [hbmpBoss]
	mov    [BossImg], eax
	mov    [CheeseFlag], False
	mov    [BossStart], False
	mov    [flagYUMMY], False
	mov    [lenYUMMY], 1
	invoke SetRect, rectYUMMY, 900,200,1100,400
	invoke SetRect, bosstxtRect, 0,0,0,0
	mov    [curCheeseID], 1

	ret
endp


proc	SecondScenePrepare uses eax edx esi edi,\
	hwnd, wmsg, wparam, lparam


;SET VALS ---------------------------------------
	mov	[second_scene], True
;SET ENEMY POSITION
	stdcall Screen.setObjHeightInBoxes, 0.45
	mov	edx, [box_y]
	mul	edx
	add	eax, [EnemyH_dd]
	mov	[EnemyAlive], True
	stdcall Enemy.setStartPos, EnemyXY, 1.05, eax
	;ENEMY SPEED
	mov	[EnemySpeed], 22

;SET TIMERS
	invoke	SetTimer, [hwnd], ENEMYTIMER, ms/10*2, DogAnimTimer
	invoke	SetTimer, [hwnd], FLYENEMYTIMER, ms/10, FlyAnimTimer
	invoke	SetTimer, [hwnd], 100, ms/65, FlyBallTimer

;SET CHIP
	mov	[x_pos], 25
	cmp	[boxUpped], True
	jne	@f
	mov	[ChipState], ChipBoxStay
	jmp	.set_box
@@:
	mov	[ChipState], ChipStay

;SET BOX START VALUES
.set_box:
	mov	[uppedBoxAdr], Boxes_coordinates+32*3
	mov	[uppedBoxID], (box_col-4)

	stdcall Screen.setObjHeightInBoxes, 0.8
	mov	ecx, [box_y]
	mul	ecx
;SET FLYENEMY STARTPOS
	mov [FlyEnemyMoveUp], False
	mov [FlyEnemyAlive], True
	stdcall Enemy.setStartPos, FlyEnemyXY, 0.80 ,eax
	stdcall Enemy.setStartPos, FlyBallXY, 0.80, eax

	stdcall Screen.setObjHeightInBoxes, 0.45
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates, 0, eax, 0,0,3
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates+8*4, 0.33, eax, 0,0,2
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates+8*7, 0.65, eax, 0,0,1
	ret
endp

proc	SecondSceneClear,\
			hwnd
	;CLEAR PREV SCENE
	mov  [second_scene], False

	mov  [EnemyAlive], False
	mov  [FlyEnemyAlive], False
	invoke		   KillTimer, [hwnd], ENEMYTIMER
	invoke		   KillTimer, [hwnd], FLYENEMYTIMER
	invoke		   KillTimer, [hwnd], FLYBALLTIMER
	ret
endp








proc	FirstSceneClear,\
			hwnd
	;CLEAR PREV SCENE
	mov   [first_scene], False
	invoke	KillTimer, [hwnd],ENEMYTIMER
	ret
endp
proc	FirstScenePrepare uses eax edx ecx esi edi

	mov	[bkgPainted], False
	mov	eax, [y_floor_dd]
	mov	[CLEARRECT.bottom], eax
;SET BOX VALS
	stdcall       Box.Relocation, Boxes_coordinates, Boxes_coordinates_newScene
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates, 0.2, 2, 0,0,0
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates+8, 0.2, 1, 0,0,2
	stdcall Screen.setObjHeightInBoxes, 0.45
	stdcall Box.setStartCoordinates,\
				       Boxes_coordinates+8*4, 1.03, eax, 4,0,0

;SET ENEMY VAL
	stdcall Screen.setObjHeightInBoxes, 0.45
	mov	edx, [box_y]
	mul	edx
	add	eax, [EnemyH_dd]
	stdcall Enemy.setStartPos, EnemyXY, 1.05, eax

	invoke	SetTimer, [global_hwnd], ENEMYTIMER, ms/10*33/10, DogAnimTimer
;SET CHIP
	mov	[x_pos], 20
	mov	eax, [y_floor_dd]
	sub	eax, [H]
	dec	eax
	mov	[y_pos], eax
	mov	[HP], 3

	mov	[text_flag], FALSE
	mov	[text_len], 1
	invoke	SetRect, txtRect, 10,150,900,400
;CLEAR POINTS COUNTER
	mov ecx, points_len
	mov edi, points
@@:
	mov byte[edi], '0'
	inc edi
	loop	@B
	ret
endp
;========================= END PREPARE ====================================



	include 'units\Keyboard\Keyboard.c'
	include 'units\Screen\Screen.c'
	include 'units\Random\Random.c'
	include 'classes\MainHeroes\Chip.c'
	include 'classes\MainHeroes\Chip.Move.c'
	include 'classes\Box\Box.c'
	include 'classes\Box\BoxShoot.c'
	include 'classes\Enemy\Enemy.c'
	include 'classes\Boss\Boss.c'
	include 'classes\Boss\Cheese.c'
	include 'units\Phys\Falling.c'
	include 'units\Phys\Jump.c'
	include 'units\Phys\Contact.c'
	include 'units\Keyboard\KeyPress.c'
	include 'classes\fly\fly.c'
	include 'units\TimerAnim\timeranim.c'

section '.data' data readable writeable


  _class TCHAR 'FASMWIN32',0
  _classBtn TCHAR 'BUTTON',0
  _titleBtn TCHAR 'Play',0
  _classS TCHAR 'STATIC',0
  _title TCHAR 'Chip&Dale DLC',0
  _error TCHAR 'Startup failed.',0

  wc WNDCLASS 0,WindowProc,0,0,NULL,NULL,NULL,COLOR_WINDOWTEXT,NULL,_class

  msg MSG
  ps  PAINTSTRUCT
  rcClient RECT
;BMP HANDLES
  hBitmap	dd ?
  hbmpPoints	dd ?
  hbmpBox	dd ?
  hbmpBoxDestroy dd ?
  hbmpBoxDestroyMask dd ?
  hbmpBKG	dd ?
  hbmpHP	dd ?
  hbmpEnemy	dd ?
  hbmpEnemyMask dd ?
  hbmpFlyEnemy	dd ?
  hbmpFlyBall	dd ?
  hbmpFlyBallMask dd ?
  hbmpCheese	dd ?
  hbmpCheeseMask dd ?
  hbmpRedCheese  dd ?
  hbmpBlackCheese  dd ?
  hbmpBoss	 dd ?
  hbmpBossHeal	 dd ?
  hbmpBossMask	 dd ?
  hbmpBossAtt	 dd ?
  hbmpMenu	 dd ?
  hbmpFinal	 dd ?

;save hwnd
  global_hwnd	dd ?

;other handles
  hIcon 	dd ?
  hdc		dd ?
  hMemDC	dd ?
  hBkgDC	dd ?
  hfont 	dd ?

  bkg_music	  db 'play World_theme.mp3',0




	include 'units\Screen\Screen.du'
	include 'units\phys\phys.du'
	include 'classes\MainHeroes\Chip.du'
	include 'classes\Box\Box.du'
	include 'classes\Enemy\Enemy.du'
	include 'classes\Fly\Fly.du'
	include 'units\Random\Random.du'
	include 'classes\Boss\Boss.du'
	include 'classes\Boss\Cheese.du'



section '.idata' import data readable writeable

  library kernel32,'KERNEL32.DLL',\
	  user32, 'USER32.DLL',\
	  gdi32, 'GDI32.DLL',\
	  winmm,'WINMM.DLL'

  include 'api\kernel32.inc'
  include 'api\user32.inc'
  include 'api\gdi32.inc'

import winmm,\
mciSendString,'mciSendStringA'

section '.rsrc' resource data readable



Menu=4
ICONS=5

ChipStay=20
ChipGo=21
ChipJump=22
ChipBoxStay=25
ChipBoxGo=26
ChipBoxPush=27
ChipDieMask=30
HitPoints=65

pictBox=51
pictBoxDestroy=52
pictBoxDestroyMask=53

pictBKG=60

Boss=300
SmallBossAtt=310
BossAtt=301
SmallBoss=302
BossCheese=400
BCheeseMask=401
BRedCheese=402
BBlackCheese=403
BossSmallCheese=405
BSmallCheeseMask=406
BSmallRedCheese=407
BSmallBlackCheese=408
Final=999

Points=777

Dog=10
DogMask=11
Fly=12
FlyBall=14
FlyBallMask=15



directory	RT_BITMAP,bitmaps,\
		RT_ICON, icons,\
		RT_GROUP_ICON, group_icons,\
		RT_MANIFEST,manifests

resource	bitmaps,\
		ChipStay, LANG_NEUTRAL,cStay,\
		ChipGo, LANG_NEUTRAL, cGo,\
		ChipBoxStay, LANG_NEUTRAL, cBoxStay,\
		ChipBoxGo, LANG_NEUTRAL, cBoxGo,\
		ChipJump, LANG_NEUTRAL, cJump,\
		ChipBoxPush, LANG_NEUTRAL, cPushBox,\
		ChipDieMask, LANG_NEUTRAL,cDieMask,\
		pictBox, LANG_NEUTRAL, pictBoxes,\
		pictBoxDestroy, LANG_NEUTRAL, pictBoxesDestroy,\
		pictBoxDestroyMask, LANG_NEUTRAL, pictBoxesDestroyMask,\
		pictBKG, LANG_NEUTRAL, BackGround,\
		Dog, LANG_NEUTRAL, dog,\
		DogMask, LANG_NEUTRAL, dogmask,\
		Fly, LANG_NEUTRAL, fly,\
		FlyBall, LANG_NEUTRAL, flyball,\
		FlyBallMask, LANG_NEUTRAL, flyballmask,\
		HitPoints, LANG_NEUTRAL, hitpoints,\
		Boss, LANG_NEUTRAL, boss,\
		SmallBoss, LANG_NEUTRAL, smallboss,\
		BossAtt,LANG_NEUTRAL, bossatt,\
		SmallBossAtt,LANG_NEUTRAL, smallbossatt,\
		BossCheese,LANG_NEUTRAL, cheese,\
		BCheeseMask, LANG_NEUTRAL, cheeseMask,\
		BRedCheese, LANG_NEUTRAL, redcheese,\
		BBlackCheese,LANG_NEUTRAL, blackcheese,\
		BossSmallCheese,LANG_NEUTRAL, smallcheese,\
		BSmallCheeseMask, LANG_NEUTRAL, smallcheeseMask,\
		BSmallRedCheese, LANG_NEUTRAL, smallredcheese,\
		BSmallBlackCheese,LANG_NEUTRAL, smallblackcheese,\
		Menu, LANG_NEUTRAL, menubmp,\
		Points, LANG_NEUTRAL, pointsbmp,\
		Final, LANG_NEUTRAL, final
resource	manifests,\
		1, LANG_NEUTRAL, manifest

resource	icons,\
		ICONS+1, LANG_NEUTRAL, icon128,\
		ICONS+2, LANG_NEUTRAL, icon64,\
		ICONS+3, LANG_NEUTRAL, icon32,\
		ICONS+4, LANG_NEUTRAL, icon16
resource	group_icons,\
		ICONS, LANG_NEUTRAL, main_icon

bitmap	dog,	'images/Dog.bmp'
bitmap	dogmask,'images/Dogmask.bmp'
bitmap	fly, 'images/flyanim.bmp'
bitmap	flyballmask, 'images/flyballmask.bmp'
bitmap	flyball, 'images/flyball.bmp'
bitmap	cStay, 'images/ChipStay.bmp'
bitmap	cGo, 'images/ChipGo.bmp'
bitmap	cBoxStay, 'images/ChipBoxStay.bmp'
bitmap	cBoxGo, 'images/ChipBoxUp.bmp'
bitmap	cPushBox, 'images/ChipPushBox.bmp'
bitmap	cJump, 'images/ChipJump.bmp'
bitmap	cDieMask, 'images/ChipDieMask.bmp'
bitmap	pictBoxes, 'images/Box.bmp'
bitmap	pictBoxesDestroy, 'images/BoxDestroy.bmp'
bitmap	pictBoxesDestroyMask, 'images/boxDestroyMask.bmp'
bitmap	BackGround, 'images/bkg.bmp'
bitmap	hitpoints, 'images/HP.bmp'
bitmap	boss, 'images/Boss.bmp'
bitmap	smallboss, 'images/smallboss.bmp'
bitmap	bossatt, 'images/BosAttack.bmp'
bitmap	smallbossatt, 'images/smallbosattack.bmp'
bitmap	cheese, 'images/Cheese.bmp'
bitmap	cheeseMask, 'images/CheeseMask.bmp'
bitmap	redcheese, 'images/RedCheese.bmp'
bitmap	blackcheese, 'images/BlackCheese.bmp'
bitmap	smallcheese, 'images/smallCheese.bmp'
bitmap	smallcheeseMask, 'images/smallCheeseMask.bmp'
bitmap	smallredcheese, 'images/smallRedCheese.bmp'
bitmap	smallblackcheese, 'images/smallBlackCheese.bmp'
bitmap	menubmp, 'images/menu5.bmp'
bitmap	pointsbmp, 'images/points.bmp'
bitmap	final, 'images/finalscene.bmp'


icon	main_icon, icon128, 'icons/icon128.ico',\
		   icon64, 'icons/icon64.ico',\
		   icon32, 'icons/icon32.ico',\
		   icon16, 'icons/icon16.ico'

resdata manifest
      file 'Chip.EXE.1.manifest'
endres

