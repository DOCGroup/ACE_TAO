#include "ciao/CIAO_common.h"
#include "ciao/Dynamic_Component_Servant_Base.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (ciao,
           Dynamic_Component_Servant_Base,
           "$Id$")

namespace CIAO
{
  Dynamic_Component_Servant_Base::Dynamic_Component_Servant_Base (void)
    : container_ (0)
  {
    ACE_ASSERT (0);
  }

  Dynamic_Component_Servant_Base::Dynamic_Component_Servant_Base
    (Session_Container * c)
    : container_ (c)
  {
  }

  Dynamic_Component_Servant_Base::~Dynamic_Component_Servant_Base (void)
  {
  }
}
