/* Handles parsing the Options provided to the user.
// $Id$

   Copyright (C) 1989 Free Software Foundation, Inc.
   written by Douglas C. Schmidt (schmidt@ics.uci.edu)

This file is part of GNU GPERF.

GNU GPERF is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU GPERF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU GPERF; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA.  */

#include "ace/Get_Opt.h"
#include "Options.h"
#include "Iterator.h"

/* Global option coordinator for the entire program. */
Options option;       

/* Current program version. */
extern char *version_string;

/* Size to jump on a collision. */
static const int DEFAULT_JUMP_VALUE = 5;

/* Default name for generated lookup function. */
static const char *const DEFAULT_NAME = "in_word_set";

/* Default name for the key component. */
static const char *const DEFAULT_KEY = "name";

/* Default name for the generated class. */
static const char *const DEFAULT_CLASS_NAME = "Perfect_Hash";

/* Default name for generated hash function. */
static const char *const DEFAULT_HASH_NAME = "hash";

/* Default delimiters that separate keywords from their attributes. */
static const char *const DEFAULT_DELIMITERS = ",\n";

int Options::option_word;
int Options::total_switches;
int Options::total_keysig_size;
int Options::size;
int Options::key_pos;
int Options::jump;
int Options::initial_asso_value;
int Options::argument_count;
int Options::iterations;
char **Options::argument_vector;
const char *Options::function_name;
const char *Options::key_name;
const char *Options::class_name;
const char *Options::hash_name;
const char *Options::delimiters;
char Options::key_positions[MAX_KEY_POS];

/* Prints program usage to standard error stream. */

inline void 
Options::usage (void) 
{ 
  ACE_ERROR ((LM_ERROR, "Usage: %n [-acCdDef[num]gGhH<hashname>i<init>Ijk<keys>K<keyname>lL<language>nN<function name>oOprs<size>S<switches>tTvZ<class name>].\n"
                "(type %n -h for help)\n"));
}

/* Output command-line Options. */

void 
Options::print_options (void)
{ 
  int i;

  printf ("/* Command-line: ");

  for (i = 0; i < argument_count; i++) 
    printf ("%s ", argument_vector[i]);
   
  printf (" */");
}

/* Sorts the key positions *IN REVERSE ORDER!!*
   This makes further routines more efficient.  Especially when generating code.
   Uses a simple Insertion Sort since the set is probably ordered.
   Returns 1 if there are no duplicates, 0 otherwise. */

inline int 
Options::key_sort (char *base, int len) 
{
  int i, j;

  for (i = 0, j = len - 1; i < j; i++) 
    {
      int curr, tmp;
      
      for (curr = i + 1,tmp = base[curr]; curr > 0 && tmp >= base[curr - 1]; curr--) 
        if ((base[curr] = base[curr - 1]) == tmp) /* oh no, a duplicate!!! */
          return 0;

      base[curr] = tmp;
    }

  return 1;
}

/* Sets the default Options. */

Options::Options (void) 
{ 
  key_positions[0]    = WORD_START;
  key_positions[1]    = WORD_END;
  key_positions[2]    = EOS;
  total_keysig_size  = 2;
  delimiters          = DEFAULT_DELIMITERS;
  jump                = DEFAULT_JUMP_VALUE;
  option_word         = DEFAULTCHARS | C;
  function_name       = DEFAULT_NAME;
  key_name            = DEFAULT_KEY;
  hash_name           = DEFAULT_HASH_NAME;
  class_name          = DEFAULT_CLASS_NAME;
  total_switches      = size = 1;
  initial_asso_value  = iterations = 0;
}

/* Dumps option status when debug is set. */

