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
#include "XSCRT/XMLSchema.hpp"
#include "XSCRT/Parser.hpp"

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
      Domain ();
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

#endif // CDD_HPP
