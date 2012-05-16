// $Id$

#include "XML_File_Intf.h"
#include "tools/XML/XML_Typedefs.h"
#include "Deployment.hpp"
#include "DP_Handler.h"
#include "DD_Handler.h"
#include "Common.h"
#include "XMLSchema/id_map.hpp"

#include "dance/Deployment/Deployment_DataC.h"
#include "dance/Deployment/Deployment_TargetDataC.h"
#include "dance/Logger/Log_Macros.h"

namespace DAnCE
{
  namespace Config_Handlers
  {
    XML_File_Intf::XML_File_Intf (const ACE_TCHAR *file)
      : file_ (file),
        idl_dp_ (0)
    {
      DANCE_TRACE("XML_File_Intf::constructor");
    }

    XML_File_Intf::~XML_File_Intf (void)
    {
      DANCE_TRACE("XML_File_Intf::destructor");

      //      XML_Helper::XML_HELPER.terminate_parser ();
    }

    bool
    XML_File_Intf::read_process_plan (const ACE_TCHAR *file)
    {
      DANCE_TRACE("XML_File_Intf::read_process_plan");

      try
        {
          if (!XML_Helper_type::XML_HELPER.is_initialized ())
            return false;

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
            (LM_TRACE, DLINFO ACE_TEXT ("XML_File_Intf::read_process_plan - ")
                       ACE_TEXT ("Constructing DOM\n")));
          XERCES_CPP_NAMESPACE::DOMDocument *dom =
            XML_Helper_type::XML_HELPER.create_dom ((file));

          if (dom == 0)
            {
              DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::read_process_plan - ")
                           ACE_TEXT ("Failed to open file %s\n"), file));
              return false;
            }

          XERCES_CPP_NAMESPACE::DOMElement *foo = dom->getDocumentElement ();
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
            (LM_TRACE, DLINFO ACE_TEXT ("XML_File_Intf::read_process_plan - ")
                       ACE_TEXT ("DOMElement pointer: %u\n"), foo));

          ID_Map::TSS_ID_Map* TSS_ID_Map (ACE_Singleton<ID_Map::TSS_ID_Map, ACE_Null_Mutex>::instance());
          (*TSS_ID_Map)->reset ();

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
            DLINFO ACE_TEXT ("XML_File_Intf::read_process_plan - ")
            ACE_TEXT ("Parsing XML file with XSC\n")));
          deploymentPlan dp =
            DAnCE::Config_Handlers::reader::DeploymentPlan (dom);

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
            DLINFO ACE_TEXT ("XML_File_Intf::read_process_plan - ")
            ACE_TEXT ("Processing using config handlers\n")));
          DP_Handler dp_handler (dp);

          this->idl_dp_.reset (dp_handler.plan ());

          if (this->idl_dp_.get ())
            return true;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::caught - ")
            ACE_TEXT ("CORBA Exception whilst parsing XML into IDL\n"),
            ex._info ().c_str ()));
          throw Config_Error (this->file_,
                              ex._info ().c_str ());
        }
      catch (Config_Error &ex)
        {
          throw ex;
        }
      catch (...)
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::caught - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          throw Config_Error (this->file_,
            ACE_TEXT ("Unexpected C++ exception whilst parsing XML"));
        }

      return false;
    }

    bool
    XML_File_Intf::read_process_domain (const ACE_TCHAR *file)
    {
      DANCE_TRACE("XML_File_Intf::read_process_domain");

      try
        {
          if (!XML_Helper_type::XML_HELPER.is_initialized ())
            return false;

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
            DLINFO ACE_TEXT ("XML_File_Intf::read_process_domain - ")
                       ACE_TEXT ("Constructing DOM\n")));
          XERCES_CPP_NAMESPACE::DOMDocument *dom =
            XML_Helper_type::XML_HELPER.create_dom ((file));

          if (dom == 0)
            {
              DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
                (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::read_process_domain - ")
                           ACE_TEXT ("Failed to open file %s\n"), file));
              return false;
            }

          XERCES_CPP_NAMESPACE::DOMElement *foo = dom->getDocumentElement ();
          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE,
            (LM_TRACE, DLINFO ACE_TEXT ("XML_File_Intf::read_process_domain - ")
                       ACE_TEXT ("DOMElement pointer: %u\n"), foo));

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
            DLINFO ACE_TEXT ("XML_File_Intf::read_process_domain - ")
                       ACE_TEXT ("Parsing XML file with XSC\n")));

          ID_Map::TSS_ID_Map* TSS_ID_Map (ACE_Singleton<ID_Map::TSS_ID_Map, ACE_Null_Mutex>::instance());
          (*TSS_ID_Map)->reset ();

          Domain dp =
            DAnCE::Config_Handlers::reader::domain (dom);

          DANCE_DEBUG (DANCE_LOG_EVENT_TRACE, (LM_TRACE,
            DLINFO ACE_TEXT ("XML_File_Intf::read_process_domain - ")
                       ACE_TEXT ("Processing using config handlers\n")));
          DD_Handler dp_handler (dp);

          this->idl_domain_.reset (dp_handler.domain_idl ());

          if (this->idl_domain_.get ())
            return true;
        }
      catch (CORBA::Exception &ex)
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::caught - ")
                           ACE_TEXT ("CORBA Exception whilst parsing XML into IDL\n"),
                           ex._info ().c_str ()));
          throw Config_Error (this->file_,
                              ex._info ().c_str ());
        }
      catch (Config_Error &ex)
        {
          throw ex;
        }
      catch (...)
        {
          DANCE_ERROR (DANCE_LOG_TERMINAL_ERROR,
            (LM_ERROR, DLINFO ACE_TEXT ("XML_File_Intf::caught - ")
                      ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          throw Config_Error (this->file_,
                              ACE_TEXT ("Unexpected C++ exception whilst parsing XML"));
        }

      return false;
    }

    ::Deployment::DeploymentPlan const *
    XML_File_Intf::get_plan (void)
    {
      DANCE_TRACE("get_plan");

      if (this->idl_dp_.get () == 0 && !this->read_process_plan (this->file_.c_str ()))
        return 0;

      return this->idl_dp_.get ();
    }

    ::Deployment::DeploymentPlan *
    XML_File_Intf::release_plan (void)
    {
      DANCE_TRACE("XML_File_Intf::get_plan");

      if (this->idl_dp_.get () == 0 && !this->read_process_plan (this->file_.c_str ()))
        return 0;
      return this->idl_dp_.release ();
    }

    ::Deployment::Domain const *
    XML_File_Intf::get_domain (void)
    {
      DANCE_TRACE("get_domain");

      if (this->idl_domain_.get () == 0 && !this->read_process_domain (this->file_.c_str ()))
        return 0;

      return this->idl_domain_.get ();
    }

    ::Deployment::Domain *
    XML_File_Intf::release_domain (void)
    {
      DANCE_TRACE("XML_File_Intf::get_domain");

      if (this->idl_domain_.get () == 0 && !this->read_process_domain (this->file_.c_str ()))
        return 0;
      return this->idl_domain_.release ();
    }

    void
    XML_File_Intf::add_search_path (const ACE_TCHAR *environment,
                                    const ACE_TCHAR *relpath)
    {
      DANCE_TRACE("XML_File_Intf::add_search_path");
      XML_Helper_type::XML_HELPER.get_resolver ().get_resolver ().add_path (environment, relpath);
    }

  }
}
