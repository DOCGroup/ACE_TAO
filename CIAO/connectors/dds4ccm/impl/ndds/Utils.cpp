// $Id$

#include "Utils.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    const char * translate_instancehandle (DDS::InstanceHandle_t hnd)
      {
        char * ret = 0;
        ACE_OS::sprintf (ret,
                         "length <%l> - isValid <%l>",
                         hnd.length, hnd.isValid);
        return ret;
      }
  }
}
