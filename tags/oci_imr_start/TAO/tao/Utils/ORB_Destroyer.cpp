#include "ORB_Destroyer.h"


ACE_RCSID (Utils,
           ORB_Destroyer,
           "$Id$")
void
TAO::Utils::ORB_Destroyer_Functor::operator() (CORBA::ORB_ptr orb)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // @@ Cannot let exceptions escape, yet we need to log them!
    }
  ACE_ENDTRY;
}
