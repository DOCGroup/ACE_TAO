
// $Id$

#include "test_i.h"

char * 
foo_i::all_str (const char *inarg,
                char *& inoutarg,
                CORBA::String_out outarg
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  cout << "inarg: " << inarg << endl;
  cout << "inoutarg: " << inoutarg << endl;

  const char *new_inout = "new_inout";
  const char *new_out = "out";
  const char *new_retval = "return";

  CORBA::string_free (inoutarg);

  inoutarg = CORBA::string_alloc (ACE_OS::strlen (new_inout));
  outarg = CORBA::string_alloc (ACE_OS::strlen (new_out));
  char *retval = CORBA::string_alloc (ACE_OS::strlen (new_retval));

  ACE_OS::sprintf (inoutarg, "%s", new_inout);
  ACE_OS::sprintf (outarg.ptr (), "%s", new_out);
  ACE_OS::sprintf (retval, "%s", new_retval);

  return retval;
}
