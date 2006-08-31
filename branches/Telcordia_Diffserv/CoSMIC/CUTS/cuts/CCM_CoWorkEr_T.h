// -*- C++ -*-

//=============================================================================
/**
 * @file    CUTS_CCM_CoWorkEr_T.h
 *
 * $Id$
 *
 * This file contains the base templates for defining CoWorkEr components.
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_COWORKER_T_H_
#define _CUTS_COWORKER_T_H_

#include "cuts/CCM_CoWorkEr.h"
#include "cuts/CCM_Event_Producer_T.h"
#include "cuts/Event_Handler_Manager_T.h"
#include "cuts/Trigger_T.h"
#include "ace/Unbounded_Set.h"

//=============================================================================
/**
 * @class CUTS_CCM_CoWorkEr_T
 *
 * @brief CoWorkEr aspect for benchmarking components in CUTS
 *
 * This class contains methods and declarations that are needed begin
 * transforming a component into a CoWorkEr component. Once this class
 * has been parameterized by the target component, the next step is
 * to overload the methods you want to benchmark. The final step is to
 * program the behavior of the component and use the provided benchmarking
 * methods in the to collect the performance data.
 *
 * This entire process is handle by the Workload Modeling Language (WML).
 */
//=============================================================================

template <typename COMPONENT, typename COMPONENT_CONTEXT>
class CUTS_CCM_CoWorkEr_T :
  public virtual COMPONENT,
  public CUTS_CCM_CoWorkEr
{
public:
  /// Type of component wrapped by this class.
  typedef COMPONENT Component_Type;

  /// Type definition for component type.
  typedef COMPONENT_CONTEXT Component_Context;

  /// Type definition for the event producer.
  typedef CUTS_CCM_Event_Producer_T <COMPONENT_CONTEXT> Event_Producer;

  /// Constructor.
  CUTS_CCM_CoWorkEr_T (void);

  /// Destructor.
  virtual ~CUTS_CCM_CoWorkEr_T (void);

  /// Set the session context for the component.
  virtual void set_session_context (
    ::Components::SessionContext_ptr ctx
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException,
                     ::Components::CCMException));

  /// Enter the preactivate state.
  virtual void
    ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException));

  /// Enter the passivate state.
  virtual void
    ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException));

  virtual void
    ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((::CORBA::SystemException, ::Components::CCMException));

protected:
  // @@ This needs to be turned into a worker.
  /// Type definition for the periodic trigger.
  typedef CUTS_Periodic_Trigger_T <COMPONENT> Periodic_Event;

  /// Context for the component.
  COMPONENT_CONTEXT * context_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/CCM_CoWorkEr_T.inl"
#endif

#include "cuts/CCM_CoWorkEr_T.cpp"

#endif // !define _CUTS_COWORKER_T_H_