Options::~Options (void) 
{ 
  if (option_word & DEBUG)
    {
      char *ptr;

      fprintf (stderr, "\ndumping Options:\nDEBUG is.......: %s\nORDER is.......: %s"
               "\nANSI is........: %s\nTYPE is........: %s\nGNU is.........: %s"
               "\nRANDOM is......: %s\nDEFAULTCHARS is: %s\nSWITCH is......: %s"
               "\nPOINTER is.....: %s\nNOLENGTH is....: %s\nLENTABLE is....: %s"
               "\nDUP is.........: %s\nFAST is........: %s\nCOMP is.....: %s"
               "\nNOTYPE is......: %s\nGLOBAL is......: %s\nCONST is....: %s"
               "\nCPLUSPLUS is...: %s\nC is...........: %s\nENUM is.....: %s"
               "\nSTRCASECMP is...: %s\nOPTIMIZE is...........: %s"
               "\niterations = %d\nlookup function name = %s\nhash function name = %s"
               "\nkey name = %s\njump value = %d\nmax associcated value = %d"
               "\ninitial associated value = %d\ndelimiters = %s\nnumber of switch statements = %d\n",
               option_word & DEBUG ? "enabled" : "disabled", 
               option_word & ORDER ? "enabled" : "disabled",
               option_word & ANSI ? "enabled" : "disabled",
               option_word & TYPE ? "enabled" : "disabled",
               option_word & GNU ? "enabled" : "disabled",
               option_word & RANDOM ? "enabled" : "disabled",
               option_word & DEFAULTCHARS ? "enabled" : "disabled",
               option_word & SWITCH ? "enabled" : "disabled",
               option_word & POINTER ? "enabled" : "disabled",
               option_word & NOLENGTH ? "enabled" : "disabled",
               option_word & LENTABLE ? "enabled" : "disabled",
               option_word & DUP ? "enabled" : "disabled",
               option_word & FAST ? "enabled" : "disabled",
               option_word & COMP ? "enabled" : "disabled",
               option_word & NOTYPE ? "enabled" : "disabled",
               option_word & GLOBAL ? "enabled" : "disabled",
               option_word & CONST ? "enabled" : "disabled",
               option_word & CPLUSPLUS ? "enabled" : "disabled",
               option_word & C ? "enabled" : "disabled",
               option_word & ENUM ? "enabled" : "disabled",
               option_word & STRCASECMP ? "enabled" : "disabled",
               option_word & OPTIMIZE ? "enabled" : "disabled",
               iterations, function_name, hash_name, key_name, jump, size - 1,
               initial_asso_value, delimiters, total_switches);
      if (option_word & ALLCHARS) 
        fprintf (stderr, "all characters are used in the hash function\n");

      fprintf (stderr, "maximum keysig size = %d\nkey positions are: \n",
               total_keysig_size);

      for (ptr = key_positions; *ptr != EOS; ptr++) 
        if (*ptr == WORD_END) 
          fprintf (stderr, "$\n");
        else 
          fprintf (stderr, "%d\n", *ptr);

      fprintf (stderr, "finished dumping Options\n");
    }
}


/* Parses the command line Options and sets appropriate flags in option_word. */

