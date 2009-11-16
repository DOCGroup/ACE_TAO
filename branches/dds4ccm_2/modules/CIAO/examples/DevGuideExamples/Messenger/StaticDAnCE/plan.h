// $Id$

#include "Session_Container.h"

extern "C" ::Components::HomeExecutorBase_ptr create_MessengerHome_Impl (void);
extern "C" ::PortableServer::Servant create_MessengerHome_Servant 
    (::Components::HomeExecutorBase_ptr p,
    ::CIAO::Session_Container *c,
    const char*
    );
extern "C" ::Components::HomeExecutorBase_ptr create_ReceiverHome_Impl (void);
extern "C" ::PortableServer::Servant create_ReceiverHome_Servant 
    (::Components::HomeExecutorBase_ptr p,
    ::CIAO::Session_Container *c,
    const char*
    );
extern "C" ::Components::HomeExecutorBase_ptr create_ReceiverHome_Impl (void);
extern "C" ::PortableServer::Servant create_ReceiverHome_Servant 
    (::Components::HomeExecutorBase_ptr p,
    ::CIAO::Session_Container *c,
    const char*
    );
extern "C" ::Components::HomeExecutorBase_ptr create_AdministratorHome_Impl (void);
extern "C" ::PortableServer::Servant create_AdministratorHome_Servant 
    (::Components::HomeExecutorBase_ptr p,
    ::CIAO::Session_Container *c,
    const char*
    );
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
HomeAttributes homes_table[]= 
{
  {"Messenger_Instance", "create_MessengerHome_Impl", create_MessengerHome_Impl, "create_MessengerHome_Servant", create_MessengerHome_Servant},
  {"First_Receiver_Instance", "create_ReceiverHome_Impl", create_ReceiverHome_Impl, "create_ReceiverHome_Servant", create_ReceiverHome_Servant},
  {"Second_Receiver_Instance", "create_ReceiverHome_Impl", create_ReceiverHome_Impl, "create_ReceiverHome_Servant", create_ReceiverHome_Servant},
  {"Administrator_Instance", "create_AdministratorHome_Impl", create_AdministratorHome_Impl, "create_AdministratorHome_Servant", create_AdministratorHome_Servant}
};

