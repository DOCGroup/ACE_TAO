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

#include "SenderEC.h"
#include "Sender_exec_export.h"

#include "tao/LocalObject.h"

namespace Producer_Impl

  /**
   * @class Producer_exec_i
   *
   * Producer executor implementation class.
   */
  class PRODUCER_EXEC_Export Producer_exec_i :
    public virtual EC_Benchmark::Producer_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    EC_exec_i ();

    /// Initialize with a default frequency.
    EC_exec_i (CORBA::Long hz);

    /// Default destructor.
    ~EC_exec_i ();

    // Attribute operations.

    virtual CORBA::Long hertz (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void hertz (CORBA::Long hertz
                        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from supported interface(s)

    virtual void start (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void stop (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual CORBA::Boolean active (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
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

    /// Helper function to be called back by timeout_Handler
    void pulse (void);

  protected:
    /// Frequency
    CORBA::Long hertz_;

    /// Copmponent specific context
    BasicSP::CCM_EC_Context_var context_;

    /// An active object that actually trigger the generation of
    /// periodic events.
    timeout_Handler pulser_;
  };

  /**
   * @class ECHome_exec_i
   *
   * EC home executor implementation class.
   */
  class EC_EXEC_Export ECHome_exec_i :
    public virtual BasicSP::CCM_ECHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    ECHome_exec_i ();

    /// Default dtor.
    ~ECHome_exec_i ();

    // Explicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    new_EC (CORBA::Long hertz
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

extern "C" EC_EXEC_Export ::Components::HomeExecutorBase_ptr
createECHome_Impl (void);

#endif /* CIAO_ECGEN_EXEC_H */
