#include "ORBInitializer_Registry.h"
#include "PortableInterceptorC.h"
#include "ORB.h"
#include "TAO_Singleton.h"
#include "ORB_Constants.h"
#include "TAO_Singleton_Manager.h"
#include "SystemException.h"

#include "ace/Static_Object_Lock.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Log_Msg.h"

ACE_RCSID (tao,
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
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  size_t const initializer_count (this->initializers_.size ());
  for (size_t i = 0; i < initializer_count; ++i)
    {
      ACE_TRY
        {
          this->initializers_[i]->pre_init (info
                                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
          // Ignore errors from pre_init, just continue with the next
          // initializer
        }
      ACE_ENDTRY;
    }
}

void
TAO::ORBInitializer_Registry::post_init (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  size_t const initializer_count (this->initializers_.size ());
  for (size_t i = 0; i < initializer_count; ++i)
    {
      ACE_TRY
        {
          this->initializers_[i]->post_init (info
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHALL
        {
          // Ignore errors from post_init, just continue with the next
          // initializer
        }
      ACE_ENDTRY;
    }
}

namespace TAO
{
  ACE_STATIC_SVC_DEFINE (ORBInitializer_Registry,
                         ACE_TEXT ("ORBInitializer_Registry"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (ORBInitializer_Registry),
                         ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                         0)

  ACE_FACTORY_DEFINE (TAO_PI_CLIENT, ORBInitializer_Registry)
}
