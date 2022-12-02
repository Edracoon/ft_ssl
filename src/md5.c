#include "ft_ssl.h"

// Github Tutorial:	https://github.com/Zunawe/md5-c
// Wikipedia:		https://en.wikipedia.org/wiki/MD5#Pseudocode
// MD5 RFC:			https://www.rfc-editor.org/rfc/rfc1321

/*
 * Before starting here is some important definitions:
 * Size definition
 *  - 1 bit = 1/8 byte (1 or 0 in binary)
 *  - byte == 8 bits
 *  - word == 2 bytes (16 bits)
 *  - doubleword == 4 bytes (32 bits) -> also called 32-bit words
 *  - quadword   == 8 bytes (64 bits) -> also called 64-bit words
 * Binary operators
 *  - AND: &			(True of both bits are 1) 					-> https://en.wikipedia.org/wiki/Logical_conjunction#Truth_table
 *  - OR:  |			(True if one of the bits is 1)				-> https://en.wikipedia.org/wiki/Logical_disjunction#Semantics
 *  - XOR: ^			(True if one of the bits is 1 but not both)	-> https://en.wikipedia.org/wiki/Exclusive_or#Truth_table
 *  - NOT: ~			(Invert the bits)							-> https://en.wikipedia.org/wiki/Negation#Definition
 *  - Left shift: <<	(Shift the bits to the left)				-> https://en.wikipedia.org/wiki/Bitwise_operation#Shifts
 *  - Right shift: >>	(Shift the bits to the right)				-> https://en.wikipedia.org/wiki/Bitwise_operation#Shifts
 */

/*
 * These function are used to collapse three 32-bit words into one 32-bit word
*/
#define F(x, y, z) ((x & y) | (~x & z)) // X and Y, or not X and Z
#define G(x, y, z) ((x & z) | (y & ~z)) // X and Z, or Y and not Z
#define H(x, y, z) (x ^ y ^ z)			// X xor Y xor Z
#define I(x, y, z) (y ^ (x | ~z))		// Y xor (X or not Z)

/*
 * Rotates a 32-bit word to the left by n bits (aka circular shift)
 * See: https://qnaplus.com/left-and-right-rotate-bits-of-a-number-in-c/
 * Also: https://www.geeksforgeeks.org/c-program-to-rotate-bits-of-a-number/
 * 		 with that https://practice.geeksforgeeks.org/problems/rotate-bits4524/1
 */
// #define leftRotate(x, n) ((x << n) | (x >> (32 - n)))
uint32_t leftRotate(uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); }

