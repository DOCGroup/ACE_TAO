//$Id$
#ifndef CCD_HPP
#define CCD_HPP

// Forward declarations.
//
namespace CIAO
{
  namespace Config_Handlers
  {
    class ComponentInterfaceDescription;
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
    class ComponentInterfaceDescription : public ::XSCRT::Type
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

      // specificType
      //
      public:
      bool specificType_p () const;
      ::XMLSchema::string< char > const& specificType () const;
      ::XMLSchema::string< char >& specificType ();
      void specificType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > specificType_;

      // supportedType
      //
      public:
      bool supportedType_p () const;
      ::XMLSchema::string< char > const& supportedType () const;
      ::XMLSchema::string< char >& supportedType ();
      void supportedType (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > supportedType_;

      // idlFile
      //
      public:
      bool idlFile_p () const;
      ::XMLSchema::string< char > const& idlFile () const;
      ::XMLSchema::string< char >& idlFile ();
      void idlFile (::XMLSchema::string< char > const& );

      protected:
      ::std::auto_ptr< ::XMLSchema::string< char > > idlFile_;

      // configProperty
      //
      public:
      bool configProperty_p () const;
      ::CIAO::Config_Handlers::Property const& configProperty () const;
      ::CIAO::Config_Handlers::Property& configProperty ();
      void configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > configProperty_;

      // port
      //
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentPortDescription >::iterator port_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentPortDescription >::const_iterator port_const_iterator;
      port_iterator begin_port ();
      port_iterator end_port ();
      port_const_iterator begin_port () const;
      port_const_iterator end_port () const;
      void add_port (::CIAO::Config_Handlers::ComponentPortDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ComponentPortDescription > port_;

      // property
      //
      public:
      bool property_p () const;
      ::CIAO::Config_Handlers::ComponentPropertyDescription const& property () const;
      ::CIAO::Config_Handlers::ComponentPropertyDescription& property ();
      void property (::CIAO::Config_Handlers::ComponentPropertyDescription const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPropertyDescription > property_;

      // infoProperty
      //
      public:
      bool infoProperty_p () const;
      ::CIAO::Config_Handlers::Property const& infoProperty () const;
      ::CIAO::Config_Handlers::Property& infoProperty ();
      void infoProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::auto_ptr< ::CIAO::Config_Handlers::Property > infoProperty_;

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
      ComponentInterfaceDescription ();
      ComponentInterfaceDescription (::XSCRT::XML::Element< char > const&);
      ComponentInterfaceDescription (ComponentInterfaceDescription const& s);

      ComponentInterfaceDescription&
      operator= (ComponentInterfaceDescription const& s);

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

#endif // CCD_HPP
