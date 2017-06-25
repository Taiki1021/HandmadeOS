%include"selecter.inc"

global alltraps

extern trap

;vector.Sの割り込みハンドラはすべて割り込み番号をpushしてこれを呼び出す。
;この関数はここまでpushした（あるいはアーキテクチャの割り込み機構によってpushされた）情報へのポインタを引数にtrapを呼び出す。
;trapは割り込み番号をもとにidtとは別に独自のテーブルに登録されている割り込みハンドラを渡された引数にして呼び出す。
alltraps:
	push ds
	push es
	push fs
	push gs
	pushad
	
	mov ax,SysDataSelecter
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	push esp
	call trap
	add esp,4

	popad
	pop gs
	pop fs
	pop es
	pop ds
	add esp,0x8
	iret
