#include "Servant_Activator.h"
#include "Port_Activator.h"

#include "CIAO_common.h"

ACE_RCSID (ciao,
           Servant_Activator,
           "$Id$")

namespace CIAO
{
  Servant_Activator::Servant_Activator (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o)),
      // @@ TODO, avoid this magic number
      pa_ (64),
      slot_index_ (0)
  {
  }

  Servant_Activator::~Servant_Activator (void)
  {
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 guard,
                 this->mutex_);

      size_t const sz =  this->slot_index_;

      for (size_t t = 0; t != sz; ++t)
        {
          Port_Activator *&tmp = this->pa_[t];

          delete tmp;
        }
    }
  }

  bool
  Servant_Activator::update_port_activator (
    const PortableServer::ObjectId &oid)
  {
    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->mutex_,
                        0);
      size_t const sz = this->slot_index_;
      
      for (size_t t = 0; t != sz; ++t)
        {
          Port_Activator *&tmp = this->pa_[t];
          
          if (ACE_OS::strcmp (tmp->oid (), str.in ()) == 0)
            {
              tmp->oid ("dummy");
            }
        }
    }
    
    return true;
  }

  PortableServer::Servant
  Servant_Activator::incarnate (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr)
  {
    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);

    if (CIAO::debug_level () > 9)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "CIAO (%P|%t) - Servant_Activator::incarnate, "
                    "activating port name [%s] \n",
                    str.in ()));
      }

    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->mutex_,
                        0);

      size_t const sz = this->slot_index_;
      Port_Activator *tmp = 0;

      for (size_t t = 0; t != sz; ++t)
        {
          if (this->pa_.get (tmp, t) == -1)
            {
              throw CORBA::OBJECT_NOT_EXIST ();
            }

          if (tmp == 0)
            {
              if (CIAO::debug_level () > 9)
                ACE_ERROR ((LM_ERROR,
                            "CIAO (%P|%t) - Servant_Activator::incarnate (),"
                            " value from the array is null \n"));
              continue;
            }
            
          if (ACE_OS::strcmp (tmp->oid (),
                              str.in ()) == 0)
            {
              // We should try avoiding making outbound calls with the
              // lock held. Oh well, let us get some sense of sanity in
              // CIAO to do think about these.
              if (CIAO::debug_level () > 5)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "Activating Port %s\n",
                              str.in ()));
                }

              return this->pa_[t]->activate (oid);
            }
        }
    }
    
    throw CORBA::OBJECT_NOT_EXIST ();
  }

  void
  Servant_Activator::etherealize (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr ,
                                  PortableServer::Servant servant,
                                  CORBA::Boolean ,
                                  CORBA::Boolean)
  {
    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);

    size_t const sz = this->slot_index_;
    Port_Activator *tmp = 0;

    for (size_t t = 0; t != sz; ++t)
      {
        if (this->pa_.get (tmp, t) == -1)
          {
            ACE_DEBUG ((LM_DEBUG, "Could not get Port Activator\n"));
            continue;
          }

        if (tmp == 0)
          {
            ACE_DEBUG ((LM_DEBUG, "Port Activator is NULL\n"));
            continue;
          }
          
        if (ACE_OS::strcmp (tmp->oid (),
                            str.in ()) == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "Deactivating Port %s\n",
                        str.in ()));
            this->pa_[t]->deactivate (servant);
          }
      }
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
      {
        this->pa_.size ((this->slot_index_ + 1));
      }

    if (this->pa_.set (pa, this->slot_index_) == 0)
      {
        ++this->slot_index_;

        if (CIAO::debug_level () > 9)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "CIAO (%P|%t) - Servant_Activator::"
                        "register_port_activator"
                        " with port name [%s],"
                        " the slot_index_ is [%d] \n",
                        pa->name (),
                        this->slot_index_));
          }

        return true;
      }

    return false;
  }
}