void 
Options::operator() (int argc, char *argv[])
{ 
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt getopt (argc, argv, "adcCDe:Ef:gGhH:i:Ij:k:K:lL:nN:oOprs:S:tTvZ:");
  int option_char;

  argument_count  = argc;
  argument_vector = argv;

  while ((option_char = getopt ()) != -1)
    {
      switch (option_char)
        {
        case 'a':               /* Generated coded uses the ANSI prototype format. */
          { 
            option_word |= ANSI;
            break;
          }
        case 'c':               /* Generate strncmp rather than strcmp. */
          {
            option_word |= COMP;
            break;
          }
        case 'C':               /* Make the generated tables readonly (const). */
          {
            option_word |= CONST;
            break;
          }
        case 'd':               /* Enable debugging option. */
          { 
            option_word |= DEBUG;
            ACE_ERROR ((LM_ERROR, "Starting program %n, version %s, with debuggin on.\n",
                          version_string));
            break;
          }   
        case 'D':               /* Enable duplicate option. */
          { 
            option_word |= DUP;
            break;
          }   
        case 'e': /* Allows user to provide keyword/attribute separator */
          {
            option.delimiters = getopt.optarg;
            break;
          }
        case 'E':
          {
            option_word |= ENUM;
            break;
          }
        case 'f':               /* Generate the hash table ``fast.'' */
          {
            option_word |= FAST;
            if ((iterations = atoi (getopt.optarg)) < 0) 
              {
                ACE_ERROR ((LM_ERROR, "iterations value must not be negative, assuming 0\n"));
                iterations = 0;
              }
            break;
          }
        case 'g':               /* Use the ``inline'' keyword for generated sub-routines. */
          { 
            option_word |= GNU;
            break;
          }
        case 'G':               /* Make the keyword table a global variable. */
          { 
                                                option_word |= GLOBAL;
            break;
          }
        case 'h':               /* Displays a list of helpful Options to the user. */
          { 
            ACE_ERROR ((LM_ERROR, 
                          "-a\tGenerate ANSI standard C output code, i.e., function prototypes.\n"
                          "-c\tGenerate comparison code using strncmp rather than strcmp.\n"
                          "-C\tMake the contents of generated lookup tables constant, i.e., readonly.\n"
                          "-d\tEnables the debugging option (produces verbose output to the standard error).\n"
                          "-D\tHandle keywords that hash to duplicate values.  This is useful\n"
                          "\tfor certain highly redundant keyword sets.  It enables the -S option.\n"
                          "-e\tAllow user to provide a string containing delimiters used to separate\n"
                          "\tkeywords from their attributes.  Default is \",\\n\"\n"
                          "-E\tDefine constant values using an enum local to the lookup function\n"
                          "\trather than with defines\n"
                          "-f\tGenerate the gen-perf.hash function ``fast.''  This decreases GPERF's\n"
                          "\trunning time at the cost of minimizing generated table-size.\n"
                          "\tThe numeric argument represents the number of times to iterate when\n"
                          "\tresolving a collision.  `0' means ``iterate by the number of keywords.''\n"
                          "-g\tAssume a GNU compiler, e.g., g++ or gcc.  This makes all generated\n"
                          "\troutines use the ``inline'' keyword to remove cost of function calls.\n"
                          "-G\tGenerate the static table of keywords as a static global variable,\n"
                          "\trather than hiding it inside of the lookup function (which is the\n"
                          "\tdefault behavior).\n"
                          "-h\tPrints this mesage.\n"
                          "-H\tAllow user to specify name of generated hash function. Default\n"
                          "\tis `hash'.\n"
                          "-i\tProvide an initial value for the associate values array.  Default is 0.\n"
			  "-I\tGenerate comparison code using case insensitive string comparison, e.g.,\n"
			  "\tstrncasecmp or strcasecmp.\n"
                          "\tSetting this value larger helps inflate the size of the final table.\n"
                          "-j\tAffects the ``jump value,'' i.e., how far to advance the associated\n"
                          "\tcharacter value upon collisions.  Must be an odd number, default is %d.\n"
                          "-k\tAllows selection of the key positions used in the hash function.\n"
                          "\tThe allowable choices range between 1-%d, inclusive.  The positions\n"
                          "\tare separated by commas, ranges may be used, and key positions may\n"
                          "\toccur in any order.  Also, the meta-character '*' causes the generated\n"
                          "\thash function to consider ALL key positions, and $ indicates the\n"
                          "\t``final character'' of a key, e.g., $,1,2,4,6-10.\n"
                          "-K\tAllow use to select name of the keyword component in the keyword structure.\n"
                          "-l\tCompare key lengths before trying a string comparison.  This helps\n"
                          "\tcut down on the number of string comparisons made during the lookup.\n"
                          "-L\tGenerates code in the language specified by the option's argument.  Languages\n"
                          "\thandled are currently C++ and C.  The default is C.\n"
                          "-n\tDo not include the length of the keyword when computing the hash function\n"
                          "-N\tAllow user to specify name of generated lookup function.  Default\n"
                          "\tname is `in_word_set.'\n"
                          "-o\tReorders input keys by frequency of occurrence of the key sets.\n"
                          "\tThis should decrease the search time dramatically.\n"
			  "-O\tOptimize the generated lookup function by assuming that all input keywords \n"
			  "\tare members of the keyset from the keyfile.\n"
                          "-p\tChanges the return value of the generated function ``in_word_set''\n"
                          "\tfrom its default boolean value (i.e., 0 or 1), to type ``pointer\n"
                          "\tto wordlist array''  This is most useful when the -t option, allowing\n"
                          "\tuser-defined structs, is used.\n"
                          "-r\tUtilizes randomness to initialize the associated values table.\n"
                          "-s\tAffects the size of the generated hash table.  The numeric argument\n"
                          "\tfor this option indicates ``how many times larger or smaller'' the associated\n"
                          "\tvalue range should be, in relationship to the number of keys, e.g. a value of 3\n"
                          "\tmeans ``allow the maximum associated value to be about 3 times larger than the\n"
                          "\tnumber of input keys.''  Conversely, a value of -3 means ``make the maximum\n"
                          "\tassociated value about 3 times smaller than the number of input keys.\n"
                          "\tA larger table should decrease the time required for an unsuccessful search,\n"
                          "\tat the expense of extra table space.  Default value is 1.\n"
                          "-S\tCauses the generated C code to use a switch statement scheme, rather\n"
                          "\tthan an array lookup table.  This can lead to a reduction in both\n"
                          "\ttime and space requirements for some keyfiles.  The argument to\n"
                          "\tthis option determines how many switch statements are generated.\n"
                          "\tA value of 1 generates 1 switch containing all the elements, a value of 2\n"
                          "\tgenerates 2 tables with 1/2 the elements in each table, etc.  This\n"
                          "\tis useful since many C compilers cannot correctly generate code for\n"
                          "\tlarge switch statements.\n"
                          "-t\tAllows the user to include a structured type declaration for \n"
                          "\tgenerated code. Any text before %%%% is consider part of the type\n"
                          "\tdeclaration.  Key words and additional fields may follow this, one\n"
                          "\tgroup of fields per line.\n"
                          "-T\tPrevents the transfer of the type declaration to the output file.\n"
                          "\tUse this option if the type is already defined elsewhere.\n"
                          "-v\tPrints out the current version number\n"
                          "-Z\tAllow user to specify name of generated C++ class.  Default\n"
                          "\tname is `Perfect_Hash.'\n%e%a", DEFAULT_JUMP_VALUE, (MAX_KEY_POS - 1), usage, 1));
          }
        case 'H':               /* Sets the name for the hash function */
          {
            hash_name = getopt.optarg;
            break;
          }
        case 'i':               /* Sets the initial value for the associated values array. */
          { 
            if ((initial_asso_value = atoi (getopt.optarg)) < 0) 
              ACE_ERROR ((LM_ERROR, "Initial value %d should be non-zero, ignoring and continuing.\n", initial_asso_value));
            if (option[RANDOM])
              ACE_ERROR ((LM_ERROR, "warning, -r option superceeds -i, ignoring -i option and continuing\n"));
            break;
          }
         case 'I':
           {
             option_word |= STRCASECMP;
             break;
           }
        case 'j':               /* Sets the jump value, must be odd for later algorithms. */
          { 
            if ((jump = atoi (getopt.optarg)) < 0)
              ACE_ERROR ((LM_ERROR, "Jump value %d must be a positive number.\n%e%a", jump, usage, 1));
            else if (jump && ACE_EVEN (jump))
              ACE_ERROR ((LM_ERROR, "Jump value %d should be odd, adding 1 and continuing...\n", jump++));
            break;
          }
        case 'k':               /* Sets key positions used for hash function. */
          { 
            const int BAD_VALUE = -1;
            int       value;
            Iterator  expand (getopt.optarg, 1, MAX_KEY_POS - 1, WORD_END, BAD_VALUE, EOS);
            
            if (*getopt.optarg == '*') /* Use all the characters for hashing!!!! */
              option_word = (option_word & ~DEFAULTCHARS) | ALLCHARS;
            else 
              {
                char *l_key_pos;

                for (l_key_pos = key_positions; (value = expand ()) != EOS; l_key_pos++) 
                  if (value == BAD_VALUE)
                    ACE_ERROR ((LM_ERROR, "Illegal key value or range, use 1,2,3-%d,'$' or '*'.\n%e%a",
                                   MAX_KEY_POS - 1, usage, 1));
                  else 
                    *l_key_pos = value;;

                *l_key_pos = EOS;

                if (! (total_keysig_size = (l_key_pos - key_positions))) 
                  ACE_ERROR ((LM_ERROR, "No keys selected.\n%e%a", usage, 1));
                else if (! key_sort (key_positions, total_keysig_size))
                  ACE_ERROR ((LM_ERROR, "Duplicate keys selected\n%e%a", usage, 1));

                if (total_keysig_size != 2 
                    || (key_positions[0] != 1 || key_positions[1] != WORD_END))
                  option_word &= ~DEFAULTCHARS;
              }
            break;
          }
        case 'K':               /* Make this the keyname for the keyword component field. */
          {
            key_name = getopt.optarg;
            break;
          }
        case 'l':               /* Create length table to avoid extra string compares. */
          { 
            option_word |= LENTABLE;
            break;
          }
        case 'L':               /* Deal with different generated languages. */
          {
            option_word &= ~C;
            if (!strcmp (getopt.optarg, "C++"))
              option_word |= (CPLUSPLUS | ANSI);
            else if (!strcmp (getopt.optarg, "C"))
              option_word |= C;
            else 
              {
                ACE_ERROR ((LM_ERROR, "unsupported language option %s, defaulting to C\n", getopt.optarg));
                option_word |= C;
              }
            break;
          }
        case 'n':               /* Don't include the length when computing hash function. */
          { 
            option_word |= NOLENGTH;
            break; 
          }
        case 'N':               /* Make generated lookup function name be optarg */
          { 
            function_name = getopt.optarg;
            break;
          }
        case 'o':               /* Order input by frequency of key set occurrence. */
          { 
            option_word |= ORDER;
            break;
          }   
 	case 'O':
 	  {
 	    option_word |= OPTIMIZE;
 	    break;
 	  }
        case 'p':               /* Generated lookup function now a pointer instead of int. */
          { 
            option_word |= POINTER;
            break;
          }
        case 'r':               /* Utilize randomness to initialize the associated values table. */
          { 
            option_word |= RANDOM;
            if (option.initial_asso_value != 0)
              ACE_ERROR ((LM_ERROR, "warning, -r option superceeds -i, disabling -i option and continuing\n"));
            break;
          }
        case 's':               /* Range of associated values, determines size of final table. */
          { 
            if (abs (size = atoi (getopt.optarg)) > 50) 
              ACE_ERROR ((LM_ERROR, "%d is excessive, did you really mean this?! (type %n -h for help)\n", size));
            break; 
          }       
        case 'S':               /* Generate switch statement output, rather than lookup table. */
          { 
            option_word |= SWITCH;
            if ((option.total_switches = atoi (getopt.optarg)) <= 0)
              ACE_ERROR ((LM_ERROR, "number of switches %s must be a positive number\n%e%a", getopt.optarg, usage, 1));
            break;
          }
        case 't':               /* Enable the TYPE mode, allowing arbitrary user structures. */
          { 
            option_word |= TYPE;
            break;
          }
                                case 'T':   /* Don't print structure definition. */
                                        {
                                                option_word |= NOTYPE;
                                                break;
                                        }
        case 'v':               /* Print out the version and quit. */
          ACE_ERROR ((LM_ERROR, "%n: version %s\n%e\n%a", version_string, usage, 1));
        case 'Z':               /* Set the class name. */
          {
            class_name = getopt.optarg;
            break;
          }
        default: 
          ACE_ERROR ((LM_ERROR, "%e%a", usage, 1));
        }
      
    }

  if (argv[getopt.optind] && ! freopen (argv[getopt.optind], "r", stdin))
    ACE_ERROR ((LM_ERROR, "Cannot open keyword file %p\n%e%a", argv[getopt.optind], usage, 1));
  
  if (++getopt.optind < argc) 
    ACE_ERROR ((LM_ERROR, "Extra trailing arguments to %n.\n%e%a", usage, 1));
}

