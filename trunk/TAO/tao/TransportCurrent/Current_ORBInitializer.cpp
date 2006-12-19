// $Id:$

#ifndef CURRENT_ORBINITIALIZER_CPP
#define CURRENT_ORBINITIALIZER_CPP

#include "tao/ORB_Constants.h"
#include "tao/TransportCurrent/Current_ORBInitializer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {

    template <typename Impl>
    Current_ORBInitializer<Impl>::Current_ORBInitializer(const ACE_TCHAR* id)
      : Current_ORBInitializer_Base (id)
    {
    }


    template <typename Impl>
    TAO::Transport::Current_ptr
    Current_ORBInitializer<Impl>::make_current_instance (TAO_ORB_Core* core,
                                                         size_t tss_slot_id
                                                         ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
      {
        // Create the Current
        Current_ptr tmp = 0;
        ACE_NEW_THROW_EX (tmp,
                          Impl (core, tss_slot_id),
                          CORBA::NO_MEMORY
                            (CORBA::SystemException::_tao_minor_code (TAO::VMCID,
                                                                      ENOMEM),
                             CORBA::COMPLETED_NO));
        ACE_CHECK;

        return tmp;
      }

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* CURRENT_ORBINITIALIZER_CPP */
