// $Id$
#ifndef CDP_HPP
#define CDP_HPP

// Forward declarations.
//
namespace Deployment
{
  class DeploymentPlan;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "ccd.hpp"

namespace Deployment
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
    ::Deployment::ComponentInterfaceDescription const& realizes () const;
    ::Deployment::ComponentInterfaceDescription& realizes ();
    void realizes (::Deployment::ComponentInterfaceDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ComponentInterfaceDescription > realizes_;

    // implementation
    // 
    public:
    typedef ::std::vector< ::Deployment::MonolithicDeploymentDescription >::iterator implementation_iterator;
    typedef ::std::vector< ::Deployment::MonolithicDeploymentDescription >::const_iterator implementation_const_iterator;
    implementation_iterator begin_implementation ();
    implementation_iterator end_implementation ();
    implementation_const_iterator begin_implementation () const;
    implementation_const_iterator end_implementation () const;
    void add_implementation (::Deployment::MonolithicDeploymentDescription const& );

    protected:
    ::std::vector< ::Deployment::MonolithicDeploymentDescription > implementation_;

    // instance
    // 
    public:
    typedef ::std::vector< ::Deployment::InstanceDeploymentDescription >::iterator instance_iterator;
    typedef ::std::vector< ::Deployment::InstanceDeploymentDescription >::const_iterator instance_const_iterator;
    instance_iterator begin_instance ();
    instance_iterator end_instance ();
    instance_const_iterator begin_instance () const;
    instance_const_iterator end_instance () const;
    void add_instance (::Deployment::InstanceDeploymentDescription const& );

    protected:
    ::std::vector< ::Deployment::InstanceDeploymentDescription > instance_;

    // connection
    // 
    public:
    typedef ::std::vector< ::Deployment::PlanConnectionDescription >::iterator connection_iterator;
    typedef ::std::vector< ::Deployment::PlanConnectionDescription >::const_iterator connection_const_iterator;
    connection_iterator begin_connection ();
    connection_iterator end_connection ();
    connection_const_iterator begin_connection () const;
    connection_const_iterator end_connection () const;
    void add_connection (::Deployment::PlanConnectionDescription const& );

    protected:
    ::std::vector< ::Deployment::PlanConnectionDescription > connection_;

    // dependsOn
    // 
    public:
    typedef ::std::vector< ::Deployment::ImplementationDependency >::iterator dependsOn_iterator;
    typedef ::std::vector< ::Deployment::ImplementationDependency >::const_iterator dependsOn_const_iterator;
    dependsOn_iterator begin_dependsOn ();
    dependsOn_iterator end_dependsOn ();
    dependsOn_const_iterator begin_dependsOn () const;
    dependsOn_const_iterator end_dependsOn () const;
    void add_dependsOn (::Deployment::ImplementationDependency const& );

    protected:
    ::std::vector< ::Deployment::ImplementationDependency > dependsOn_;

    // artifact
    // 
    public:
    typedef ::std::vector< ::Deployment::ArtifactDeploymentDescription >::iterator artifact_iterator;
    typedef ::std::vector< ::Deployment::ArtifactDeploymentDescription >::const_iterator artifact_const_iterator;
    artifact_iterator begin_artifact ();
    artifact_iterator end_artifact ();
    artifact_const_iterator begin_artifact () const;
    artifact_const_iterator end_artifact () const;
    void add_artifact (::Deployment::ArtifactDeploymentDescription const& );

    protected:
    ::std::vector< ::Deployment::ArtifactDeploymentDescription > artifact_;

    // infoProperty
    // 
    public:
    typedef ::std::vector< ::Deployment::Property >::iterator infoProperty_iterator;
    typedef ::std::vector< ::Deployment::Property >::const_iterator infoProperty_const_iterator;
    infoProperty_iterator begin_infoProperty ();
    infoProperty_iterator end_infoProperty ();
    infoProperty_const_iterator begin_infoProperty () const;
    infoProperty_const_iterator end_infoProperty () const;
    void add_infoProperty (::Deployment::Property const& );

    protected:
    ::std::vector< ::Deployment::Property > infoProperty_;

    public:
    DeploymentPlan (::Deployment::ComponentInterfaceDescription const& realizes__);

    DeploymentPlan (::XSCRT::XML::Element< char > const&);
    DeploymentPlan (DeploymentPlan const& s);

    DeploymentPlan&
    operator= (DeploymentPlan const& s);

    private:
    char regulator__;
  };
}

namespace Deployment
{
}

#include <XMLSchema/Traversal.hpp>

