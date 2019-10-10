typedef struct {
	unsigned int af, bc, de, hl, ix, iy, Cf, Zf;	/* 2 registers 0..65535 */
} REGS;

REGS				*_REGs( void );												// this returns address to internal word registers
void				IntDos(void);												// CALL 5. Call a DOS Function
void				IntBios(void);												// CALL 1Ch (RST 30h), set IX,IY. Call a Bios Function from MSXDOS
void 				DosCLS (void);  											// CLS for Dos
void 				Exit(char n);												// Proper Exit program, and come back to MSX DOS, sending  n as error code
