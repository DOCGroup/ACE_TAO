// -*- C++ -*-
/**
 * @defgroup Plan_Deployment Plan_Deployment
 *
 * The implementation of the Web_Input_Adapter involves a CGI script
 * that enables an interactive user (sitting at a web browser) to
 * enact live deployments of arbitrary deployment plans.  The
 * necessary interface to RACE is provided through an IDL interface
 * (CIAO::RACE::Plan_Deployment), which is declared in
 * Web_Input_Adapter_Interface.idl and implemented in these files.
 *
 * @author Edward Mulholland <emulholl@atl.lmco.com>
 */
/**
 * @file
 * @ingroup Plan_Deployment
 * @brief  C++ implementation of the Plan_Deployment facet
 *         provided by Web_Input_Adapter.
 *
 * @copydoc Plan_Deployment
 * $Id$
 */

#ifndef CIAO_PLAN_DEPLOYMENT_EXEC_H
#define CIAO_PLAN_DEPLOYMENT_EXEC_H

#include /**/ "ace/pre.h"

#include "Web_Input_AdapterEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Web_Input_Adapter_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Web_Input_Adapter_Impl
    {
      /*
       * Forward declarations
       */
      class WEB_INPUT_ADAPTER_EXEC_Export Web_Input_Adapter_exec_i;
      class Deploy_and_Teardown_Helper;

      /**
       * @brief Implementation of CIAO::RACE::Plan_Deployment IDL
       *        interface.
       */
      class WEB_INPUT_ADAPTER_EXEC_Export Plan_Deployment_exec_i
        : public virtual ::CIAO::RACE::CCM_Plan_Deployment,
          public virtual TAO_Local_RefCounted_Object
      {
        public:

        /// @name Constructor and Destructor
        //@{
        Plan_Deployment_exec_i (Web_Input_Adapter_exec_i &);
        virtual ~Plan_Deployment_exec_i (void);
        //@}

        /// @name Operations from CIAO::RACE::Plan_Deployment
        /// See CIAO::RACE::Plan_Deployment interface documentation.
        //@{
        virtual ::CIAO::RACE::Component_AllocationSeq * allocate_plan (
            const char * uri,
            ::CORBA::Boolean use_race
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual void deploy_plan (
            const char * uri,
            ::CORBA::Boolean use_race
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual void teardown_plan (
            const char * uri
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::CIAO::RACE::Plan_UriSeq * get_deployed_plan_uris (
            ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));
        //@}

      private:
        Web_Input_Adapter_exec_i & component_; ///< Component executor
        Deploy_and_Teardown_Helper * helper_;  ///< Pimpl idiom pointer

        /// @name Prohibited Functions
        //@{
        /** Prohibited function. **/
        Plan_Deployment_exec_i (const Plan_Deployment_exec_i &);
        /** Prohibited function. **/
        Plan_Deployment_exec_i & operator =
          (const Plan_Deployment_exec_i &);
        //@}
      };
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PLAN_DEPLOYMENT_EXEC_H */
