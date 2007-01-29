// $Id$

#include "tao/PortableServer/Direct_Collocation_Upcall_Wrapper.h"

#if (TAO_HAS_MINIMUM_CORBA == 0)
# include "tao/PortableServer/ForwardRequestC.h"
#endif  /* TAO_HAS_MINIMUM_CORBA == 0 */

#include "tao/Abstract_Servant_Base.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/Environment.h"

ACE_RCSID (PortableServer,
           Direct_Collocation_Upcall_Wrapper,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::Direct_Collocation_Upcall_Wrapper::upcall (
  CORBA::Object_ptr obj,
  CORBA::Object_out forward_obj,
  TAO::Argument ** args,
  int num_args,
  const char * op,
  size_t op_len,
  TAO::Collocation_Strategy strategy)
    ACE_THROW_SPEC ((CORBA::Exception))
{
  TAO_Abstract_ServantBase * const servant = obj->_servant ();

  TAO_Collocated_Skeleton collocated_skel;
  int const status = servant->_find (op,
                                     collocated_skel,
                                     strategy,
                                     op_len);

  if (status == -1)
    {
      throw ::CORBA::BAD_OPERATION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

  try
    {
      collocated_skel (servant,
                       args,
                       num_args);
    }
#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  catch (const ::PortableServer::ForwardRequest& forward_request)
    {
      forward_obj =
        CORBA::Object::_duplicate (forward_request.forward_reference.in ());
      return;
    }
#else
  catch (const ::CORBA::Exception&)
    {
      ACE_UNUSED_ARG (forward_obj);
      throw;
    }
#endif /* TAO_HAS_MINIMUM_CORBA && !CORBA_E_COMPACT && !CORBA_E_MICRO*/
}

TAO_END_VERSIONED_NAMESPACE_DECL
