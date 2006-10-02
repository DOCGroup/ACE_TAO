// -*- C++ -*-
/**
 * @defgroup Component_Control Component_Control
 *
 * The implementation of the Web_Input_Adapter involves a CGI script
 * that needs to know the facet-based uses-provides topology of the
 * deployed RACE components, in order to provide a visualization of
 * this topology and a user interface for manipulating RACE
 * components.  This topology is discovered through an IDL interface
 * declared in Web_Input_Adapter_Interface.idl and implemented in
 * these files.
 *
 * @author Edward Mulholland <emulholl@atl.lmco.com>
 */
/**
 * @file
 * @ingroup Component_Control
 * @brief  C++ implementation of the Component_Control facet
 *         provided by Web_Input_Adapter.
 *
 * @copydoc Component_Control
 * $Id$
 */

#ifndef CIAO_COMPONENT_CONTROL_EXEC_H
#define CIAO_COMPONENT_CONTROL_EXEC_H

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
      class WEB_INPUT_ADAPTER_EXEC_Export Web_Input_Adapter_exec_i;

      /**
       * @brief Implementation of CIAO::RACE::Component_Control IDL
       *        interface.
       */
      class WEB_INPUT_ADAPTER_EXEC_Export Component_Control_exec_i
        : public virtual ::CIAO::RACE::CCM_Component_Control,
          public virtual TAO_Local_RefCounted_Object
      {
        public:

        /// @name Constructor and Destructor
        //@{
        Component_Control_exec_i (Web_Input_Adapter_exec_i &);
        virtual ~Component_Control_exec_i (void);
        //@}

        /// @name Operations from CIAO::RACE::Component_Control
        /// See CIAO::RACE::Component_Control interface documentation.
        //@{
        virtual ::CORBA::Boolean is_initialized (
            ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual void init_component_tree (
            ::Components::CCMObject_ptr root
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException
          ));

        virtual ::CIAO::RACE::Component_Tree_Node * get_component_tree (
            ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::CIAO::RACE::Component_Control::TreeNotInitialized
          ));

        virtual ::CIAO::RACE::AttbuteSeq * get_component_attributes (
            const char * component_name
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::CIAO::RACE::Component_Control::TreeNotInitialized,
            ::CIAO::RACE::Component_Control::NoSuchComponent
          ));

        virtual void set_component_attribute (
            const char * component_name,
            const ::CIAO::RACE::Attbute & bute
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
          )
          ACE_THROW_SPEC ((
            ::CORBA::SystemException,
            ::CIAO::RACE::Component_Control::TreeNotInitialized,
            ::CIAO::RACE::Component_Control::NoSuchComponent
          ));
        //@}
      private:

        class Impl;
        Impl * impl_; ///< Pimpl idiom pointer.

        /// @name Prohibited Functions
        //@{
        /** Prohibited function. **/
        Component_Control_exec_i (const Component_Control_exec_i &);
        /** Prohibited function. **/
        Component_Control_exec_i & operator =
          (const Component_Control_exec_i &);
        //@}
      };
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_COMPONENT_CONTROL_EXEC_H */
