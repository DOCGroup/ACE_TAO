// $Id$
#include "CIAO_ComponentServer_Impl.h"

#include "ciao/CIAO_common.h"

#include "CIAO_CS_ClientC.h"
#include "CIAO_Container_Impl.h"
#include "CIAO_PropertiesC.h"

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ComponentServer_i::CIAO_ComponentServer_i (const ACE_CString &uuid,
                                                    CORBA::ORB_ptr orb,
                                                    PortableServer::POA_ptr poa)
      : uuid_ (uuid),
        orb_ (CORBA::ORB::_duplicate (orb)),
        poa_ (PortableServer::POA::_duplicate (poa)),
        containers_ ()
    {
      CIAO_TRACE("CIAO_ComponentServer_i::CIAO_ComponentServer_i");
    }

    // Destructor
    CIAO_ComponentServer_i::~CIAO_ComponentServer_i (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::~CIAO_ComponentServer_i");
    }

    void
    CIAO_ComponentServer_i::shutdown (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::shutdown");

      CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ComponentServer_i::shutdown - ORB shutdown request received at %C.\n",
                  this->uuid_.c_str ()));

      if (!this->containers_.is_empty ())
        CIAO_ERROR ((LM_ERROR, CLINFO "CIAO_ComponentServer_i::shutdown - ComponentServer %C still containers!\n",
                     this->uuid_.c_str ()));

      this->orb_->shutdown ();
    }


    ::Components::ConfigValues *
    CIAO_ComponentServer_i::configuration (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::configuration");
      return this->config_values_.out ();
    }


    ::Components::Deployment::ServerActivator_ptr
    CIAO_ComponentServer_i::get_server_activator (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::get_server_activator");
      return this->serv_act_.in ();
    }

    ::Components::Deployment::Container_ptr
    CIAO_ComponentServer_i::create_container (const ::Components::ConfigValues & config)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::create_container");

      try
        {
          CIAO_DEBUG ((LM_INFO, CLINFO "CIAO_ComponentServer_i::create_container - Request received with %u config values\n",
                       config.length ()));

          CORBA::PolicyList policies;
          const char *name = 0;
          CIAO_Container_i *cont = 0;
          ACE_NEW_THROW_EX (cont,
                            CIAO_Container_i (config, 0, name, &policies, this->ci_.in (),
                              this->orb_.in (), this->poa_.in ()),
                            CORBA::NO_MEMORY ());

          CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ComponentServer_i::create_container - "
                       "Container servant successfully allocated.\n"));

          PortableServer::ServantBase_var safe_config = cont;
          CIAO::Deployment::Container_var cont_var = cont->_this  ();

          this->containers_.insert (CIAO::Deployment::Container::_duplicate(cont_var.in ()));

          CIAO_DEBUG ((LM_INFO, CLINFO "CIAO_ComponentServer_i::create_container - Container successfully activated and stored,"
                       "now manage %u containers\n",
                       this->containers_.size ()));

          return cont_var._retn ();
        }
      catch (const CORBA::NO_MEMORY &)
        {
          CIAO_ERROR ((LM_CRITICAL, CLINFO "CIAO_ComponentServer_Impl: Out of memory exception whilst creating container.\n"));
          throw;
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, "CIAO_ComponentServer_Impl: Caught unknown exception\n"));
        }

      throw Components::CreateFailure ();
    }



    void
    CIAO_ComponentServer_i::remove_container (::Components::Deployment::Container_ptr cref)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::remove_container");

      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container - remove request received.\n"));

      if (CORBA::is_nil (cref))
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ComponentServer_i::remove_container - "
                       "Error: Received nil container reference\n"));
          throw Components::RemoveFailure ();
        }

      if (this->containers_.is_empty ())
        {
          CIAO_ERROR ((LM_ERROR, CLINFO
                       "CIAO_ComponentServer_i::remove_container - "
                       "Error: I don't manage any containers!\n"));
          throw Components::RemoveFailure ();
        }

      try
        {
          CONTAINERS::ITERATOR i (this->containers_.begin ());

          // @@ TODO: artifact from when this was a sequence, should probably use .find,
          // which will work properly with the new parameterized set class.
          for (CONTAINERS::iterator i = this->containers_.begin ();
               i.done () != 1; i.advance ())
            {
              if (CORBA::is_nil (*i))
                {
                  ACE_ERROR ((LM_WARNING, CLINFO
                              "CIAO_ComponentServer_i::remove_container - "
                              "Managed container reference is nil, skipping.\n"));
                  continue;
                }

              if ((*i)->_is_equivalent (cref))
                {
                  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container - Found container, invoking remove....\n"));
                  cref->remove ();
                  if (this->containers_.remove (*i) != 0)
                    CIAO_ERROR ((LM_ERROR, CLINFO
                                 "CIAO_ComponentServer_i::remove_container - Unable to remove "
                                 "container reference from internal structure....\n"));
                  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container - Remove completed, destroying object, "
                               "now manage %u containers\n", this->containers_.size ()));
                  return;
                }
            }
        }
      catch (const CORBA::Exception &ex)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "CIAO_ComponentServer_i::remove_container - "
                       "Caught CORBA exception whilst removing container: %C\n",
                       ex._info ().c_str ()));
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, CLINFO "CIAO_ComponentServer_i::remove_container - Error: Unknown exception caught while removing a container.\n"));
        }
      throw Components::RemoveFailure ();
    }


    ::Components::Deployment::Containers *
    CIAO_ComponentServer_i::get_containers (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::get_containers");

      ::Components::Deployment::Containers *tmp = 0;
      ACE_NEW_THROW_EX (tmp,
                        ::Components::Deployment::Containers (this->containers_.size ()),
                        CORBA::NO_MEMORY ());

      ::Components::Deployment::Containers_var retval (tmp);
      CORBA::ULong pos (0);
      retval->length (this->containers_.size ());

      for (CONTAINERS::iterator i = this->containers_.begin ();
           i.done () != 1; i.advance ())
        {
          retval[pos++] = ::CIAO::Deployment::Container::_duplicate (*i);
        }

      return retval._retn ();
    }


    void
    CIAO_ComponentServer_i::remove (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::remove");

      bool successful = true;

      for (CONTAINERS::iterator i = this->containers_.begin ();
           i.done () != 1; i.advance ())
        {
          try
            {
              (*i)->remove ();
            }
          catch (const CORBA::Exception &ex)
            {
              successful = false;
              CIAO_ERROR ((LM_ERROR, CLINFO
                           "CIAO_ComponentServer_i::remove - "
                           "Intercepted CORBA exception while trying to remove a container:%C\n",
                           ex._info ().c_str ()));
            }
          catch (...)
            {
              successful = false;
              CIAO_ERROR ((LM_ERROR, CLINFO
                          "CIAO_ComponentServer_i::remove - "
                          "Intercepted exception while trying to remove a container\n"));
            }
        }

      this->containers_.reset ();

      if (!successful)
        throw ::Components::RemoveFailure ();
    }

    void
    CIAO_ComponentServer_i::init (::Components::Deployment::ServerActivator_ptr sa,
                                  Components::ConfigValues *cvs)
    {
      if (!CORBA::is_nil (sa))
        this->serv_act_ = ::Components::Deployment::ServerActivator::_duplicate(sa);

      if (cvs != 0)
        {
          this->config_values_ = cvs;

          for (CORBA::ULong i = 0; i < this->config_values_->length (); ++i)
            {
              if (ACE_OS::strcmp (CIAO::Deployment::COMPONENTINSTALLATION_REF,
                                  this->config_values_[i]->name ()) == 0)
                this->config_values_[i]->value () >>= this->ci_;
            }
        }
    }
  }
}
