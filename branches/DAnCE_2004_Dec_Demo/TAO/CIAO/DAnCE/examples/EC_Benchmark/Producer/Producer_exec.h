// $Id$

//============================================================
/**
 * @file Producer_exec.h
 *
 * Header file for the actualy Producer and ProducerHome component
 * implementation.
 *
 * @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//============================================================

#ifndef PRODUCER_EXEC_H
#define PRODUCER_EXEC_H

#include "ProducerEC.h"
#include "Producer_exec_export.h"

#include "tao/LocalObject.h"

namespace Producer_Impl
{
  /**
   * @class Producer_exec_i
   *
   * Producer executor implementation class.
   */
  class PRODUCER_EXEC_Export Producer_exec_i :
    public virtual Producer_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Producer_exec_i ();

    /// Default destructor.
    ~Producer_exec_i ();

    // Operations from supported interface(s)

    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void set_session_context (Components::SessionContext_ptr ctx
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));


    virtual void ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:
    /// Copmponent specific context
    Producer_Exec_Context_var context_;
  };

  /**
   * @class ProducerHome_exec_i
   *
   * Producer home executor implementation class.
   */
  class PRODUCER_EXEC_Export ProducerHome_exec_i :
    public virtual ProducerHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    ProducerHome_exec_i ();

    /// Default dtor.
    virtual ~ProducerHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };
}

extern "C" PRODUCER_EXEC_Export ::Components::HomeExecutorBase_ptr
createProducerHome_Impl (void);

#endif /* CIAO_PRODUCER_EXEC_H */
