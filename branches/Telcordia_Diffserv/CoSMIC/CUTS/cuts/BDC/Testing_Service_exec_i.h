// -*- C++ -*-

//=============================================================================
/**
 * @file    Testing_Service_exec_i.h
 *
 * @brief   Implementation for the Testing_Service object.
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_TEST_SERVICE_EXEC_H_
#define _CUTS_TEST_SERVICE_EXEC_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Benchmark_Data_Collector_exec_export.h"
#include "Benchmark_Data_CollectorEC.h"
#include "cuts/Testing_Service.h"
#include "tao/LocalObject.h"

namespace CUTS
{
  // Forward decl.
  class CCM_Component_Registry;

  //===========================================================================
  /**
   * @class Testing_Service_exec_i
   *
   * @brief Implementation for the Testing_Service object/interface.
   *
   * This object implements the Testing_Service object as defined in
   * cuts/CUTS.idl. The Testing_Service object is required by all
   * components that want to partake in performance analysis. This
   * interface exposed by this implemenation allows components to
   * register themselves it and the Benchmark_Data_Collector (BDC)
   * component can collect performance data from it. The component
   * is not require to register itself with the service if it does
   * not want.
   */
  //===========================================================================

  class BENCHMARK_DATA_COLLECTOR_EXEC_Export Testing_Service_exec_i
    : public CUTS_Testing_Service,
      public virtual CCM_Testing_Service,
      public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Constructor.
    Testing_Service_exec_i (void);

    /// Destructor.
    virtual ~Testing_Service_exec_i (void);

    /**
      * Register a component with the testing service. When a component
      * is registering itself, it must submit its benchmark agent. This
      * allows the Benchmark_Data_Collector to collect its performance
      * metrics whenever needed. If the components cannot be registered
      * then an acception is thrown. The returned value is the registration
      * ID.
      *
      * @param[in]      creg      Reference to a component registration.
      * @return         Registration ID for the component.
      *
      * @exception      CUTS::Registration_Failed
      *    Failed to register the component.
      * @exception      CUTS::Registration_Limit
      *    Reached the maximum number of components allows to register.
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

    /**
     * Get the concrete implementation of the registry.
     *
     * @return Pointer to a CCM component registry.
     */
    const CCM_Component_Registry * registry_i (void) const;

    /**
     * Get the concrete implementation of the registry.
     *
     * @return Pointer to a CCM component registry.
     */
    CCM_Component_Registry * registry_i (void);
  };
}

#include /**/ "ace/post.h"

#endif  // !define _CUTS_TEST_SERVICE_EXEC_H_
