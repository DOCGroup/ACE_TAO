// -*- C++ -*-
// $Id$

#include "tao/PortableServer/Direct_Collocation_Upcall_Wrapper.h"

#if (TAO_HAS_MINIMUM_CORBA == 0)
# include "tao/PortableServer/ForwardRequestC.h"
#endif  /* TAO_HAS_MINIMUM_CORBA == 0 */

#include "tao/Abstract_Servant_Base.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO::Direct_Collocation_Upcall_Wrapper::upcall (
  CORBA::Object_ptr obj,
  CORBA::Object_out forward_obj,
  bool & is_forwarded,
  TAO::Argument ** args,
  int,
  const char * op,
  size_t op_len,
  TAO::Collocation_Strategy strategy)
{
  TAO_Abstract_ServantBase * const servant = obj->_servant ();

  TAO_Collocated_Skeleton collocated_skel;

  if (servant->_find (op, collocated_skel, strategy, op_len) == -1)
    {
      throw ::CORBA::BAD_OPERATION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO);
    }

#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
  try
    {
#endif /* TAO_HAS_MINIMUM_CORBA && !CORBA_E_COMPACT && !CORBA_E_MICRO*/
      collocated_skel (servant, args);
#if (TAO_HAS_MINIMUM_CORBA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
    }
  catch (const ::PortableServer::ForwardRequest& forward_request)
    {
      forward_obj =
        CORBA::Object::_duplicate (forward_request.forward_reference.in ());
      is_forwarded = true;
    }
#else
  ACE_UNUSED_ARG (forward_obj);
  ACE_UNUSED_ARG (is_forwarded);
#endif /* TAO_HAS_MINIMUM_CORBA && !CORBA_E_COMPACT && !CORBA_E_MICRO*/
}

TAO_END_VERSIONED_NAMESPACE_DECL
