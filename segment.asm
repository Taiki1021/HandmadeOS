[bits 32]

global Halt
global WriteFar

WriteFar:
;void WriteFar(unsigned short Selecter,int offset,void* src,int n);
	push eax 
	push es 
	push edi
	push esi
	push ebx 

;es=書き込み対象のセグメント　edi=書き込み対象のオフセット　esi=書き込むデータへのポインタ　bx=書き込むデータ量

	mov ax,[ss:(esp+22)]  ;es=(第一引数)
	mov es,ax  


	mov edi,[ss:(esp+26)] ;edi=(第二引数)

	mov esi,[ss:(esp+30)] ;esi=(第三引数)

	mov ebx,[ss:(esp+34)] ;ebx=(第四引数)

WriteFar_Loop:
	cmp ebx,0       ;While(bx!=0)
	jz WriteFar_End 

	mov al,[ds:esi] ;[es:edi]=[ds:esi]
	mov [es:edi],al

	inc edi    ;edi++ esi++ bx--
	inc esi
	dec ebx

	jmp WriteFar_Loop 
WriteFar_End:  

	pop ebx
	pop esi
	pop edi
	pop es
	pop eax
	ret


Halt:
	HLT
	jmp Halt		;終了
