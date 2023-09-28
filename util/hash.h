/*
 * hash.h
 *
 *  Created on: 28-Feb-2023
 *      Author: user
 */

#ifndef UTIL_HASH_H_
#define UTIL_HASH_H_

int hashCode(char value[], int len) {
	int h = 0;
	for (int i=0;i<len;i++) {
		h = 31 * h + (value[i] & 0xff);
	}
	return h;
}



#endif /* UTIL_HASH_H_ */