namespace Deployment
{
  namespace Traversal
  {
    struct DeploymentPlan : ::XMLSchema::Traversal::Traverser< ::Deployment::DeploymentPlan >
    {
      virtual void
      traverse (Type&);

      virtual void
      traverse (Type const&);

      virtual void
      pre (Type&);

      virtual void
      pre (Type const&);

      virtual void
      label (Type&);

      virtual void
      label (Type const&);

      virtual void
      label_none (Type&);

      virtual void
      label_none (Type const&);

      virtual void
      UUID (Type&);

      virtual void
      UUID (Type const&);

      virtual void
      UUID_none (Type&);

      virtual void
      UUID_none (Type const&);

      virtual void
      realizes (Type&);

      virtual void
      realizes (Type const&);

      virtual void
      implementation (Type&);

      virtual void
      implementation (Type const&);

      virtual void
      implementation_pre (Type&);

      virtual void
      implementation_pre (Type const&);

      virtual void
      implementation_next (Type&);

      virtual void
      implementation_next (Type const&);

      virtual void
      implementation_post (Type&);

      virtual void
      implementation_post (Type const&);

      virtual void
      instance (Type&);

      virtual void
      instance (Type const&);

      virtual void
      instance_pre (Type&);

      virtual void
      instance_pre (Type const&);

      virtual void
      instance_next (Type&);

      virtual void
      instance_next (Type const&);

      virtual void
      instance_post (Type&);

      virtual void
      instance_post (Type const&);

      virtual void
      instance_none (Type&);

      virtual void
      instance_none (Type const&);

      virtual void
      connection (Type&);

      virtual void
      connection (Type const&);

      virtual void
      connection_pre (Type&);

      virtual void
      connection_pre (Type const&);

      virtual void
      connection_next (Type&);

      virtual void
      connection_next (Type const&);

      virtual void
      connection_post (Type&);

      virtual void
      connection_post (Type const&);

      virtual void
      connection_none (Type&);

      virtual void
      connection_none (Type const&);

      virtual void
      dependsOn (Type&);

      virtual void
      dependsOn (Type const&);

      virtual void
      dependsOn_pre (Type&);

      virtual void
      dependsOn_pre (Type const&);

      virtual void
      dependsOn_next (Type&);

      virtual void
      dependsOn_next (Type const&);

      virtual void
      dependsOn_post (Type&);

      virtual void
      dependsOn_post (Type const&);

      virtual void
      dependsOn_none (Type&);

      virtual void
      dependsOn_none (Type const&);

      virtual void
      artifact (Type&);

      virtual void
      artifact (Type const&);

      virtual void
      artifact_pre (Type&);

      virtual void
      artifact_pre (Type const&);

      virtual void
      artifact_next (Type&);

      virtual void
      artifact_next (Type const&);

      virtual void
      artifact_post (Type&);

      virtual void
      artifact_post (Type const&);

      virtual void
      artifact_none (Type&);

      virtual void
      artifact_none (Type const&);

      virtual void
      infoProperty (Type&);

      virtual void
      infoProperty (Type const&);

      virtual void
      infoProperty_pre (Type&);

      virtual void
      infoProperty_pre (Type const&);

      virtual void
      infoProperty_next (Type&);

      virtual void
      infoProperty_next (Type const&);

      virtual void
      infoProperty_post (Type&);

      virtual void
      infoProperty_post (Type const&);

      virtual void
      infoProperty_none (Type&);

      virtual void
      infoProperty_none (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };
  }
}

#include <XMLSchema/Writer.hpp>

namespace Deployment
{
  namespace Writer
  {
    struct DeploymentPlan : Traversal::DeploymentPlan, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::DeploymentPlan Type;
      DeploymentPlan (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      realizes (Type const&);

      virtual void
      implementation_pre (Type const&);

      virtual void
      implementation_next (Type const&);

      virtual void
      implementation_post (Type const&);

      virtual void
      instance_pre (Type const&);

      virtual void
      instance_next (Type const&);

      virtual void
      instance_post (Type const&);

      virtual void
      connection_pre (Type const&);

      virtual void
      connection_next (Type const&);

      virtual void
      connection_post (Type const&);

      virtual void
      dependsOn_pre (Type const&);

      virtual void
      dependsOn_next (Type const&);

      virtual void
      dependsOn_post (Type const&);

      virtual void
      artifact_pre (Type const&);

      virtual void
      artifact_next (Type const&);

      virtual void
      artifact_post (Type const&);

      virtual void
      infoProperty_pre (Type const&);

      virtual void
      infoProperty_next (Type const&);

      virtual void
      infoProperty_post (Type const&);

      protected:
      DeploymentPlan ();
    };
  }
}

namespace Deployment
{
}

#endif // CDP_HPP
