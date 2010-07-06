// $Id$

#include "Servant_Activator.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/PortableServer/PortableServer_Functions.h"

namespace CIAO
{
  Servant_Activator_i::Servant_Activator_i (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o)),
      slot_index_ (0)
  {
  }

  Servant_Activator_i::~Servant_Activator_i (void)
  {
    CIAO_TRACE ("Servant_Activator_i::~Servant_Activator_i");
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

      CIAO_DEBUG (6, (LM_INFO, CLINFO
                  "Servant_Activator_i::update_port_activator - "
                  "Attempting to update port name [%C]\n",
                  str.in ()));

      Port_Activators::iterator pa_iter = this->pa_.find (str.in ());

      if (pa_iter != this->pa_.end ())
        {
          this->pa_.erase (pa_iter);
        }
      else
        {
          return false;
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

    CIAO_DEBUG (6, (LM_INFO, CLINFO
                "Servant_Activator_i::incarnate - "
                "Attempting to activate port name [%C]\n",
                str.in ()));

    Port_Activators::iterator pa_iter;

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->mutex_,
                          CORBA::NO_RESOURCES ());

      pa_iter = this->pa_.find (str.in ());
    }

    if (pa_iter == this->pa_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Activator_i::incarnate - "
                     "Unable to find suitable port activator for ObjectID %C\n",
                     str.in ()));
        throw CORBA::OBJECT_NOT_EXIST ();
      }

    if (CORBA::is_nil (pa_iter->second))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Activator_i::incarnate - "
                     "Port Activator for ObjectId %C was nil!\n",
                     str.in ()));
        throw CORBA::OBJECT_NOT_EXIST ();
      }

    CIAO_DEBUG (6, (LM_INFO, CLINFO
                 "Servant_Activator_i::incarnate - Activating Port %C\n",
                 str.in ()));

    return pa_iter->second->activate (oid);
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

    CIAO_DEBUG (9, (LM_TRACE, CLINFO "Servant_Activator_i::etherealize - "
                 "Attempting to etherealize servant with object ID %C\n",
                 str.in ()));

    Port_Activators::iterator pa_iter;

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->mutex_,
                          CORBA::NO_RESOURCES ());

      pa_iter = this->pa_.find (str.in ());
    }

    if (pa_iter == this->pa_.end ())
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Activator_i::etherealize - "
                     "Unable to find suitable port activator for ObjectID %C\n",
                     str.in ()));
        throw CORBA::OBJECT_NOT_EXIST ();
      }

    if (CORBA::is_nil (pa_iter->second))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Activator_i::etherealize - "
                     "Port Activator for ObjectId %C was nil!\n",
                     str.in ()));
        throw CORBA::OBJECT_NOT_EXIST ();
      }

    pa_iter->second->deactivate (servant);
  }

  bool
  Servant_Activator_i::register_port_activator (Port_Activator_ptr pa)
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->mutex_,
                      false);

    CIAO_DEBUG (6, (LM_INFO, CLINFO "Servant_Activator_i::register_port_activator - "
                 "Registering a port activator for port [%C] with ObjectID [%C]\n",
                 pa->name (), pa->oid ()));

    try
      {
	CORBA::String_var safe_oid (pa->oid ());
        this->pa_ [safe_oid.in ()] = Port_Activator::_duplicate (pa);
      }
    catch (...)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "Servant_Activator_i::register_port_activator - "
                     "Unable to register a port activator for port [%C] with ObjectID [%C]\n",
                     pa->name (), pa->oid ()));
        return false;
      }

    return true;
  }
}
