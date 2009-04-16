#include "Servant_Activator.h"

#include "tao/PortableServer/PortableServer_Functions.h"
#include "ciao/CIAO_common.h"

ACE_RCSID (ciao,
           Servant_Activator_i,
           "$Id$")

namespace CIAO
{
  Servant_Activator_i::Servant_Activator_i (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o)),
      // @@ TODO, avoid this magic number
      pa_ (64),
      slot_index_ (0)
  {
  }

  Servant_Activator_i::~Servant_Activator_i (void)
  {
    CIAO_TRACE ("Servant_Activator_i::~Servant_Activator_i");
    /* _var should take care of this now.
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
    */
  }

  bool
  Servant_Activator_i::update_port_activator (
    const PortableServer::ObjectId &oid)
  {
    CIAO_TRACE ("Servant_Activator_i::update_port_activator");

    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);
    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->mutex_,
                          CORBA::NO_RESOURCES ());
      size_t const sz = this->slot_index_;

      for (size_t t = 0; t != sz; ++t)
        {
          Port_Activator_var tmp = this->pa_[t];

          if (ACE_OS::strcmp (tmp->oid (), str.in ()) == 0)
            {
              tmp->oid ("dummy");
            }
        }
    }

    return true;
  }

  PortableServer::Servant
  Servant_Activator_i::incarnate (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr)
  {
    CIAO_TRACE ("Servant_Activator_i::incarnate");

    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid);

    CIAO_DEBUG ((LM_INFO, CLINFO
                "Servant_Activator_i::incarnate, "
                "Attempting to activate port name [%C]\n",
                str.in ()));

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->mutex_,
                          CORBA::NO_RESOURCES ());

      size_t const sz = this->slot_index_;
      Port_Activator_var tmp;

      for (size_t t = 0; t != sz; ++t)
        {
          if (this->pa_.get (tmp, t) == -1)
            {
              throw CORBA::OBJECT_NOT_EXIST ();
            }

          if (tmp == 0)
            {
                CIAO_ERROR ((LM_ERROR, CLINFO
                            "Servant_Activator_i::incarnate (),"
                            " value from the array is null\n"));
              continue;
            }

          if (ACE_OS::strcmp (tmp->oid (),
                              str.in ()) == 0)
            {
              // We should try avoiding making outbound calls with the
              // lock held. Oh well, let us get some sense of sanity in
              // CIAO to do think about these.
              CIAO_DEBUG ((LM_INFO, CLINFO
                      "Servant_Activator_i::incarnate - Activating Port %C\n",
                      str.in ()));

              return this->pa_[t]->activate (oid);
            }
        }
    }

    throw CORBA::OBJECT_NOT_EXIST ();
  }

  void
  Servant_Activator_i::etherealize (const PortableServer::ObjectId &oid,
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
        Port_Activator_var pa;
        if (this->pa_.get (pa, t) == -1)
          {
            CIAO_ERROR ((LM_WARNING, CLINFO
              "Servant_Activator_i::etherealize - Could not get Port Activator\n"));
            continue;
          }

        if (tmp == 0)
          {
            CIAO_ERROR ((LM_WARNING, CLINFO
              "Servant_Activator_i::etherealize - Port Activator is NULL\n"));
            continue;
          }

        if (ACE_OS::strcmp (tmp->oid (),
                            str.in ()) == 0)
          {
            CIAO_DEBUG ((LM_INFO, CLINFO
                        "Servant_Activator_i::etherealize - Deactivating Port %C\n",
                        str.in ()));
            this->pa_[t]->deactivate (servant);
          }
      }
  }

  bool
  Servant_Activator_i::register_port_activator (Port_Activator *pa)
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

        CIAO_DEBUG ((LM_INFO, CLINFO
                    "Servant_Activator_i::"
                    "register_port_activator"
        " with port name [%C],"
                    " the slot_index_ is [%d]\n",
                    pa->name (),
                    this->slot_index_));

        return true;
      }

    return false;
  }
}
