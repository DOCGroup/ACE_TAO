// $Id$

// Handles parsing the Options provided to the user.

/* Copyright (C) 1989 Free Software Foundation, Inc.
   written by Douglas C. Schmidt (schmidt@ics.uci.edu)

This file is part of GNU GPERF.

GNU GPERF is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 1, or (at your option) any
later version.

GNU GPERF is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU GPERF; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111,
USA.  */

#include "Options.h"

ACE_RCSID(src, Options, "$Id$")

#if defined (ACE_HAS_GPERF)

#include "ace/Get_Opt.h"
#include "Iterator.h"

// Global option coordinator for the entire program.
Options option;

// Current program version.
extern char *version_string;

// Size to jump on a collision.
static const int DEFAULT_JUMP_VALUE = 5;

// Default name for generated lookup function.
static const char *const DEFAULT_NAME = "in_word_set";

// Default name for the key component.
static const char *const DEFAULT_KEY = "name";

// Default name for the generated class.
static const char *const DEFAULT_CLASS_NAME = "Perfect_Hash";

// Default name for generated hash function.
static const char *const DEFAULT_HASH_NAME = "hash";

// Default delimiters that separate keywords from their attributes.
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

// Prints program usage to standard error stream.

void
Options::usage (void)
{
  ACE_ERROR ((LM_ERROR,
              "Usage: %n [-acCdDef[num]gGhH<hashname>i<init>IjJ"
              "k<keys>K<keyname>lL<language>mMnN<function name>o"
              "Oprs<size>S<switches>tTvVZ<class name>].\n"
              "(type %n -h for help)\n"));
}

// Output command-line Options.

void
Options::print_options (void)
{
  int i;

  ACE_OS::printf ("/* Command-line: ");

  for (i = 0; i < argument_count; i++)
    ACE_OS::printf ("%s ", 
                    argument_vector[i]);

  ACE_OS::printf (" */");
}

// Sorts the key positions *IN REVERSE ORDER!!* This makes further
// routines more efficient.  Especially when generating code.  Uses a
// simple Insertion Sort since the set is probably ordered.  Returns 1
// if there are no duplicates, 0 otherwise.

int
Options::key_sort (char *base, int len)
{
  int i, j;

  for (i = 0, j = len - 1; i < j; i++)
    {
      int curr, tmp;

      for (curr = i + 1, tmp = base[curr];
           curr > 0 && tmp >= base[curr - 1];
           curr--)
        if ((base[curr] = base[curr - 1]) == tmp) 
          // Oh no, a duplicate!!!
          return 0;

      base[curr] = tmp;
    }

  return 1;
}

// Sets the default Options.

Options::Options (void)
{
  key_positions[0] = WORD_START;
  key_positions[1] = WORD_END;
  key_positions[2] = EOS;
  total_keysig_size = 2;
  delimiters = DEFAULT_DELIMITERS;
  jump = DEFAULT_JUMP_VALUE;
  option_word = DEFAULTCHARS | C;
  function_name = DEFAULT_NAME;
  key_name = DEFAULT_KEY;
  hash_name = DEFAULT_HASH_NAME;
  class_name = DEFAULT_CLASS_NAME;
  total_switches = size = 1;
  initial_asso_value = iterations = 0;
}

// Dumps option status when debug is set.

