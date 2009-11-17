// $Id$

#include "Node_Locator.h"
#include "ace/Read_Buffer.h"
#include "DAnCE/Logger/Log_Macros.h"

namespace DAnCE
{
  Node_Locator::Node_Locator (CORBA::ORB_ptr orb,
                              CosNaming::NamingContext_ptr nc)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      nc_ (CosNaming::NamingContext::_duplicate (nc))
  {
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::locate_node (const ACE_TCHAR *name)
  {
    ACE_TString ior;

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
  Node_Locator::process_node_map (const ACE_TCHAR *filename)
  {
    DANCE_TRACE ("Node_Locator::process_node_map");

    if (filename == 0)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::process_node_map - ")
                      ACE_TEXT("Error: Provided with nil filename\n")));
        return false;
      }

    FILE *inf = ACE_OS::fopen (filename, ACE_TEXT("r"));

    if (inf == 0)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::process_node_map - ")
                      ACE_TEXT("Fail to open node manager map data file: <%s>\n"),
                      filename));
        return false;
      }

    ACE_Read_Buffer reader (inf, true);

    ACE_TCHAR * string = 0;
    // Read from the file line by line
    while ((string = ACE_TEXT_CHAR_TO_TCHAR (reader.read ('\n'))) != 0)
      {
        if (ACE_OS::strlen (string) == 0) continue;

        // Search from the right to the first space
        const ACE_TCHAR* ior_start = ACE_OS::strrchr (string, ' ');
        // Search from the left to the first space
        const ACE_TCHAR* dest_end = ACE_OS::strchr (string, ' ');

        // The destination is first followed by some spaces
        ACE_TString destination (string, dest_end - string);
        // And then the IOR
        ACE_TString ior (ior_start + 1,  ACE_OS::strlen (ior_start + 1));
        reader.alloc ()->free (string);

        DANCE_DEBUG ((LM_INFO, DLINFO ACE_TEXT("Node_Locator::process_node_map - ")
                      ACE_TEXT("Storing IOR %C for destination %C\n"),
                      ior.c_str (), destination.c_str ()));
        this->nodes_.bind (destination, ior);
      }

    return true;
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::resolve_ior (const ACE_TCHAR *name, const ACE_TCHAR *ior)
  {
    DANCE_TRACE ("Node_Locator::resolve_ior");

    DANCE_DEBUG ((LM_DEBUG, DLINFO ACE_TEXT("Node_Locator::resolve_ior - ")
                  ACE_TEXT("Resolving ior %s for destination %s\n"),
                  ior, name));

    CORBA::Object_var obj = this->orb_->string_to_object (ior);

    ::Deployment::NodeManager_var nm = ::Deployment::NodeManager::_narrow (obj.in ());

    if (CORBA::is_nil (nm.in ()))
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::resolve_ior - ")
                      ACE_TEXT("Error: Unable to retrieve reference for destination ")
                      ACE_TEXT("%s and ior %s\n"),
                      name, ior));
      }

    return nm._retn ();
  }

  void
  Node_Locator::store_ior (const ACE_TCHAR *name, const ACE_TCHAR *ior)
  {
    DANCE_TRACE ("Node_Locator::store_ior");
    this->nodes_.bind (name, ior);
  }

  ::Deployment::NodeManager_ptr
  Node_Locator::ns_lookup (const ACE_TCHAR *nodename)
  {
    DANCE_TRACE ("Node_Locator::ns_lookup");

    if (CORBA::is_nil (this->nc_.in ()))
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Nameservice lookup of %s failed because there is no naming service.\n"),
                      nodename));
        return ::Deployment::NodeManager::_nil ();
      }

    try
      {
        CosNaming::Name name;
        name.length (1);

        name[0].id = ACE_TEXT_ALWAYS_CHAR (nodename);
        name[0].kind = "NodeManager";

        CORBA::Object_var obj = this->nc_->resolve (name);
        ::Deployment::NodeManager_var nm = ::Deployment::NodeManager::_narrow (obj.in ());

        if (CORBA::is_nil (nm.in ()))
          {
            DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                          ACE_TEXT("Unable to narrow provided reference for node %s\n"),
                          nodename));
          }

        return nm._retn ();
      }
    catch (const CORBA::Exception &e)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Caught CORBA exception while looking up name %s:%C\n"),
                      nodename, e._info ().c_str ()));
      }
    catch (...)
      {
        DANCE_ERROR ((LM_ERROR, DLINFO ACE_TEXT("Node_Locator::ns_lookup - ")
                      ACE_TEXT("Caught unexpected exception while looking up name %s\n"),
                      nodename));
      }

    return ::Deployment::NodeManager::_nil ();
  }
}

