// $Id$

#include "Session_Container.h"

extern "C"
::Components::HomeExecutorBase_ptr create_Null_ComponentHome_Impl (void);

extern "C"
::PortableServer::Servant create_Null_ComponentHome_Servant (
  ::Components::HomeExecutorBase_ptr p,
  ::CIAO::Session_Container *c,
  const char*);

struct HomeAttributes
{
  char const * component_instance_name_;
  /// Specify the entrypoint to component executor DLL.
  char const * executor_entrypt_;
  ::CIAO::HomeFactory executor_fptr_;
  /// Specify the entrypoint to component servant DLL.
  char const * servant_entrypt_;
  ::CIAO::ServantFactory servant_fptr_;
};

/// Homes
HomeAttributes homes_table[] =
{
  {
    "Null_Component.ComponentImplementations.Null_Assembly_Container."
    "Null_Assembly.Null_Component",
    "create_Null_ComponentHome_Impl",
    create_Null_ComponentHome_Impl,
    "create_Null_ComponentHome_Servant",
    create_Null_ComponentHome_Servant
  }
};

