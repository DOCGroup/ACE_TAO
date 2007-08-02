// $Id$

extern "C" ::Components::HomeExecutorBase_ptr create_Null_Component_Home_Impl (void);
extern "C" ::PortableServer::Servant create_Null_Component_Home_Servant
		(::Components::HomeExecutorBase_ptr p,
		::CIAO::Session_Container *c,
		const char*);

// Below two extern "C++" is to fixing the "Anachronism" warning of Sun C++ 5.7

extern "C++" ::Components::HomeExecutorBase_ptr create_Null_Component_Home_Impl_wrapper (void)
{
  return create_Null_Component_Home_Impl ();
}

extern "C++" ::PortableServer::Servant create_Null_Component_Home_Servant_wrapper
		(::Components::HomeExecutorBase_ptr p,
		::CIAO::Session_Container *c,
		const char* no_op)
{
  return create_Null_Component_Home_Servant (p, c, no_op);
}

struct HomeAttributes
{
  char const * component_instance_name_;
  /// Specify the entrypoint to component executor DLL.
  char const * executor_entrypt_;
  ::CIAO::HomeFactory executor_fptr_;
  /// Specify the entrypoint to component servant DLL.
  char const *  servant_entrypt_;
  ::CIAO::ServantFactory servant_fptr_;
};

// Homes
static const HomeAttributes homes_table[] =
{
  {
    "Null_Component_ComponentImplementations_Null_Assembly_Container_Null_Assembly_Null_Component",
    "create_Null_Component_Home_Impl",
    create_Null_Component_Home_Impl_wrapper,
    "create_Null_Component_Home_Servant",
    create_Null_Component_Home_Servant_wrapper
  },
};


