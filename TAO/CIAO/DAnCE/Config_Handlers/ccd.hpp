// $Id$
#ifndef CCD_HPP
#define CCD_HPP

#include "vc6-4786.h"

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
#include "XMLSchema/Types.hpp"

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
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator supportedType_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator supportedType_const_iterator;
      supportedType_iterator begin_supportedType ();
      supportedType_iterator end_supportedType ();
      supportedType_const_iterator begin_supportedType () const;
      supportedType_const_iterator end_supportedType () const;
      void add_supportedType (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > supportedType_;

      // idlFile
      // 
      public:
      typedef ::std::vector< ::XMLSchema::string< char > >::iterator idlFile_iterator;
      typedef ::std::vector< ::XMLSchema::string< char > >::const_iterator idlFile_const_iterator;
      idlFile_iterator begin_idlFile ();
      idlFile_iterator end_idlFile ();
      idlFile_const_iterator begin_idlFile () const;
      idlFile_const_iterator end_idlFile () const;
      void add_idlFile (::XMLSchema::string< char > const& );

      protected:
      ::std::vector< ::XMLSchema::string< char > > idlFile_;

      // configProperty
      // 
      public:
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::iterator configProperty_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::Property >::const_iterator configProperty_const_iterator;
      configProperty_iterator begin_configProperty ();
      configProperty_iterator end_configProperty ();
      configProperty_const_iterator begin_configProperty () const;
      configProperty_const_iterator end_configProperty () const;
      void add_configProperty (::CIAO::Config_Handlers::Property const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::Property > configProperty_;

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
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentPropertyDescription >::iterator property_iterator;
      typedef ::std::vector< ::CIAO::Config_Handlers::ComponentPropertyDescription >::const_iterator property_const_iterator;
      property_iterator begin_property ();
      property_iterator end_property ();
      property_const_iterator begin_property () const;
      property_const_iterator end_property () const;
      void add_property (::CIAO::Config_Handlers::ComponentPropertyDescription const& );

      protected:
      ::std::vector< ::CIAO::Config_Handlers::ComponentPropertyDescription > property_;

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
