%include"selecter.inc"
[org 0]
[bits 16]

jmp 0x07C0:start

start:
mov ax,cs ;ds,ssをcsに合わせる
mov ds,ax
mov ss,ax

cli

mov ax,0x1000 ;0x1000:0000にAドライブの0番目のシリンダの1番目のセクタをHead=0で読み込む
mov es,ax
mov bx,0
mov ah,2
mov al,1
mov ch,0
mov cl,2
mov dh,0
mov dl,0
int 0x13

lgdt [gdtr]

mov eax,cr0		
or eax,0x00000001	;保護モードに入る
mov cr0,eax

jmp $+2	
nop
nop

mov ax,SysDataSelecter
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

db 0x66
db 0x67
jmp DWORD SysCodeSelecter:0000	;読み込んだプログラムに飛ぶ

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
dw 0xFFFF
dw 0x0000
db 0x01
db 0x9A
db 0xCF
db 0x00

;SysDataSelecter
dw 0xFFFF
dw 0x0000
db 0x01
db 0x92
db 0xCF
db 0x00

;VideoSelecter
dw 0xFFFF
dw 0x8000
db 0x0B
db 0x92
db 0x40
db 0x00

gdt_end:

;=======================================================

times 510-($-$$) db 0

dw 0xAA55
