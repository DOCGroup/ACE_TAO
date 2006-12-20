// $Id$

#include "tao/PI/ORBInitInfo.h"
#include "tao/TransportCurrent/Current_ORBInitializer_Base.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Transport
  {

    Current_ORBInitializer_Base::Current_ORBInitializer_Base(const ACE_TCHAR* id)
      : id_ (id)
    {
    }

    Current_ORBInitializer_Base::~Current_ORBInitializer_Base(void)
    {
    }



    void
    Current_ORBInitializer_Base::pre_init (PortableInterceptor::ORBInitInfo_ptr info
                                           ACE_ENV_ARG_DECL_NOT_USED )
      ACE_THROW_SPEC( (CORBA::SystemException) )
    {
      // Narrow to a TAO_ORBInitInfo object to get access to the
      // allocate_tss_slot_id() TAO extension.
      TAO_ORBInitInfo_var tao_info =
        TAO_ORBInitInfo::_narrow (info
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (tao_info.in ()))
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) TAO::Transport::ORBInitializer::pre_init - "
                        "Panic: unable to narrow the ORBInitInfo_ptr\n"));

          ACE_THROW (CORBA::INTERNAL ());
        }

      // Reserve a TSS slot in the ORB core internal TSS resources for the
      // thread-specific portion of the Current object.
      size_t tss_slot = tao_info->allocate_tss_slot_id (0
                                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Create the Current
      Current_var current (this->make_current_instance (tao_info->orb_core (),
                                                        tss_slot
                                                        ACE_ENV_ARG_PARAMETER));

      info->register_initial_reference (ACE_TEXT_ALWAYS_CHAR (this->id_.fast_rep ()),
                                        current.in ());

    }

    void
    Current_ORBInitializer_Base::post_init (PortableInterceptor::ORBInitInfo_ptr
                                            ACE_ENV_ARG_DECL_NOT_USED )
      ACE_THROW_SPEC( (CORBA::SystemException) )
    {
    }

  }

}

TAO_END_VERSIONED_NAMESPACE_DECL
