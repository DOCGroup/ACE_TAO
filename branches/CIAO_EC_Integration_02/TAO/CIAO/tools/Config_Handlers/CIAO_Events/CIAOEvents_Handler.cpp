// $Id$
#include "Utils/XML_Helper.h"
#include "CIAOEvents_Handler.h"
#include "CIAOEvents.hpp"
#include "ciao/Deployment_EventsC.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    CIAOEvents_Handler::CIAOEvents_Handler (const ACE_TCHAR *file) :
      idl_esd_(0),
      esd_(0),
      retval_ (false)
    {
      XML_Helper helper;

      XERCES_CPP_NAMESPACE::DOMDocument *dom =
        helper.create_dom (file);

      if (!dom)
        throw CIAOEvents_Handler::NoESD ();

      this->esd_.reset (new CIAOEventsDef
        (CIAOEvents (dom)));

      if (!this->build_esd ())
        throw NoESD ();
    }

    CIAOEvents_Handler::CIAOEvents_Handler (CIAOEventsDef *esd):
      idl_esd_(0),
      esd_(esd),
      retval_(false)
    {
      if(!this->build_esd())
        throw NoESD ();
    }


    CIAOEvents_Handler::~CIAOEvents_Handler (void)
    {
    }

    bool
    CIAOEvents_Handler::build_esd ()
    {
      this->idl_esd_.reset ( new ::CIAO::DAnCE::EventServiceDeploymentDescriptions );


      CORBA::ULong num (this->idl_esd_->length ());

      this->idl_esd_->length (this->esd_->count_eventServiceConfiguration ());

      for (CIAOEventsDef::eventServiceConfiguration_const_iterator i = this->esd_->begin_eventServiceConfiguration ();
           i != this->esd_->end_eventServiceConfiguration ();
           i++)
      {
           CIAO::DAnCE::EventServiceDeploymentDescription a_esd;

           a_esd.name = CORBA::string_dup (i->name ().c_str ());
           a_esd.node = CORBA::string_dup (i->node ().c_str ());

           switch (i->type ().integral ())
           {
             case ::CIAO::Config_Handlers::EventServiceType::EC_l:
               a_esd.type = CIAO::DAnCE::EC;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::NOTIFY_l:
               a_esd.type = CIAO::DAnCE::NOTIFY;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::RTEC_l:
               a_esd.type = CIAO::DAnCE::RTEC;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::RTNOTIFY_l:
               a_esd.type = CIAO::DAnCE::RTNOTIFY;
               break;
             default:
             ACE_ERROR ((LM_ERROR,
                        "Invalid event service type\n"));
             return false;
           }

           a_esd.svc_cfg_file = CORBA::string_dup  (i->svc_cfg_file ().c_str ());

           (*this->idl_esd_)[num] = a_esd;
           num++;
      }
      return true;
    }


    ::CIAO::DAnCE::EventServiceDeploymentDescriptions const *
        CIAOEvents_Handler::esd_idl () const
      throw (CIAOEvents_Handler::NoESD)
      {
        if(!this->idl_esd_.get())
          throw NoESD ();

         //else
        return this->idl_esd_.get();
      }

    ::CIAO::DAnCE::EventServiceDeploymentDescriptions *
        CIAOEvents_Handler::esd_idl ()
      throw (CIAOEvents_Handler::NoESD)
      {
        if(!this->idl_esd_.get())
          throw NoESD();

        //else
        return this->idl_esd_.release();
      }
   }
}

