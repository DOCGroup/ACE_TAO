#include "ciao/Servants/Swapping/Dynamic_Component_Servant_Base.h"

ACE_RCSID (ciao,
           Dynamic_Component_Servant_Base,
           "$Id$")

namespace CIAO
{
  Dynamic_Component_Servant_Base::Dynamic_Component_Servant_Base (void)
    : container_ (0)
  {
    // This constructor is here to keep MSVC happy and should
    // not be used at all. This constructor should be removed 
    // in the future. Until then, we have the ACE_ASSERT
    // below to detect a runtime call to this constructor.
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
