#ifndef _TESTING_SERVICE_EXEC_I_H_
#define _TESTING_SERVICE_EXEC_I_H_

#include /**/ "ace/pre.h"

#include "Benchmark_Data_Collector_Ex_svnt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_Ex_exec_export.h"
#include "Testing_Service_exec_i.h"

// forward declarations
class CUTS_Database_Service;

namespace CUTS
{
  //===========================================================================
  /**
   * @class Testing_Service_Ex_exec_i
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EX_EXEC_Export Testing_Service_Ex_exec_i :
    virtual public Testing_Service_exec_i
  {
  public:
    /// Constructor.
    Testing_Service_Ex_exec_i (
      CUTS_Database_Service & database_service);

    /// Destructor.
    virtual ~Testing_Service_Ex_exec_i (void);

    /**
     * Register a component. This method is similar to the its
     * parent class's method but it register the component into
     * the database as well.
     *
     * @param[in]     creg      Reference to a component registration.
     * @return        Registration ID for the component.
     *
     * @exception   CUTS::Registration_Failed
     *  Failed to register the component.
     * @exception   CUTS::Registration_Limit
     *  Reached the maximum number of components allowed to
     *  register.
     */
    virtual ::CORBA::Long register_component (
      const ::CUTS::Component_Registration & creg
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::Registration_Failed,
                       ::CUTS::Registration_Limit));

     /**
      * Unregister the component with the testing service. The
      * regisration ID used in the method is the one returned to the
      * component by register_component ().
      *
      * @param[in]      creg      Reference to a component registration.
      *
      * @exception      CUTS::ID_Not_Found
      *    Specified registration ID was not found.
      */
    virtual void unregister_component (
      const ::CUTS::Component_Registration & creg
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((::CORBA::SystemException,
                       ::CUTS::ID_Not_Found));

  private:
    /// Reference to the database service.
    CUTS_Database_Service & database_service_;
  };
}

#include /**/ "ace/post.h"

#endif  // !defined _TESTING_SERVICE_EXEC_I_H_
