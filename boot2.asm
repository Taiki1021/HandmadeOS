%include"selector.inc"
[bits 32]


PM_start:
jmp $
mov ax,SysDataSelecter
mov ds,ax
mov es,ax
mov fs,ax
mov gs,ax
mov ss,ax

mov ax,VideoSelecter
mov es,ax
mov di,0
mov bx,0x7FF
mov al,' '
mov ah,0xE7
mov cx,1
call FillMemory
jmp $

FillMemory: 
;Fill given memory area    
;
;Arguments
;
;es=Memory selecter
;di=Target address(offset)
;ax=Char to fill with 
;bx=Count cx=flag(1=Word 0=Byte)

push di
push bx
cmp cx,1
jz FillMemory_WriteW  ;if(CX){FillMemory_WriteW}else{FillMemory_WriteB}

FillMemory_WriteB:
mov [es:di],al
inc di
dec bx
jnz FillMemory_WriteB
jmp FillMemory_End

FillMemory_WriteW:
mov [es:di],ax
add di,2
dec bx
jnz FillMemory_WriteW

FillMemory_End:
pop bx
pop di
ret

times 512-($-$$) db 0
