// $Id$
#ifndef CDP_HPP
#define CDP_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class DeploymentPlan;
  }
}

#include <memory>
#include <vector>
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

#include "ccd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    class DeploymentPlan : public ::XSCRT::Type
    {
      //@@ VC6 anathema
      typedef ::XSCRT::Type Base__;

      // label
      //
      public:
      bool label_p () const;
      ::XMLSchema::string< char > const& label () const;
      ::XMLSchema::string< char >& label ();
      void label (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > label_;

      // UUID
      //
      public:
      bool UUID_p () const;
      ::XMLSchema::string< char > const& UUID () const;
      ::XMLSchema::string< char >& UUID ();
      void UUID (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > UUID_;

      // realizes
      //
      public:
      ::CIAO::Config_Handlers::ComponentInterfaceDescription const& realizes () const;
      ::CIAO::Config_Handlers::ComponentInterfaceDescription& realizes ();
      void realizes (::CIAO::Config_Handlers::ComponentInterfaceDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentInterfaceDescription > realizes_;

      // implementation
      //
      public:
      ::CIAO::Config_Handlers::MonolithicDeploymentDescription const& implementation () const;
      ::CIAO::Config_Handlers::MonolithicDeploymentDescription& implementation ();
      void implementation (::CIAO::Config_Handlers::MonolithicDeploymentDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::MonolithicDeploymentDescription > implementation_;

      // instance
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::InstanceDeploymentDescription >::iterator instance_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::InstanceDeploymentDescription >::const_iterator instance_const_iterator;
      instance_iterator begin_instance ();
      instance_iterator end_instance ();
      instance_const_iterator begin_instance () const;
      instance_const_iterator end_instance () const;
      void add_instance (::CIAO::Config_Handlers::InstanceDeploymentDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::InstanceDeploymentDescription > instance_;

      // connection
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::PlanConnectionDescription >::iterator connection_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::PlanConnectionDescription >::const_iterator connection_const_iterator;
      connection_iterator begin_connection ();
      connection_iterator end_connection ();
      connection_const_iterator begin_connection () const;
      connection_const_iterator end_connection () const;
      void add_connection (::CIAO::Config_Handlers::PlanConnectionDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::PlanConnectionDescription > connection_;

      // dependsOn
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ImplementationDependency >::iterator dependsOn_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ImplementationDependency >::const_iterator dependsOn_const_iterator;
      dependsOn_iterator begin_dependsOn ();
      dependsOn_iterator end_dependsOn ();
      dependsOn_const_iterator begin_dependsOn () const;
      dependsOn_const_iterator end_dependsOn () const;
      void add_dependsOn (::CIAO::Config_Handlers::ImplementationDependency const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ImplementationDependency > dependsOn_;

      // artifact
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ArtifactDeploymentDescription >::iterator artifact_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ArtifactDeploymentDescription >::const_iterator artifact_const_iterator;
      artifact_iterator begin_artifact ();
      artifact_iterator end_artifact ();
      artifact_const_iterator begin_artifact () const;
      artifact_const_iterator end_artifact () const;
      void add_artifact (::CIAO::Config_Handlers::ArtifactDeploymentDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ArtifactDeploymentDescription > artifact_;

      // infoProperty
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::iterator infoProperty_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::const_iterator infoProperty_const_iterator;
      infoProperty_iterator begin_infoProperty ();
      infoProperty_iterator end_infoProperty ();
      infoProperty_const_iterator begin_infoProperty () const;
      infoProperty_const_iterator end_infoProperty () const;
      void add_infoProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Property > infoProperty_;

      public:
      DeploymentPlan ();
      DeploymentPlan (::XSCRT::XML::Element< char > const&);
      DeploymentPlan (DeploymentPlan const& s);

      DeploymentPlan&
      operator= (DeploymentPlan const& s);

      private:
      char regulator__;
    };
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // CDP_HPP
