// $Id$
#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include /**/ "ace/pre.h"

#include "Controller_svnt.h"
#include "Poller.h"
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
      class Descriptor: public virtual ::CIAO::RACE::CCM_Descriptors
      {
      public:

        Descriptor (Poller *poller);

        virtual ~Descriptor ();

        virtual void
        register_string (const ::Deployment::PackageConfiguration & pcd,
                         const ::Deployment::DeploymentPlan & plan)
          throw (::CORBA::SystemException);

        virtual void
        unregister_string (const char * UUID)
          throw (::CORBA::SystemException);

      protected:

        virtual int
        parse_deployment_plan (const ::Deployment::DeploymentPlan & plan,
                               App_String & app_string);

        virtual int
        parse_package_config (const ::Deployment::PackageConfiguration & pcd,
                              App_String & app_string);

        virtual int
        parse_path (Deployment::Properties &properties,
                    App_String & app_string);

        virtual char*
        get_path_name (Deployment::Properties &properties);

        virtual CORBA::Double
        get_deadline (Deployment::Properties &properties,
                      const char *path_name);

        virtual char*
        get_path (Deployment::Properties &properties,
                  const char *path_name);

        virtual CUTS::Path_Sequence
        tokenize_path (const char* path);


      private:

        Poller *poller_;

      };
    }
  }
}

#endif /* DESCRIPTOR_H */
