// -*- C++ -*-

//=============================================================================
/**
 * @file      Database_Service_exec_i.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _DATABASE_SERVICE_EXEC_H_
#define _DATABASE_SERVICE_EXEC_H_

#include "cuts/config.h"
#include "Benchmark_Data_Collector_Ex_exec_export.h"
#include "Benchmark_Data_Collector_ExEC.h"
#include "tao/LocalObject.h"

// Forward decl.
class CUTS_Database_Service;

namespace CUTS
{
  //===========================================================================
  /**
   * @class Database_Service_exec_i
   *
   * @brief Implemenation of the BDC_Control_Handle_Ex interface.
   *
   * This implemenation is an extension of the BDC_Control_Handle
   * implementation. To prevent rewriting of the base interface, we
   * are inheriting its implementation. However, this object must
   * implement the all the methods of the extended version.
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export Database_Service_exec_i :
    public virtual CUTS::CCM_Database_Service,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /**
     * Initializing constructor.
     *
     * @param[in]     dbase_svc     Reference to a database service.
     */
    Database_Service_exec_i (CUTS_Database_Service & dbase_svc);

    /// Destructor.
    virtual ~Database_Service_exec_i (void);

    /**
     * Create a new test in the database. This causes all collected
     * data to be moved to a new test. This also stops the current
     * test.
     *
     * @return  The id of the new test.
     */
    virtual CORBA::Long create_new_test (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Stops the current test, if there is one.
    virtual void stop_current_test (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Stops the current test, if there is one.
    virtual CORBA::Long current_test (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Reference to the database service.
    CUTS_Database_Service & dbase_svc_;
  };
}

#endif  /* !defined _DATABASE_SERVICE_EXEC_H_ */
