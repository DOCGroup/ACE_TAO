// $Id$

//================================================================
/**
 * @file CAOC_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CAOC_EXEC_H
#define CIAO_CAOC_EXEC_H

#include "CAOCEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class CAOC_exec_i
   *
   * The Command and Operations Center implementation class.
   */
  class CAOC_EXEC_Export CAOC_exec_i :
    public virtual BBN_UAV::CAOC_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    CAOC_exec_i (void);

    /// Default destructor.
    ~CAOC_exec_i (void);

    // Operations from BBN_UAV::CAOC

    virtual void
    push_target_located (BBN_UAV::TargetLocated *ev
                         ACE_ENV_ARG_DECL)
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
    CORBA::String_var str_;

    /// Copmponent specific context
    BBN_UAV::CCM_CAOC_Context_var context_;
  };

  /**
   * @class CAOCHome_exec_i
   *
   * CAOC home executor implementation class.
   */
  class CAOC_EXEC_Export CAOCHome_exec_i :
    public virtual BBN_UAV::CCM_CAOCHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    CAOCHome_exec_i ();

    /// Default dtor.
    ~CAOCHome_exec_i ();

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
extern "C" CAOC_EXEC_Export ::Components::HomeExecutorBase_ptr
createCAOCHome_Impl (void);

#endif /* CIAO_CAOC_EXEC_H*/
