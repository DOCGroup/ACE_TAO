// $Id$

#include "tao/Utils/Implicit_Deactivator.h"
#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::Utils::Implicit_Deactivation_Functor::operator () (
   PortableServer::ServantBase * servant) throw ()
{

  try
    {
      PortableServer::POA_var poa (servant->_default_POA ());

      PortableServer::ObjectId_var id (poa->servant_to_id (servant));

      poa->deactivate_object (id.in());
    }
  catch (...)
    {
      // @@ Cannot let exceptions escape, yet we need to log them!
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
