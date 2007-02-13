// -*- C++ -*-

//============================================================
/**
 * @file RoundTrip_exec.h
 *
 * $Id$
 *
 * Header file for the Executor implementation.
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//============================================================

#ifndef NODEAPP_ROUNDTRIP_EXEC_H
#define NODEAPP_ROUNDTRIP_EXEC_H

#include "NodeAppTest_RoundTripEC.h"
#include "RoundTrip_exec_export.h"
#include "tao/LocalObject.h"

namespace CIDL_RoundTrip_Impl
{

  /**
   * @class RoundTrip_exec_i
   *
   * RoundTrip executor implementation class.
   */
  class NODEAPPTEST_ROUNDTRIP_EXEC_Export RoundTrip_exec_i
    : public virtual NodeAppTest::LatencyTest,
      public virtual RoundTrip_Exec,
      public virtual TAO_Local_RefCounted_Object
  {

  public:
    /// Default constructor.
    RoundTrip_exec_i ();

    /// Operation to test the data
    virtual CORBA::Long cube_long (CORBA::Long data)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /*
      virtual ::NodeAppTest::CCM_LatencyTest*
      get_latency ()
      ACE_THROW_SPEC ((CORBA::SystemException));
    */

    NodeAppTest::CCM_LatencyTest_ptr
    get_facet_1()
      ACE_THROW_SPEC ((CORBA::SystemException));


    NodeAppTest::CCM_LatencyTest_ptr
    get_facet_2()
      ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent
    virtual void set_session_context (Components::SessionContext_ptr ctx)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_activate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_passivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ccm_remove ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    /// Helper function to be called back by timeout_Handler
    void pulse (void);

    // CIAO defined methods
    virtual void ciao_preactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void ciao_postactivate ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:

    /// Destructor.
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~RoundTrip_exec_i ();

  };

  //
  //
  //
  class LatencyTest_Impl : public virtual NodeAppTest::CCM_LatencyTest,
                           public virtual TAO_Local_RefCounted_Object
  {
  public:
    LatencyTest_Impl ()
    {
    }

    virtual CORBA::Long cube_long (CORBA::Long data)
      ACE_THROW_SPEC ((CORBA::SystemException));
  };

  /**
   * @class RoundTripHome_exec_i
   *
   * RoundTrip home executor implementation class.
   */
  class NODEAPPTEST_ROUNDTRIP_EXEC_Export RoundTripHome_exec_i :
    public virtual RoundTripHome_Exec,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    RoundTripHome_exec_i ();

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create ()
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

  protected:

    /// Destructor.
    /**
     * Protected destructor to enforce proper memory management
     * through the reference counting mechanism.
     */
    ~RoundTripHome_exec_i ();

  };

  extern "C" NODEAPPTEST_ROUNDTRIP_EXEC_Export
  ::Components::HomeExecutorBase_ptr createRoundTripHome_Impl (void);
}

#endif /* NODEAPP_ROUNDTRIPGEN_EXEC_H */
