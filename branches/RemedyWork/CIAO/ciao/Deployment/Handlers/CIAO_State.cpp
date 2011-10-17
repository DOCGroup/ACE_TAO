// $Id$

#include "CIAO_State.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  void
  Deployment_State::close (void)
  {
    this->containers_.clear ();
    this->instance_container_.clear ();
    this->homes_.clear ();
    this->components_.clear ();
  }

  void
  Deployment_State::add_container (const char *id,
                                   CIAO::Container_ptr container)
  {
    CIAO_TRACE ("Deployment_State::add_container");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->container_mutex_,
                        CORBA::NO_RESOURCES ());

    if (CIAO_debug_level && // Let's only perform this lookup if we have logging enabled.
        this->containers_.find (id) != this->containers_.end ())
      {
        CIAO_ERROR (1, (LM_WARNING, CLINFO
                        "Deployment_State::add_container - "
                        "Warning:  Attempting to add duplicate container reference\n"));
      }

    this->containers_[id] = CIAO::Container::_duplicate (container);
  }

  void
  Deployment_State::remove_container (const char *id)
  {
    CIAO_TRACE ("Deployment_State::remove_container");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->container_mutex_,
                        CORBA::NO_RESOURCES ());

    CONTAINERS::iterator pos = this->containers_.find (id);

    if (pos != this->containers_.end ())
      this->containers_.erase (pos);
  }

  CIAO::Container_ptr
  Deployment_State::fetch_container (const char *id)
  {
    CIAO_TRACE ("Deployment_State::fetch_container");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->container_mutex_,
                        CORBA::NO_RESOURCES ());

    CONTAINERS::iterator pos = this->containers_.find (id);

    if (pos == this->containers_.end ())
      return CIAO::Container::_nil ();

    return CIAO::Container::_duplicate (pos->second.in ());
  }

  void
  Deployment_State::add_home (const char *id,
                              const char *cont_id,
                              Components::CCMHome_ptr home)
  {
    CIAO_TRACE ("Deployment_State::add_home");

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->home_mutex_,
                          CORBA::NO_RESOURCES ());

      if (CIAO_debug_level && // Let's only perform this lookup if we have logging enabled.
          this->homes_.find (id) != this->homes_.end ())
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                          "Deployment_State::add_home - "
                          "Warning:  Attempting to add duplicate home reference\n"));
        }
      this->homes_[id] = Components::CCMHome::_duplicate (home);
    }

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->instance_container_mutex_,
                          CORBA::NO_RESOURCES ());
      this->instance_container_[id] = cont_id;
    }
  }

  void
  Deployment_State::remove_home (const char *id)
  {
    CIAO_TRACE ("Deployment_State::remove_home");
    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->home_mutex_,
                          CORBA::NO_RESOURCES ());

      HOMES::iterator pos = this->homes_.find (id);

      if (pos != this->homes_.end ())
        this->homes_.erase (pos);
    }

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->instance_container_mutex_,
                          CORBA::NO_RESOURCES ());
      INSTANCE_CONTAINER::iterator cont =
        this->instance_container_.find (id);

      if (cont != this->instance_container_.end ())
        this->instance_container_.erase (cont);
    }
  }

  Components::CCMHome_ptr
  Deployment_State::fetch_home (const char *id)
  {
    CIAO_TRACE ("Deployment_State::fetch_home");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->home_mutex_,
                        CORBA::NO_RESOURCES ());

    HOMES::iterator pos = this->homes_.find (id);

    if (pos == this->homes_.end ())
      return Components::CCMHome::_nil ();

    return Components::CCMHome::_duplicate (pos->second.in ());
  }

  void
  Deployment_State::add_component (const char *id,
                                   const char *cont_id,
                                   Components::CCMObject_ptr component)
  {
    CIAO_TRACE ("Deployment_State::add_component");

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->component_mutex_,
                          CORBA::NO_RESOURCES ());

      if (CIAO_debug_level && // Let's only perform this lookup if we have logging enabled.
          this->components_.find (id) != this->components_.end ())
        {
          CIAO_ERROR (1, (LM_WARNING, CLINFO
                          "Deployment_State::add_component - "
                          "Warning:  Attempting to add duplicate component reference\n"));
        }
      this->components_[id] = Components::CCMObject::_duplicate (component);
    }
    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->instance_container_mutex_,
                          CORBA::NO_RESOURCES ());
      this->instance_container_[id] = cont_id;
    }
  }

  void
  Deployment_State::remove_component (const char *id)
  {
    CIAO_TRACE ("Deployment_State::remove_component");

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->component_mutex_,
                          CORBA::NO_RESOURCES ());
      COMPONENTS::iterator pos = this->components_.find (id);

      if (pos != this->components_.end ())
        this->components_.erase (pos);
    }

    {
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                          guard,
                          this->instance_container_mutex_,
                          CORBA::NO_RESOURCES ());
      INSTANCE_CONTAINER::iterator cont =
        this->instance_container_.find (id);

      if (cont != this->instance_container_.end ())
        this->instance_container_.erase (cont);
    }
  }

  Components::CCMObject_ptr
  Deployment_State::fetch_component (const char *id)
  {
    CIAO_TRACE ("Deployment_State::fetch_component");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->component_mutex_,
                        CORBA::NO_RESOURCES ());

    COMPONENTS::iterator pos = this->components_.find (id);

    if (pos == this->components_.end ())
      return Components::CCMObject::_nil ();

    return Components::CCMObject::_duplicate (pos->second.in ());
  }

  const char *
  Deployment_State::instance_to_container (const char *id)
  {
    CIAO_TRACE ("Deployment_State::instance_to_container");

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        guard,
                        this->instance_container_mutex_,
                        CORBA::NO_RESOURCES ());
    INSTANCE_CONTAINER::const_iterator cont =
      this->instance_container_.find (id);

    if (cont != this->instance_container_.end ())
      return cont->second.c_str ();

    CIAO_ERROR (1, (LM_ERROR, CLINFO
                    "Deployment_State::instance_to_container - "
                    "Error: Unknown instance ID <%C>\n",
                    id));
    return 0;
  }
}
