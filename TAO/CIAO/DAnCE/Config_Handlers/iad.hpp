// $Id$
#ifndef IAD_HPP
#define IAD_HPP

// Forward declarations.
//
namespace Deployment
{
  class NamedImplementationArtifact;
  class ImplementationArtifactDescription;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

namespace Deployment
{
  class NamedImplementationArtifact : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // name
    // 
    public:
    ::XMLSchema::string< char > const& name () const;
    ::XMLSchema::string< char >& name ();
    void name (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > name_;

    // referencedArtifact
    // 
    public:
    ::Deployment::ImplementationArtifactDescription const& referencedArtifact () const;
    ::Deployment::ImplementationArtifactDescription& referencedArtifact ();
    void referencedArtifact (::Deployment::ImplementationArtifactDescription const& );

    protected:
    ::std::auto_ptr< ::Deployment::ImplementationArtifactDescription > referencedArtifact_;

    public:
    NamedImplementationArtifact (::XMLSchema::string< char > const& name__,
    ::Deployment::ImplementationArtifactDescription const& referencedArtifact__);

    NamedImplementationArtifact (::XSCRT::XML::Element< char > const&);
    NamedImplementationArtifact (NamedImplementationArtifact const& s);

    NamedImplementationArtifact&
    operator= (NamedImplementationArtifact const& s);

    private:
    char regulator__;
  };

  class ImplementationArtifactDescription : public ::XSCRT::Type
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

    // location
    // 
    public:
    typedef ::std::vector< ::XMLSchema::string< char > >::iterator location_iterator;
    typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator location_const_iterator;
    location_iterator begin_location ();
    location_iterator end_location ();
    location_const_iterator begin_location () const;
    location_const_iterator end_location () const;
    void add_location (::XMLSchema::string< char > const& );

    protected:
    ::std::vector< ::XMLSchema::string< char > > location_;

    // execParameter
    // 
    public:
    bool execParameter_p () const;
    ::Deployment::Property const& execParameter () const;
    ::Deployment::Property& execParameter ();
    void execParameter (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > execParameter_;

    // deployRequirement
    // 
    public:
    bool deployRequirement_p () const;
    ::Deployment::Requirement const& deployRequirement () const;
    ::Deployment::Requirement& deployRequirement ();
    void deployRequirement (::Deployment::Requirement const& );

    protected:
    ::std::auto_ptr< ::Deployment::Requirement > deployRequirement_;

    // dependsOn
    // 
    public:
    typedef ::std::vector< ::Deployment::NamedImplementationArtifact >::iterator dependsOn_iterator;
    typedef ::std::vector< ::Deployment::NamedImplementationArtifact >::const_iterator dependsOn_const_iterator;
    dependsOn_iterator begin_dependsOn ();
    dependsOn_iterator end_dependsOn ();
    dependsOn_const_iterator begin_dependsOn () const;
    dependsOn_const_iterator end_dependsOn () const;
    void add_dependsOn (::Deployment::NamedImplementationArtifact const& );

    protected:
    ::std::vector< ::Deployment::NamedImplementationArtifact > dependsOn_;

    // infoProperty
    // 
    public:
    bool infoProperty_p () const;
    ::Deployment::Property const& infoProperty () const;
    ::Deployment::Property& infoProperty ();
    void infoProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > infoProperty_;

    // contentLocation
    // 
    public:
    bool contentLocation_p () const;
    ::XMLSchema::string< char > const& contentLocation () const;
    ::XMLSchema::string< char >& contentLocation ();
    void contentLocation (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > contentLocation_;

    public:
    ImplementationArtifactDescription ();

    ImplementationArtifactDescription (::XSCRT::XML::Element< char > const&);
    ImplementationArtifactDescription (ImplementationArtifactDescription const& s);

    ImplementationArtifactDescription&
    operator= (ImplementationArtifactDescription const& s);

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
    struct NamedImplementationArtifact : ::XMLSchema::Traversal::Traverser< ::Deployment::NamedImplementationArtifact >
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
      name (Type&);

      virtual void
      name (Type const&);

      virtual void
      referencedArtifact (Type&);

      virtual void
      referencedArtifact (Type const&);

      virtual void
      post (Type&);

      virtual void
      post (Type const&);
    };

    struct ImplementationArtifactDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::ImplementationArtifactDescription >
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
      location (Type&);

      virtual void
      location (Type const&);

      virtual void
      location_pre (Type&);

      virtual void
      location_pre (Type const&);

      virtual void
      location_next (Type&);

      virtual void
      location_next (Type const&);

      virtual void
      location_post (Type&);

      virtual void
      location_post (Type const&);

      virtual void
      location_none (Type&);

      virtual void
      location_none (Type const&);

      virtual void
      execParameter (Type&);

      virtual void
      execParameter (Type const&);

      virtual void
      execParameter_none (Type&);

      virtual void
      execParameter_none (Type const&);

      virtual void
      deployRequirement (Type&);

      virtual void
      deployRequirement (Type const&);

      virtual void
      deployRequirement_none (Type&);

      virtual void
      deployRequirement_none (Type const&);

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
      infoProperty (Type&);

      virtual void
      infoProperty (Type const&);

      virtual void
      infoProperty_none (Type&);

      virtual void
      infoProperty_none (Type const&);

      virtual void
      contentLocation (Type&);

      virtual void
      contentLocation (Type const&);

      virtual void
      contentLocation_none (Type&);

      virtual void
      contentLocation_none (Type const&);

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
    struct NamedImplementationArtifact : Traversal::NamedImplementationArtifact, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::NamedImplementationArtifact Type;
      NamedImplementationArtifact (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      name (Type const&);

      virtual void
      referencedArtifact (Type const&);

      protected:
      NamedImplementationArtifact ();
    };

    struct ImplementationArtifactDescription : Traversal::ImplementationArtifactDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::ImplementationArtifactDescription Type;
      ImplementationArtifactDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      location_pre (Type const&);

      virtual void
      location_next (Type const&);

      virtual void
      location_post (Type const&);

      virtual void
      execParameter (Type const&);

      virtual void
      deployRequirement (Type const&);

      virtual void
      dependsOn_pre (Type const&);

      virtual void
      dependsOn_next (Type const&);

      virtual void
      dependsOn_post (Type const&);

      virtual void
      infoProperty (Type const&);

      virtual void
      contentLocation (Type const&);

      protected:
      ImplementationArtifactDescription ();
    };
  }
}

namespace Deployment
{
}

#endif // IAD_HPP
