// $Id$
#ifndef CDD_HPP
#define CDD_HPP

// Forward declarations.
//
namespace Deployment
{
  class Domain;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

namespace Deployment
{
  class Domain : public ::XSCRT::Type
  {
    //@@ VC6 anathema
    typedef ::XSCRT::Type Base__;

    // label
    // 
    public:
    ::XMLSchema::string< char > const& label () const;
    ::XMLSchema::string< char >& label ();
    void label (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > label_;

    // UUID
    // 
    public:
    ::XMLSchema::string< char > const& UUID () const;
    ::XMLSchema::string< char >& UUID ();
    void UUID (::XMLSchema::string< char > const& );

    protected:
    ::std::auto_ptr< ::XMLSchema::string< char > > UUID_;

    // sharedResource
    // 
    public:
    bool sharedResource_p () const;
    ::Deployment::SharedResource const& sharedResource () const;
    ::Deployment::SharedResource& sharedResource ();
    void sharedResource (::Deployment::SharedResource const& );

    protected:
    ::std::auto_ptr< ::Deployment::SharedResource > sharedResource_;

    // node
    // 
    public:
    typedef ::std::vector< ::Deployment::Node >::iterator node_iterator;
    typedef ::std::vector< ::Deployment::Node >::const_iterator node_const_iterator;
    node_iterator begin_node ();
    node_iterator end_node ();
    node_const_iterator begin_node () const;
    node_const_iterator end_node () const;
    void add_node (::Deployment::Node const& );

    protected:
    ::std::vector< ::Deployment::Node > node_;

    // infoProperty
    // 
    public:
    bool infoProperty_p () const;
    ::Deployment::Property const& infoProperty () const;
    ::Deployment::Property& infoProperty ();
    void infoProperty (::Deployment::Property const& );

    protected:
    ::std::auto_ptr< ::Deployment::Property > infoProperty_;

    public:
    Domain (::XMLSchema::string< char > const& label__,
    ::XMLSchema::string< char > const& UUID__);

    Domain (::XSCRT::XML::Element< char > const&);
    Domain (Domain const& s);

    Domain&
    operator= (Domain const& s);

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
    struct Domain : ::XMLSchema::Traversal::Traverser< ::Deployment::Domain >
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
      UUID (Type&);

      virtual void
      UUID (Type const&);

      virtual void
      sharedResource (Type&);

      virtual void
      sharedResource (Type const&);

      virtual void
      sharedResource_none (Type&);

      virtual void
      sharedResource_none (Type const&);

      virtual void
      node (Type&);

      virtual void
      node (Type const&);

      virtual void
      node_pre (Type&);

      virtual void
      node_pre (Type const&);

      virtual void
      node_next (Type&);

      virtual void
      node_next (Type const&);

      virtual void
      node_post (Type&);

      virtual void
      node_post (Type const&);

      virtual void
      infoProperty (Type&);

      virtual void
      infoProperty (Type const&);

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
    struct Domain : Traversal::Domain, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::Domain Type;
      Domain (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      sharedResource (Type const&);

      virtual void
      node_pre (Type const&);

      virtual void
      node_next (Type const&);

      virtual void
      node_post (Type const&);

      virtual void
      infoProperty (Type const&);

      protected:
      Domain ();
    };
  }
}

namespace Deployment
{
}

#endif // CDD_HPP
