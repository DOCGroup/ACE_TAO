// $Id$
#include "Utils/XML_Typedefs.h"
#include "SRD_Handler.h"
#include "CIAOServerResources.hpp"
#include "CLA_Handler.h"
#include "OC_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    SRD_Handler::SRD_Handler (const ACE_TCHAR *file) :
      idl_srd_(0),
      srd_ (0),
      retval_ (false)
    {
      XERCES_CPP_NAMESPACE::DOMDocument *dom =
        XML_HELPER.create_dom (file);

      if (!dom)
        throw SRD_Handler::NoSRD ();

      this->srd_.reset (new ServerResourcesDef
                        (ServerResources (dom)));

      if (!this->build_srd ())
        throw NoSRD ();
    }

    SRD_Handler::SRD_Handler (ServerResourcesDef *srd):
      idl_srd_(0),
      srd_(srd),
      retval_(false)
    {
      if(!this->build_srd())
        throw NoSRD ();
    }

    SRD_Handler::SRD_Handler (::CIAO::DAnCE::ServerResource *srd):
      idl_srd_(srd),
      srd_(0),
      retval_(false)
    {
      if(!this->build_xsc())
        throw NoSRD ();
    }

    SRD_Handler::~SRD_Handler (void)
    {
    }

    bool
    SRD_Handler::build_srd ()
    {
      this->idl_srd_.reset ( new ::CIAO::DAnCE::ServerResource );

      if(this->srd_->cmdline_p())
        {
          this->retval_ = CLA_Handler::commandline_arguments(*this->srd_,
                                                             this->idl_srd_->args);
        }

      if(this->srd_->svcconf_p())
        {
          this->idl_srd_->svcconf = (srd_->svcconf().uri().c_str());
        }

      this->retval_ = OC_Handler::orb_config (this->srd_->orbConfigs (),
                                              this->idl_srd_->orb_config);

      if (this->srd_->id_p ())
        {
          this->idl_srd_->Id = srd_->id ().c_str ();
        }

      return this->retval_;
    }

    bool
    SRD_Handler::build_xsc ()
    {
      size_t len; //For checking the length of src data members

      // Load the ORBConfigs and create the XSC structure for SRD
      this->srd_.reset (new ServerResourcesDef (OC_Handler::orb_config (this->idl_srd_->orb_config)));


      //Note: Why did we make the CmdlineOptions a separate class instead of a
      //      vector of strings???

      ServerCmdlineOptions sco;

      //First load up the CmdlineArgs if they exist
      len = idl_srd_->args.length();
      for(size_t i = 0; i < len; i++)
        {
          XMLSchema::string < ACE_TCHAR > curr ((this->idl_srd_->args[i]));
          sco.add_arg(curr);
        }
      this->srd_->cmdline (sco);

      //Then a similar thing for the svcconf
      XMLSchema::string < ACE_TCHAR > curr (this->idl_srd_->svcconf);
      ACESvcConf asc;
      asc.uri(curr);
      this->srd_->svcconf (asc);

      XMLSchema::string < ACE_TCHAR > id (this->idl_srd_->Id.in ());
      this->srd_->id  (id);

      return true;
    }

    ::CIAO::DAnCE::ServerResource const *
    SRD_Handler::srd_idl () const
      throw (SRD_Handler::NoSRD)
    {
      if(!this->idl_srd_.get())
        throw NoSRD ();

      //else
      return this->idl_srd_.get();
    }

    ::CIAO::DAnCE::ServerResource *
    SRD_Handler::srd_idl ()
      throw (SRD_Handler::NoSRD)
    {
      if(!this->idl_srd_.get())
        throw NoSRD();

      //else
      return this->idl_srd_.release();
    }

    ServerResourcesDef const *
    SRD_Handler::srd_xsc () const
      throw (SRD_Handler::NoSRD)
    {
      if(!this->srd_.get())
        throw NoSRD ();

      //else
      return this->srd_.get();
    }

    ServerResourcesDef *
    SRD_Handler::srd_xsc ()
      throw (SRD_Handler::NoSRD)
    {
      if(!this->srd_.get())
        throw NoSRD ();

      //else
      return this->srd_.release();
    }
  }
}


