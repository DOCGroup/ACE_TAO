// $Id$
#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include /**/ "ace/pre.h"

#include "Controller_svnt.h"
#include "Poller.h"
#include "common.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      /**
       * @class Descriptor
       *
       * @brief Parses the IDL structures to retrieve necessary information.
       *
       * This class is the "entry point" for the RACE controller. The input
       * to the controller includes the DeploymentPlan IDL structure and
       * the PackageConfiguration IDL structure. These structures are
       * parsed to retrieve necessary information such as UUID, name,
       * importance, and the QoS requirement -- critical path and the e-2-e
       * deadline associated with the critical path -- associated with the
       * application string (actually the "plan" since currently an
       * application string is mapped to an "plan").
       */
      class Descriptor: public virtual ::CIAO::RACE::CCM_Descriptors
      {
      public:

        /**
         * Constructor. Take in a reference to the poller object as an
         * input param.
         * @param poller Reference to a poller object.
         */
        Descriptor (Poller *poller);

        /**
         * Default destructor.
         */
        virtual ~Descriptor ();

        /**
         * This is the "entry point" to the RACE Controller. This method is
         * invoked by RACE Output Adapter upon successfull deployment of an
         * application string.
         *
         * @param pcd The PackageConfiguration IDL structure. This IDL is
         * used to retrieve (1) importance, and (2) critical path, and (3)
         * e-2-e deadline associated with the critical path.
         *
         * @param plan The DeploymentPlan IDL structure. This IDL is used
         * to retrieve the UUID and the name of the application string.
         */
        virtual void
        register_string (const ::Deployment::PackageConfiguration & pcd,
                         const ::Deployment::DeploymentPlan & plan)
          throw (::CORBA::SystemException);

        virtual void
        unregister_string (const char * UUID)
          throw (::CORBA::SystemException);

      protected:

        /// Couple of "worker" methods.

        /**
         * Parse the DeploymentPlan IDL structure and populate the UUID and
         * the name fileds of the app-string structure.
         *
         * @param plan DeploymentPlan IDL structure.
         *
         * @param app_string An App_string structure that is populated by
         * this method.
         */
        virtual int
        parse_deployment_plan (const ::Deployment::DeploymentPlan & plan,
                               App_String & app_string);

        /**
         * Parse the PackageConfiguration IDL structure and populate the
         * importance, critical-path, and e-2-e deadline fileds of the
         * app-string structure.
         *
         * @param pcd PackageConfiguration IDL structure.
         *
         * @param app_string An App_string structure that is populated by
         * this method.
         */
        virtual int
        parse_package_config (const ::Deployment::PackageConfiguration & pcd,
                              App_String & app_string);

        /**
         * Parse all the config-properties associated with an application
         * string to retrieve the critical path.
         *
         * @param properties Config-properties associated with an
         * application string.
         *
         * @param app_string An App_string structure that is populated by
         * this method.
         */
        virtual int
        parse_path (Deployment::Properties &properties,
                    App_String & app_string);

        /**
         * This is one of the"worker methods" for the parse_path ()
         * method. Parses the config-properties to obtain the name of the
         * critical path.
         *
         * @param properties Config-properties.
         *
         * @return Name of the critical path.
         */
        virtual char*
        get_path_name (Deployment::Properties &properties);

        /**
         * This is one of the "worker methods" for the parse_path ()
         * method. Parses the config-properties to obtain the e-2-e
         * deadline associated with the given critical.
         *
         * @param properties Config-properties.
         *
         * @param path_name Name of the critical path.
         *
         * @return e-2-e deadline associated with the critical path.
         */
        virtual CORBA::Double
        get_deadline (Deployment::Properties &properties,
                      const char *path_name);

        /**
         * Parse all the config-properties associated with an application
         * string to retrieve the importance of the given critical path.
         *
         * @param properties Config-properties associated with an
         * application string.
         *
         * @param path_name Name of the critical path.
         *
         * @return importance associated with the critical path.
         */
        virtual CORBA::Long
        get_importance (Deployment::Properties &properties,
                        const char *path_name);

        /**
         * This is one of the "worker methods" for the parse_path ()
         * method. Parses the config-properties to obtain the critical path
         * in string format given the path name.
         *
         * @param properties Config-properties.
         *
         * @param path_name Name of the critical path.
         *
         * @return Critical path in string format.
         */
        virtual char*
        get_path (Deployment::Properties &properties,
                  const char *path_name);

        /**
         * This is one of the "worker methods" for the parse_path ()
         * method. Convert the critical path from string format to
         * Critical_Path format.
         *
         * @param path critical path in string format.
         *
         * @return Critical path in Critical_Sequence format.
         */
        virtual Critical_Path
        tokenize_path (const char* path);

      private:

        /// Object reference of the poller object.
        Poller *poller_;
      };

    } /* namespace CIDL_Controller_Impl */
  }  /* namespace RACE */
} /* namespace CIAO */

#endif /* DESCRIPTOR_H */
