// $Id$

#include "test_i.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"
#include "orbsvcs/SSLIOP/SSLIOP_Transport.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Server, test_i, "$Id$")

void
Simple_Server_i::ping (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
Simple_Server_i::validate_protocol (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((Simple_Server::WrongProtocolType,
                   CORBA::SystemException))
{
  if (this->validated_ == NOT_VALIDATED)
    {
      ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                         ace_mon,
                         this->mutex_));

      if (this->validated_ != NOT_VALIDATED)
        return;

      TAO_Transport_Cache_Manager::HASH_MAP &map =
        this->orb_->orb_core ()->lane_resources ().transport_cache ().map ();

      TAO_Transport_Cache_Manager::HASH_MAP_ITER st_iter =
        map.begin ();

      TAO_Transport_Cache_Manager::HASH_MAP_ITER end_iter =
        map.end ();

      for (TAO_Transport_Cache_Manager::HASH_MAP_ITER iter = st_iter;
           iter != end_iter;
           ++iter)
        {
          TAO_Transport *t =
            (*iter).int_id_.transport ();

          // @@ Worst possible way to check. If SSLIOP had a tag
          // things would have been a  lot simpler.
          TAO_SSLIOP_Transport *ssl_t =
            ACE_dynamic_cast (TAO_SSLIOP_Transport *,
                              t);

          // There should be no SSL Transport
          if (ssl_t != 0)
            {
              this->validated_ = VALIDATED_NOSUCCESS;
              break;
            }
        }

    }

  if (this->validated_ == VALIDATED_NOSUCCESS)
    ACE_THROW (Simple_Server::WrongProtocolType ());
}

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Request in thread [%t]\n"));
  ACE_Time_Value tv (0, 15000);
  ACE_OS::sleep (tv);
  return x;
}

void
Simple_Server_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
