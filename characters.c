#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Returns the number of bytes a character takes up according to UTF-8 
unsigned int num_bytes(char b) {
  //    0xxxxxxx        1xxxxxxx
  //  & 10000000        10000000
  //  ----------        --------
  //    00000000        10000000
  if((b & 0b10000000) == 0) {
    return 1;
  }
  // & is a "bitwise operator"
  // This overall strategy is called "bitmasking"
  //   110xxxxx         010xxxxx    100xxxxx    111xxxxx ...
  // & 11100000
  // ----------         --------    --------    --------
  //   11000000         01000000    10000000    11100000
  else if((b & 0b11100000) == 0b11000000) { return 2; }
  else if((b & 0b11110000) == 0b11100000) { return 3; }
  else if((b & 0b11111000) == 0b11110000) { return 4; }
  else if((b & 0b11111100) == 0b11111000) { return 5; }
  else if((b & 0b11111110) == 0b11111100) { return 6; }
  else { return -1; }
}


// Returns the amount of unicode characters in a string not taking into account the size of bytes from UTF-8
unsigned int utf8_strlen(char* unicode) {
  unsigned int len = strlen(unicode); // count of bytes in this string
  unsigned int bytes_seen = 0;
  unsigned int unicode_characters_seen = 0;
  while(bytes_seen < len) {
    unsigned int bytes_this_char = num_bytes(unicode[bytes_seen]);
    bytes_seen += bytes_this_char;
    unicode_characters_seen += 1;
  }
  return unicode_characters_seen;
}

/**
 * Returns the number of bytes taken by the first n UTF-8 characters
 * in the given UTF-8 string
 * 
 * This is useful for later work with UTF-8 strings where we may need
 * to allocate or copy substrings based on UTF-8 character indices!
 * 
 * If the value of n is greater than the number of unicode characters,
 * returns -1.
 * 
 * Examples:
 *   bytes_for("José", 3) -> 3
 *   bytes_for("Ülo", 3) -> 4
 *   bytes_for("José", 4) -> 5
 *   bytes_for("成龙", 1) -> 3
 *   bytes_for("成龙", 2) -> 6
 *   bytes_for("成龙", 3) -> -1
 */

// Function takes a UTF-8 string and a character count, and returns the number of bytes in that UTF-8 string taken up by the first n characters
unsigned int bytes_for(char* unicode, unsigned int n) {
  // Counter to keep track of the total number of bytes
  int numBytesTotal = 0;

  // If character count n exceeds UTF-8 specified length (amount of characters) of the string, function returns -1
  if (n > utf8_strlen(unicode)) {
    return -1;
  }

  /*
    Variable j used as index to access a character in the string
    Incremented by the number of bytes the character takes up, given by the return value of the num_bytes function
  */ 
  int j = 0;

  // For loop that loops n number of times
  for (int i = 0; i < n; i++) {
    // Variable that stores the number of bytes of a single character, given by the return value of the num_bytes function
    int numBytes = num_bytes(unicode[j]);

    // Increments counter by number of bytes
    numBytesTotal += numBytes;

    // Increments index counter by number of bytes
    j += numBytes;
  }

  // Returns total number of bytes in the UTF-8 string taken up by the first n characters
  return numBytesTotal;
}



int main(int argc, char** argv) {

  if(argc < 2) {
    printf("Try running with ./welcome your-name\n");
    return 1;
  }
  

  char* name = argv[1];
  int length = strlen(name);
  unsigned int ulen = utf8_strlen(name);
  printf("Hi %s, your name is %d characters long according to utf8_strlen.\n", name, ulen);

  // gives wrong num of bytes because after getting num of bytes for that character, I need to increment the counter by that amount to reach the next valid character
  
  // Converts input string to an integer using atoi function
  int numChars = atoi(argv[2]);

  // Prints the amount of bytes in the UTF-8 string taken up by the first n characters by calling bytes_for function
  printf("%d\n", bytes_for(name, numChars));











  /*for (int i = 0; i < ulen; i++) {
    int numBytes = num_bytes(name[0]);
    printf("Number of bytes at %d index: %d\n", i, numBytes);
  }
  */


  //int num = argv[2];
  //unsigned int bytes = bytes_for(name, 3);
  //printf("Bytes for %s is %d\n", name, bytes);

  //printf("The number of bytes needed for the first character are: %c\n", name[0]);

  /*printf("The invididual characters are: \n");
  for(int i = 0; i < length; i += 1) {
    unsigned char letter = name[i];
    printf("%d(%x) ", letter, letter);
  }
  printf("\n");
*/
  return 0;
}


/*
  Some extra code for reference that's not part of the starter above:


  unsigned char li[4] = "李";
  printf("%d(%x) should be 1 byte long: %d\n", 'a', 'a', num_bytes('a'));
  printf("%d(%x) should be 3 bytes long: %d\n", li[0], li[0], num_bytes(li[0]));
  unsigned char e[3] = "é";
  printf("%d(%x) should be 2 bytes long: %d\n", e[0], e[0], num_bytes(e[0]));



*/

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters José
// Hi José, your name is 5 characters long according to strlen.
// The first character by indexing is J
// 5 characters long? That seems weird
/*
    74(4a) 111(6f) 115(73) 195(c3) 169(a9)

    4a -> (0100) (1010) // start with 0!
    6f -> (0110) (1111) // start with 0!
    73 -> (0111) (0011) // start with 0!
    c3 -> (1100) (0011) // start with 1!
    a9 -> (1010) (1001) // start with 1!
*/

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters Ülo
// Hi Ülo, your name is 4 characters long according to strlen.
// The first character by indexing is �
// What is this question mark character?

// @jpolitz ➜ /workspaces/week2 (main) $ ./characters 李
// Hi 李, your name is 3 characters long according to strlen.
// The first character by indexing is �
// Too long and question mark
/*
    230(e6) 157(9d) 142(8e)

    e6 -> (1110) (0110)
    9d -> (1001) (1100)
    8e -> (1000) (1110)
*/