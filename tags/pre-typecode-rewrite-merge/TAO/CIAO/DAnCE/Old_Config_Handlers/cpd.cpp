// $Id$
#include "cpd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // PackagedComponentImplementation
    // 

    PackagedComponentImplementation::
    PackagedComponentImplementation ()
    :
    name_ (new ::XMLSchema::string< char > ()),
    referencedImplementation_ (new ::CIAO::Config_Handlers::ComponentImplementationDescription ()),
    regulator__ ()
    {
      name_->container (this);
      referencedImplementation_->container (this);
    }

    PackagedComponentImplementation::
    PackagedComponentImplementation (::CIAO::Config_Handlers::PackagedComponentImplementation const& s)
    :
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    referencedImplementation_ (new ::CIAO::Config_Handlers::ComponentImplementationDescription (*s.referencedImplementation_)),
    regulator__ ()
    {
      name_->container (this);
      referencedImplementation_->container (this);
    }

    ::CIAO::Config_Handlers::PackagedComponentImplementation& PackagedComponentImplementation::
    operator= (::CIAO::Config_Handlers::PackagedComponentImplementation const& s)
    {
      name (s.name ());

      referencedImplementation (s.referencedImplementation ());

      return *this;
    }


    // PackagedComponentImplementation
    // 
    ::XMLSchema::string< char > const& PackagedComponentImplementation::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& PackagedComponentImplementation::
    name ()
    {
      return *name_;
    }

    void PackagedComponentImplementation::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // PackagedComponentImplementation
    // 
    ::CIAO::Config_Handlers::ComponentImplementationDescription const& PackagedComponentImplementation::
    referencedImplementation () const
    {
      return *referencedImplementation_;
    }

    ::CIAO::Config_Handlers::ComponentImplementationDescription& PackagedComponentImplementation::
    referencedImplementation ()
    {
      return *referencedImplementation_;
    }

    void PackagedComponentImplementation::
    referencedImplementation (::CIAO::Config_Handlers::ComponentImplementationDescription const& e)
    {
      *referencedImplementation_ = e;
    }


    // ComponentPackageDescription
    // 

    ComponentPackageDescription::
    ComponentPackageDescription ()
    :
    regulator__ ()
    {
    }

    ComponentPackageDescription::
    ComponentPackageDescription (::CIAO::Config_Handlers::ComponentPackageDescription const& s)
    :
    label_ (s.label_.get () ? new ::XMLSchema::string< char > (*s.label_) : 0),
    UUID_ (s.UUID_.get () ? new ::XMLSchema::string< char > (*s.UUID_) : 0),
    realizes_ (s.realizes_.get () ? new ::CIAO::Config_Handlers::ComponentInterfaceDescription (*s.realizes_) : 0),
    configProperty_ (s.configProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.configProperty_) : 0),
    implementation_ (s.implementation_.get () ? new ::CIAO::Config_Handlers::PackagedComponentImplementation (*s.implementation_) : 0),
    infoProperty_ (s.infoProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.infoProperty_) : 0),
    contentLocation_ (s.contentLocation_.get () ? new ::XMLSchema::string< char > (*s.contentLocation_) : 0),
    regulator__ ()
    {
      if (label_.get ()) label_->container (this);
      if (UUID_.get ()) UUID_->container (this);
      if (realizes_.get ()) realizes_->container (this);
      if (configProperty_.get ()) configProperty_->container (this);
      if (implementation_.get ()) implementation_->container (this);
      if (infoProperty_.get ()) infoProperty_->container (this);
      if (contentLocation_.get ()) contentLocation_->container (this);
    }

    ::CIAO::Config_Handlers::ComponentPackageDescription& ComponentPackageDescription::
    operator= (::CIAO::Config_Handlers::ComponentPackageDescription const& s)
    {
      if (s.label_.get ()) label (*(s.label_));
      else label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.UUID_.get ()) UUID (*(s.UUID_));
      else UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.realizes_.get ()) realizes (*(s.realizes_));
      else realizes_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentInterfaceDescription > (0);

      if (s.configProperty_.get ()) configProperty (*(s.configProperty_));
      else configProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.implementation_.get ()) implementation (*(s.implementation_));
      else implementation_ = ::std::auto_ptr< ::CIAO::Config_Handlers::PackagedComponentImplementation > (0);

      if (s.infoProperty_.get ()) infoProperty (*(s.infoProperty_));
      else infoProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.contentLocation_.get ()) contentLocation (*(s.contentLocation_));
      else contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      return *this;
    }


    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    label_p () const
    {
      return label_.get () != 0;
    }

    ::XMLSchema::string< char > const& ComponentPackageDescription::
    label () const
    {
      return *label_;
    }

    ::XMLSchema::string< char >& ComponentPackageDescription::
    label ()
    {
      return *label_;
    }

    void ComponentPackageDescription::
    label (::XMLSchema::string< char > const& e)
    {
      if (label_.get ())
      {
        *label_ = e;
      }

      else
      {
        label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        label_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    UUID_p () const
    {
      return UUID_.get () != 0;
    }

    ::XMLSchema::string< char > const& ComponentPackageDescription::
    UUID () const
    {
      return *UUID_;
    }

    ::XMLSchema::string< char >& ComponentPackageDescription::
    UUID ()
    {
      return *UUID_;
    }

    void ComponentPackageDescription::
    UUID (::XMLSchema::string< char > const& e)
    {
      if (UUID_.get ())
      {
        *UUID_ = e;
      }

      else
      {
        UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        UUID_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    realizes_p () const
    {
      return realizes_.get () != 0;
    }

    ::CIAO::Config_Handlers::ComponentInterfaceDescription const& ComponentPackageDescription::
    realizes () const
    {
      return *realizes_;
    }

    ::CIAO::Config_Handlers::ComponentInterfaceDescription& ComponentPackageDescription::
    realizes ()
    {
      return *realizes_;
    }

    void ComponentPackageDescription::
    realizes (::CIAO::Config_Handlers::ComponentInterfaceDescription const& e)
    {
      if (realizes_.get ())
      {
        *realizes_ = e;
      }

      else
      {
        realizes_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentInterfaceDescription > (new ::CIAO::Config_Handlers::ComponentInterfaceDescription (e));
        realizes_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    configProperty_p () const
    {
      return configProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& ComponentPackageDescription::
    configProperty () const
    {
      return *configProperty_;
    }

    ::CIAO::Config_Handlers::Property& ComponentPackageDescription::
    configProperty ()
    {
      return *configProperty_;
    }

    void ComponentPackageDescription::
    configProperty (::CIAO::Config_Handlers::Property const& e)
    {
      if (configProperty_.get ())
      {
        *configProperty_ = e;
      }

      else
      {
        configProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        configProperty_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    implementation_p () const
    {
      return implementation_.get () != 0;
    }

    ::CIAO::Config_Handlers::PackagedComponentImplementation const& ComponentPackageDescription::
    implementation () const
    {
      return *implementation_;
    }

    ::CIAO::Config_Handlers::PackagedComponentImplementation& ComponentPackageDescription::
    implementation ()
    {
      return *implementation_;
    }

    void ComponentPackageDescription::
    implementation (::CIAO::Config_Handlers::PackagedComponentImplementation const& e)
    {
      if (implementation_.get ())
      {
        *implementation_ = e;
      }

      else
      {
        implementation_ = ::std::auto_ptr< ::CIAO::Config_Handlers::PackagedComponentImplementation > (new ::CIAO::Config_Handlers::PackagedComponentImplementation (e));
        implementation_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    infoProperty_p () const
    {
      return infoProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& ComponentPackageDescription::
    infoProperty () const
    {
      return *infoProperty_;
    }

    ::CIAO::Config_Handlers::Property& ComponentPackageDescription::
    infoProperty ()
    {
      return *infoProperty_;
    }

    void ComponentPackageDescription::
    infoProperty (::CIAO::Config_Handlers::Property const& e)
    {
      if (infoProperty_.get ())
      {
        *infoProperty_ = e;
      }

      else
      {
        infoProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        infoProperty_->container (this);
      }
    }

    // ComponentPackageDescription
    // 
    bool ComponentPackageDescription::
    contentLocation_p () const
    {
      return contentLocation_.get () != 0;
    }

    ::XMLSchema::string< char > const& ComponentPackageDescription::
    contentLocation () const
    {
      return *contentLocation_;
    }

    ::XMLSchema::string< char >& ComponentPackageDescription::
    contentLocation ()
    {
      return *contentLocation_;
    }

    void ComponentPackageDescription::
    contentLocation (::XMLSchema::string< char > const& e)
    {
      if (contentLocation_.get ())
      {
        *contentLocation_ = e;
      }

      else
      {
        contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        contentLocation_->container (this);
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    // PackagedComponentImplementation
    //

    PackagedComponentImplementation::
    PackagedComponentImplementation (::XSCRT::XML::Element< char > const& e)
    :
    Base__ (e),
    name_ (new ::XMLSchema::string< char > ()),
    referencedImplementation_ (new ::CIAO::Config_Handlers::ComponentImplementationDescription ()),
    regulator__ ()
    {
      name_->container (this);
      referencedImplementation_->container (this);

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          ::XMLSchema::string< char > t (e);
          name (t);
        }

        else if (n == "referencedImplementation")
        {
          ::CIAO::Config_Handlers::ComponentImplementationDescription t (e);
          referencedImplementation (t);
        }

        else 
        {
        }
      }
    }

    // ComponentPackageDescription
    //

    ComponentPackageDescription::
    ComponentPackageDescription (::XSCRT::XML::Element< char > const& e)
    :
    Base__ (e),
    regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "label")
        {
          ::XMLSchema::string< char > t (e);
          label (t);
        }

        else if (n == "UUID")
        {
          ::XMLSchema::string< char > t (e);
          UUID (t);
        }

        else if (n == "realizes")
        {
          ::CIAO::Config_Handlers::ComponentInterfaceDescription t (e);
          realizes (t);
        }

        else if (n == "configProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          configProperty (t);
        }

        else if (n == "implementation")
        {
          ::CIAO::Config_Handlers::PackagedComponentImplementation t (e);
          implementation (t);
        }

        else if (n == "infoProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          infoProperty (t);
        }

        else if (n == "contentLocation")
        {
          ::XMLSchema::string< char > t (e);
          contentLocation (t);
        }

        else 
        {
        }
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
  }
}

