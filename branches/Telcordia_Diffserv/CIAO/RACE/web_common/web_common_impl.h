// -*- C++ -*-
//==========================================================================
/**
 *  @file    web_common_impl.h
 *
 *  @brief   C++ header file for classes that implement the
 *           eventtype(s) from web_common.idl.
 *
 *  web_common.idl declares IDL 3 Eventtype(s) with methods.  Such
 *  eventtypes require user implementation.  This file contains the
 *  C++ class declarations for eventtype implementations and
 *  associated eventtype factories.
 *
 *  $Id$
 *
 *  @author Edward Mulholland <emulholl@atl.lmco.com>
 */
//==========================================================================

#ifndef RACE_WEB_COMMON_IMPL_H
#define RACE_WEB_COMMON_IMPL_H

#include "WEB_COMMON_impl_export.h"
#include "web_commonC.h"
#include "tao/Valuetype/ValueFactory.h"

namespace CIAO {
  namespace Plan_Launcher {
    class Plan_Launcher_i;
  }
}

namespace OBV_CIAO
{
  namespace RACE
  {
    /**
     * @brief Implement the Web_Deployment eventtype from
     *        web_common.idl.
     */
    class WEB_COMMON_IMPL_Export Web_Deployment_impl
      : public virtual Web_Deployment,
        public virtual CORBA::ValueBase,
        public virtual CORBA::DefaultValueRefCountBase
    {
    public:

      /// @brief Construct a Web_Deployment_impl from a flattened
      ///        deployment plan URI.
      ///
      /// The plan at this URI will be deserialized and
      /// stored in the \p plan member of this valuetype.
      Web_Deployment_impl (const char * plan_uri,
                           const char * pcd_uri);

      /// @brief Construct a Web_Deployment_impl to store a
      ///        given deployment plan.
      ///
      /// The \p plan field will be initialized as a copy of the
      /// given plan; the \p plan_uri field will be left blank,
      /// and can be
      /// filled in later by someone who knows it.
      Web_Deployment_impl (const Deployment::DeploymentPlan & plan,
                           const Deployment::PackageConfiguration & pcd);

      /// Destroy a Web_Deployment_impl.
      virtual ~Web_Deployment_impl ();

      /// Launch plan.
      /**
       * @return the DomainApplicationManager for the newly launched
       *         plan.
       */
      virtual ::Deployment::DomainApplicationManager_ptr launch_plan (
           ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ));

      /// Teardown plan.
      /**
       * @return a boolean indicating whether or not the plan was
       *         successfully torn down.
       */
      virtual ::CORBA::Boolean teardown_plan (
           ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
        )
        ACE_THROW_SPEC ((
          ::CORBA::SystemException
        ));

    protected:

      /// 0-arg constructor for use by the factory in unmarshalling.
      Web_Deployment_impl ();

      // Make factory a friend so that it can a access the 0-arg
      // constructor
      friend class Web_Deployment_Factory;

    private:

      /// This Plan Launcher is a DAnCE artifact that encapsulates the
      /// procedure for deploying a deployment plan.
      ::CIAO::Plan_Launcher::Plan_Launcher_i * plan_launcher_i_;
    };

    /**
     * @brief Eventtype factory for Web_Deployment_Factory.
     *
     * This class is an eventtype factory.  It must be registered with
     * TAO using a macro (TAO_OBV_REGISTER_FACTORY or
     * CIAO_REGISTER_OBV_FACTORY) in order to make sure that
     * events of type CIAO::RACE::Web_Deployment are unmarshalled to
     * the C++ class OBV_CIAO::RACE::Web_Deployment_impl
     * defined above.
     */
    class WEB_COMMON_IMPL_Export Web_Deployment_Factory
      : public CORBA::ValueFactoryBase
    {
      friend class Web_Deployment;
    protected:
      /// Destroy Web_Deployment_Factory
      virtual ~Web_Deployment_Factory ();
    private:
      /// Instantiate Web_Deployment
      TAO_OBV_CREATE_RETURN_TYPE (Web_Deployment) create_for_unmarshal
      (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
    };
  }
}

#endif /* RACE_WEB_COMMON_IMPL_H */
