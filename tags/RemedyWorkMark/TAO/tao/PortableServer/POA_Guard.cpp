// $Id$

#include "tao/PortableServer/POA_Guard.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    POA_Guard::POA_Guard (::TAO_Root_POA &poa, bool check_for_destruction)
      : guard_ (poa.lock ())
    {
      if (!this->guard_.locked ())
        throw
          CORBA::INTERNAL (
            CORBA::SystemException::_tao_minor_code (
              TAO_GUARD_FAILURE,
              0),
            CORBA::COMPLETED_NO);

      // Check if a non-servant upcall is in progress.  If a non-servant
      // upcall is in progress, wait for it to complete.  Unless of
      // course, the thread making the non-servant upcall is this thread.
      poa.object_adapter ().wait_for_non_servant_upcalls_to_complete ();

      if (check_for_destruction && poa.cleanup_in_progress ())
        {
          throw
            CORBA::BAD_INV_ORDER (
              CORBA::SystemException::_tao_minor_code (
                TAO_POA_BEING_DESTROYED,
                0),
              CORBA::COMPLETED_NO);
        }
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