const	unsigned int	s[64] = {
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

const	unsigned int	K[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

unsigned int	applyPadding(unsigned char **str)
{
	unsigned int	originalLength = strLen(*(char **)str);
	unsigned int	newLength = originalLength;

	/*
	* Step 1: Paddind
	* The provided message is padded by appending bits to the end until its length is congruent to 448 % 512 bits. (448 modulo 512 == 64)
	* In other words, the message is padded so that its length is 64 bits less than the next multiple of 512.
	* If the original message's length already meets this requirement before padding, it is still padded with 512 bits.
	* The padding is simply a single "1" bit at the end of the message followed by enough "0" bits to satisfy the length condition above.
	* Exemple:			 	  "H  e  l  l  o  ,     w  o  r  l  d  !"
	* Converted to hexa: 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21
	* So now to '1' and all the '0' to make the length congruent to 448 % 512 bits :
	*   0x 48 65 6c 6c 6f 2c 20 77 6f 72 6c 64 21 80 00 00
	*	0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	*	0x 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
	*	0x 00 00 00 00 00 00 00 00
	* The 80 is the '1' bit and the 00 are the '0' bits. (80 is 128 in decimal meaning 128 = 1000 0000)
	*/
	while (newLength % 64 != 56)
		newLength++;

	/* Create a new string with our new total length */
	unsigned char *paddedStr = myAlloc((newLength + 1) * sizeof(unsigned char));

	/* Copy the content in our new string */
	for (unsigned int i = 0; i < originalLength; i++)
		paddedStr[i] = (*str)[i];

	/* Add the '1' bit and 7 other '0' (128) */
	paddedStr[originalLength] = 0x80;

	/* Then pad with only '0' */
	for (unsigned int i = originalLength + 1; i < newLength; i++)
		paddedStr[i] = 0x00;

	/*
	 * Step 2: Appending the Length
	 * Next, the length of the message modulus 2^64 is appended in little endian
	 * to the message to round out the total length to a multiple of 512.
	 * This length is the number of bits in the original message, modulus 2^64.
	 * It's common to split this number into two 32-bit words, so keep careful track of which bytes are put where
	 * the highest order byte should be the last byte in the message.
	 * This will round out the length of the whole message to a multiple of 512.
	 */
	unsigned int	modBitLength = (originalLength * 8); // % (unsigned long long)pow(2, 64); // -> This is useless since our message is always be less than 2^64
	/* To append the length: uint_32 is 4 chars https://stackoverflow.com/questions/121059/conversion-from-32-bit-integer-to-4-chars */
	for (unsigned int i = 0; i < 4; i++)
        paddedStr[newLength - i] = (modBitLength >> (i * 8)) & 0xff;
	printf("newLength -> %d, modOriginalBitLength -> %d\n", newLength, modBitLength);
	/* Free the old string */
	free(*str);

	*str = paddedStr;
	return (newLength);
}

unsigned char	*md5(SSL *ssl, unsigned char *str)
{
	(void)ssl;
	/*
	 * Step 1 and 2 explained in applyPadding
	 */
	unsigned int totalLength = applyPadding(&str);

	/*
	 * Step 3: Initialize the MD Buffer
	 * A four-word buffer (A,B,C,D) is used to compute the message digest.
	 * Here each of A, B, C, D is a 32-bit register.
	 * These registers are initialized to the following values in hexadecimal, low-order bytes first.
	 * A = 0x67452301 ; B = 0xefcdab89 ; C = 0x98badcfe ; D = 0x10325476
	 * (They are in little-endian order)
	 */
	unsigned int	a0 = 0x67452301;
	unsigned int	b0 = 0xefcdab89;
	unsigned int	c0 = 0x98badcfe;
	unsigned int	d0 = 0x10325476;

	/*
	 * Step 4: Process the message in successive 512-bit chunks
	 * - The message is split into blocks of 512 bits.
	 * - Each block of 512 bits is split into sixteen 32 bits blocks (doubleword aka 4 chars). (16 * 32 == 512)
	 */
	for (unsigned int i = 0; i < totalLength; i += 64) /* Breaking into 512 bits (char = 8 bits and 8 * 64 = 512) */
	{
		/* Initialize hash value for this chunk */
		unsigned int	A = a0;
		unsigned int	B = b0;
		unsigned int	C = c0;
		unsigned int	D = d0;

		/* Break chunk into sixteen 32-bit words (unsigned int is 32 bits aka uint32_t) */
		unsigned int	*M = (unsigned int *)myAlloc(16 * sizeof(unsigned int));
		/* We want to concatenate 4 chars into M[j] */
		/* see https://stackoverflow.com/questions/57155608/how-can-i-concatenate-multiple-chars-into-an-int */
		for (unsigned int j = 0; j < 16; j++)
			M[j] = str[i + j] | (str[i + j + 1] << 8) | (str[i + j + 2] << 16) | (str[i + j + 3] << 24);

		/* Main loop */
		for (unsigned int j = 0; j < 64; j++)
		{
			unsigned int F, g;
			if (j < 16)
			{
				F = F(B, C, D); // (B & C) | ((~B) & D);
				g = j;
			}
			else if (j < 32)
			{
				F = G(B, C, D); // (D & B) | ((~D) & C);
				g = (5 * j + 1) % 16;
			}
			else if (j < 48)
			{
				F = H(B, C, D); // B ^ C ^ D;
				g = (3 * j + 5) % 16;
			}
			else
			{
				F = I(B, C, D); // C ^ (B | (~D));
				g = (7 * j) % 16;
			}


			F = F + A + K[j] + M[g];  // M[g] is our 32-bits word (4 chars)
			A = D;
			D = C;
			C = B;
			B = B + leftRotate(F, s[j]);
		}
	    // Add this chunk's hash to the result buffer
		a0 = a0 + A;
		b0 = b0 + B;
		c0 = c0 + C;
		d0 = d0 + D;
	}
	/* Compute the final hash */
	/* Wikipedia: char digest[16] = a0 + b0 + c0 + d0 (Output is in little-endian) */
	unsigned char *digest = myAlloc(16 * sizeof(unsigned char));
	for (unsigned int i = 0; i < 4; i++)
	{
		unsigned int n = i == 0 ? a0 : (i == 1 ? b0 : (i == 2 ? c0 : d0));
		/*
		 * We fill 4 index by loop by decomposing
		 * our buffers (a, b, c, d -> currently n)
		 * into 4 chars using bit shifting
		 * See: https://stackoverflow.com/questions/121059/conversion-from-32-bit-integer-to-4-chars
		 */
		digest[i * 4 + 0] = (unsigned char)((n & 0xff));
        digest[i * 4 + 1] = (unsigned char)((n >> 8) & 0xff);
        digest[i * 4 + 2] = (unsigned char)((n >> 16 )& 0xff);
        digest[i * 4 + 3] = (unsigned char)((n >> 24) & 0xff);
	}

	return digest;
}