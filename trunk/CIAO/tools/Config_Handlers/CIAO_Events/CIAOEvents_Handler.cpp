// $Id$
#include "Utils/XML_Helper.h"
#include "CIAOEvents_Handler.h"
#include "CIAOEvents.hpp"
#include "DAnCE/Deployment/Deployment_EventsC.h"

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


      this->idl_esd_->length (this->esd_->count_eventServiceConfiguration ());
      CORBA::ULong pos_i = 0;
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
               a_esd.type = CIAO::EC;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::NOTIFY_l:
               a_esd.type = CIAO::NOTIFY;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::RTEC_l:
               a_esd.type = CIAO::RTEC;
               break;
             case ::CIAO::Config_Handlers::EventServiceType::RTNOTIFY_l:
               a_esd.type = CIAO::RTNOTIFY;
               break;
             default:
             ACE_ERROR ((LM_ERROR,
                        "Invalid event service type\n"));
             return false;
           }

           a_esd.svc_cfg_file = CORBA::string_dup  (i->svc_cfg_file ().c_str ());


           // Populate filtering information for this event channel
           a_esd.filters.length (i->count_filter ());
           CORBA::ULong pos_j = 0;
           for (EventServiceDescription::filter_const_iterator j = i->begin_filter ();
                j != i->end_filter ();
                j++)
           {
              a_esd.filters[pos_j].name = CORBA::string_dup (j->name ().c_str ());
              switch (j->type ().integral ())
              {
                  case ::CIAO::Config_Handlers::FilterType::CONJUNCTION_l:
                    a_esd.filters[pos_j].type = CIAO::DAnCE::CONJUNCTION;
                    break;
                  case ::CIAO::Config_Handlers::FilterType::DISJUNCTION_l:
                    a_esd.filters[pos_j].type = CIAO::DAnCE::DISJUNCTION;
                    break;
                  case ::CIAO::Config_Handlers::FilterType::LOGICAL_AND_l:
                    a_esd.filters[pos_j].type = CIAO::DAnCE::LOGICAL_AND;
                    break;
                  case ::CIAO::Config_Handlers::FilterType::NEGATE_l:
                    a_esd.filters[pos_j].type = CIAO::DAnCE::NEGATE;
                    break;
                    default:
                    ACE_ERROR ((LM_ERROR,
                              "Invalid filter type\n"));
                  return false;
               }

               a_esd.filters[pos_j].sources.length (j->count_source ());
               CORBA::ULong pos_k = 0;
               for (Filter::source_const_iterator k = j->begin_source ();
                    k != j->end_source ();
                    k++)
               {
                 a_esd.filters[pos_j].sources[pos_k] = CORBA::string_dup (k->c_str ());
                 pos_k++;

               }
               pos_j++;
            }

           // Populate address server information for this event channel
           a_esd.addr_servs.length (i->count_addr_serv ());
           pos_j = 0;
           for (EventServiceDescription::addr_serv_const_iterator j = i->begin_addr_serv ();
                j != i->end_addr_serv ();
                j++)
           {
              a_esd.addr_servs[pos_j].name = CORBA::string_dup (j->name ().c_str ());
              a_esd.addr_servs[pos_j].port = j->port ();
              a_esd.addr_servs[pos_j].address = CORBA::string_dup (j->address ().c_str ());
              pos_j++;
            }

           // Populate UDP sender information for this event channel
           a_esd.senders.length (i->count_udp_sender ());
           pos_j = 0;
           for (EventServiceDescription::udp_sender_const_iterator j = i->begin_udp_sender ();
                j != i->end_udp_sender ();
                j++)
           {
              a_esd.senders[pos_j].name = CORBA::string_dup (j->name ().c_str ());
              a_esd.senders[pos_j].addr_serv_id = 
                CORBA::string_dup (j->addr_serv_id ().c_str ());
              pos_j++;
            }

           // Populate UDP receiver information for this event channel
           a_esd.receivers.length (i->count_udp_receiver ());
           pos_j = 0;
           for (EventServiceDescription::udp_receiver_const_iterator j = i->begin_udp_receiver ();
                j != i->end_udp_receiver ();
                j++)
           {
              a_esd.receivers[pos_j].name = CORBA::string_dup (j->name ().c_str ());
              a_esd.receivers[pos_j].addr_serv_id = 
                CORBA::string_dup (j->addr_serv_id ().c_str ());
              a_esd.receivers[pos_j].is_multicast = j->is_multicast ();
              a_esd.receivers[pos_j].listen_port = j->listen_port ();
              pos_j++;
            }

           (*this->idl_esd_)[pos_i] = a_esd;
           pos_i++;
      }
      return true;
    }

    ::CIAO::DAnCE::EventServiceDeploymentDescriptions const *
    CIAOEvents_Handler::esd_idl () const
    {
      if(!this->idl_esd_.get())
        throw NoESD ();

      //else
      return this->idl_esd_.get();
    }

    ::CIAO::DAnCE::EventServiceDeploymentDescriptions *
    CIAOEvents_Handler::esd_idl ()
    {
      if(!this->idl_esd_.get())
        throw NoESD();

      //else
      return this->idl_esd_.release();
    }
  }
}

