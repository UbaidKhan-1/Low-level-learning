//        +----- Pseudo Code -----+

//		n = 100 //rows
//		addr = SCREEN
//		while (n>=i){
//			RAM[addr] = -1
//			addr += 32
//			i++
//		}

//        +----- Hack Assembly -----+

// [SCREEN_Address = 0x4000 , RES = 512 x 256]
@i
M = 1// i = 1

@R0
D = A // D = RAM[0]

@n
M = D // n = 100 (rows)

@SCREEN
D = A // D = SCREEN

@Addr
M = D // addr = SCREEN (current address)

(LOOP)
@Addr
A = M // A = RAM[Addr]
M = -1 // RAM[RAM[Addr]] = -1 (16 bits)

@32  
D = A // D = 32  
  
@Addr  
M = M+D // current address += 32  
  
@i  
M = M+1 // i++  
  
D = M // D = i  
@n  
D = M-D // D = n-i  
  
@LOOP  
D; JGT

(END)
@END
0;JMP

//   +-----------------------------------+
