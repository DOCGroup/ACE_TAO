
#include "test_i.h"
#include "ace/streams.h"

char *
test_impl::test_op (const char * in_arg)
{
  cout << "got " << ACE_OS::strlen (in_arg) << " bytes" << endl;
  return CORBA::string_dup (in_arg);
}