Options::~Options (void)
{
  if (ACE_BIT_ENABLED (option_word, DEBUG))
    {
      char *ptr;

      ACE_OS::fprintf (stderr, "\ndumping Options:\nDEBUG is.......: %s\nORDER is.......: %s"
                       "\nANSI is........: %s\nTYPE is........: %s\nINLINE is.........: %s"
                       "\nRANDOM is......: %s\nDEFAULTCHARS is: %s\nSWITCH is......: %s"
                       "\nPOINTER is.....: %s\nNOLENGTH is....: %s\nLENTABLE is....: %s"
                       "\nDUP is.........: %s\nFAST is........: %s\nCOMP is.....: %s"
                       "\nNOTYPE is......: %s\nGLOBAL is......: %s\nCONSTANT is....: %s"
                       "\nCPLUSPLUS is...: %s\nC is...........: %s\nENUM is.....: %s"
                       "\nSTRCASECMP is...: %s\nOPTIMIZE is...........: %s"
                       "\niterations = %d\nlookup function name = %s\nhash function name = %s"
                       "\nkey name = %s\njump value = %d\nmax associcated value = %d"
                       "\ninitial associated value = %d\ndelimiters = %s\nnumber of switch statements = %d\n",
                       ACE_BIT_ENABLED (option_word, DEBUG) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, ORDER) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, ANSI) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, TYPE) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, INLINE) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, RANDOM) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, DEFAULTCHARS) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, SWITCH) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, POINTER) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, NOLENGTH) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, LENTABLE) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, DUP) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, FAST) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, COMP) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, NOTYPE) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, GLOBAL) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, CONSTANT) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, CPLUSPLUS) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, C) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, ENUM) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, STRCASECMP) ? "enabled" : "disabled",
                       ACE_BIT_ENABLED (option_word, OPTIMIZE) ? "enabled" : "disabled",
                       iterations, function_name, hash_name, key_name, jump, size - 1,
                       initial_asso_value, delimiters, total_switches);
      if (ACE_BIT_ENABLED (option_word, ALLCHARS))
        ACE_OS::fprintf (stderr, "all characters are used in the hash function\n");

      ACE_OS::fprintf (stderr, "maximum keysig size = %d\nkey positions are: \n",
               total_keysig_size);

      for (ptr = key_positions; *ptr != EOS; ptr++)
        if (*ptr == WORD_END)
          ACE_OS::fprintf (stderr, "$\n");
        else
          ACE_OS::fprintf (stderr, "%d\n", *ptr);

      ACE_OS::fprintf (stderr, "finished dumping Options\n");
    }
}

// Parses the command line Options and sets appropriate flags in option_word.

