// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux/CodeGen
//
// = FILENAME
//    gen_names.cpp
//
//    Generates unique names
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================


#include "codegen.h"

ACE_RCSID(CodeGen, gen_names, "$Id$")


// POA names generator
int 
Demux_Test_CodeGenerator::gen_poa_names (void)
{
  if (this->gen_names (this->poa_array_, this->num_POAs_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%n:%l) Demux_Test_CodeGenerator::gen_poa_names - "
                         "Failed to generate poa names\n"),
                        -1);
    }

  // We store the generated names in a file
  FILE *fp;

  if ((fp = ACE_OS::fopen ("poa_names.dat", "w")) == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%n:%l) Demux_Test_CodeGenerator::gen_poa_names - "
                         "Failed to open file\n"),
                        -1);
    }

  // now output the names
  ACE_CString *str;
  ACE_Unbounded_Queue_Iterator <ACE_CString> iter (this->poa_array_);

  // start with the first element
  (void)iter.first ();
  while (!iter.done ())
    {
      // grab the next element waiting to be grabbed
      (void) iter.next (str);
      
      ACE_OS::fprintf (fp, "%s\n", str->c_str ());

      (void) iter.advance ();
    }

  ACE_OS::fclose (fp);

  return 0;
}

// object table generator
int 
Demux_Test_CodeGenerator::gen_object_names (void)
{
  if (this->gen_names (this->obj_array_, this->num_objs_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%n:%l) Demux_Test_CodeGenerator::gen_object_names - "
                         "Failed to generate object names\n"),
                        -1);
    }
  return 0;
}

// method table generator
int 
Demux_Test_CodeGenerator::gen_operation_names (void)
{
  if (this->gen_names (this->op_array_, this->num_ops_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%n:%l) Demux_Test_CodeGenerator::gen_object_names - "
                         "Failed to generate object names\n"),
                        -1);
    }

  return 0;
}


// generic unique name generator
int 
Demux_Test_CodeGenerator::gen_names (ACE_Unbounded_Queue<ACE_CString> &arr, 
                                     int limit) 
{
  int i; // loop index

  // Generate "limit" number of distinct strings
  for (i = 0; i < limit; i++)
    {
      // inserts a new string at the tail in array "arr" 
      if (this->insert_unique_string (arr) == -1) 
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%n:%l) Demux_Test_CodeGenerator::gen_names - "
                             "Failed to insert unique name\n"),
                            -1);
        }
    }
  
  return 0;
}

// generate a distinct string and insert it at the tail
int 
Demux_Test_CodeGenerator::insert_unique_string (ACE_Unbounded_Queue<ACE_CString> &arr)
{
  long rnd;
  ACE_CString s;
  long status;

  // get a random number between 3 and 32
  rnd = this->gen_rand (3,32); 

  // This random number is used as a string length of the distinct string to be
  // generated. 

  do 
    {
      if (this->create_string (rnd, s) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%n:%l) Demux_Test_CodeGenerator::insert_unique_string - "
                             "Failed to create string\n"),
                            -1);
        }

      status = this->is_present (arr, s);
      if (status == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%n:%l) Demux_Test_CodeGenerator::insert_unique_string - "
                             "is_present check failed\n"),
                            -1);
        }
    } 
  while (status > 0);
  
  // save it at this location
  if (arr.enqueue_tail (s) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%n:%l) Demux_Test_CodeGenerator::insert_unique_string - "
                         "Failed to enqueue\n"),
                        -1);
    }  
  return 0;
}

// generate a random number in the given limits
long 
Demux_Test_CodeGenerator::gen_rand (int low, int up)
{
  return (lrand48 () % (up - low + 1)) + low;
}

// check if the given string is present in the array
int
Demux_Test_CodeGenerator:: is_present (ACE_Unbounded_Queue<ACE_CString> &arr,
                                       ACE_CString &s) 
{
  ACE_CString *str;
  ACE_Unbounded_Queue_Iterator <ACE_CString> iter (arr);

  // start with the first element
  if (!iter.first ())
    {
      // queue is empty
      return 0;
    }
  while (!iter.done ())
    {
      // grab the next element waiting to be grabbed
      (void) iter.next (str);

      // check if they match
      if (!str->compare (s))
        return 1;  // they match

      (void) iter.advance ();
    }
  return 0; 	// not present
}

// create a string of length = len
int 
Demux_Test_CodeGenerator::create_string (int len, ACE_CString &s)
{
  int i;
  char ch;

  for (i = 0; i < len; i++)
    {
      ch = this->gen_valid_char (i);
      s += ACE_CString (&ch);
    }
  return 0;
}

// Generate a valid character for a C++ variable
char 
Demux_Test_CodeGenerator::gen_valid_char (int posn)
{
  // for posn == 0, we cannot return a digit or an _ as the first character as
  // that does not constitute a valid IDL identifier
  char ch;
  while (1) 
    {
      ch = this->gen_rand (48, 122);
      if (isdigit (ch) || isupper (ch) || islower (ch) || (ch == '_'))
        {
          if ((posn == 0) && (isdigit (ch) || (ch == '_')))
            continue;
          break;	//otherwise
        }
    }
  return ch;
}

