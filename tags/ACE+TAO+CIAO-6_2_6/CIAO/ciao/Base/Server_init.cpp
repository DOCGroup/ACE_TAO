// $Id$

#include "Server_init.h"

#include "ciao/Base/Client_init.h"
#include "ciao/Valuetype_Factories/Cookies.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  int
  Server_init (CORBA::ORB_ptr o)
  {
    Client_init (o);
    CIAO_REGISTER_VALUE_FACTORY (o,
                                 CIAO::Cookie_Impl_init,
                                 Components::Cookie);
    return 0;
  }
} /* namespace CIAO */
