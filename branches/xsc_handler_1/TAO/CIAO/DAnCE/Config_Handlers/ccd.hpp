// $Id$
#ifndef CCD_HPP
#define CCD_HPP

// Forward declarations.
//
namespace Deployment
{
  class ComponentInterfaceDescription;
}

#include <memory>
#include <vector>
#include <XMLSchema/Types.hpp>

#include "Basic_Deployment_Data.hpp"

namespace Deployment
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
    typedef ::std::vector< ::Deployment::Property >::iterator configProperty_iterator;
    typedef ::std::vector< ::Deployment::Property >::const_iterator configProperty_const_iterator;
    configProperty_iterator begin_configProperty ();
    configProperty_iterator end_configProperty ();
    configProperty_const_iterator begin_configProperty () const;
    configProperty_const_iterator end_configProperty () const;
    void add_configProperty (::Deployment::Property const& );

    protected:
    ::std::vector< ::Deployment::Property > configProperty_;

    // port
    // 
    public:
    typedef ::std::vector< ::Deployment::ComponentPortDescription >::iterator port_iterator;
    typedef ::std::vector< ::Deployment::ComponentPortDescription >::const_iterator port_const_iterator;
    port_iterator begin_port ();
    port_iterator end_port ();
    port_const_iterator begin_port () const;
    port_const_iterator end_port () const;
    void add_port (::Deployment::ComponentPortDescription const& );

    protected:
    ::std::vector< ::Deployment::ComponentPortDescription > port_;

    // property
    // 
    public:
    typedef ::std::vector< ::Deployment::ComponentPropertyDescription >::iterator property_iterator;
    typedef ::std::vector< ::Deployment::ComponentPropertyDescription >::const_iterator property_const_iterator;
    property_iterator begin_property ();
    property_iterator end_property ();
    property_const_iterator begin_property () const;
    property_const_iterator end_property () const;
    void add_property (::Deployment::ComponentPropertyDescription const& );

    protected:
    ::std::vector< ::Deployment::ComponentPropertyDescription > property_;

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

namespace Deployment
{
}

#include <XMLSchema/Traversal.hpp>

namespace Deployment
{
  namespace Traversal
  {
    struct ComponentInterfaceDescription : ::XMLSchema::Traversal::Traverser< ::Deployment::ComponentInterfaceDescription >
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
      specificType (Type&);

      virtual void
      specificType (Type const&);

      virtual void
      specificType_none (Type&);

      virtual void
      specificType_none (Type const&);

      virtual void
      supportedType (Type&);

      virtual void
      supportedType (Type const&);

      virtual void
      supportedType_pre (Type&);

      virtual void
      supportedType_pre (Type const&);

      virtual void
      supportedType_next (Type&);

      virtual void
      supportedType_next (Type const&);

      virtual void
      supportedType_post (Type&);

      virtual void
      supportedType_post (Type const&);

      virtual void
      supportedType_none (Type&);

      virtual void
      supportedType_none (Type const&);

      virtual void
      idlFile (Type&);

      virtual void
      idlFile (Type const&);

      virtual void
      idlFile_pre (Type&);

      virtual void
      idlFile_pre (Type const&);

      virtual void
      idlFile_next (Type&);

      virtual void
      idlFile_next (Type const&);

      virtual void
      idlFile_post (Type&);

      virtual void
      idlFile_post (Type const&);

      virtual void
      idlFile_none (Type&);

      virtual void
      idlFile_none (Type const&);

      virtual void
      configProperty (Type&);

      virtual void
      configProperty (Type const&);

      virtual void
      configProperty_pre (Type&);

      virtual void
      configProperty_pre (Type const&);

      virtual void
      configProperty_next (Type&);

      virtual void
      configProperty_next (Type const&);

      virtual void
      configProperty_post (Type&);

      virtual void
      configProperty_post (Type const&);

      virtual void
      configProperty_none (Type&);

      virtual void
      configProperty_none (Type const&);

      virtual void
      port (Type&);

      virtual void
      port (Type const&);

      virtual void
      port_pre (Type&);

      virtual void
      port_pre (Type const&);

      virtual void
      port_next (Type&);

      virtual void
      port_next (Type const&);

      virtual void
      port_post (Type&);

      virtual void
      port_post (Type const&);

      virtual void
      port_none (Type&);

      virtual void
      port_none (Type const&);

      virtual void
      property (Type&);

      virtual void
      property (Type const&);

      virtual void
      property_pre (Type&);

      virtual void
      property_pre (Type const&);

      virtual void
      property_next (Type&);

      virtual void
      property_next (Type const&);

      virtual void
      property_post (Type&);

      virtual void
      property_post (Type const&);

      virtual void
      property_none (Type&);

      virtual void
      property_none (Type const&);

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
    struct ComponentInterfaceDescription : Traversal::ComponentInterfaceDescription, 
    virtual ::XSCRT::Writer< char >
    {
      typedef ::Deployment::ComponentInterfaceDescription Type;
      ComponentInterfaceDescription (::XSCRT::XML::Element< char >&);

      virtual void
      traverse (Type const&);

      virtual void
      label (Type const&);

      virtual void
      UUID (Type const&);

      virtual void
      specificType (Type const&);

      virtual void
      supportedType_pre (Type const&);

      virtual void
      supportedType_next (Type const&);

      virtual void
      supportedType_post (Type const&);

      virtual void
      idlFile_pre (Type const&);

      virtual void
      idlFile_next (Type const&);

      virtual void
      idlFile_post (Type const&);

      virtual void
      configProperty_pre (Type const&);

      virtual void
      configProperty_next (Type const&);

      virtual void
      configProperty_post (Type const&);

      virtual void
      port_pre (Type const&);

      virtual void
      port_next (Type const&);

      virtual void
      port_post (Type const&);

      virtual void
      property_pre (Type const&);

      virtual void
      property_next (Type const&);

      virtual void
      property_post (Type const&);

      virtual void
      infoProperty_pre (Type const&);

      virtual void
      infoProperty_next (Type const&);

      virtual void
      infoProperty_post (Type const&);

      virtual void
      contentLocation (Type const&);

      protected:
      ComponentInterfaceDescription ();
    };
  }
}

namespace Deployment
{
}

#endif // CCD_HPP
