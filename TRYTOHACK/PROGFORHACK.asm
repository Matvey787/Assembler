00000100  E80800            call 0x10b
00000103  E84900            call 0x14f
00000106  B8004C            mov ax,0x4c00
00000109  CD21              int 0x21
0000010B  33C9              xor cx,cx
0000010D  BEBC01            mov si,0x1bc
00000110  B400              mov ah,0x0
00000112  CD16              int 0x16
00000114  3C0D              cmp al,0xd
00000116  7428              jz 0x140
00000118  3C08              cmp al,0x8
0000011A  740D              jz 0x129
0000011C  B40E              mov ah,0xe
0000011E  CD10              int 0x10
00000120  8804              mov [si],al
00000122  46                inc si
00000123  41                inc cx
00000124  83F914            cmp cx,byte +0x14
00000127  7CE7              jl 0x110
00000129  83F900            cmp cx,byte +0x0
0000012C  7412              jz 0x140
0000012E  4E                dec si
0000012F  49                dec cx
00000130  B40E              mov ah,0xe
00000132  B008              mov al,0x8
00000134  CD10              int 0x10
00000136  B020              mov al,0x20
00000138  CD10              int 0x10
0000013A  B008              mov al,0x8
0000013C  CD10              int 0x10
0000013E  EBD0              jmp short 0x110
00000140  C60400            mov byte [si],0x0
00000143  89361F02          mov [0x21f],si
00000147  8B0E1F02          mov cx,[0x21f]
0000014B  E85A00            call 0x1a8
0000014E  C3                ret
0000014F  BEBC01            mov si,0x1bc
00000152  BF1402            mov di,0x214
00000155  8A04              mov al,[si]
00000157  8A1D              mov bl,[di]
00000159  3AC3              cmp al,bl
0000015B  7516              jnz 0x173
0000015D  3C00              cmp al,0x0
0000015F  7404              jz 0x165
00000161  46                inc si
00000162  47                inc di
00000163  EBF0              jmp short 0x155
00000165  BEDD01            mov si,0x1dd
00000168  E82F00            call 0x19a
0000016B  8B0E1B02          mov cx,[0x21b]
0000016F  E83600            call 0x1a8
00000172  C3                ret
00000173  BEF401            mov si,0x1f4
00000176  E82100            call 0x19a
00000179  8B0E1D02          mov cx,[0x21d]
0000017D  E82800            call 0x1a8
00000180  E80100            call 0x184
00000183  C3                ret
00000184  BED001            mov si,0x1d0
00000187  B40E              mov ah,0xe
00000189  E80E00            call 0x19a
0000018C  BEBC01            mov si,0x1bc
0000018F  8B0E1902          mov cx,[0x219]
00000193  E81200            call 0x1a8
00000196  E80100            call 0x19a
00000199  C3                ret
0000019A  B40E              mov ah,0xe
0000019C  8A04              mov al,[si]
0000019E  3C00              cmp al,0x0
000001A0  7405              jz 0x1a7
000001A2  CD10              int 0x10
000001A4  46                inc si
000001A5  EBF5              jmp short 0x19c
000001A7  C3                ret
000001A8  B40E              mov ah,0xe
000001AA  B00A              mov al,0xa
000001AC  CD10              int 0x10
000001AE  B008              mov al,0x8
000001B0  83F900            cmp cx,byte +0x0
000001B3  7405              jz 0x1ba
000001B5  CD10              int 0x10
000001B7  49                dec cx
000001B8  EBF6              jmp short 0x1b0
000001BA  C3                ret
000001BB  0000              add [bx+si],al
000001BD  0000              add [bx+si],al
000001BF  0000              add [bx+si],al
000001C1  0000              add [bx+si],al
000001C3  0000              add [bx+si],al
000001C5  0000              add [bx+si],al
000001C7  0000              add [bx+si],al
000001C9  0000              add [bx+si],al
000001CB  0000              add [bx+si],al
000001CD  0000              add [bx+si],al
000001CF  00596F            add [bx+di+0x6f],bl
000001D2  7520              jnz 0x1f4
000001D4  656E              gs outsb
000001D6  7465              jz 0x23d
000001D8  7265              jc 0x23f
000001DA  643A00            cmp al,[fs:bx+si]
000001DD  59                pop cx
000001DE  6F                outsw
000001DF  7520              jnz 0x201
000001E1  686176            push word 0x7661
000001E4  65206164          and [gs:bx+di+0x64],ah
000001E8  6D                insw
000001E9  696E207269        imul bp,[bp+0x20],word 0x6972
000001EE  67687473          push word 0x7374
000001F2  2100              and [bx+si],ax
000001F4  59                pop cx
000001F5  6F                outsw
000001F6  7520              jnz 0x218
000001F8  656E              gs outsb
000001FA  7465              jz 0x261
000001FC  7265              jc 0x263
000001FE  6420696E          and [fs:bx+di+0x6e],ch
00000202  636F72            arpl [bx+0x72],bp
00000205  7265              jc 0x26c
00000207  637420            arpl [si+0x20],si
0000020A  7061              jo 0x26d
0000020C  7373              jnc 0x281
0000020E  776F              ja 0x27f
00000210  7264              jc 0x276
00000212  2100              and [bx+si],ax
00000214  3132              xor [bp+si],si
00000216  3334              xor si,[si]
00000218  004900            add [bx+di+0x0],cl
0000021B  3E0029            add [ds:bx+di],ch
0000021E  0000              add [bx+si],al
00000220  00                db 0x00
