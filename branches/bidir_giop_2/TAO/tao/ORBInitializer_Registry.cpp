// -*- C++ -*-
//
// $Id$

#include "ORBInitializer_Registry.h"
#include "ORB.h"
#include "TAO_Singleton_Manager.h"
#include "ace/Object_Manager.h"

#if !defined (__ACE_INLINE__)
# include "tao/ORBInitializer_Registry.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ORBInitializer_Registry, "$Id$")


void
PortableInterceptor::register_orb_initializer (
  PortableInterceptor::ORBInitializer_ptr init,
  CORBA::Environment &ACE_TRY_ENV)
{
  // Using ACE_Static_Object_Lock::instance() precludes
  // <register_orb_initializer> from being called within a static
  // object CTOR.
  ACE_MT (ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                     *ACE_Static_Object_Lock::instance ()));

  // Make sure TAO's singleton manager is initialized.
  if (TAO_Singleton_Manager::instance ()->init () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) register_orb_initializer: ")
                  ACE_TEXT ("Unable to pre-initialize TAO\n")));
    }

  CORBA_ORB::init_orb_globals (ACE_TRY_ENV);
  ACE_CHECK;

  // Make sure the following is done after the global ORB
  // initialization since we need to have exceptions initialized.

  TAO_ORBInitializer_Registry::instance ()->register_orb_initializer (
    init,
    ACE_TRY_ENV);
}

// ------------------------------------------------------------------

TAO_ORBInitializer_Registry::TAO_ORBInitializer_Registry (void)
  : initializers_ ()
{
}

TAO_ORBInitializer_Registry::~TAO_ORBInitializer_Registry (void)
{
  size_t initializer_count = this->initializers_.size ();
  for (size_t i = 0; i < initializer_count; ++i)
    CORBA::release (this->initializers_[i]);
}

void
TAO_ORBInitializer_Registry::register_orb_initializer (
  PortableInterceptor::ORBInitializer_ptr init,
  CORBA::Environment &ACE_TRY_ENV)
{
  if (!CORBA::is_nil (init))
    {
      // Increase the length of the ORBInitializer array by one.
      size_t cur_len = this->initializers_.size ();
      size_t new_len = cur_len + 1;
      if (this->initializers_.size (new_len) != 0)
        ACE_THROW (CORBA::INTERNAL ());

      // @@ Do we need to duplicate the reference to the
      //    ORBInitializer?

      // Add the given ORBInitializer to the sequence.
      this->initializers_[cur_len] =
        PortableInterceptor::ORBInitializer::_duplicate (init);
    }
  else
    ACE_THROW (CORBA::INV_OBJREF (
                 CORBA_SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));
}

void
TAO_ORBInitializer_Registry::pre_init (
  PortableInterceptor::ORBInitInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG(ACE_TRY_ENV);
  size_t initializer_count = this->initializers_.size ();
  for (size_t i = 0; i < initializer_count; ++i)
    {
      this->initializers_[i]->pre_init (info
                                        TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_ORBInitializer_Registry::post_init (
  PortableInterceptor::ORBInitInfo_ptr info,
  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG(ACE_TRY_ENV);
  size_t initializer_count = this->initializers_.size ();
  for (size_t i = 0; i < initializer_count; ++i)
    {
      this->initializers_[i]->post_init (info
                                         TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Singleton<TAO_ORBInitializer_Registry, ACE_SYNCH_MUTEX>;
template class ACE_Array_Base<PortableInterceptor::ORBInitializer_ptr>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Singleton<TAO_ORBInitializer_Registry, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Array_Base<PortableInterceptor::ORBInitializer_ptr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
