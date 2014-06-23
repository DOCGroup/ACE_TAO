// $Id$

/**
 * This program checks if the compiler doesn't have a certain bug
 * that we encountered.
 */

#include "test_config.h"

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT("Compiler_Features_33_Test"));

  int retval = 1;
  char _tao_discriminant =  '\x0';

  switch (_tao_discriminant)
  {
    case '\x0':
    case '\x1':
    case '\x2':
    case '\x3':
    case '\x4':
    case '\x5':
    case '\x6':
    case '\a':
    case '\b':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case '\xe':
    case '\xf':
    case '\x10':
    case '\x11':
    case '\x12':
    case '\x13':
    case '\x14':
    case '\x15':
    case '\x16':
    case '\x17':
    case '\x18':
    case '\x19':
    case '\x1a':
    case '\x1b':
    case '\x1c':
    case '\x1d':
    case '\x1e':
    case '\x1f':
    case ' ':
    case '!':
    case '"':
    case '#':
    case '$':
    case '%':
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '-':
    case '.':
    case '/':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case ':':
    case ';':
    case '<':
    case '=':
    case '>':
    case '?':
    case '@':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '[':
    case '\\':
    case ']':
    case '^':
    case '_':
    case '`':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case '{':
    case '|':
    case '}':
    case '~':
    case '\x7f':
    case '\x80':
    case '\x81':
    case '\x82':
    case '\x83':
    case '\x84':
    case '\x85':
    case '\x86':
    case '\x87':
    case '\x88':
    case '\x89':
    case '\x8a':
    case '\x8b':
    case '\x8c':
    case '\x8d':
    case '\x8e':
    case '\x8f':
    case '\x90':
    case '\x91':
    case '\x92':
    case '\x93':
    case '\x94':
    case '\x95':
    case '\x96':
    case '\x97':
    case '\x98':
    case '\x99':
    case '\x9a':
    case '\x9b':
    case '\x9c':
    case '\x9d':
    case '\x9e':
    case '\x9f':
    case '\xa0':
    case '\xa1':
    case '\xa2':
    case '\xa3':
    case '\xa4':
    case '\xa5':
    case '\xa6':
    case '\xa7':
    case '\xa8':
    case '\xa9':
    case '\xaa':
    case '\xab':
    case '\xac':
    case '\xad':
    case '\xae':
    case '\xaf':
    case '\xb0':
    case '\xb1':
    case '\xb2':
    case '\xb3':
    case '\xb4':
    case '\xb5':
    case '\xb6':
    case '\xb7':
    case '\xb8':
    case '\xb9':
    case '\xba':
    case '\xbb':
    case '\xbc':
    case '\xbd':
    case '\xbe':
    case '\xbf':
    case '\xc0':
    case '\xc1':
    case '\xc2':
    case '\xc3':
    case '\xc4':
    case '\xc5':
    case '\xc6':
    case '\xc7':
    case '\xc8':
    case '\xc9':
    case '\xca':
    case '\xcb':
    case '\xcc':
    case '\xcd':
    case '\xce':
    case '\xcf':
    case '\xd0':
    case '\xd1':
    case '\xd2':
    case '\xd3':
    case '\xd4':
    case '\xd5':
    case '\xd6':
    case '\xd7':
    case '\xd8':
    case '\xd9':
    case '\xda':
    case '\xdb':
    case '\xdc':
    case '\xdd':
    case '\xde':
    case '\xdf':
    case '\xe0':
    case '\xe1':
    case '\xe2':
    case '\xe3':
    case '\xe4':
    case '\xe5':
    case '\xe6':
    case '\xe7':
    case '\xe8':
    case '\xe9':
    case '\xea':
    case '\xeb':
    case '\xec':
    case '\xed':
    case '\xee':
    case '\xef':
    case '\xf0':
    case '\xf1':
    case '\xf2':
    case '\xf3':
    case '\xf4':
    case '\xf5':
    case '\xf6':
    case '\xf7':
    case '\xf8':
    case '\xf9':
    case '\xfa':
    case '\xfb':
    case '\xfc':
    case '\xfd':
    case '\xfe':
    case '\xff':
      {
        // Test works
        retval = 0;
      }
      break;
    }

  if (retval != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ERROR: Switch doesn't worked as expected\n")));
  }
  else
  {
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Switch worked as expected\n")));
  }

  ACE_END_TEST;

  return retval;
}

