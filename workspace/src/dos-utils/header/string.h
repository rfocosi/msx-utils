char				CharToLower	( char c );										// convert to lowercase
char				CharToUpper	( char c );										// convert to uppercase
void				StrCopy( char *dst, char *src ); 							// copy string from src to dst
void				NStrCopy( char *dst, char *src, int n ); 					// copy string from src to dst with no more than n characters
void				StrConcat( char *dst, char *src ); 							// concatenate string from src to dst
void				NStrConcat( char *dst, char *src, int n ); 					// concatenate string from src to dst with no more than n characters
int					StrLen( char *adr ); 										//returns length of string
int					StrCompare( char *s1, char *s2 ); 							// compares two strings s1 and s2,  returns -1 (s1<s2), 0 (s1=s2), 1 (s1>s2)
int					NStrCompare( char *s1, char *s2, int n ); 					// compares two strings s1 and s2, no more than n characters,  returns -1 (s1<s2), 0 (s1=s2), 1 (s1>s2)
int					StrChr( char *adr, char c );  								// returns i, for which adr[i] = c, or -1 if not found
int					StrPosStr( char *s1, char *s2 ); 							// finds substring s2 in string s1 and returns position s1[i], returns -1 if not found
int					StrSearch( char *s1, char *s2 ); 							// returns the the first occurrence in the string s1 of any character from the string s2, or -1 if not found
int					StrPosChr( char *adr, char c ); 							// returns the last i, for which adr[i] = c, or -1 if not found
void				StrLeftTrim( char *adr ); 									// removes left spaces
void				StrRightTrim( char *adr ); 									// removes right spaces
void				StrReplaceChar( char *adr, char c, char nc );				// replaces all chars c to nc in string adr
char*				StrReverse(char *str);
char*				Itoa(int num, char* str, int base);
