00000100  8CC8              mov ax,cs
00000102  8ED8              mov ds,ax
00000104  8EC0              mov es,ax
00000106  E9C800            jmp 0x1d1
00000109  0000              add [bx+si],al
0000010B  0000              add [bx+si],al
0000010D  0000              add [bx+si],al
0000010F  0000              add [bx+si],al
00000111  0000              add [bx+si],al
00000113  0000              add [bx+si],al
00000115  0000              add [bx+si],al
00000117  0000              add [bx+si],al
00000119  0000              add [bx+si],al
0000011B  0000              add [bx+si],al
0000011D  0000              add [bx+si],al
0000011F  0000              add [bx+si],al
00000121  0000              add [bx+si],al
00000123  0000              add [bx+si],al
00000125  0000              add [bx+si],al
00000127  0000              add [bx+si],al
00000129  0000              add [bx+si],al
0000012B  0000              add [bx+si],al
0000012D  0000              add [bx+si],al
0000012F  0000              add [bx+si],al
00000131  0000              add [bx+si],al
00000133  0000              add [bx+si],al
00000135  0000              add [bx+si],al
00000137  0000              add [bx+si],al
00000139  0000              add [bx+si],al
0000013B  0000              add [bx+si],al
0000013D  0000              add [bx+si],al
0000013F  0000              add [bx+si],al
00000141  0000              add [bx+si],al
00000143  0000              add [bx+si],al
00000145  0000              add [bx+si],al
00000147  0000              add [bx+si],al
00000149  0000              add [bx+si],al
0000014B  0000              add [bx+si],al
0000014D  0000              add [bx+si],al
0000014F  0000              add [bx+si],al
00000151  0000              add [bx+si],al
00000153  0000              add [bx+si],al
00000155  0000              add [bx+si],al
00000157  0000              add [bx+si],al
00000159  0000              add [bx+si],al
0000015B  0000              add [bx+si],al
0000015D  0000              add [bx+si],al
0000015F  0000              add [bx+si],al
00000161  0000              add [bx+si],al
00000163  0000              add [bx+si],al
00000165  0000              add [bx+si],al
00000167  0000              add [bx+si],al
00000169  0000              add [bx+si],al
0000016B  0000              add [bx+si],al
0000016D  0000              add [bx+si],al
0000016F  0000              add [bx+si],al
00000171  0000              add [bx+si],al
00000173  0000              add [bx+si],al
00000175  0000              add [bx+si],al
00000177  0000              add [bx+si],al
00000179  0000              add [bx+si],al
0000017B  0000              add [bx+si],al
0000017D  0000              add [bx+si],al
0000017F  0000              add [bx+si],al
00000181  0000              add [bx+si],al
00000183  0000              add [bx+si],al
00000185  0000              add [bx+si],al
00000187  0000              add [bx+si],al
00000189  0000              add [bx+si],al
0000018B  0000              add [bx+si],al
0000018D  0000              add [bx+si],al
0000018F  0000              add [bx+si],al
00000191  0000              add [bx+si],al
00000193  0000              add [bx+si],al
00000195  0000              add [bx+si],al
00000197  0000              add [bx+si],al
00000199  0000              add [bx+si],al
0000019B  0000              add [bx+si],al
0000019D  0000              add [bx+si],al
0000019F  0000              add [bx+si],al
000001A1  0000              add [bx+si],al
000001A3  0000              add [bx+si],al
000001A5  0000              add [bx+si],al
000001A7  0000              add [bx+si],al
000001A9  0000              add [bx+si],al
000001AB  0000              add [bx+si],al
000001AD  0000              add [bx+si],al
000001AF  0000              add [bx+si],al
000001B1  0000              add [bx+si],al
000001B3  0000              add [bx+si],al
000001B5  0000              add [bx+si],al
000001B7  0000              add [bx+si],al
000001B9  0000              add [bx+si],al
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
000001CF  0000              add [bx+si],al
000001D1  50                push ax
000001D2  55                push bp
000001D3  8BEC              mov bp,sp
000001D5  C746026002        mov word [bp+0x2],0x260
000001DA  5D                pop bp
000001DB  E84C00            call 0x22a
000001DE  3BD0              cmp dx,ax
000001E0  B91A02            mov cx,0x21a
000001E3  B91A02            mov cx,0x21a
000001E6  EB0D              jmp short 0x1f5
000001E8  90                nop
000001E9  0C00              or al,0x0
000001EB  0000              add [bx+si],al
000001ED  0000              add [bx+si],al
000001EF  0000              add [bx+si],al
000001F1  0000              add [bx+si],al
000001F3  0000              add [bx+si],al
000001F5  33DB              xor bx,bx
000001F7  E80000            call 0x1fa
000001FA  5B                pop bx
000001FB  5F                pop di
000001FC  5E                pop si
000001FD  8A0EC602          mov cl,[0x2c6]
00000201  32ED              xor ch,ch
00000203  FC                cld
00000204  F3A6              repe cmpsb
00000206  7403              jz 0x20b
00000208  EB0B              jmp short 0x215
0000020A  90                nop
0000020B  B409              mov ah,0x9
0000020D  BA6502            mov dx,0x265
00000210  CD21              int 0x21
00000212  EB08              jmp short 0x21c
00000214  90                nop
00000215  B409              mov ah,0x9
00000217  BA8502            mov dx,0x285
0000021A  CD21              int 0x21
0000021C  B44C              mov ah,0x4c
0000021E  CD21              int 0x21
00000220  50                push ax
00000221  55                push bp
00000222  8BEC              mov bp,sp
00000224  C74602F701        mov word [bp+0x2],0x1f7
00000229  5D                pop bp
0000022A  B409              mov ah,0x9
0000022C  BA4A02            mov dx,0x24a
0000022F  CD21              int 0x21
00000231  B40A              mov ah,0xa
00000233  BAE901            mov dx,0x1e9
00000236  CD21              int 0x21
00000238  58                pop ax
00000239  50                push ax
0000023A  55                push bp
0000023B  8BEC              mov bp,sp
0000023D  C74602EB01        mov word [bp+0x2],0x1eb
00000242  5D                pop bp
00000243  50                push ax
00000244  33D2              xor dx,dx
00000246  33C0              xor ax,ax
00000248  C3                ret
00000249  00456E            add [di+0x6e],al
0000024C  7465              jz 0x2b3
0000024E  7220              jc 0x270
00000250  7468              jz 0x2ba
00000252  65207061          and [gs:bx+si+0x61],dh
00000256  7373              jnc 0x2cb
00000258  776F              ja 0x2c9
0000025A  7264              jc 0x2c0
0000025C  3A20              cmp ah,[bx+si]
0000025E  0A24              or ah,[si]
00000260  44                inc sp
00000261  45                inc bp
00000262  44                inc sp
00000263  3332              xor si,[bp+si]
00000265  59                pop cx
00000266  6F                outsw
00000267  7520              jnz 0x289
00000269  686176            push word 0x7661
0000026C  65207265          and [gs:bp+si+0x65],dh
00000270  636569            arpl [di+0x69],sp
00000273  7665              jna 0x2da
00000275  64206164          and [fs:bx+di+0x64],ah
00000279  6D                insw
0000027A  696E207269        imul bp,[bp+0x20],word 0x6972
0000027F  67687473          push word 0x7374
00000283  2124              and [si],sp
00000285  41                inc cx
00000286  7265              jc 0x2ed
00000288  20796F            and [bx+di+0x6f],bh
0000028B  7520              jnz 0x2ad
0000028D  61                popa
0000028E  207265            and [bp+si+0x65],dh
00000291  61                popa
00000292  6C                insb
00000293  206861            and [bx+si+0x61],ch
00000296  636B65            arpl [bp+di+0x65],bp
00000299  723F              jc 0x2da
0000029B  204C6F            and [si+0x6f],cl
0000029E  6F                outsw
0000029F  6F                outsw
000002A0  6F                outsw
000002A1  6F                outsw
000002A2  6F                outsw
000002A3  7365              jnc 0x30a
000002A5  722E              jc 0x2d5
000002A7  2E2E204168        and [cs:bx+di+0x68],al
000002AC  61                popa
000002AD  686861            push word 0x6168
000002B0  210A              and [bp+si],cx
000002B2  49                dec cx
000002B3  6E                outsb
000002B4  636F72            arpl [bx+0x72],bp
000002B7  7265              jc 0x31e
000002B9  637420            arpl [si+0x20],si
000002BC  7061              jo 0x31f
000002BE  7373              jnc 0x333
000002C0  776F              ja 0x331
000002C2  7264              jc 0x328
000002C4  2124              and [si],sp
000002C6  054869            add ax,0x6948
000002C9  6E                outsb
000002CA  743A              jz 0x306
000002CC  206265            and [bp+si+0x65],ah
000002CF  68696E            push word 0x6e69
000002D2  64207C30          and [fs:si+0x30],bh
000002D6  3030              xor [bx+si],dh
000002D8  3030              xor [bx+si],dh
000002DA  314635            xor [bp+0x35],ax
000002DD  2033              and [bp+di],dh
000002DF  334442            xor ax,[si+0x42]
000002E2  20786F            and [bx+si+0x6f],bh
000002E5  7220              jc 0x307
000002E7  62782C            bound di,[bx+si+0x2c]
000002EA  62787C            bound di,[bx+si+0x7c]
000002ED  2020              and [bx+si],ah
000002EF  61                popa
000002F0  6E                outsb
000002F1  642020            and [fs:bx+si],ah
000002F4  7C30              jl 0x326
000002F6  3030              xor [bx+si],dh
000002F8  3030              xor [bx+si],dh
000002FA  3232              xor dh,[bp+si]
000002FC  3020              xor [bx+si],ah
000002FE  353020            xor ax,0x2030
00000301  7075              jo 0x378
00000303  7368              jnc 0x36d
00000305  206178            and [bx+di+0x78],ah
00000308  7C20              jl 0x32a
0000030A  2032              and [bp+si],dh
0000030C  3920              cmp [bx+si],sp
0000030E  696E737472        imul bp,[bp+0x73],word 0x7274
00000313  7563              jnz 0x378
00000315  7469              jz 0x380
00000317  6F                outsw
00000318  6E                outsb
00000319  7324              jnc 0x33f
