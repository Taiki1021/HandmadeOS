[bits 32]

global Halt
global WriteFar
global ReadFar

WriteFar:
;void WriteFar(unsigned short Selecter,int offset,void* src,int n,int sstep,int dstep);
	push eax 
	push es 
	push edi
	push esi
	push ebx 

;es=書き込み対象のセグメント　edi=書き込み対象のオフセット　esi=書き込むデータへのポインタ　bx=書き込むデータ量

	mov ax,[ss:(esp+24)]  ;es=(第一引数)
	mov es,ax  


	mov edi,[ss:(esp+28)] ;edi=(第二引数)

	mov esi,[ss:(esp+32)] ;esi=(第三引数)

	mov ebx,[ss:(esp+36)] ;ebx=(第四引数)

	mov ecx,[ss:(esp+40)]
	
	mov edx,[ss:(esp+44)]


WriteFar_Loop:
	cmp ebx,0       ;While(bx!=0)
	jz WriteFar_End 

	mov al,[ds:esi] ;[es:edi]=[ds:esi]
	mov [es:edi],al

	add edi,edx    ;edi++ esi++ bx--
	add esi,ecx
	dec ebx

	jmp WriteFar_Loop 
WriteFar_End:  

	pop ebx
	pop esi
	pop edi
	pop es
	pop eax
	ret

ReadFar:
;void ReadFar(unsigned short Selecter,int offset,void* dist,int n,int sstep,int dstep);
	push eax 
	push es 
	push edi
	push esi
	push ebx 

;es=読み込み対象のセグメント　edi=読み込み対象のオフセット　esi=読み込むデータへのポインタ　bx=読み込むデータ量

	mov ax,[ss:(esp+24)]  ;es=(第一引数)
	mov es,ax  

	mov esi,[ss:(esp+28)] ;esi=(第二引数)

	mov edi,[ss:(esp+32)] ;edi=(第三引数)

	mov ebx,[ss:(esp+36)] ;ebx=(第四引数)

	mov ecx,[ss:(esp+40)]

	mov edx,[ss:(esp+44)]

ReadFar_Loop:
	cmp ebx,0       ;While(bx!=0)
	jz ReadFar_End 

	mov al,[es:esi]
	mov [ds:edi],al ;[es:edi]=[ds:esi]

	add edi,edx    ;edi++ esi++ bx--
	add esi,ecx
	dec ebx

	jmp ReadFar_Loop 
ReadFar_End:  

	pop ebx
	pop esi
	pop edi
	pop es
	pop eax
	ret



Halt:
	HLT
	jmp Halt		;終了
