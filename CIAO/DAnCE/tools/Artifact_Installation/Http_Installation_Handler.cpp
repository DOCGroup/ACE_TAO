// $Id$

#include "Http_Installation_Handler.h"
#include "Installation_Repository_Manager.h"
#include "DAnCE/Logger/Log_Macros.h"
#include "ace/Auto_Ptr.h"
#include "ace/INet/HTTP_URL.h"
#include "ace/INet/HTTP_ClientRequestHandler.h"
#include "ace/INet/String_IOStream.h"

namespace DAnCE
{
  class HTTPInstallationRequestHandler
    : public ACE::HTTP::ClientRequestHandler
  {
    public:
      HTTPInstallationRequestHandler (const ACE_CString& plan_uuid)
        : plan_uuid_ (plan_uuid) {}
      virtual ~HTTPInstallationRequestHandler () {}

    protected:
      virtual void handle_response (const ACE::HTTP::URL& url, const ACE::HTTP::Response& response)
        {
          if (!response.get_status ().is_ok ())
            {
              ACE::IOS::CString_OStream serr;
              if (response.get_status ().is_valid ())
                serr << "negative HTTP response " << (int)response.get_status ().get_status ()
                      << response.get_status ().get_reason ().c_str ();
              else
                serr << "invalid HTTP response";
              serr << " : url=" << url.to_string ().c_str ();

              throw Deployment::PlanError (plan_uuid_.c_str (), serr.str ().c_str ());
            }
        }

      virtual void handle_request_error (const ACE::HTTP::URL& url)
        {
          ACE::IOS::CString_OStream serr;
          serr << "Failed to handle request for " << url.to_string ().c_str ()
               << " (" << ACE_OS::last_error () << ')';

          throw Deployment::PlanError (plan_uuid_.c_str (), serr.str ().c_str ());
        }

      virtual void handle_connection_error (const ACE::HTTP::URL& url)
        {
          ACE::IOS::CString_OStream serr;
          serr << "Failed to set up connection for " << url.to_string ().c_str ()
               << " (" << ACE_OS::last_error () << ')';

          throw Deployment::PlanError (plan_uuid_.c_str (), serr.str ().c_str ());
        }

    private:
      ACE_CString plan_uuid_;
  };


  /*
   *  HttpInstallationHandler
   */

  ACE_STATIC_SVC_DEFINE (HttpInstallationHandlerSvc,
                         ACE_TEXT ("HttpInstallationHandler"),
                         ACE_SVC_OBJ_T,
                         &ACE_SVC_NAME (HttpInstallationHandlerSvc),
                         ACE_Service_Type::DELETE_THIS
                          | ACE_Service_Type::DELETE_OBJ,
                         0)
  ACE_FACTORY_DEFINE (Artifact_Installation_Handler, HttpInstallationHandlerSvc)

  HttpInstallationHandlerSvc::HttpInstallationHandlerSvc ()
    {
    }

  HttpInstallationHandlerSvc::~HttpInstallationHandlerSvc ()
    {
      THandlerSingleton::close ();
    }

  ArtifactInstallationHandler* HttpInstallationHandlerSvc::handler_instance ()
    {
      return THandlerSingleton::instance ();
    }

  int HttpInstallationHandlerSvc::Initializer (void)
  {
    return ACE_Service_Config::process_directive (ace_svc_desc_HttpInstallationHandlerSvc);
  }

  /*
   *  HttpInstallationHandler
   */

  const std::string HttpInstallationHandler::protocol ("http");

  HttpInstallationHandler::HttpInstallationHandler ()
    {
      DANCE_TRACE ( "HttpInstallationHandler::HttpInstallationHandler");
    }

  HttpInstallationHandler::~HttpInstallationHandler ()
    {
      DANCE_TRACE ( "HttpInstallationHandler::~HttpInstallationHandler");
    }

  const std::string& HttpInstallationHandler::protocol_prefix (void)
    {
      return HttpInstallationHandler::protocol;
    }

  void HttpInstallationHandler::initialize ()
    {
    }

  void HttpInstallationHandler::clear ()
    {
      // we're finished installing; close all cached connections
      ACE::HTTP::ClientRequestHandler::connection_cache ().close_all_connections ();
    }

  void HttpInstallationHandler::install (const std::string& plan_uuid,
                                         std::string& location,
                                         const TPropertyMap& properties)
    {
      InstallationRepository::auto_ptr repo (
        InstallationRepositoryManager::instance ()->open_repository (plan_uuid));

      if (repo)
        {
          ArtifactInstallationProperties propreader (properties, this->protocol_prefix());

          ACE::HTTP::URL http_url;
          if (!http_url.parse (location.c_str ()))
            {
              ACE::IOS::CString_OStream serr;
              serr << "failed to parse artifact location : "
                   << location;
              throw Deployment::PlanError (plan_uuid.c_str (), serr.str ().c_str ());
            }
          else
            {
              std::string path = propreader.get_property (
                  ArtifactInstallationProperties::PROP_NAME);
              if (path.empty ())
                path = this->basename (location, '/');
              bool exclusive = false;
              const std::string& propval = propreader.get_property (
                  ArtifactInstallationProperties::PROP_EXCLUSIVE);
              exclusive = (!propval.empty () && (propval == "yes" || propval == "true"));
              ACE_Auto_Ptr<ostream> pofs (repo->create_artifact(path, exclusive));
              if (pofs.get ()!=0 && (*pofs))
                {
                  HTTPInstallationRequestHandler request_handler (plan_uuid.c_str ());

                  http_url.open (request_handler);
                  if (request_handler.response_stream ())
                    {
                      // copy artifact content
                      (*pofs) << request_handler.response_stream ().rdbuf ();
                      // check state
                      if (!(*pofs))
                        {
                          ACE::IOS::CString_OStream serr;
                          serr << "failed to copy artifact from "
                              << location
                              << " to "
                              << path.c_str ();
                          throw Deployment::PlanError (plan_uuid.c_str (), serr.str ().c_str ());
                        }
                      location = path.c_str ();
                    }
                  else
                    {
                      // We should never get here since the request handler should already
                      // have thrown appropriate exceptions
                      ACE::IOS::CString_OStream serr;
                      serr << "Failed to handle request for " << http_url.to_string ().c_str ()
                          << " (" << ACE_OS::last_error () << ')';

                      throw Deployment::PlanError (plan_uuid.c_str (), serr.str ().c_str ());
                    }
                }
              else
                {
                  ACE::IOS::CString_OStream serr;
                  serr << "failed to create repository artifact version for "
                      << path;
                  throw Deployment::PlanError (plan_uuid.c_str (), serr.str ().c_str ());
                }
            }
        }
      else
        {
          throw Deployment::PlanError (plan_uuid.c_str (), "failed to open installation repository");
        }
    }

  void HttpInstallationHandler::remove (const std::string& plan_uuid,
                                        const std::string& location)
    {
      InstallationRepository::auto_ptr repo (
        InstallationRepositoryManager::instance ()->open_repository (plan_uuid));

      if (repo)
        {
          if (repo->has_artifact (location))
            {
              if (!repo->remove_artifact (location))
                {
                  std::string err = "failed to remove artifact : ";
                  err += location;
                  throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
                }
            }
          else
            {
              std::string err = "cannot remove non-existing artifact : ";
              err += location;
              throw Deployment::PlanError (plan_uuid.c_str (), err.c_str ());
            }
        }
      else
        {
          throw Deployment::PlanError (plan_uuid.c_str (), "failed to open installation repository");
        }
    }

};
