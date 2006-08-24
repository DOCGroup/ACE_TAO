#ifndef DESCRIPTOR_CPP
#define DESCRIPTOR_CPP

#include "Descriptor.h"
#include "Config_Handlers/DnC_Dump.h"
#include <sstream>
#include "ace/SString.h"
#include "cuts/CUTSC.h"
#include "ciao/CIAO_common.h"

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_Controller_Impl
    {
      #define CRITICAL_PATH "edu.vanderbilt.dre.CIAO.RACE.criticalPath"
      #define DEADLINE "/Deadline"
      #define ELEMENT ';'
      #define PORT ','

      Descriptor::Descriptor (Poller *poller)
        : poller_ (poller)
      {
      }

      Descriptor::~Descriptor ()
      {}

      int
      Descriptor::parse_deployment_plan
      (const ::Deployment::DeploymentPlan & plan,
       App_String & app_string)
      {
        /// First dump the plan.
        //        Deployment::DnC_Dump::dump (plan);

        /// Frst get the plan id and the label.
        app_string.plan_id_ = CORBA::string_dup (plan.UUID);
        app_string.label_ = CORBA::string_dup (plan.label);


        /// Now get the InstanceDeploymentDescriptions which contains the
        /// information about the components in this plan.
        Deployment::InstanceDeploymentDescriptions
          idd = plan.instance;

        /// Now parse the InstanceDeploymentDescriptions to get the
        /// component_id and the name of the node where the component is
        /// depoyed.

        if (idd.length () > 0)
          {

            for (CORBA::ULong i (0); i < idd.length (); ++i)
              {
                Component_Info comp_info;
                comp_info.id_ = CORBA::string_dup (idd [i].name);
                comp_info.node_ = CORBA::string_dup (idd [i].node);
                app_string.components_.push_back (comp_info);
              }
            return 0;

          }
        return -1;

      }
      void
      Descriptor::unregister_string (const char * UUID)
        throw (::CORBA::SystemException)
      {
        CORBA::String_var temp = CORBA::string_dup (UUID);
        this->poller_->unregister_string (temp);
      }


      void
      Descriptor::register_string
      (const ::Deployment::PackageConfiguration & pcd,
       const ::Deployment::DeploymentPlan & plan
       ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((::CORBA::SystemException))
      {
        App_String app_string;
        /// Register the string with the poller only if the input
        /// descriptors contain the critical path information.
        if ((this->parse_package_config (pcd, app_string) == 0) &&
            (this->parse_deployment_plan (plan, app_string) == 0))
          {
            this->poller_->register_string (app_string);
            this->poller_->start_poller ();
          }
      }

      int
      Descriptor::parse_package_config
      (const ::Deployment::PackageConfiguration &pcd,
       App_String & app_string)
      {
        //        Deployment::DnC_Dump::dump (pcd);
        /// First get the ComponentPackageDescriptions from the
        /// PackageConfiguration.
        Deployment::ComponentPackageDescriptions base_package
          = pcd.basePackage;

        /// Now iterate thro each element in the
        /// ComponentPackageDescription sequence.

        for (CORBA::ULong i = 0; i < base_package.length (); ++i)
          {
            /// Obtaining the PackagedComponentImplementations from each
            /// element in the sequence.
            Deployment::PackagedComponentImplementations &impl =
              base_package [i].implementation;

            /// Now iterate thro each element in the
            /// PackagedComponentImplementation sequence.
            for (CORBA::ULong j = 0; j < impl.length (); ++j)
              {
                /// Ontaining the ComponentImplementationDescription form
                /// each element in the sequence.
                Deployment::ComponentImplementationDescription &cid =
                  impl [j].referencedImplementation;

                /// After getting the CID, parse the CID to obtain the
                /// critical path sequence.

                /// Since there is only one critical path now, if a
                /// critical path is found, there is no need to parse the
                /// remaining config properties.
                if (this->parse_path (cid.configProperty, app_string) == 0)
                  {
                    return 0;
                  }
              }
          }
        return -1;

      }

      char*
      Descriptor::get_path_name (Deployment::Properties &properties)
      {
        const char * path_name;

        /// Get the name of the critical path by seaching for the string
        /// edu.vanderbilt.dre.CIAO.RACE.criticalPath.
        for (CORBA::ULong ctr (0); ctr < properties.length (); ++ctr)
          {
            Deployment::Property & property = properties [ctr];
            if (ACE_OS::strcmp (property.name.in (),
                                CRITICAL_PATH) == 0)
              {
                if (CIAO::debug_level () > 5 )
                  {
                    ACE_DEBUG ((LM_DEBUG, "RACE %N %l "
                                "Found critical path\n"));
                  }

                CORBA::Any & value = property.value;
                CORBA::TypeCode_var tc = value.type ();
                if (tc->kind () == CORBA::tk_string)
                  {
                    value >>= path_name;
                    return CORBA::string_dup (path_name);
                  }
              }
          }
        return 0;
      }

      CORBA::Double
      Descriptor::get_deadline (Deployment::Properties &properties,
                                const char *path_name)
      {
        /// Now search for path_name+"_Deadline"
        std::stringstream path_deadline;
        path_deadline << path_name;
        path_deadline << DEADLINE;
        CORBA::Double deadline;

        for (CORBA::ULong ctr = 0; ctr < properties.length (); ++ctr)
          {
            Deployment::Property & property = properties [ctr];
            if (ACE_OS::strcmp (property.name.in (),
                                path_deadline.str ().c_str ()) == 0)
              {
                CORBA::Any & value = property.value;
                CORBA::TypeCode_var tc = value.type ();
                if (tc->kind () == CORBA::tk_double)
                  {
                    value >>= deadline;
                    if (CIAO::debug_level () > 5 )
                      {
                        ACE_DEBUG ((LM_DEBUG,
                                    "RACE %N %l "
                                    "Critical path deadline is:%.2f\n",
                                    deadline));
                      }
                    return deadline;
                  }
              }
          }
        return 0;
      }

      char*
      Descriptor::get_path (Deployment::Properties &properties,
                            const char *path_name)
      {
        const char *path;
        /// Now search for path_name
        for (CORBA::ULong ctr = 0; ctr < properties.length (); ++ctr)
          {
            Deployment::Property & property = properties [ctr];
            if (ACE_OS::strcmp (property.name.in (),
                                path_name) == 0)
              {
                CORBA::Any & value = property.value;
                CORBA::TypeCode_var tc = value.type ();
                if (tc->kind () == CORBA::tk_string)
                  {
                    value >>= path;
                    if (CIAO::debug_level () > 5 )
                      {
                        ACE_DEBUG ((LM_DEBUG, "RACE %N %l "
                                    "Critical path is:%s\n",
                                    path));
                      }
                    return CORBA::string_dup (path);
                  }
              }
          }

        return 0;
      }


      int
      Descriptor::parse_path (Deployment::Properties &properties,
                              App_String & app_string)
      {
        /// Dump all the properties.
//         for (CORBA::ULong ctr (0); ctr < properties.length (); ++ctr)
//           Deployment::DnC_Dump::dump (properties [ctr]);

        CORBA::String_var critical_path_name;
        CORBA::String_var critical_path;

        critical_path_name =  this->get_path_name (properties);
        if (critical_path_name.in ())
          {
            /// Populate the deadline filed of the app_string.
            app_string.deadline_ =
              this->get_deadline (properties, critical_path_name.in ());

            critical_path =
              this->get_path (properties, critical_path_name.in ());

            if (critical_path.in ())
              {
                /// Populate the critical path filed of the app_string.
                app_string.critical_path_ =
                  this->tokenize_path (critical_path.in ());
                return 0;
              }
          }
        return -1;
      }


      CUTS::Path_Sequence
      Descriptor::tokenize_path (const char* path)
      {
        CORBA::String_var critical_path (CORBA::string_dup (path));
        ACE_Tokenizer tok (critical_path);
        CUTS::Path_Element path_element;
        CUTS::Path_Sequence path_seq;
        CORBA::ULong ctr = 0;
        tok.delimiter_replace (ELEMENT, 0);
        for (char *p = tok.next (); p; p = tok.next (), ++ctr)
          {
            ACE_Tokenizer temp_tok (p);
            temp_tok.delimiter_replace (PORT, 0);

            path_element.node = CORBA::string_dup (temp_tok.next ());
            path_element.src = CORBA::string_dup (temp_tok.next ());
            path_element.dst = CORBA::string_dup (temp_tok.next ());

            path_seq.length (ctr + 1);
            path_seq [ctr] = path_element;
          }
        return path_seq;
      }
    }
  }
}

#endif /*DESCRIPTOR_CPP */
