#include "ORBInitializer_Registry.h"
#include "ORBInitInfo.h"

#include "tao/ORB.h"
#include "tao/ORB_Constants.h"
#include "tao/SystemException.h"

#include "ace/Static_Object_Lock.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"

ACE_RCSID (PI,
           ORBInitializer_Registry,
           "$Id$")

TAO::ORBInitializer_Registry::ORBInitializer_Registry (void)
  : lock_ (),
    initializers_ ()
{
}

void
TAO::ORBInitializer_Registry::register_orb_initializer (
  PortableInterceptor::ORBInitializer_ptr init
  ACE_ENV_ARG_DECL)
{
  if (!CORBA::is_nil (init))
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 guard,
                 this->lock_);

      // Increase the length of the ORBInitializer array by one.
      size_t const cur_len = this->initializers_.size ();
      size_t const new_len = cur_len + 1;
      if (this->initializers_.size (new_len) != 0)
        ACE_THROW (CORBA::INTERNAL ());

      // Add the given ORBInitializer to the sequence.
      this->initializers_[cur_len] =
        PortableInterceptor::ORBInitializer::_duplicate (init);
    }
  else
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA::SystemException::_tao_minor_code (
                   0,
                   EINVAL),
                 CORBA::COMPLETED_NO));
}

void
TAO::ORBInitializer_Registry::pre_init (
  TAO_ORB_Core *orb_core,
  int argc,
  char *argv[],
  PortableInterceptor::SlotId &slotid
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

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
      ACE_CHECK;

      TAO_ORBInitInfo_var orb_init_info_ = orb_init_info_temp;

      for (size_t i = 0; i < initializer_count; ++i)
        {
          this->initializers_[i]->pre_init (orb_init_info_.in ()
                                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      slotid = orb_init_info_temp->slot_count ();

      // Invalidate the ORBInitInfo instance to prevent future
      // modifications to the ORB.  This behavior complies with the
      // PortableInterceptor specification.
      orb_init_info_temp->invalidate ();
    }
}

void
TAO::ORBInitializer_Registry::post_init (
  TAO_ORB_Core *orb_core,
  int argc,
  char *argv[],
  PortableInterceptor::SlotId &slotid
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

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
      ACE_CHECK;

      TAO_ORBInitInfo_var orb_init_info_ = orb_init_info_temp;

      for (size_t i = 0; i < initializer_count; ++i)
        {
          this->initializers_[i]->post_init (orb_init_info_.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      slotid = orb_init_info_->slot_count ();

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

