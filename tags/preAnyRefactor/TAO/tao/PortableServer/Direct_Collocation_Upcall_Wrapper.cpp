// $Id$

#include "Direct_Collocation_Upcall_Wrapper.h"

#if (TAO_HAS_MINIMUM_CORBA == 0)
# include "ForwardRequestC.h"
#endif  /* TAO_HAS_MINIMUM_CORBA == 0 */

#include "tao/Abstract_Servant_Base.h"
#include "tao/SystemException.h"
#include "tao/ORB_Constants.h"
#include "tao/Object.h"
#include "tao/Environment.h"

ACE_RCSID (PortableServer,
           Direct_Collocation_Upcall_Wrapper,
           "$Id$")

void
TAO::Direct_Collocation_Upcall_Wrapper::upcall (
  CORBA::Object_ptr obj,
  CORBA::Object_out forward_obj,
  TAO::Argument ** args,
  int num_args,
  const char * op,
  size_t op_len,
  TAO::Collocation_Strategy strategy
  ACE_ENV_ARG_DECL
)
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
      ACE_THROW (CORBA::BAD_OPERATION (CORBA::OMGVMCID | 2, CORBA::COMPLETED_NO));
    }

  ACE_TRY
    {
      collocated_skel (servant,
                       args,
                       num_args
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
#if (TAO_HAS_MINIMUM_CORBA == 0)
  ACE_CATCH (PortableServer::ForwardRequest, forward_request)
    {
      forward_obj =
        CORBA::Object::_duplicate (forward_request.forward_reference.in ());
      return;
    }
#else
  ACE_CATCHANY
    {
      ACE_UNUSED_ARG (forward_obj);
      ACE_RE_THROW;
    }
#endif /* TAO_HAS_MINIMUM_CORBA */
  ACE_ENDTRY;
  ACE_CHECK;
}

