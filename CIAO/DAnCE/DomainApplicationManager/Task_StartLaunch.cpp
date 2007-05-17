//
// $Id$
//
#include "Task_StartLaunch.h"

CIAO::Task_StartLaunch::
Task_StartLaunch (
      const char * node_name,
      const ::Deployment::NodeApplicationManager_ptr nam,
      const ::Deployment::Properties & configProperty)
  : node_name_ (node_name)
  , nam_ (::Deployment::NodeApplicationManager::_duplicate (nam))
  , configProperty_ (configProperty)
{
}

int
CIAO::Task_StartLaunch::svc (void)
{
  try
    {
      ::Deployment::Application_var temp_application = 
        this->nam_->startLaunch (this->configProperty_,
                                 retn_connections_.out (),
                                 0);

      // Narrow down to NodeApplication object reference
      this->retn_node_app_ =
        ::Deployment::NodeApplication::_narrow (temp_application.in ());

      if (CORBA::is_nil (retn_node_app_.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      "DAnCE (%P|%t) Task_StartLaunch.cpp -"
                      "CIAO::Task_StartLaunch::svc -"
                      "ERROR while narrowing the NAM "
                      "for the node [%s] \n",
                      node_name_.c_str ()));

          ACE_CString error
            ("The DomainApplicationManager receives a nil "
              "reference of NodeApplication after calling "
              "startLaunch on NodeApplicationManager.\n");
          ACE_ERROR ((LM_ERROR, error.c_str ()));

          throw
            (Deployment::StartError
                ("DomainApplicationManager_Impl::startLaunch",
                  error.c_str ()));
        }
    }
  catch (const CORBA::Exception&)
    {
      return 0;
    }
  return 0;
}

::Deployment::Connections
CIAO::Task_StartLaunch::get_connections ()
{
  return this->retn_connections_.in ();
}

::Deployment::NodeApplication_ptr
CIAO::Task_StartLaunch::get_node_app ()
{
  return ::Deployment::NodeApplication::_duplicate (
    this->retn_node_app_.in ());
}
