%include"selecter.inc"
[org 0]
[bits 16]

	jmp 0x07C0:start

start:
	mov ax,cs ;ds,ssをcsに合わせる
	mov ds,ax
	mov ss,ax

Kernel_data_Load:
	mov si,5

	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,2	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
Kernel_data_Load_Retry:
	mov ax,0x07E0 ;0x07E0:0000にAドライブの0番目のシリンダの1番目のセクタをHead=0で読み込む
	mov es,ax
	mov ah,2	;読み込み
	mov al,1      ;読み込むセクタ数
	mov dl,0      ;Aドライブ

	int 0x13
	jc Kernel_data_Load_Retry  ;エラーが起きた場合はリトライ
	dec si			;カウンタを下げて
	jz Kernel_data_Load_End	;0でなければ
	add bx,0x200		;ターゲットのアドレスを512バイト移動
	inc cl			;読み込むセクタ位置を一つずらす
	cmp cl,19		;もし最後のセクタまで行ったら
	jz Kernel_data_Load_inccyl   ;シリンダを移動する
	jmp Kernel_data_Load_Retry
Kernel_data_Load_inccyl:
	mov cl,1
	inc ch
	jmp Kernel_data_Load_Retry
Kernel_data_Load_End:

Kernel_code_Load:
	mov si,18

	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,7	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
Kernel_code_Load_Retry:
	mov ax,0x1000 ;0x1000:0000にAドライブの0番目のシリンダの1番目のセクタをHead=0で読み込む
	mov es,ax
	mov ah,2	;読み込み
	mov al,1      ;読み込むセクタ数
	mov dl,0      ;Aドライブ

	int 0x13
	jc Kernel_code_Load_Retry  ;エラーが起きた場合はリトライ
	dec si			;カウンタを下げて
	jz Kernel_code_Load_End	;0でなければ
	add bx,0x200		;ターゲットのアドレスを512バイト移動
	inc cl			;読み込むセクタ位置を一つずらす
	cmp cl,19		;もし最後のセクタまで行ったら
	jz Kernel_code_Load_inccyl   ;シリンダを移動する
	jmp Kernel_code_Load_Retry
Kernel_code_Load_inccyl:
	mov cl,1
	inc ch
	jmp Kernel_code_Load_Retry
Kernel_code_Load_End:


	mov dx,0x3F2	;フロッピーディスクのモーターの電源を切る
	xor al,al
	out dx,al

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


	lgdt [gdtr]    ;GDTを登録

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
	mov ax,SysDataSelecter
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,0x3600-5 ;カーネルスタックの初期位置はデータセグメントの一番最後

	mov ax,VideoSelecter
	mov es,ax
	mov esi,0
	mov edi,0
	mov cx,0x7FF
;PrintLoop:
;	mov ah,[ds:esi]
;	mov [es:edi],ah
;	inc esi
;	add edi,2
;	dec cx
;	jnz PrintLoop

;	jmp $
	

	jmp SysCodeSelecter:0000


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
	dw 0x0000
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
gdt_end:

;=======================================================

	times 510-($-$$) db 0

	dw 0xAA55
