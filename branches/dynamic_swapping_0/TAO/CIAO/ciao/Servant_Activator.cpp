#include "Servant_Activator.h"
#include "CIAO_common.h"
#include "Port_Activator.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (ciao,
           Servant_Activator,
           "$Id$")

namespace CIAO
{
  Servant_Activator::Servant_Activator (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o))
    // @@ TODO, avoid this magic number
    , pa_ (64)
    , slot_index_ (0)
  {
  }

  Servant_Activator::~Servant_Activator (void)
  {
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 guard,
                 this->mutex_);

      const unsigned int sz =  this->slot_index_;

      for (unsigned int t = 0; t != sz; ++t)
        {
          Port_Activator *&tmp = this->pa_[t];

          delete tmp;
        }
    }
  }

  PortableServer::Servant
  Servant_Activator::incarnate (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest))
  {
    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);

    if (CIAO::debug_level () > 10)
      ACE_DEBUG ((LM_DEBUG,
                  "CIAO (%P|%t) - Servant_Activator::incarnate, "
                  "activating port name [%s] \n",
                  str.in ()));

    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->mutex_,
                        0);

      const unsigned int sz = this->slot_index_;

      Port_Activator *tmp = 0;

      for (unsigned int t = 0; t != sz; ++t)
        {
          if (this->pa_.get (tmp, t) == -1)
            ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                              0);

          if (tmp == 0)
            {
              if (CIAO::debug_level () > 10)
                ACE_ERROR ((LM_ERROR,
                            "CIAO (%P|%t) - Servant_Activator::incarnate (),"
                            " value from the array is null \n"));
              continue;
            }
          if (ACE_OS::strcmp (tmp->name (),
                              str.in ()) == 0)
            // We should try avoiding making outbound calls with the
            // lock held. Oh well, let us get some sense of sanity in
            // CIAO to do think about these.
            return this->pa_[t]->activate (oid
                                           ACE_ENV_ARG_PARAMETER);
        }
    }
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      0);
  }

  void
  Servant_Activator::etherealize (const PortableServer::ObjectId &,
                                  PortableServer::POA_ptr ,
                                  PortableServer::Servant ,
                                  CORBA::Boolean ,
                                  CORBA::Boolean
                                  ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
  {
    /// Need to investigate what needs to be handled here..
  }

  bool
  Servant_Activator::register_port_activator (Port_Activator *pa)
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->mutex_,
                      false);

    // @@ TODO, need to implement a better algorithm here.
    //
    if (this->slot_index_ >= this->pa_.size ())
      this->pa_.size ((this->slot_index_ + 1));

    if (this->pa_.set (pa, this->slot_index_) == 0)
      {
        ++this->slot_index_;

        if (CIAO::debug_level () > 10)
          ACE_DEBUG ((LM_DEBUG,
                      "CIAO (%P|%t) - Servant_Activator::register_port_activator,"
                      " the slot_index_ is [%d] \n",
                      this->slot_index_));

        return true;
      }

    return false;
  }
}
