%include"selecter.inc"

;ブートローダーではカーネルの読み込みと最低限の設定をしてカーネルに処理を移す
;細かい設定はカーネルの方で行う

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
	mov dl,0	;Aドライブ
	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,2	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
	call Load

	mov ax,0x1000 
	mov es,ax	;読み込み先のセグメント
	mov si,44	;読み込むセクタ数
	mov dl,0	;Aドライブ
	mov dh,0	;ヘッダ番号
	mov ch,0	;シリンダ番号
	mov cl,7	;セクタ番号
	mov bx,0   	;ターゲットアドレス(オフセット)
	call Load

;データロード
	;0x16000~0x9f800 がフロッピーイメージ
;	mov ax,0x1600
;	mov es,ax	;読み込み先のセグメント
;	mov si,1100	;読み込むセクタ数
;	mov dl,1	;Bドライブ
;	mov dh,0	;ヘッダ番号
;	mov ch,0	;シリンダ番号
;	mov cl,1	;セクタ番号
;	mov bx,0   	;ターゲットアドレス(オフセット)
;	call Load

	mov dx,0x3F2	;フロッピーディスクのモーターの電源を切る
	xor al,al
	out dx,al

	cli

;GDT IDTの登録
	lgdt [gdtr]    ;仮のGDTを登録 あとでカーネルがちゃんとGDTを決める

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

Load:
	pushad
	mov ah,2	;読み込み
	mov al,1      ;読み込むセクタ数
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

[bits 32]
PM_start:
;セグメントレジスタ初期化
	mov ax,SysDataSelecter
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,0x3600-5+0x7E00 ;カーネルスタックの初期位置はデータセグメントの一番最後

;カーネルに飛ぶ
	jmp SysCodeSelecter:0000


;========GDT==========================================
;ここにはカーネルを起動するための必要最低限のディスクリプタを定義する
gdtr:
	dw gdt_end-gdt-1
	dd gdt+0x7C00

	times 0x150-($-$$) db 0
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
	dw 0xFFFF
	dw 0x0000
	db 0x00
	db 0x92
	db 0xCF
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
