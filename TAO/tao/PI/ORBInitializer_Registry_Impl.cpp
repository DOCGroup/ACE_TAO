// $Id$

#include "tao/PI/ORBInitializer_Registry_Impl.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/PI/PICurrent.h"
#include "tao/PI/PI_ORBInitializer.h"
#include "tao/PI/ClientRequestInterceptor_Factory_Impl.h"
#include "tao/PI/PICurrent_Loader.h"
#include "tao/PI/PolicyFactory_Loader.h"

#include "tao/ORB_Core.h"
#include "tao/ORB_Constants.h"

#include "ace/Static_Object_Lock.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::ORBInitializer_Registry::ORBInitializer_Registry (void)
  : lock_ (),
    initializers_ ()
{
}

int
TAO::ORBInitializer_Registry::init (int, ACE_TCHAR *[])
{
#if !defined (TAO_AS_STATIC_LIBS) && (TAO_HAS_INTERCEPTORS == 1)
  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_ClientRequestInterceptor_Adapter_Factory_Impl);

  ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_PICurrent_Loader);

  PortableInterceptor::ORBInitializer_ptr temp_orb_initializer =
    PortableInterceptor::ORBInitializer::_nil ();

  PortableInterceptor::ORBInitializer_var orb_initializer;

  try
    {
      /// Register the PI ORBInitializer.

      ACE_NEW_THROW_EX (temp_orb_initializer,
                        TAO_PI_ORBInitializer,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      orb_initializer = temp_orb_initializer;

      this->register_orb_initializer (orb_initializer.in ());
    }
  catch (const ::CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        {
          ex._tao_print_exception ("(%P|%t) Caught exception:");
        }
      return -1;
    }
#endif /* !TAO_AS_STATIC_LIBS  && TAO_HAS_INTERCEPTORS == 1 */
  return 0;
}

int
TAO::ORBInitializer_Registry::fini (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    -1);

  // Release all initializers in the array
  size_t const initializer_count (this->initializers_.size ());
  for (size_t i = initializer_count; i > 0;)
    {
      --i;
      if (TAO_debug_level > 8)
        {
          TAOLIB_DEBUG((LM_DEBUG, ACE_TEXT("TAO (%P|%t) - ORBInitializer_Registry::fini clearing %d @%@\n"),
            i, this->initializers_[i].operator->()
            ));
        }
      this->initializers_[i] = PortableInterceptor::ORBInitializer::_nil();
    }

  return 0;
}

void
TAO::ORBInitializer_Registry::register_orb_initializer (
  PortableInterceptor::ORBInitializer_ptr init)
{
  if (!CORBA::is_nil (init))
    {
      ACE_GUARD (TAO_SYNCH_RECURSIVE_MUTEX,
                 guard,
                 this->lock_);

      // Increase the length of the ORBInitializer array by one.
      size_t const cur_len = this->initializers_.size ();
      size_t const new_len = cur_len + 1;
      if (this->initializers_.size (new_len) != 0)
        throw ::CORBA::INTERNAL ();

      // Add the given ORBInitializer to the sequence.
      if (TAO_debug_level > 8)
        {
          TAOLIB_DEBUG((LM_DEBUG, ACE_TEXT("TAO (%P|%t) - ORBInitializer_Registry::register_orb_initializer %d @%@\n"),
            cur_len, init
            ));
        }

      this->initializers_[cur_len] =
        PortableInterceptor::ORBInitializer::_duplicate (init);
    }
  else
    throw ::CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   0,
                   EINVAL),
                 CORBA::COMPLETED_NO);
}

size_t
TAO::ORBInitializer_Registry::pre_init (
  TAO_ORB_Core *orb_core,
  int argc,
  char *argv[],
  PortableInterceptor::SlotId &slotid)
{
  ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                    guard,
                    this->lock_,
                    0);

  size_t const initializer_count (this->initializers_.size ());

  if (initializer_count > 0)
    {
      TAO_ORBInitInfo * orb_init_info_temp = 0;

      ACE_NEW_THROW_EX (orb_init_info_temp,
                        TAO_ORBInitInfo (orb_core,
                                         argc,
                                         argv,
                                         slotid),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      TAO_ORBInitInfo_var orb_init_info_ = orb_init_info_temp;

      for (size_t i = 0; i < initializer_count; ++i)
        {
          this->initializers_[i]->pre_init (orb_init_info_.in ());
        }

      slotid = orb_init_info_temp->slot_count ();

      // Invalidate the ORBInitInfo instance to prevent future
      // modifications to the ORB.  This behavior complies with the
      // PortableInterceptor specification.
      orb_init_info_temp->invalidate ();
    }

  return initializer_count;
}

void
TAO::ORBInitializer_Registry::post_init (
  size_t pre_init_count,
  TAO_ORB_Core *orb_core,
  int argc,
  char *argv[],
  PortableInterceptor::SlotId slotid)
{
  if (pre_init_count > 0)
    {
      ACE_GUARD (TAO_SYNCH_RECURSIVE_MUTEX,
                 guard,
                 this->lock_);

      TAO_ORBInitInfo * orb_init_info_temp = 0;

      ACE_NEW_THROW_EX (orb_init_info_temp,
                        TAO_ORBInitInfo (orb_core,
                                         argc,
                                         argv,
                                         slotid),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            0,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      TAO_ORBInitInfo_var orb_init_info_ = orb_init_info_temp;

      for (size_t i = 0; i < pre_init_count; ++i)
        {
          this->initializers_[i]->post_init (orb_init_info_.in ());
        }

#if TAO_HAS_INTERCEPTORS == 1
      CORBA::Object_ptr picurrent_ptr = orb_core->pi_current ();
      PortableInterceptor::SlotId slot_count = orb_init_info_->slot_count ();

      if (CORBA::is_nil (picurrent_ptr) && slot_count != 0)
        {
          // Force instantiation of the PICurrent object. If we do not do it
          // now, the slot count will be lost.
          CORBA::Object_var tmp = orb_core->resolve_picurrent ();
          picurrent_ptr = orb_core->pi_current ();
        }

      if (!CORBA::is_nil (picurrent_ptr))
        {
          TAO::PICurrent *pi = dynamic_cast <TAO::PICurrent*> (picurrent_ptr);

          if (pi)
            {
              pi->initialize (slot_count);
            }
        }
#endif /* TAO_HAS_INTERCEPTORS == 1 */

      // Invalidate the ORBInitInfo instance to prevent future
      // modifications to the ORB.  This behavior complies with the
      // PortableInterceptor specification.
      orb_init_info_temp->invalidate ();
    }
}

ACE_STATIC_SVC_DEFINE (ORBInitializer_Registry,
                       ACE_TEXT ("ORBInitializer_Registry"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (ORBInitializer_Registry),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_NAMESPACE_DEFINE (TAO_PI, ORBInitializer_Registry, TAO::ORBInitializer_Registry)

TAO_END_VERSIONED_NAMESPACE_DECL

