// $Id$

#ifndef PLAN_LAUNCHER_BASE_IMPL_CPP
#define PLAN_LAUNCHER_BASE_IMPL_CPP

#include "Plan_Launcher_Base_Impl_T.h"

#include "ace/Env_Value_T.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"

#include "Deployment/Deployment_PlanErrorC.h"
#include "Deployment/Deployment_ApplicationC.h"
#include "Deployment/Deployment_ApplicationManagerC.h"
#include "DAnCE/Logger/Log_Macros.h"

#include "Deployment_Failure.h"

#ifdef GEN_OSTREAM_OPS
#include <iostream>
#include <sstream>
#endif /* GEN_OSTREAM_OPS */

namespace DAnCE
{
  template <typename Manager, typename AppManager, typename Application>
  Plan_Launcher_Base_Impl <Manager, AppManager, Application> ::
  Plan_Launcher_Base_Impl(CORBA::ORB_ptr orb,
                          Manager_ptr manager)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      manager_ (Manager::_duplicate (manager))
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl");
    if (CORBA::is_nil (this->orb_.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - "
                         "Plan Launcher requires a valid ORB\n"));
        throw Deployment_Failure ("Plan launcher requires a valid ORB\n");
      }

    if (CORBA::is_nil (this->manager_.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Plan_Launcher_Base_Impl::Plan_Launcher_Base_Impl - "
                         "Plan Launcher requires a valid Manager reference.\n"));
        throw Deployment_Failure ("Plan launcher requires a valid Manager reference\n");
      }
  }

  template <typename Manager, typename AppManager, typename Application>
  Plan_Launcher_Base_Impl < Manager, AppManager, Application> ::~Plan_Launcher_Base_Impl()
  {
  }

  template <typename Manager, typename AppManager, typename Application>
  CORBA::Object_ptr
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::prepare_plan (const ::Deployment::DeploymentPlan &plan)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::prepare_plan");


#ifdef GEN_OSTREAM_OPS
    {
      std::ostringstream plan_stream;
      plan_stream << plan << std::endl;
      DANCE_DEBUG (9, (LM_TRACE, DLINFO "Convert_Plan - Input plan: %C\n",
                       plan_stream.str ().c_str ()));
    }
