/*
 * exercise_11_3_2.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RADIX  128

/*
 * hash
 *
 * Hash a character string to a slot using the division method. The character
 * string is interpreted as an integer expressed as a radix-128 number. That
 * is, a string of [a_r a_{r-1} ... a_1 a_0] is treated as an integer of
 * a_r * 128^r + a_{r-1} * 128^{r-1} + ... + a_1 * 128 + a_0.
 *
 * @parameter
 *   slot_num - the number of slots that the hash table has
 */
unsigned int hash(const char *const str, const unsigned int slot_num)
{
    unsigned int i;
    short radix_mod;
    unsigned int result;

    result = 0;
    radix_mod = 1;
    i = strlen(str);
    while (i-- > 0) {
	result = (str[i] * radix_mod + result) % slot_num;
	radix_mod = (RADIX * radix_mod) % slot_num;
    }

    return result;
}

#define STR_LEN_MAX  10
#define SLOT_MAX     200

int main()
{
    char str[STR_LEN_MAX + 1];
    unsigned int i, str_len, slot_num;

    srand(time(NULL));

    /* Build a random string. */
    str_len = 1 + ((unsigned int) rand() + (unsigned int) rand()) % STR_LEN_MAX;
    for (i = 0; i < str_len; i++)
	str[i] = 1 + rand() % (RADIX - 1);
    str[str_len] = '\0';

    /* Print the string and its ASCII number sequence. */
    printf("The given string: ");
    puts(str);
    printf("The ASCII number sequence: ");
    for (i = 0; i < str_len - 1; i++)
	printf("%d, ", str[i]);
    printf("%d\n", str[i]);

    slot_num = 1 + ((unsigned int) rand() + (unsigned int) rand()) % SLOT_MAX;

    /* Print the hashing result. */
    printf("The hash table has %u slots, and the string is hashed to slot %u.\n",
	    slot_num, hash(str, slot_num));

    return 0;
}

