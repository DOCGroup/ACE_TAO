// $Id: $
#include "Utils/XML_Helper.h"
#include "NetQoS_Handler.h"
#include "NetQoSRequirements.hpp"
#include "ciao/NetQoSC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    NetQoS_Handler::NetQoS_Handler (const ACE_TCHAR *file) :
      idl_netqos_(0),
      netqos_ (0),
      retval_ (false)
    {
      XML_Helper helper;

      XERCES_CPP_NAMESPACE::DOMDocument *dom =
        helper.create_dom (file);

      if (!dom)
        throw NetQoS_Handler::NoNetQoS ();

      this->netqos_.reset (new NetQoSRequirementsDef
                        (NetQoSRequirements (dom)));

      if (!this->build_netqos ())
        throw NoNetQoS ();
    }

    NetQoS_Handler::NetQoS_Handler (NetQoSRequirementsDef *netqos):
      idl_netqos_(0),
      netqos_(netqos),
      retval_(false)
    {
      if(!this->build_netqos())
        throw NoNetQoS ();
    }

    NetQoS_Handler::NetQoS_Handler (::CIAO::DAnCE::NetQoSRequirement *netqos):
      idl_netqos_(netqos),
      netqos_(0),
      retval_(false)
    {
      if(!this->build_xsc())
        throw NoNetQoS ();
    }


    NetQoS_Handler::~NetQoS_Handler (void)
    {
    }

    bool
    NetQoS_Handler::build_netqos ()
    {
/*
      this->idl_netqos_.reset ( new ::CIAO::DAnCE::NetQoSRequirement );

      if(this->netqos_->cmdline_p())
        {
          this->retval_ = CLA_Handler::commandline_arguments(*this->netqos_,
                                                             this->idl_netqos_->args);
        }

      if(this->netqos_->svcconf_p())
        {
          this->idl_netqos_->svcconf = (netqos_->svcconf().uri().c_str());
        }

      this->retval_ = OC_Handler::orb_config (this->netqos_->orbConfigs (),
                                              this->idl_netqos_->orb_config);

      if (this->netqos_->id_p ())
        {
          this->idl_netqos_->Id = netqos_->id ().c_str ();
        }
*/
      return this->retval_;
    }

    bool
    NetQoS_Handler::build_xsc ()
    {
/*
      size_t len; //For checking the length of src data members

      // Load the ORBConfigs and create the XSC structure for NetQoS
      this->netqos_.reset (new NetQoSRequirementsDef (OC_Handler::orb_config (this->idl_netqos_->orb_config)));


      //Note: Why did we make the CmdlineOptions a separate class instead of a
      //      vector of strings???

      ServerCmdlineOptions sco;

      //First load up the CmdlineArgs if they exist
      len = idl_netqos_->args.length();
      for(size_t i = 0; i < len; i++)
        {
          XMLSchema::string < ACE_TCHAR > curr ((this->idl_netqos_->args[i]));
          sco.add_arg(curr);
        }
      this->netqos_->cmdline (sco);

      //Then a similar thing for the svcconf
      XMLSchema::string < ACE_TCHAR > curr (this->idl_netqos_->svcconf);
      ACESvcConf asc;
      asc.uri(curr);
      this->netqos_->svcconf (asc);

      XMLSchema::string < ACE_TCHAR > id (this->idl_netqos_->Id.in ());
      this->netqos_->id  (id);
*/
      return true;
    }

    ::CIAO::DAnCE::NetQoSRequirement const *
    NetQoS_Handler::netqos_idl () const
      throw (NetQoS_Handler::NoNetQoS)
    {
      if(!this->idl_netqos_.get())
        throw NoNetQoS ();

      //else
      return this->idl_netqos_.get();
    }

    ::CIAO::DAnCE::NetQoSRequirement *
    NetQoS_Handler::netqos_idl ()
      throw (NetQoS_Handler::NoNetQoS)
    {
      if(!this->idl_netqos_.get())
        throw NoNetQoS();

      //else
      return this->idl_netqos_.release();
    }

    NetQoSRequirementsDef const *
    NetQoS_Handler::netqos_xsc () const
      throw (NetQoS_Handler::NoNetQoS)
    {
      if(!this->netqos_.get())
        throw NoNetQoS ();

      //else
      return this->netqos_.get();
    }

    NetQoSRequirementsDef *
    NetQoS_Handler::netqos_xsc ()
      throw (NetQoS_Handler::NoNetQoS)
    {
      if(!this->netqos_.get())
        throw NoNetQoS ();

      //else
      return this->netqos_.release();
    }
  }
}


