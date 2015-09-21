section .text ; start of program code
global _start
_start: 

mov ebx, esp
mov esp, ebx
mov al, 0x46
AND [EBP-0x10], EAX
AND EAX, [EBP+0x08]
;AND [CH-0x10], AL
;AND AL, [CH+0x08]

;add edx,[ecx+0x1]
;add [ecx+0x1],edx
;add ecx,[edx+0x1]
;add [edx+0x1],ecx
;mov dl, 0x8
;mov [ebx+0x4], ebx
;mov ebx, [ebx+0x4]
;dec eax;
;dec al;
;dec ebx;
;dec bh;
;inc eax;
;inc al;
;inc ebx;
;inc bh;
;ret
;ret 1
;ret -1
;ret 65000
;ret 412511111
;pop esp;[esp+0x1]
;or eax, 0x24
;or al, ah
;or esp, [ebp+0x1]
;or [ebp-0x1], ebp
;or [eax-0x1], esp
;or [esp-0x1], eax
;or [esp-0x1], esp
;or [edx-0x1], edx
;or [esp-0x1], esp
;nop
;or esp, esp
;or esp, [esp]
;or [esp], esp
not eax
;xor eax, eax
;xor ebx, ebx
;xor ebx, 0x1
;xor bx, 0x1
;xor bl, 0x1
;xor bh, 0x1
;xor bl, 0x12356774
;sub ebx, 0xbffff341
;xor ebx, 0xbffff341
;xor cx, 0x01
;xor 0xbffff356, eax
;xor 0xbffff356, 0xbffff42a
;xor 0xbffff356, [0xbffff42a]
;xor 0x1, 0x45
;xor 0x1, al
;xor bl, 0x1
;xor ecx, [ecx-0x1]
;xor ecx, eax
;xor [ecx-0x1], ecx
;xor [ecx-0x1], [ecx+0x2]



xor eax, eax
mov al, 0x1
int 0x80
