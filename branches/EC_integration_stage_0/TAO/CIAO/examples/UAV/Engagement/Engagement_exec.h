// $Id$

//================================================================
/**
 * @file Engagement_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_ENGAGEMENT_EXEC_H
#define CIAO_ENGAGEMENT_EXEC_H

#include "EngagementEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class Engagement_exec_i
   *
   * The Engagement implementation class.
   */
  class ENGAGEMENT_EXEC_Export Engagement_exec_i :
    public virtual BBN_UAV::Engagement_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Engagement_exec_i (void);

    /// Default destructor.
    ~Engagement_exec_i (void);

    // Operations from BBN_UAV::Engagement

    virtual void
    push_prepare_engage (BBN_UAV::PrepareEngage *ev
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
    BBN_UAV::CCM_Engagement_Context_var context_;
  };

  /**
   * @class EngagementHome_exec_i
   *
   * Engagement home executor implementation class.
   */
  class ENGAGEMENT_EXEC_Export EngagementHome_exec_i :
    public virtual BBN_UAV::CCM_EngagementHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    EngagementHome_exec_i ();

    /// Default dtor.
    ~EngagementHome_exec_i ();

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
extern "C" ENGAGEMENT_EXEC_Export ::Components::HomeExecutorBase_ptr
createEngagementHome_Impl (void);

#endif /* CIAO_ENGAGEMENT_EXEC_H*/
