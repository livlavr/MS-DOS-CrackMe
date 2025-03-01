.model tiny
.code
.186
locals @@
org 100h
;------------------------------------------------------------------------|
;------------------------------------------------------------------------|
Start:	jmp Main																																		

am_i_fooled	db 0h
dont_fool_me	db "If you'll try to fool me again, i'll slap your little ass!$"
wrong_password	db "Access Denied!$"
good_password	db "Correct Password, Welcome!$"
pas_check0	dw 01505h
user_password	db 00050h dup(0h)
pas_check1	dw 01505h
;wrong_password	db - TODO hacker can rewrite wrong_password message
password	db "PASSME"

Main:
	
	;xor ax, ax
	;mov ah, 02h
	;xor bx, bx
	;mov bx, offset password
	;add bx, 1h
	;mov dl, [bx]
	;int 21h

	;check jmp check
	;exit
	;check
	;jmp exit

	mov ax, [pas_check0]
	mov bx, [pas_check1]
	cmp ax, bx
	je GoodBoy
	inc am_i_fooled
	;jmp Exit
GoodBoy:
	xor ax, ax
	mov ah, 3fh
	xor bx, bx
	mov cx, 0ffffh
	mov byte ptr dx, offset user_password
	int 21h		

	jmp Check

Exit:
	mov ax, 4c00h
	int 21h

Check:	
	call CheckPasswords
	pushf

	xor ax, ax
	mov ah, 09h
	;mov bx, 0001h
	;mov cx, 20h		; ???????????????????????????
	
	popf
	je CorrectInput

WrongInput:
	mov byte ptr dx, offset wrong_password
	int 21h
	jmp Exit 
	
CorrectInput:
	cmp am_i_fooled, 1h
	je DontFoolMe

	mov byte ptr dx, offset good_password
	int 21h
	jmp Exit

DontFoolMe:
	mov byte ptr dx, offset dont_fool_me
	int 21h
	jmp Exit 
;------------------------------------------------------------------------|
; Broke: si, di, df, es
;------------------------------------------------------------------------|
CheckPasswords	proc
	cld
	mov byte ptr si, offset password
	mov byte ptr di, offset user_password
	push ds
	pop es
	cmpsb
	 
	ret
CheckPasswords	endp
;------------------------------------------------------------------------|
end	Start