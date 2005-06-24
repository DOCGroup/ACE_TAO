// $Id$

// This file contains executor implementations.

#if !defined (HELLO_EXECUTORS_H)
#define HELLO_EXECUTORS_H

#include "helloEC.h"
#include "hello_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_hello_example
{
  class HelloWorld_Impl :
    public virtual HelloWorld_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    HelloWorld_Impl ();

    /// Default dtor.
    ~HelloWorld_Impl ();

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx
				      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
		      Components::CCMException));

    virtual char *sayhello (const char *username
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  protected:
    HelloWorld_Exec_Context_var context_;
  };

  class HelloHome_Impl :
    public virtual HelloHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    HelloHome_Impl ();

    /// Default dtor.
    ~HelloHome_Impl ();

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((Components::CCMException,
		      CORBA::SystemException));
  };

  // We still need to figure out a way to clean up the object created by
  // the factory correctly.   Like we did in ACE_FACTORY macro, with a
  // Gobbler function.

  extern "C" HELLO_EXEC_Export ::Components::HomeExecutorBase_ptr
  createHelloHome_Impl (void);
}

#endif /* HELLO_EXECUTORS_H */

