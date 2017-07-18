%include"selecter.inc"
[org 0]
	jmp 0x07C0:start

	times 0xF-($-$$) db 0
start:
;セグメントレジスタ初期化
	mov ax,cs ;ds,ssをcsに合わせる
	mov ds,ax
	mov ax,0x9000
	mov ss,ax
	mov sp,0xFFFF


;カーネルロード
	mov ax,0x07E0 
	mov es,ax	;読み込み先のセグメント
	mov si,5	;読み込むセクタ数
	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,2	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
	call Load

	mov ax,0x1000 
	mov es,ax	;読み込み先のセグメント
	mov si,24	;読み込むセクタ数
	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,7	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
	call Load

	mov dx,0x3F2	;フロッピーディスクのモーターの電源を切る
	xor al,al
	out dx,al


;PICの設定
	cli

	mov al,0x11
	out 0x20,al
	dw 0x00eb,0x00eb
	out 0xA0,al
	dw 0x00eb,0x00eb
	
	mov al,0x20
	out 0x21,al
	dw 0x00eb,0x00eb
	mov al,0x28
	out 0xA1,al
	dw 0x00eb,0x00eb

	mov al,0x04
	out 0x21,al
	dw 0x00eb,0x00eb
	mov al,0x02
	out 0xA1,al
	dw 0x00eb,0x00eb

	mov al,0x01
	out 0x21,al
	dw 0x00eb,0x00eb
	out 0xA1,al
	dw 0x00eb,0x00eb

	mov al,0xFB
	xor al,0x03
	out 0x21,al
	dw 0x00eb,0x00eb
	mov al,0xFF
	xor al,0x00
	out 0xA1,al

;A20ゲートオン
	call waitkbdout
	mov al,0xd1
	out 0x64,al
	call waitkbdout
	mov al,0xdf
	out 0x60,al
	call waitkbdout

	

;GDT IDTの登録
	lgdt [gdtr]    ;GDTを登録
	lidt [idtr]

;保護モードに入る
	mov eax,cr0		
	or eax,0x00000001	;保護モードに入る
	mov cr0,eax

	jmp $+2	
	nop
	nop

	db 0x66
	db 0x67
	jmp DWORD BootSelecter:PM_start

[bits 32]
PM_start:
;セグメントレジスタ初期化
	mov ax,SysDataSelecter
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,0x3600-5 ;カーネルスタックの初期位置はデータセグメントの一番最後

;	mov ax,VideoSelecter
;	mov es,ax
;	mov esi,0
;	mov edi,0
;	mov cx,0x7FF
;PrintLoop:
;	mov ah,[ds:esi]
;	mov [es:edi],ah
;	inc esi
;	add edi,2
;	dec cx
;	jnz PrintLoop

;	jmp $
	
;カーネルに飛ぶ
	jmp SysCodeSelecter:0000


[bits 16]

waitkbdout:
	in al,0x64
	test al,2
	jnz waitkbdout
	ret

Load:
	pushad
	mov ah,2	;読み込み
	mov al,1      ;読み込むセクタ数
	mov dl,0      ;Aドライブ
Load_Retry:
	pushad
	int 0x13	;読み込み
	popad
	jnc Load_Next  ;エラーが起きた場合はリトライ
	pushad
	mov ah,0x00
	mov dl,0x00
	int 0x13	;ドライブのリセット
	popad
	jmp Load_Retry
Load_Next:
	dec si		;カウンタを下げて
	jz Load_End	;0なら終了
	add bx,0x200	;ターゲットのアドレスを512バイト移動
	inc cl		;読み込むセクタ位置を一つずらす
	cmp cl,18	;もし最後のセクタまで行ったら
	jbe Load_Retry   ;シリンダを移動する
Load_inchead:
	mov cl,1
	inc dh
	cmp dh,2
	jb Load_Retry
Load_inccyl:
	mov dh,0
	inc ch
	jmp Load_Retry
Load_End:
	popad
	ret




;========IDTR========================================
idtr:
	dw 256*8-1
	dd 0


;========GDT==========================================


gdtr:
	dw gdt_end-gdt-1
	dd gdt+0x7C00

gdt: 
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0

;SysCodeSelecter
	dw 0x8200
	dw 0x0000
	db 0x01
	db 0x9A
	db 0x40
	db 0x00

;SysDataSelecter
	dw 0x3600
	dw 0x7E00
	db 0x00
	db 0x92
	db 0x40
	db 0x00

;VideoSelecter
	dw 0x8000
	dw 0x8000
	db 0x0B
	db 0x92
	db 0x40
	db 0x00

;GDTSelecter
	dw gdt_end-gdt-1
	dw gdt+0x7C00
	db 0x00
	db 0x92
	db 0x40
	db 0x00

;IDTSelecter
	dw 0x0800
	dw 0x0000
	db 0x00
	db 0x92
	db 0x40
	db 0x00

;UsrCodeSelecter
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x9A
	db 0x40
	db 0x00

;UsrDataSelecter
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x92
	db 0x40
	db 0x00

;BootSelecter
	dw 0x0200
	dw 0x7C00
	db 0x00
	db 0x9A
	db 0x40
	db 0x00

;MallocSelecter
	dw 0xFFFF
	dw 0x0000
	db 0x10
	db 0x92
	db 0x4F
	db 0x00

gdt_end:

;=======================================================

	times 510-($-$$) db 0

	dw 0xAA55
