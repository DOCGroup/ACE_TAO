#include "Implicit_Deactivator.h"
#include "tao/PortableServer/Servant_Base.h"

ACE_RCSID (Utils,
           Implicit_Deactivator,
           "$Id$")
void
TAO::Utils::Implicit_Deactivation_Functor::operator () (
   PortableServer::ServantBase * servant)
  ACE_THROW_SPEC (())
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      PortableServer::POA_var poa (servant->_default_POA (
          ACE_ENV_SINGLE_ARG_PARAMETER));
      ACE_TRY_CHECK;

      PortableServer::ObjectId_var id (poa->servant_to_id (servant
                                                           ACE_ENV_ARG_PARAMETER));
      ACE_TRY_CHECK;

      poa->deactivate_object (id.in()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // @@ Cannot let exceptions escape, yet we need to log them!
    }
  ACE_ENDTRY;
}
