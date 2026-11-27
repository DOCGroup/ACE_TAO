#include "ace/streams.h"

#include "test_i.h"

#include <cstring>

char *
test_impl::test_op (char const *in_arg)
{
  cout << "servant got " << std::strlen (in_arg) << " bytes" << endl;
  return CORBA::string_dup (in_arg);
}
