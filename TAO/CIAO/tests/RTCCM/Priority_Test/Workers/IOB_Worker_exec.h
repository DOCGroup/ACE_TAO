// $Id$

/**
 * @file IOB_Worker_exec.h
 *
 * Header file for the actual IOB_Worker and IOB_WorkerHome component
 * implementations.  These classes are the implementations of local
 * interfaces defined in WorkerEI.idl.
 *
 * @author Nanbor Wang <nanbor@cse.wustl.edu>
 */

#ifndef IOB_WORKER_EXEC_H
#define IOB_WORKER_EXEC_H

#include "WorkerEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class IOB_Worker_exec
   *
   * A compute-bound worker executor implementation class.
   */
  class WORKER_EXEC_Export IOB_Worker_exec :
    public virtual Priority_Test::Worker_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    IOB_Worker_exec ();

    /// Default destructor.
    ~IOB_Worker_exec ();

    // Operations from Priority_Test::Worker

    virtual Priority_Test::CCM_Common_Ops_ptr
    get_some_device (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Priority_Test::Common_Ops

    virtual CORBA::ULong
    do_work (CORBA::ULong work,
             CORBA::ULong aux
             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  protected:
    /// Copmponent specific context
    Priority_Test::CCM_Worker_Context_var context_;
  };

  /**
   * @class IOB_Worker_exec
   *
   * IOB_Worker home executor implementation class.
   */
  class WORKER_EXEC_Export IOB_Worker_Home_exec :
    public virtual Priority_Test::CCM_Worker_Home,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    IOB_Worker_Home_exec ();

    /// Default dtor.
    ~IOB_Worker_Home_exec ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" WORKER_EXEC_Export ::Components::HomeExecutorBase_ptr
createWorkerHome_Impl (void);

#endif /* IOB_WORKER_EXEC_H */
