/**
 ** This is Public Domain Software.
 ** 
 ** The author disclaims copyright to this source code.  
 ** In place of a legal notice, here is a blessing:
 **
 **    May you do good and not evil.
 **    May you find forgiveness for yourself and forgive others.
 **    May you share freely, never taking more than you give.
 **/
 
#ifndef __OLCB_UTIL_H__
#define __OLCB_UTIL_H__

inline byte parseHexDigit(char ch) {
	byte val = ch - '0';
	if (val > 9)
		val = (ch - 'A') + 10;
	return val & 0xF;
}


#endif  //__OLCB_UTIL_H__