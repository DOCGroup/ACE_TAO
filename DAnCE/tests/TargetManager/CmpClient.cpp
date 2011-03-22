// $Id$
/**
 * @file CmpClient.cpp
 *
 * @brief This file contains a client of TargetManager.
 *
 * It picks up the TM ior from the Targetmanager.ior file
 * present in the current directory, and makes the calls on the
 * TM to getAllResources and getAvailable resources
 */

#include "Deployment/Deployment_DataC.h"
#include "Deployment/Deployment_ResourceCommitmentManagerC.h"
#include "DAnCE/TargetManager/TargetManagerC.h"
#include "ace/iosfwd.h"
#include "ace/FILE_IO.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_Addr.h"
#include "ace/Get_Opt.h"
#include "Config_Handlers/DnC_Dump.h"

/**
 * TM_Tester contains the code to test the TM Component
 */
namespace TM_Tester
{
  /**
   * writes the extracted data to file
   */
  void write_to_file (::Deployment::Domain domain);

  /// variable contains IOR of the TM
  const ACE_TCHAR * stringified_TM_IOR;

  /// if add or delete from domain
  bool add_to_domain = true;

  /// whether to test update domain or not
  bool call_update = false;

  /// contains the host name
  const ACE_TCHAR* host_name;

    /// parses the arguments and extracts the params
  bool parse_args (int argc, ACE_TCHAR *argv[])
  {
    ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:u:d"));
    int c;
    while ((c = get_opts ()) != -1)
      switch (c)
      {
        case 't':
          stringified_TM_IOR = get_opts.opt_arg ();
          break;
        case 'u':
          host_name = get_opts.opt_arg ();
          call_update = true;
          break;
        case 'd':
          add_to_domain = false;
          break;
        case '?':  // display help for use of the server.
        default:
          DANCE_ERROR_RETURN (1, (LM_ERROR,
                "usage:  %s\n"
                "-t <TM_IOR>\n"
                "-u <host_name in update>\n"
                "-n <delete , default add>\n"
                "\n",
                argv [0]),
              false);
      }

    return true;
  }
}

  /// The main function
  int ACE_TMAIN (int argc, ACE_TCHAR* argv[])
  {
    try {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (!TM_Tester::parse_args (argc, argv))
        return -1;

      // Use the first argument to create the factory object reference,
      // in real applications we use the naming service, but let's do
      // the easy part first!
      CORBA::Object_var factory_object =
        orb->string_to_object (TM_Tester::stringified_TM_IOR);

      // Now downcast the object reference to the appropriate type
      CIAO::TargetManagerImpl_var targetCmp =
        CIAO::TargetManagerImpl::_narrow (factory_object.in ());

      // Now get the facet reference from the target Manager Component
      Deployment::TargetManager_ptr targetI = targetCmp->provide_targetMgr ();

      // Now make calls on the Target Manager facet

      try
      {
        Deployment::Domain_var domainV = targetI->getAllResources ();
        ::Deployment::DnC_Dump::dump (domainV);
      }
      catch(CORBA::NO_IMPLEMENT &)
      {
        DANCE_ERROR (1, (LM_ERROR ,"Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
      }
      catch(CORBA::Exception &)
      {
        DANCE_ERROR (1, (LM_ERROR ,"Error:TargetManager:CORBA Generic Exception\n"));
        DANCE_ERROR (1, (LM_ERROR ,"Error:TargetManager:Exception in TargetManager call\n"));
      }


      // make a call to the commit resources .....

      bool resource_available = true;

      ::Deployment::ResourceAllocations resource_seq;

      resource_seq.length (1);

      resource_seq[0].elementName = CORBA::string_dup ("TargetManagerNode_1");

      resource_seq[0].resourceName = CORBA::string_dup ("Processor");

      resource_seq[0].property.length (1);
      resource_seq[0].property[0].name =
        CORBA::string_dup ("LoadAverage");

      CORBA::Long d = 20;
      resource_seq[0].property[0].value <<= d;

      ::Deployment::ResourceCommitmentManager_ptr manager =
        ::Deployment::ResourceCommitmentManager::_nil ();

      try
      {
        manager = targetI->commitResources (resource_seq);
      }
      catch(const CORBA::NO_IMPLEMENT &)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
      }
      catch (const ::Deployment::ResourceCommitmentFailure& e)
      {
        resource_available = 0;
        DANCE_ERROR (1, (LM_ERROR, "TargetManager commitResources ResourceCommitmentFailure Exception\n"));

        DANCE_ERROR (1, (LM_ERROR ,
              "ResourceCommitmentFailure\n reason=[%C]\n elementName=[%C]\n resourceName=[%C]\n propertyName=[%C]\n",
              e.reason.in (),
              resource_seq[e.index].elementName.in (),
              resource_seq[e.index].resourceName.in (),
              e.propertyName.in ()));
      }
      catch(const CORBA::Exception &)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:commitResources Exception\n"));
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA Generic Exception\n"));
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:Exception in TargetManager call"));
      }

      // Make a call to release resources , if resource < 0
      try
      {
        {
          d = 10;
          resource_seq[0].property[0].value <<= d;
          manager->releaseResources (resource_seq);
        }
      }
      catch(const CORBA::NO_IMPLEMENT &)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
      }
      catch (const Deployment::ResourceCommitmentFailure&)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager releaseResources ResourceNotAvailable Exception\n"));
      }
      catch(const CORBA::Exception &)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:releaseResources Exception\n"));
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA Generic Exception\n"));
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:Exception in TargetManager call"));
      }

      // Here make a call on the TM with update domain and node deletion

      ::Deployment::Domain updated;
      updated.node.length (1);
      updated.node[0].name = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (TM_Tester::host_name));

      ::CORBA::StringSeq elements;
      elements.length (0);


      if (TM_Tester::call_update)
      {
        if (TM_Tester::add_to_domain)
        {
          try
          {
            targetI->updateDomain (elements , updated, ::Deployment::Add);
          }
          catch(CORBA::NO_IMPLEMENT &)
          {
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
          }
          catch(CORBA::Exception &)
          {
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA Generic Exception\n"));
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:Exception in UpdateDomain call"));
          }
        }
        else
        {
          try
          {
            targetI->updateDomain (elements , updated, ::Deployment::Delete);
          }
          catch(CORBA::NO_IMPLEMENT &)
          {
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
          }
          catch(CORBA::Exception &)
          {
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA Generic Exception\n"));
            DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:Exception in UpdateDomain call"));
          }
        }
      }

      // Now make a call of getAvailableResources on the TargetManager ...
      try
      {
        Deployment::Domain_var domainV = targetI->getAvailableResources();

        // here write things to file ...
        TM_Tester::write_to_file (domainV.in());

        ::Deployment::DnC_Dump::dump (domainV);
      }
      catch(CORBA::NO_IMPLEMENT &)
      {
        DANCE_ERROR (1, (LM_ERROR, "Error:TargetManager:CORBA::NO_IMPLEMENT thrown\n"));
      }
      catch(CORBA::Exception &)
      {
        DANCE_ERROR (1, (LM_ERROR ,"Error:TargetManager:CORBA Generic Exception\n"));
        DANCE_ERROR (1, (LM_ERROR,  "Error:TargetManager:Exception in TargetManager call\n"));
      }

      // Finally destroy the ORB
      orb->destroy ();
    }
    catch (CORBA::Exception &)
    {
      DANCE_ERROR (1, (LM_ERROR,  "Error:TargetManager:CORBA exception raised!\n"));
    }
    return 0;
  }

