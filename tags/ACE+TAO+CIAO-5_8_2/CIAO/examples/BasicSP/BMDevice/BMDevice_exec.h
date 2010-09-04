// $Id$

// ================================================================
/**
 * @file BMDevice_exec.h
 *
 * Header file for the actual BMDevice and BMDeviceHome component
 * implementations.  These classes are the implementations of local
 * interfaces defined in BMDeviceEI.idl.
 *
 * @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
// ================================================================

#ifndef CIAO_BMDEVICE_EXEC_H
#define CIAO_BMDEVICE_EXEC_H

#include "BMDevice_exec_export.h"
#include "BMDeviceEC.h"
#include "tao/LocalObject.h"

namespace MyImpl
{
  /**
   * @class ReadData_Impl
   *
   * Implementation of the ReadData interface
   */
  class BMDEVICE_EXEC_Export ReadData_Impl :
    public virtual ::BasicSP::CCM_ReadData,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Constructor
    ReadData_Impl(const char* name);

    void set_name (const char* name);

    virtual char *
    get_data (void);

    ~ReadData_Impl (void);

  private:
    CORBA::String_var str_;
  };
  
  /**
   * @class BMDEVICE_exec_i
   *
   * An example RateGen executor implementation class.
   */
  class BMDEVICE_EXEC_Export BMDevice_exec_i :
    public virtual CIAO_BasicSP_BMDevice_Impl::BMDevice_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default constructor.
    BMDevice_exec_i (void);

    /// Default destructor.
    ~BMDevice_exec_i (void);

    // Operations from BasicSP::BMDevice

    virtual BasicSP::CCM_ReadData_ptr
    get_data_read (void);

    virtual void
    push_timeout_value (BasicSP::TimeOut *ev);

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx);

    virtual void
    configuration_complete (void);

    virtual void
    ccm_activate (void);

    virtual void
    ccm_passivate (void);

    virtual void
    ccm_remove (void);
  protected:

   /// Copmponent specific context
    BasicSP::CCM_BMDevice_Context_var context_;

    private:
    MyImpl::ReadData_Impl* data_read_;
  };

  extern "C" BMDEVICE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_BasicSP_BMDevice_Impl (void);

  /**
   * @class BMDeviceHome_exec_i
   *
   * BMDevice home executor implementation class.
   */
  class BMDEVICE_EXEC_Export BMDeviceHome_exec_i :
    public virtual CIAO_BasicSP_BMDevice_Impl::BMDeviceHome_Exec,
    public virtual ::CORBA::LocalObject
  {
  public:
    /// Default ctor.
    BMDeviceHome_exec_i (void);

    /// Default dtor.
    ~BMDeviceHome_exec_i (void);

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  };
}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" BMDEVICE_EXEC_Export ::Components::HomeExecutorBase_ptr
createBMDeviceHome_Impl (void);

#endif /* CIAO_BMDEVICE_EXEC_H*/
