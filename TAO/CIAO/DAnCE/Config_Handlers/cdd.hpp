// $Id$
#ifndef CDD_HPP
#define CDD_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class Domain;
  }
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

namespace CIAO
{
  namespace Config_Handlers
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
      ::CIAO::Config_Handlers::SharedResource const& sharedResource () const;
      ::CIAO::Config_Handlers::SharedResource& sharedResource ();
      void sharedResource (::CIAO::Config_Handlers::SharedResource const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::SharedResource > sharedResource_;

      // node
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::Node >::iterator node_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Node >::const_iterator node_const_iterator;
      node_iterator begin_node ();
      node_iterator end_node ();
      node_const_iterator begin_node () const;
      node_const_iterator end_node () const;
      void add_node (::CIAO::Config_Handlers::Node const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Node > node_;

      // infoProperty
      // 
      public:
      bool infoProperty_p () const;
      ::CIAO::Config_Handlers::Property const& infoProperty () const;
      ::CIAO::Config_Handlers::Property& infoProperty ();
      void infoProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > infoProperty_;

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
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#include <XMLSchema/Traversal.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      struct Domain : ::XMLSchema::Traversal::Traverser< ::CIAO::Config_Handlers::Domain >
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
}

#include <XMLSchema/Writer.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      struct Domain : Traversal::Domain, 
      virtual ::XSCRT::Writer< char >
      {
        typedef ::CIAO::Config_Handlers::Domain Type;
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
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

#endif // CDD_HPP