int  
Options::operator[] (Option_Type option) /* True if option enable, else false. */
{ 
  return option_word & option;
}

void
Options::operator = (enum Option_Type opt) /* Enables option OPT. */
{
  option_word |= opt;
}

void
Options::operator != (enum Option_Type opt) /* Disables option OPT. */
{
  option_word &= ~opt;
}

void 
Options::reset (void) /* Initializes the key Iterator. */
{ 
  key_pos = 0;
}

int 
Options::get (void) /* Returns current key_position and advanced index. */
{ 
  return key_positions[key_pos++];
}

void 
Options::set_asso_max (int r) /* Sets the size of the table size. */
{ 
  size = r;
}

int 
Options::get_asso_max (void) /* Returns the size of the table size. */
{ 
  return size;
}

int 
Options::get_max_keysig_size (void) /* Returns total distinct key positions. */
{ 
  return total_keysig_size;
}

void
Options::set_keysig_size (int a_size) /* Sets total distinct key positions. */
{ 
  total_keysig_size = a_size;
}

int 
Options::get_jump (void) /* Returns the jump value. */
{ 
  return jump;
}

const char *
Options::get_function_name (void) /* Returns the generated function name. */
{ 
  return function_name;
}

const char *
Options::get_key_name (void) /* Returns the keyword key name. */
{
  return key_name;
}

const char *
Options::get_hash_name (void) /* Returns the hash function name. */
{
  return hash_name;
}

const char *
Options::get_class_name (void) /* Returns the generated class name. */
{
  return class_name;
}

int 
Options::initial_value (void) /* Returns the initial associated character value. */
{ 
  return initial_asso_value;
}

int 
Options::get_iterations (void) /* Returns the iterations value. */
{ 
  return iterations;
}

const char *
Options::get_delimiter () /* Returns the string used to delimit keywords from other attributes. */
{
  return delimiters;
}

int
Options::get_total_switches () /* Gets the total number of switch statements to generate. */
{
  return total_switches;
}




