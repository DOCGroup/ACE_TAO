// $Id$

#include "ESD_Handler.h"
#include "Basic_Deployment_Data.hpp"
#include "DAnCE/Deployment/Deployment_DataC.h"
#include "cdp.hpp"

namespace DAnCE
{
  namespace Config_Handlers
  {
    void
    ESD_Handler::es_deployment_descrs (
        const DeploymentPlan &src,
        Deployment::EventServiceDeploymentDescriptions& dest)
    {
      DANCE_TRACE("ESD_Handler::instance_deployment_descrs");
      DeploymentPlan::es_const_iterator esd_e = src.end_instance ();

      CORBA::ULong pos = 0;
      dest.length (src.count_instance ());
      for (DeploymentPlan::es_const_iterator esd_b =
             src.begin_instance ();
           esd_b != esd_e;
           ++esd_b)
        {
          ESD_Handler::instance_deployment_descr ((*esd_b),
                    dest[pos], pos);
          pos++;
        }
    }


    void
    ESD_Handler::es_deployment_descr (
        const EventServiceDeploymentDescription& src,
        Deployment::EventServiceDeploymentDescription& dest,
        CORBA::ULong pos)
    {
      DANCE_TRACE("IDD_Handler::es_deployment_descr");
      try
      {
        dest.name = src.name ().c_str ();
        dest.node = src.node ().c_str ();
        dest.svc_cfg_file = src.svc_cfg_file ().c_str ());

        switch (desc.type ().integral ())
        {
          case EventServiceType::EC_l:
            dest.type = Deployment::EC;
            break;

          case EventServiceType::RTEC_l:
            dest.type = Deployment::RTEC;
            break;

          case EventServiceType::NOTIFY_l:
            dest.type = Deployment::NOTIFY;
            break;

          case EventServiceType::RTNOTIFY_l:
            dest.type = Deployment::RTNOTIFY;
            break;

          default:
            throw Config_Error (desc.requirementName (),
                    "Unknown EventServiceType.");
            break;
          }
      }
      catch (Config_Error &ex)
      {
        ex.name_ = src.name ()  + ":" + ex.name_;
        throw ex;
      }
          // Done!
    }

    EventServiceDeploymentDescription
    ESD_Handler::es_deployment_descr (
        const Deployment::EventServiceDeploymentDescription& src)
    {
      DANCE_TRACE("ESD_Handler::es_deployment_descr - reverse");
      //Get all the string
      XMLSchema::string < ACE_TCHAR > name ((src.name));
      XMLSchema::string < ACE_TCHAR > node ((src.node));
      XMLSchema::string < ACE_TCHAR > svc_cfg_file ((src.svc_cfg_file));

      // Instantiate the IDD
      EventServiceDeploymentDescription esd (
        name,
        node,
        EventServiceType::RTEC_l,
        svc_cfg_file);

      switch (src.EventServiceType)
      {
      case Deployment::EC:
        esd.EventServiceType (EventServiceType::EC);
        break;

      case Deployment::RTEC:
        esd.EventServiceType (EventServiceType::RTEC);
        break;

      case Deployment::NOTIFY:
        esd.EventServiceType (EventServiceType::NOTIFY);
        break;

      case Deployment::RTNOTIFY:
        esd.EventServiceType (EventServiceType::RTNOTIFY);
        break;

      default:
        throw Config_Error (src.resourceName.in (),
                "Unknown EventServiceType.");
        break;
      }

      return esd;
    }

  }
}
