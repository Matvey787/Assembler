00000100  E80800            call 0x10b
00000103  E84900            call 0x14f
00000106  B8004C            mov ax,0x4c00
00000109  CD21              int 0x21
0000010B  33C9              xor cx,cx
0000010D  BE5F01            mov si,0x15f
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
00000124  83F90D            cmp cx,byte +0xd
00000127  75E7              jnz 0x110
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
00000143  89363102          mov [0x231],si
00000147  8B0E3102          mov cx,[0x231]
0000014B  E88100            call 0x1cf
0000014E  C3                ret
0000014F  BE5F01            mov si,0x15f
00000152  BF2602            mov di,0x226
00000155  33C9              xor cx,cx
00000157  83F900            cmp cx,byte +0x0
0000015A  EB0D              jmp short 0x169
0000015C  90                nop
0000015D  0C00              or al,0x0
0000015F  0000              add [bx+si],al
00000161  0000              add [bx+si],al
00000163  0000              add [bx+si],al
00000165  0000              add [bx+si],al
00000167  0000              add [bx+si],al
00000169  33C9              xor cx,cx
0000016B  90                nop
0000016C  90                nop
0000016D  90                nop
0000016E  90                nop
0000016F  90                nop
00000170  90                nop
00000171  90                nop
00000172  90                nop
00000173  90                nop
00000174  90                nop
00000175  90                nop
00000176  90                nop
00000177  90                nop
00000178  90                nop
00000179  90                nop
0000017A  90                nop
0000017B  90                nop
0000017C  8A04              mov al,[si]
0000017E  8A1D              mov bl,[di]
00000180  3AC3              cmp al,bl
00000182  7516              jnz 0x19a
00000184  3C00              cmp al,0x0
00000186  7404              jz 0x18c
00000188  46                inc si
00000189  47                inc di
0000018A  EBF0              jmp short 0x17c
0000018C  BEEF01            mov si,0x1ef
0000018F  E82F00            call 0x1c1
00000192  8B0E2D02          mov cx,[0x22d]
00000196  E83600            call 0x1cf
00000199  C3                ret
0000019A  BE0602            mov si,0x206
0000019D  E82100            call 0x1c1
000001A0  8B0E2F02          mov cx,[0x22f]
000001A4  E82800            call 0x1cf
000001A7  E80100            call 0x1ab
000001AA  C3                ret
000001AB  BEE201            mov si,0x1e2
000001AE  B40E              mov ah,0xe
000001B0  E80E00            call 0x1c1
000001B3  BE5F01            mov si,0x15f
000001B6  8B0E2B02          mov cx,[0x22b]
000001BA  E81200            call 0x1cf
000001BD  E80100            call 0x1c1
000001C0  C3                ret
000001C1  B40E              mov ah,0xe
000001C3  8A04              mov al,[si]
000001C5  3C00              cmp al,0x0
000001C7  7405              jz 0x1ce
000001C9  CD10              int 0x10
000001CB  46                inc si
000001CC  EBF5              jmp short 0x1c3
000001CE  C3                ret
000001CF  B40E              mov ah,0xe
000001D1  B00A              mov al,0xa
000001D3  CD10              int 0x10
000001D5  B008              mov al,0x8
000001D7  83F900            cmp cx,byte +0x0
000001DA  7405              jz 0x1e1
000001DC  CD10              int 0x10
000001DE  49                dec cx
000001DF  EBF6              jmp short 0x1d7
000001E1  C3                ret
000001E2  59                pop cx
000001E3  6F                outsw
000001E4  7520              jnz 0x206
000001E6  656E              gs outsb
000001E8  7465              jz 0x24f
000001EA  7265              jc 0x251
000001EC  643A00            cmp al,[fs:bx+si]
000001EF  59                pop cx
000001F0  6F                outsw
000001F1  7520              jnz 0x213
000001F3  686176            push word 0x7661
000001F6  65206164          and [gs:bx+di+0x64],ah
000001FA  6D                insw
000001FB  696E207269        imul bp,[bp+0x20],word 0x6972
00000200  67687473          push word 0x7374
00000204  2100              and [bx+si],ax
00000206  59                pop cx
00000207  6F                outsw
00000208  7520              jnz 0x22a
0000020A  656E              gs outsb
0000020C  7465              jz 0x273
0000020E  7265              jc 0x275
00000210  6420696E          and [fs:bx+di+0x6e],ch
00000214  636F72            arpl [bx+0x72],bp
00000217  7265              jc 0x27e
00000219  637420            arpl [si+0x20],si
0000021C  7061              jo 0x27f
0000021E  7373              jnc 0x293
00000220  776F              ja 0x291
00000222  7264              jc 0x288
00000224  2100              and [bx+si],ax
00000226  3132              xor [bp+si],si
00000228  3334              xor si,[si]
0000022A  004900            add [bx+di+0x0],cl
0000022D  3E0029            add [ds:bx+di],ch
00000230  0000              add [bx+si],al
00000232  00                db 0x00