namespace TM_Tester
{
  void write_to_file (::Deployment::Domain domain)
  {
    for (size_t i = 0;i < domain.node.length ();i++)
    {
      std::ofstream out (domain.node[i].name.in ());

      // write in the node usage ...
      for (CORBA::ULong j = 0;j < domain.node[i].resource.length ();j++)
      {

        if (!ACE_OS::strcmp (domain.node[i].resource[j].name.in (), "Processor"))
        {
          CORBA::Double node_cpu;
          domain.node[i].resource[j].property[0].value >>= node_cpu;
          out << node_cpu << std::endl;
        }
        if (!ACE_OS::strcmp (domain.node[i].resource[j].name.in (), "NA_Monitor"))
        {
          std::string file_name = "NA_";
          file_name += domain.node[i].name.in ();
          ACE_FILE_IO file_io;
          ACE_FILE_Connector (file_io, ACE_FILE_Addr (ACE_TEXT_ALWAYS_CHAR (file_name.c_str ())));
          CORBA::Double na_node_cpu;
          domain.node[i].resource[j].property[0].value >>= na_node_cpu;
          char buf[BUFSIZ];
          ACE_OS::memset (buf , 0 , BUFSIZ);
          ACE_OS::sprintf (buf , "%f", na_node_cpu);
          file_io.send (buf, ACE_OS::strlen (buf));
        }
      }

      out.close ();
    }
  }
}
