#include "CIAO_ComponentServer_Impl.h"

#include <ciao/CIAO_common.h>

#include "CIAO_CS_ClientC.h"
#include "CIAO_Container_Impl.h"

namespace CIAO
{
  namespace Deployment
  {
    CIAO_ComponentServer_i::CIAO_ComponentServer_i (const ACE_CString &uuid, CORBA::ORB_ptr orb, 
						    PortableServer::POA_ptr poa)
      : uuid_ (uuid),
        orb_ (CORBA::ORB::_duplicate (orb)),
	poa_ (PortableServer::POA::_duplicate (poa))
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
      
      CIAO_DEBUG ((LM_DEBUG, CLINFO "CIAO_ComponentServer_i::shutdown: ORB shutdown request received at %s.\n",
                  this->uuid_.c_str ()));
      
      if (this->containers_->length () != 0)
        CIAO_ERROR ((LM_ERROR, CLINFO "CIAO_ComponentServer_i::shutdown: ComponentServer %s still managing %u containers!\n",
                    this->uuid_.c_str (), this->containers_->length ()));
      
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
          CIAO_DEBUG ((LM_INFO, "CIAO_ComponentServer_i::create_container: Request received with %u config values\n",
                      config.length ()));
	  
	  CORBA::PolicyList policies;
	  const char *name = "";

          CIAO_Container_i *cont = 0;
          ACE_NEW_THROW_EX (cont, 
			    CIAO_Container_i (config, 0, name, &policies, this->orb_.in (), this->poa_.in ()), 
			    CORBA::NO_MEMORY ());
          
          PortableServer::ServantBase_var safe_config = cont;
          Components::Deployment::Container_var cont_var = cont->_this  ();
          
          CORBA::ULong len = this->containers_->length () + 1;
          this->containers_->length (len);
          this->containers_[len] = cont_var;
          
          CIAO_DEBUG ((LM_INFO, "CIAO_ComponentServer_i::create_container: Container successfully cresated, "
                      "this server now manages %u containers.\n",
                      len));
          
          return this->containers_[len].in ();
        }
      catch (ACE_bad_alloc &)
        {
          CIAO_ERROR ((LM_CRITICAL, "CIAO_ComponentServer_Impl: Out of memory exception whilst creating container.\n"));
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
      
      CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container: remove request received.\n"));
      
      try
        {
          for (CORBA::ULong i = 0; i < this->containers_->length (); ++i)
            {
              if (this->containers_[i]->_is_equivalent (cref))
                {
                  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container: Found container, invoking remove....\n"));
                  cref->remove ();
                  CIAO_DEBUG ((LM_TRACE, CLINFO "CIAO_ComponentServer_i::remove_container: Remove completed, destroying object...\n"));
                }
            }
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, "CIAO_ComponentServer_i::remove_container: Error: Unknown exception caught while removing a container.\n"));
        }
      throw Components::RemoveFailure ();
    }
      
      
    ::Components::Deployment::Containers * 
    CIAO_ComponentServer_i::get_containers (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::get_containers");
      return this->containers_;
    }
      
      
    void 
    CIAO_ComponentServer_i::remove (void)
    {
      CIAO_TRACE("CIAO_ComponentServer_i::remove");
    }
    
    void 
    CIAO_ComponentServer_i::init (::Components::Deployment::ServerActivator_ptr sa,
                                  Components::ConfigValues *cvs)
    {
      this->serv_act_ = ::Components::Deployment::ServerActivator::_duplicate(sa);
      this->config_values_ = cvs;
    }
  }
}