void
Options::operator() (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt getopt (argc, argv, "adcCDe:Ef:gGhH:i:IJj:k:K:lL:mMnN:oOprs:S:tTvZ:");
  int option_char;

  argument_count  = argc;
  argument_vector = argv;

  while ((option_char = getopt ()) != -1)
    {
      switch (option_char)
        {
          // Generated coded uses the ANSI prototype format.
        case 'a':               
          {
            ACE_SET_BITS (option_word, ANSI);
            break;
          }
        // Generate strncmp rather than strcmp.
        case 'c':               
          {
            ACE_SET_BITS (option_word, COMP);
            break;
          }
        // Make the generated tables readonly (const).
        case 'C':               
          {
            ACE_SET_BITS (option_word, CONSTANT);
            break;
          }
        // Enable debugging option.
        case 'd':               
          {
            ACE_SET_BITS (option_word, DEBUG);
            ACE_ERROR ((LM_ERROR,
                        "Starting program %n, version %s, with debuggin on.\n",
                        version_string));
            break;
          }
        // Enable duplicate option.
        case 'D':               
          {
            ACE_SET_BITS (option_word, DUP);
            break;
          }
        // Allows user to provide keyword/attribute separator
        case 'e': 
          {
            option.delimiters = getopt.optarg;
            break;
          }
        case 'E':
          {
            ACE_SET_BITS (option_word, ENUM);
            break;
          }
        // Generate the hash table ``fast.''
        case 'f':               
          {
            ACE_SET_BITS (option_word, FAST);
            if ((iterations = atoi (getopt.optarg)) < 0)
              {
                ACE_ERROR ((LM_ERROR, "iterations value must not be negative, assuming 0\n"));
                iterations = 0;
              }
            break;
          }
        // Use the ``inline'' keyword for generated sub-routines.
        case 'g':               
          {
            ACE_SET_BITS (option_word, INLINE);
            break;
          }
        // Make the keyword table a global variable.
        case 'G':               
          {
            ACE_SET_BITS (option_word, GLOBAL);
            break;
          }
        // Displays a list of helpful Options to the user.
        case 'h':               
          {
            ACE_OS::fprintf (stderr,
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
                             "-g\tMake generated routines use ``inline'' to remove function overhead.\n"
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
                             "-J\tSkips '#include <string.h>' part in the output.\n"
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
                             "-m\tAvoids the warning about identical hash values. This is valid\n"
                             "\tonlyif the -D option is enabled.\n"
                             "-MSkips class definition in the output. This is valid only in C++ mode.\n"
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
                             "-v\tPrints out the current version number and exits with a value of 0\n"
                             "-v\tExits silently with a value of 0.\n"
                             "-Z\tAllow user to specify name of generated C++ class.  Default\n"
                             "\tname is `Perfect_Hash.'\n",
                             DEFAULT_JUMP_VALUE,
                             MAX_KEY_POS - 1);
            Options::usage ();
            ACE_OS::_exit (1);
          }
        // Sets the name for the hash function.
        case 'H':               
          {
            hash_name = getopt.optarg;
            break;
          }
        // Sets the initial value for the associated values array.
        case 'i':               
          {
            initial_asso_value = atoi (getopt.optarg);
            if (initial_asso_value < 0)
              ACE_ERROR ((LM_ERROR,
                          "Initial value %d should be non-zero, ignoring and continuing.\n",
                          initial_asso_value));
            if (option[RANDOM])
              ACE_ERROR ((LM_ERROR,
                          "warning, -r option superceeds -i, ignoring -i option and continuing\n"));
            break;
          }
         case 'I':
           {
             ACE_SET_BITS (option_word, STRCASECMP);
             break;
           }
         // Sets the jump value, must be odd for later algorithms.
        case 'j':               
          {
            jump = atoi (getopt.optarg);
            if (jump < 0)
              ACE_ERROR ((LM_ERROR,
                          "Jump value %d must be a positive number.\n%r%a",
                          jump,
                          &Options::usage,
                          1));
            else if (jump && ACE_EVEN (jump))
              ACE_ERROR ((LM_ERROR,
                          "Jump value %d should be odd, adding 1 and continuing...\n",
                          jump++));
            break;
          }
        // Skip including the header file string.h.
        case 'J':         
          {
            ACE_SET_BITS (option_word, SKIPSTRINGH);
            break;
          }
        // Sets key positions used for hash function.
        case 'k':               
          {
            const int BAD_VALUE = -1;
            int value;
            Iterator expand (getopt.optarg,
                             1,
                             MAX_KEY_POS - 1,
                             WORD_END,
                             BAD_VALUE,
                             EOS);

            // Use all the characters for hashing!!!!
            if (*getopt.optarg == '*') 
              option_word = (option_word & ~DEFAULTCHARS) | ALLCHARS;
            else
              {
                char *l_key_pos;

                for (l_key_pos = key_positions;
                     (value = expand ()) != EOS;
                     l_key_pos++)
                  if (value == BAD_VALUE)
                    ACE_ERROR ((LM_ERROR,
                                "Illegal key value or range, use 1,2,3-%d,'$' or '*'.\n%r%a",
                                MAX_KEY_POS - 1,
                                usage,
                                1));
                  else
                    *l_key_pos = value;;

                *l_key_pos = EOS;

                total_keysig_size = (l_key_pos - key_positions);
                if (total_keysig_size == 0)
                  ACE_ERROR ((LM_ERROR, 
                              "No keys selected.\n%r%a",
                              &Options::usage,
                              1));
                else if (key_sort (key_positions, total_keysig_size) == 0)
                  ACE_ERROR ((LM_ERROR,
                              "Duplicate keys selected\n%r%a",
                              &Options::usage,
                              1));

                if (total_keysig_size != 2
                    || (key_positions[0] != 1 || key_positions[1] != WORD_END))
                  option_word &= ~DEFAULTCHARS;
              }
            break;
          }
        // Make this the keyname for the keyword component field.
        case 'K':               
          {
            key_name = getopt.optarg;
            break;
          }
        // Create length table to avoid extra string compares.
        case 'l':               
          {
            ACE_SET_BITS (option_word, LENTABLE);
            break;
          }
        // Deal with different generated languages.
        case 'L':               
          {
            option_word &= ~C;
            if (!strcmp (getopt.optarg, "C++"))
              ACE_SET_BITS (option_word, (CPLUSPLUS | ANSI));
            else if (!strcmp (getopt.optarg, "C"))
              ACE_SET_BITS (option_word, C);
            else
              {
                ACE_ERROR ((LM_ERROR,
                            "unsupported language option %s, defaulting to C\n",
                            getopt.optarg));
                ACE_SET_BITS (option_word, C);
              }
            break;
          }
        // Don't print the warnings.
        case 'm':           
          {
            ACE_SET_BITS (option_word, MUTE);
            break;
          }
        // Skip the class definition while in C++ mode.
        case 'M': 
          {
            ACE_SET_BITS (option_word, SKIPCLASS);
            break;
          }
        // Don't include the length when computing hash function.
        case 'n':               
          {
            ACE_SET_BITS (option_word, NOLENGTH);
            break;
          }
        // Make generated lookup function name be optarg
        case 'N':               
          {
            function_name = getopt.optarg;
            break;
          }
        // Order input by frequency of key set occurrence.
        case 'o':               
          {
            ACE_SET_BITS (option_word, ORDER);
            break;
          }
 	case 'O':
 	  {
 	    ACE_SET_BITS (option_word, OPTIMIZE);
 	    break;
 	  }
        // Generated lookup function now a pointer instead of int.
        case 'p':               
          {
            ACE_SET_BITS (option_word, POINTER);
            break;
          }
        // Utilize randomness to initialize the associated values
        // table.
        case 'r':               
          {
            ACE_SET_BITS (option_word, RANDOM);
            if (option.initial_asso_value != 0)
              ACE_ERROR ((LM_ERROR,
                          "warning, -r option superceeds -i, disabling -i option and continuing\n"));
            break;
          }
        // Range of associated values, determines size of final table.
        case 's':         
          {
            size = atoi (getopt.optarg);
            if (abs (size) > 50)
              ACE_ERROR ((LM_ERROR,
                          "%d is excessive, did you really mean this?! (type %n -h for help)\n",
                          size));
            break;
          }
        // Generate switch statement output, rather than lookup table.
        case 'S':         
          {
            ACE_SET_BITS (option_word, SWITCH);
            option.total_switches = atoi (getopt.optarg);
            if (option.total_switches <= 0)
              ACE_ERROR ((LM_ERROR, 
                          "number of switches %s must be a positive number\n%r%a",
                          getopt.optarg,
                          &Options::usage,
                          1));
            break;
          }
        // Enable the TYPE mode, allowing arbitrary user structures.
        case 't':          
          {
            ACE_SET_BITS (option_word, TYPE);
            break;
          }
        // Don't print structure definition.
        case 'T':               
          {
            ACE_SET_BITS (option_word, NOTYPE);
            break;
          }
        // Print out the version and quit.
        case 'v':               
          ACE_ERROR ((LM_ERROR,
                      "%n: version %s\n%r\n%a",
                      version_string,
                      &Options::usage,
                      0));
          /* NOTREACHED */
          break;
        // Exit with value of 0 (this is useful to check if gperf exists)
        case 'V':
          ACE_OS::exit (0);
          /* NOTREACHED */
          break;
        // Set the class name.
        case 'Z':               
          {
            class_name = getopt.optarg;
            break;
          }
        default:
          ACE_ERROR ((LM_ERROR,
                      "%r%a",
                      &Options::usage,
                      1));
        }

    }

  if (argv[getopt.optind] && 
      freopen (argv[getopt.optind],
               "r",
               stdin) == 0)
    ACE_ERROR ((LM_ERROR, 
                "Cannot open keyword file %p\n%r%a",
                argv[getopt.optind],
                &Options::usage,
                1));

  if (++getopt.optind < argc)
    ACE_ERROR ((LM_ERROR,
                "Extra trailing arguments to %n.\n%r%a",
                usage,
                1));
}

