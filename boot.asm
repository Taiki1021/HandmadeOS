[org 0]
[bits 16]

jmp 0x07C0:start

start:
mov ax,cs
mov ds,ax
xor ax,ax
mov ss,ax

cli

mov ax,0x1000
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
or eax,0x00000001
mov cr0,eax

jmp $+2
nop
nop

db 0x66
db 0x67
jmp DWORD SysCodeSelecter:0000

;========GDT==========================================


gdtr:
dw gdt_end-gdt-1
dd gdt

gdt: 
dw 0
dw 0
db 0
db 0
db 0
db 0

SysCodeSelecter equ 0x08
dw 0xFFFF
dw 0x1000
db 0x00
db 0x9A
db 0xCF
db 0x00

SysDataSelecter equ 0x10
dw 0xFFFF
dw 0x1000
db 0x00
db 0x92
db 0xCF
db 0x00

VideoSelecter  equ 0x18
dw 0xFFFF
dw 0xB800
db 0x00
db 0x92
db 0x40
db 0x00

gdt_end:

;=======================================================

times 510-($-$$) db 0

dw 0xAA55
