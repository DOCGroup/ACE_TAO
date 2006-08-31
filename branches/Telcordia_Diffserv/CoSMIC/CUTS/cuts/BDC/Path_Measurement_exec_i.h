// -*- C++ -*-

//=============================================================================
/**
 * @file    Path_Measurement_exec_i.h
 *
 * @brief   Implementation for the Path_Measurement object.
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_CIAO_PATH_MEASUREMENT_EXEC_I_H_
#define _CUTS_CIAO_PATH_MEASUREMENT_EXEC_I_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_exec_export.h"
#include "Benchmark_Data_CollectorEC.h"
#include "cuts/config.h"
#include "tao/LocalObject.h"

#include <set>
#include <vector>

// forward declaration(s)
class CUTS_System_Metric;
class CUTS_Time_Metric;

namespace CUTS
{
  // foward declaration(s)
  class Testing_Service_exec_i;

  //=========================================================================
  /**
    * @class Path_Measurement_exec_i
    */
  //=========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export Path_Measurement_exec_i :
    public virtual ::CUTS::CCM_Path_Measurement,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Constructor.
    Path_Measurement_exec_i (CUTS_System_Metric & metrics);

    /// Destructor.
    virtual ~Path_Measurement_exec_i (void);

    /**
      * Bind to the specified @path. The path is a trace through
      * the system in the form of component to events calls. If the
      * path cannot be generated then an exception is thrown.
      *
      * @param[in]     path      Path of interest.
      * @return        Path ID to be used in subsequent calls.
      *
      * @exception CUTS::Binding_Failed
      *    Failed to bind to the specified path.
      */
    virtual ::CORBA::Long bind_to_path (
      const ::CUTS::Path_Sequence & path,
      ::CORBA::UShort flags
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                        ::CUTS::Binding_Failed));

    /**
      * Remove a registered path from the collection. Once this
      * method returns the the @path_id is no longer valid. Any
      * attempts to use the @path_id in subsequent operations will
      * result in an exception.
      *
      * @param[in]     path_id     ID of the path.
      */
    virtual void unbind_to_path (
        ::CORBA::Long path_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                         ::CUTS::ID_Not_Found));

    /**
      * Get the execution time for the specified path. The @a path_id
      * is the one that was returned when bind_to_path () was invoked.
      * If the @path_id is invalid then an exception will be thrown.
      * If the @path_id is not invalid then the best, average, and
      * worse execution time for the specified path is returned.
      *
      * @param[in]     path_id     ID of the path.
      * @return        Best, average and worse execution times.
      *
      * @exception CUTS::ID_Not_Found
      *      Failed to locate the specified ID.
      * @exception
      *      Failed to calculate the execution time.
      */
    virtual ::CUTS::Event_Time_Info execution_time (
        ::CORBA::Long path_id
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
        ACE_THROW_SPEC ((::CORBA::SystemException,
                          ::CUTS::ID_Not_Found,
                          ::CUTS::Operation_Failed));

    /**
      * Set the testing service for the path measurement. This will
      * allow the path measurement to query from the C-string names
      * for the given UUID
      *
      * @param[in]     ts      Pointer to the testing service.
      */
    void testing_service (Testing_Service_exec_i * ts);

  private:
    /// Type definition for the path definitions.
    typedef std::vector <CUTS_Time_Metric *> Path_Definition;

    /// This is not optimal but will due for now!!!
    typedef std::set <Path_Definition *> Path_Registry;

    /// The system metrics.
    CUTS_System_Metric & system_metrics_;

    /// Data storgae for the registered paths.
    Path_Registry paths_;

    /// Locking mechanism for thread synchronization.
    ACE_RW_Thread_Mutex lock_;

    /// Pointer to the testing service.
    Testing_Service_exec_i * testing_service_;
  };
}

#if defined (__CUTS_INLINE__)
#include "cuts/Path_Measurement_exec_i.inl"
#endif

#include /* */ "ace/post.h"

#endif