#endif /* GEN_OSTREAM_OPS */

    AppManager_var app_manager;
    try
      {
        app_manager = this->manager_->preparePlan (plan, 0);
      }
    catch (::Deployment::PlanError &ex)
      {
        ACE_CString error ("Caught PlanError exception while invoking preparePlan: ");
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::StartError &ex)
      {
        ACE_CString error ("Caught StartError exception while invoking preparePlan: ");
        error += ex.name.in ();
        error += ", " ;
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORBA exception while invoking preparePlan: ";
        error += ex._info ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch(...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - ")
                         ACE_TEXT("An exception was thrown during EM->preparePlan.\n")));
        throw Deployment_Failure ("An unexpected C++ exception was caught while invoking preparePlan");
      }

    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                     ACE_TEXT ("Plan_Launcher_Base_Impl::launch_plan - ")
                     ACE_TEXT ("after to call preparePlan\n")));

    if (CORBA::is_nil (app_manager.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - ")
                         ACE_TEXT("preparePlan call failed: ")
                         ACE_TEXT("nil ApplicationManager reference\n")));
        return 0;
      }
    else
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::prepare_plan - ")
                         ACE_TEXT("ApplicationMAnager was received from preparePlan.\n")));
      }

    return app_manager._retn ();
  }

  template <typename Manager, typename AppManager, typename Application>
  CORBA::Object_ptr
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::start_launch (CORBA::Object_ptr app_mgr,
                  const ::Deployment::Properties &properties,
                  ::Deployment::Connections_out connections)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::start_launch");

    AppManager_var app_manager (AppManager::_narrow (app_mgr));

    if (CORBA::is_nil (app_manager.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT ("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT ("Nil ApplicationManager reference\n")));
        return 0;
      }

    Deployment::Application_var da;
    try
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT("before startLaunch...\n")));
        da = app_manager->startLaunch (properties, connections);
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT("startLaunch completed...\n")));
      }
    catch (::Deployment::StartError &ex)
      {
        ACE_CString error;
        error += "Caught StartError exception while invoking startLaunch: " ;
        error += ex.name.in ();
        error +=  ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::InvalidProperty &ex)
      {
        ACE_CString error;
        error += "Caught InvalidProperty exception while invoking startLaunch: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::InvalidNodeExecParameter &ex)
      {
        ACE_CString error;
        error += "Caught InvalidNodeExecParameter exception while invoking startLaunch: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::InvalidComponentExecParameter &ex)
      {
        ACE_CString error;
        error += "Caught InvalidComponentExecParameter exception while invoking startLaunch: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::ResourceNotAvailable &ex)
      {
        ACE_CString error;
        error += "Caught ResourceNotAvailable while invoking startLaunch: ";
        error += ex.name.in ();
        error += ", Type: <";
        error += ex.resourceType.in ();
        error += "> Property Name: <";
        error += ex.propertyName.in ();
        error += "> Element: <";
        error += ex.elementName.in ();
        error += "> Resource Name: <";
        error += ex.resourceName.in ();
        error += ">";

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORBA exception while invoking startLaunch: ";
        error += ex._info ().c_str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch(...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT("An exception was thrown during DAM->startLaunch.\n")));
        throw Deployment_Failure ("Unexpected C++ exception in startLaunch\n");
      }

    if (CORBA::is_nil (da.in()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT("CIAO_PlanLauncher:startLaunch call failed: ")
                         ACE_TEXT("nil DomainApplication reference\n")));
        return 0;
      }
    else
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start_launch - ")
                         ACE_TEXT("DomainApplication was received from startLaunch\n")));
      }

    return da._retn ();
  }

  template <typename Manager, typename AppManager, typename Application>
  void
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::finish_launch (CORBA::Object_ptr app,
                   const ::Deployment::Connections &provided_connections,
                   bool start)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::finish_launch");

    Application_var application (Application::_narrow (app));

    if (CORBA::is_nil (application.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::launch_plan - ")
                         ACE_TEXT("Nil Application reference.\n")));
        throw Deployment_Failure ("Nil Application reference");
      }

    // Call finish Launch to complete the connections
    try
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::launch_plan - ")
                         ACE_TEXT("before finishLaunch\n")));
        application->finishLaunch (provided_connections,
                                   start);
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::launch_plan - ")
                         ACE_TEXT("finishLaunch completed.\n")));
      }
    catch (::Deployment::InvalidConnection &ex)
      {
        ACE_CString error;
        error += "Caught InvalidConnection exception while invoking finishLaunch: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::finish_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::Deployment::StartError &ex)
      {
        ACE_CString error;
        error += "Caught StartError exception while invoking finishLaunch: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::finish_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (::CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORB exception while invoking finishLaunch: ";
        error += ex._info ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::finish_launch - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch(...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::finish_launch - ")
                         ACE_TEXT("An exception was thrown during DA->finishLaunch.\n")));
        throw Deployment_Failure ("Unexpected C++ exception in finishLaunch");
      }
  }

  template <typename Manager, typename AppManager, typename Application>
  void
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::start (CORBA::Object_ptr app)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::start");

    Application_var application (Application::_narrow (app));

    if (CORBA::is_nil (application.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start - ")
                         ACE_TEXT("Nil Application reference.\n")));
        throw Deployment_Failure ("Nil Application reference");
      }

    try
      {
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start - ")
                         ACE_TEXT("before invoking start on application...\n")));
        application->start ();
        DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start - ")
                         ACE_TEXT("after invoking start on application...\n")));
      }
    catch (const ::Deployment::StartError &ex)
      {
        ACE_CString error;
        error += "Caught StartError exception while invoking start: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (const ::CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORBA exception while invoking start: ";
        error += ex._info ().c_str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::start - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch(...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::start - ")
                         ACE_TEXT("An exception was thrown during DA->start.\n")));
        throw Deployment_Failure ("Unexpected C++ exception in start\n");
      }
  }

  template <typename Manager, typename AppManager, typename Application>
  const char *
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::launch_plan (const ::Deployment::DeploymentPlan &plan,
                 CORBA::Object_out am_out,
                 CORBA::Object_out app_out)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::launch_plan");

    //this->create_external_connections (plan, conns.inout());

    try
      {
        CORBA::Object_var app_mgr = this->prepare_plan (plan);

        ::Deployment::Connections_var conns;

        CORBA::Object_var app = this->start_launch (app_mgr.in (),
                                                    0,
                                                    conns.out ());

        this->create_external_connections (plan,
                                           conns.inout ());

        this->finish_launch (app.in (),
                             conns,
                             false);

        this->start (app.in ());

        DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                         ACE_TEXT ("Plan_Launcher_Base_Impl::launch_plan - ")
                         ACE_TEXT ("Application Deployed successfully\n")));

        am_out = app_mgr._retn ();
        app_out = app._retn ();
      }
    catch (const CORBA::Exception& ex)
      {
        char buf[1024];
        ACE_OS::sprintf (buf, "Plan_Launcher_Base_Impl::launch_plan - CORBA EXCEPTION: <%s>\n",
                         ex._info().fast_rep());
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("%C"), buf));
        throw Deployment_Failure (buf);
      }
    catch (const Deployment_Failure &)
      {
        throw;
      }
    catch (...)
      {
        char buf[1024];
        ACE_OS::sprintf (buf, "Plan_Launcher_Base_Impl::launch_plan - EXCEPTION: non-CORBA exception\n");
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("%C"), buf));
        throw Deployment_Failure (buf);
      }

    return CORBA::string_dup (plan.UUID.in());
  }

  template <typename Manager, typename AppManager, typename Application>
  void
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::teardown_application (CORBA::Object_ptr am_obj,
                          CORBA::Object_ptr app_obj)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::teardown_application");

    AppManager_var am (AppManager::_narrow (am_obj));

    if (CORBA::is_nil (am.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::teardown_application - ")
                         ACE_TEXT("Nil ApplicationManager reference.\n")));
        throw Deployment_Failure ("Nil ApplicationManager reference in teardown_application");
      }

    Application_var app (Application::_narrow (app_obj));

    if (CORBA::is_nil (app.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::teardown_appliocation - ")
                         ACE_TEXT("Nil Application reference.\n")));
        throw Deployment_Failure ("Nil Application reference in teardown_application");
      }

    try
      {
        am->destroyApplication (app.in ());
      }
    catch (const ::Deployment::StopError &ex)
      {
        ACE_CString error;
        error += "Caught StopError exception while invoking destroyApplication: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::teardown_application - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (const CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORBA exception while invoking destroyApplication: ";
        error += ex._info ().c_str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::teardown_application - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::teardown_appliocation - ")
                         ACE_TEXT ("Caught unknown C++ exception\n")));
        throw Deployment_Failure ("Unknown C++ exception\n");
      }
  }

  template <typename Manager, typename AppManager, typename Application>
  void
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::destroy_app_manager(CORBA::Object_ptr am_obj)
  {
    DANCE_TRACE ("Plan_Launcher_Base_Impl::destroy_app_manager");

    AppManager_var am (AppManager::_narrow (am_obj));

    if (CORBA::is_nil (am.in ()))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::destroy_app_manager - ")
                         ACE_TEXT("Nil ApplicationManager reference.\n")));
        throw Deployment_Failure ("Nil ApplicationManager reference in destroy_app_manager");
      }

    try
      {
        DANCE_ERROR (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::destroy_app_manager - ")
                         ACE_TEXT ("Destroying ApplicationManager\n")));
        this->manager_->destroyManager (am.in ());
        DANCE_ERROR (6, (LM_DEBUG, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::destroy_app_manager - ")
                         ACE_TEXT ("ApplicationManager destroyed.\n")));
      }
    catch (const ::Deployment::StopError &ex)
      {
        ACE_CString error;
        error += "Caught StopError exception while invoking destroyManager: ";
        error += ex.name.in ();
        error += ", ";
        error += ex.reason.in ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::destory_app_manager - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (const CORBA::Exception &ex)
      {
        ACE_CString error;
        error += "Caught CORBA exception while invoking destroyManager: ";
        error += ex._info ().c_str ();

        DANCE_ERROR (1, (LM_ERROR, DLINFO
                         ACE_TEXT("Plan_Launcher_Base_Impl::destroy_app_manager - %C\n"),
                         error.c_str ()));
        throw Deployment_Failure (error);
      }
    catch (...)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO ACE_TEXT("Plan_Launcher_Base_Impl::destroy_app_manager - ")
                         ACE_TEXT ("Caught unknown C++ exception\n")));
        throw Deployment_Failure ("Unknown C++ exception\n");
      }
  }


  template <typename Manager, typename AppManager, typename Application>
  void
  Plan_Launcher_Base_Impl< Manager, AppManager, Application>
  ::create_external_connections(const ::Deployment::DeploymentPlan &plan,
                                Deployment::Connections &conn)
  {
    DANCE_DEBUG (6, (LM_DEBUG, DLINFO ACE_TEXT("create_external_connections - start\n")));

    for (CORBA::ULong i = 0; i < plan.connection.length(); i++)
      {
        if (plan.connection[i].externalReference.length() > 0
            && plan.connection[i].externalReference[0].provider)
          {
            DANCE_DEBUG (6, (LM_DEBUG,
                             "Plan_Launcher_i::create_external_connections - create connection %C from IOR %C\n",
                             plan.connection[i].name.in(),
                             plan.connection[i].externalReference[0].location.in()));

            try
              {
                CORBA::Object_ptr
                  obj = this->orb_->string_to_object(plan.connection[i].externalReference[0].location.in());
                if (!CORBA::is_nil (obj))
                  {
                    CORBA::ULong const indx = conn.length();
                    conn.length(indx + 1);
                    conn[indx].name= CORBA::string_dup (plan.connection[i].name.in());
                    conn[indx].endpoint.length(1L);
                    conn[indx].endpoint[0] = obj;
                  }
                else
                  {
                    DANCE_DEBUG (6, (LM_DEBUG, DLINFO
                                     ACE_TEXT("Plan_Launcher_i::create_external_connections - can't create object for IOR %C\n"),
                                     plan.connection[i].externalReference[0].location.in()));
                  }
              }
            catch (CORBA::Exception &ex)
              {
                DANCE_ERROR (1, (LM_ERROR, DLINFO
                                 ACE_TEXT("Plan_Launcher_i::create_external_connections - ")
                                 ACE_TEXT("Caught CORBA Exception while resolving endpoint for connection %C: %C\n"),
                                 plan.connection[i].name.in (),
                                 ex._info ().c_str ()));
              }
            catch (...)
              {
                 DANCE_ERROR (1, (LM_ERROR, DLINFO
                                  ACE_TEXT("Plan_Launcher_i::create_external_connections - ")
                                  ACE_TEXT("Caught C++ Exception while resolving endpoint for connection\n")));
              }
          }
      }
  }
}
#endif
