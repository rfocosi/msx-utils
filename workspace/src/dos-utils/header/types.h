int					IsAlphaNum	( char c );										// A-Za-z0-9
int					IsAlpha	( char c );											// A-Za-z
int					IsAscii	( char c );											// !..~
int					IsCntrl	( char c );											// unprintable control symbol
int					IsDigit	( char c );											// 0..9
int					IsGraph	( char c );											// has graphic representation
int					IsLower	( char c );											// lowercase test
int					IsPrintable	( char c );										// printable test
int					IsPunctuation ( char c );									// punctuation sign test
int					IsSpace	( char c );											// space test
int					IsUpper	( char c );											// uppercase test
int					IsHexDigit( char c );										// hex digit test
char 				IsPositive(int c);											// Check if positive (return 1) or Negative (return -1)
float				IntToFloat(int c);											// Return Float version of the int
void 				IntSwap(int *a, int *b);									// Swap two Int Variables
