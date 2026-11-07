// -*- C++ -*-
#include "Interceptor_Type.h"
#include "ace/OS_NS_string.h"

Interceptor_Type
get_interceptor_type (int argc, ACE_TCHAR *argv[])
{
  Interceptor_Type interceptor_type { Interceptor_Type::none };
  for (int i = 1; i < argc - 1; ++i)
    {
      if (ACE_OS::strcmp (argv[i], ACE_TEXT ("-r")) == 0)
        {
          if (ACE_OS::strcmp (argv[i+1], ACE_TEXT ("none")) == 0)
            interceptor_type = Interceptor_Type::none;
          else if (ACE_OS::strcmp (argv[i + 1], ACE_TEXT ("noop")) == 0)
            interceptor_type = Interceptor_Type::noop;
          else if (ACE_OS::strcmp (argv[i + 1], ACE_TEXT ("context")) == 0)
            interceptor_type = Interceptor_Type::context;
          else if (ACE_OS::strcmp (argv[i + 1], ACE_TEXT ("dynamic")) == 0)
            interceptor_type = Interceptor_Type::dynamic;
        }
    }
  return interceptor_type;
}
