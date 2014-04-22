// $Id$

#include "Node_Locator.h"
#include "ace/Read_Buffer.h"
#include "dance/Logger/Log_Macros.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Config_Handlers/Common.h"
#include "dance/DAnCE_Utility.h"
#include "dance/DAnCE_PropertiesC.h"


namespace DAnCE
{
  Node_Locator::Node_Locator (CORBA::ORB_ptr orb,
                              CosNaming::NamingContext_ptr nc)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      nc_ (CosNaming::NamingContext::_duplicate (nc))
  {
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::locate_node (const char *name)
  {
    ACE_CString ior;

    if (this->nodes_.find (name, ior) == 0)
      {
        return this->resolve_ior (name, ior.c_str ());
      }
    else
      {
        return this->ns_lookup (name);
      }
  }

  bool
  Node_Locator::process_cdd (const ACE_TCHAR *filename)
    {
      DANCE_TRACE ("Node_Locator::process_cdd");

      if (!filename)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_ERROR, DLINFO
                        ACE_TEXT("Node_Locator::process_cdd - ")
                        ACE_TEXT("Error: Provided with nil filename\n")));
          return false;
        }

      ::DAnCE::Config_Handlers::XML_File_Intf file (filename);
      file.add_search_path (ACE_TEXT ("DANCE_ROOT"), ACE_TEXT ("/docs/schema/"));
      ::Deployment::Domain *plan = file.release_domain ();

      if (!plan)
        {
          DANCE_ERROR (DANCE_LOG_ERROR,
                       (LM_ERROR, DLINFO
                        ACE_TEXT("Node_Locator::process_cdd - ")
                        ACE_TEXT("Error: Processing file <%C>\n"), filename));
          return false;
        }

      // install nodes
      for (CORBA::ULong i=0; i < plan->node.length (); ++i)
        {
          ::Deployment::Resource resource;

           if (!get_resource_value (DAnCE::NODE_RESOURCE_TYPE,
                                    plan->node[i].resource,
                                    resource))
             {
               DANCE_ERROR (DANCE_LOG_ERROR,
                            (LM_ERROR,
                             DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                             ACE_TEXT("Error: Resource <%C> not found.\n"),
                             DAnCE::NODE_RESOURCE_TYPE));
               return false;
             }
          const ACE_TCHAR *val = 0;
          if (!::DAnCE::Utility::get_satisfierproperty_value (DAnCE::NODE_IOR,
                                                              resource.property,
                                                              val))
            {
              DANCE_ERROR (DANCE_LOG_ERROR,
                           (LM_ERROR,
                            DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                            ACE_TEXT("Error: Property <%C> not found.\n"),
                            DAnCE::NODE_IOR));
              return false;

            }
          ACE_CString ior(val);
          ior += "/";
          ior += plan->node[i].name;
          ior += ".NodeManager";

          ACE_CString destination (plan->node[i].name);

          DANCE_DEBUG (DANCE_LOG_MAJOR_DEBUG_INFO,
                      (LM_INFO, DLINFO ACE_TEXT("Node_Locator::process_cdd - ")
                       ACE_TEXT("Storing IOR %C for destination %C\n"),
                       ior.c_str (), destination.c_str ()));

          this->nodes_.bind (destination, ior);
      }

    return true;
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::resolve_ior (const char *name, const char *ior)
  {
    DANCE_TRACE ("Node_Locator::resolve_ior");

    DANCE_DEBUG (DANCE_LOG_MINOR_EVENT,
                 (LM_DEBUG, DLINFO
                  ACE_TEXT("Node_Locator::resolve_ior - ")
                  ACE_TEXT("Resolving ior %C for destination %C\n"),
                  ior, name));

    CORBA::Object_var obj = this->orb_->string_to_object (ior);

    ::Deployment::NodeManager_var nm = ::Deployment::NodeManager::_narrow (obj.in ());

    if (CORBA::is_nil (nm.in ()))
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO ACE_TEXT("Node_Locator::resolve_ior - ")
                      ACE_TEXT("Error: Unable to retrieve reference for destination ")
                      ACE_TEXT("%C and ior %C\n"),
                      name, ior));
      }

    return nm._retn ();
  }

  void
  Node_Locator::store_ior (const char *name, const char *ior)
  {
    DANCE_TRACE ("Node_Locator::store_ior");
    this->nodes_.bind (name, ior);
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::ns_lookup (const char *nodename)
  {
    DANCE_TRACE ("Node_Locator::ns_lookup");

    if (CORBA::is_nil (this->nc_.in ()))
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Nameservice lookup of %C failed because there is no naming service.\n"),
                      nodename));
        return ::Deployment::NodeManager::_nil ();
      }

    try
      {
        CosNaming::Name name;
        name.length (1);

        name[0].id = nodename;
        name[0].kind = "NodeManager";

        CORBA::Object_var obj = this->nc_->resolve (name);

        ::Deployment::NodeManager_var nm = ::Deployment::NodeManager::_narrow (obj.in ());

        if (CORBA::is_nil (nm.in ()))
          {
            DANCE_ERROR (DANCE_LOG_ERROR,
                         (LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                          ACE_TEXT("Unable to narrow provided reference for node %C\n"),
                          nodename));
          }

        return nm._retn ();
      }
    catch (const CORBA::Exception &e)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Caught CORBA exception while looking up name %C:%C\n"),
                      nodename, e._info ().c_str ()));
      }
    catch (...)
      {
        DANCE_ERROR (DANCE_LOG_ERROR,
                     (LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Caught unexpected exception while looking up name %C\n"),
                      nodename));
      }

    return ::Deployment::NodeManager::_nil ();
  }

  bool
  Node_Locator::get_resource_value (const char *type,
                                      const ::Deployment::Resources &resources,
                                     ::Deployment::Resource  &val)
  {
    DANCE_TRACE ("Node_Locator::get_resource_value<const char *>");

    DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                 (LM_TRACE, DLINFO
                  ACE_TEXT("Node_Locator::get_resource_value - ")
                  ACE_TEXT("Finding resource for type '%C'\n"),
                  type));

    for (CORBA::ULong i = 0; i < resources.length (); ++i)
      {
        // search for the resource with resourceType
        for (CORBA::ULong k = 0;k < resources[i].resourceType.length ();k++)
          {
            if (ACE_OS::strcmp (type,
                                 resources[i].resourceType[k]) == 0)
              {
                DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
                             (LM_TRACE, DLINFO
                              ACE_TEXT("Node_Locator::get_resource_value - ")
                              ACE_TEXT("Found resource for type '%C'\n"),
                              type));

                val = resources[i];
                return true;
              }
          }
        }
      DANCE_ERROR (DANCE_LOG_ERROR,
                   (LM_WARNING, DLINFO
                    ACE_TEXT("Node_Locator::get_resource_value - ")
                    ACE_TEXT("Failed to extract resource for %C\n"),
                    type));
      return false;
    }

}