// True if option enable, else false.

int
Options::operator[] (Option_Type option)
{
  return option_word & option;
}

// Enables option OPT.

void
Options::operator = (enum Option_Type opt)
{
  ACE_SET_BITS (option_word, opt);
}

// Disables option OPT.

void
Options::operator != (enum Option_Type opt)
{
  ACE_CLR_BITS (option_word, opt);
}

// Initializes the key Iterator.

void
Options::reset (void)
{
  key_pos = 0;
}

// Returns current key_position and advanced index.

int
Options::get (void)
{
  return key_positions[key_pos++];
}

// Sets the size of the table size.

void
Options::set_asso_max (int r)
{
  size = r;
}

// Returns the size of the table size.

int
Options::get_asso_max (void)
{
  return size;
}

// Returns total distinct key positions.

int
Options::get_max_keysig_size (void)
{
  return total_keysig_size;
}

// Sets total distinct key positions.

void
Options::set_keysig_size (int a_size)
{
  total_keysig_size = a_size;
}

// Returns the jump value.

int
Options::get_jump (void)
{
  return jump;
}

// Returns the generated function name.

const char *
Options::get_function_name (void)
{
  return function_name;
}

// Returns the keyword key name.

const char *
Options::get_key_name (void)
{
  return key_name;
}

// Returns the hash function name.

const char *
Options::get_hash_name (void)
{
  return hash_name;
}

// Returns the generated class name.

const char *
Options::get_class_name (void)
{
  return class_name;
}

// Returns the initial associated character value.

int
Options::initial_value (void)
{
  return initial_asso_value;
}

// Returns the iterations value.

int
Options::get_iterations (void)
{
  return iterations;
}

// Returns the string used to delimit keywords from other attributes.

const char *
Options::get_delimiter (void)
{
  return delimiters;
}

// Gets the total number of switch statements to generate.

int
Options::get_total_switches (void)
{
  return total_switches;
}

#endif /* ACE_HAS_GPERF */
