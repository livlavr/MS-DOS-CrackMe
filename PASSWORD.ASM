.model tiny
.code
.186
locals @@
org 100h
;------------------------------------------------------------------------|
; Basic Password Checker
;------------------------------------------------------------------------|
Main:

	mov ax, [pas_check0]
	mov bx, [pas_check1]
	cmp ax, bx
	je GoodBoy
	inc am_i_fooled

GoodBoy:
	xor ax, ax
	mov ah, 3fh
	xor bx, bx
	mov cx, 0ffffh
	mov byte ptr dx, offset user_password
	int 21h

	jmp Check

hash	dw 0ea2h

Exit:
	mov ax, 4c00h
	int 21h

Check:
	call CheckPasswords
	pushf

	call JokeHash
	xor ax, ax
	mov ah, 09h
	cmp bx, [hash]
	jne DontFoolMe

	popf
	je CorrectInput

WrongInput:
	mov byte ptr dx, offset wrong_password
	int 21h
	jmp Exit

CorrectInput:
	cmp am_i_fooled, 1h
	je DontFoolMe
	;call JokeHash
	;cmp bx, [hash]
	;jne DontFoolMe

	mov byte ptr dx, offset good_password
	int 21h
	jmp Exit

DontFoolMe:
	mov byte ptr dx, offset dont_fool_me
	int 21h
	jmp Exit
;------------------------------------------------------------------------|
; Enter: Nothing
; Broke: si, di, df, es
; Output: Passwords comparing result (Flags)
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
JokeHash	proc
	cld
	mov ax, offset wrong_password
	mov cx, offset password
	sub cx, ax
	xor ax, ax	
	xor bx, bx

	mov si, offset wrong_password

@@Loop:	xor ax, ax
	lodsb
	add bx, ax
	loop @@Loop

	ret
JokeHash	endp
;------------------------------------------------------------------------|
pas_check0	dw 01505h
user_password	db 00010h dup(0h)
pas_check1	dw 01505h
wrong_password	db "Access Denied!$"
good_password	db "Correct Password, Welcome!$"
password	db "PASSME"
am_i_fooled	db 0h
dont_fool_me	db "If you'll try to fool me again, i'll slap your little ass!$"
;------------------------------------------------------------------------|
end	Main
