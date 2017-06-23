[bits 32]

global Halt
global CopyFar

CopyFar:
;void CopyFar(unsigned short DistSelecter,char* DistOffset,int DStep,unsigned short SrcSelecter,char* SrcOffset,int SStep,int n);
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push es
	push gs

;es=書き込み対象のセグメント　edi=書き込み対象のオフセット　esi=書き込むデータへのポインタ　bx=書き込むデータ量

	mov ax,[ss:(esp+36+0)]  	;DistSelecter
	mov es,ax  
	mov edi,[ss:(esp+36+4)] 	;DistOffset
	mov ebx,[ss:(esp+36+8)] 	;DStep


	mov ax,[ss:(esp+36+12)]    	;SrcSelecter
	mov gs,ax		
	mov esi,[ss:(esp+36+16)]	;SrcOffset
	mov edx,[ss:(esp+36+20)]	;SStep

	mov ecx,[ss:(esp+36+24)]	;n
CopyFar_Loop:
	cmp ecx,0       ;While(bx!=0)
	jz CopyFar_End 

	mov al,[gs:esi] ;[es:edi]=[ds:esi]
	mov [es:edi],al

	add edi,ebx    ;edi++ esi++ bx--
	add esi,edx
	dec ecx

	jmp CopyFar_Loop 
CopyFar_End:  
	pop gs 
	pop es
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret

Halt:
	HLT
	jmp Halt		;終了
