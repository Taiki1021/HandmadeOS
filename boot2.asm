%include"selector.inc"
[bits 32]


PM_start:
mov ax,SysDataSelecter ;各セグメントにデータセレクタの値を代入
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

mov ax,VideoSelecter	;ビデオメモリーの先頭から0x7FF個のax値を書き込む
mov es,ax
mov di,0
mov bx,0x7FF
mov al,'.'
mov ah,0xE7
call FillMemory_Word
jmp $			;終了



FillMemory_Word:
;Fill given memory area    
;
;Arguments
;
;es=Memory selecter
;di=Target address(offset)
;ax=Word to fill with 
;bx=Count

push di
push bx

FillMemory_WriteW:
mov [es:di],ax		;2バイトずつ書き込む
add di,2
dec bx
jnz FillMemory_WriteW

FillMemory_End:
pop bx
pop di
ret


