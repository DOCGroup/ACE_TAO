// $Id$
#include "pcd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // PackageConfiguration
    // 

    PackageConfiguration::
    PackageConfiguration ()
    :
    regulator__ ()
    {
    }

    PackageConfiguration::
    PackageConfiguration (::CIAO::Config_Handlers::PackageConfiguration const& s)
    :
    label_ (s.label_.get () ? new ::XMLSchema::string< char > (*s.label_) : 0),
    UUID_ (s.UUID_.get () ? new ::XMLSchema::string< char > (*s.UUID_) : 0),
    basePackage_ (s.basePackage_.get () ? new ::CIAO::Config_Handlers::ComponentPackageDescription (*s.basePackage_) : 0),
    reference_ (s.reference_.get () ? new ::CIAO::Config_Handlers::ComponentPackageReference (*s.reference_) : 0),
    selectRequirement_ (s.selectRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.selectRequirement_) : 0),
    configProperty_ (s.configProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.configProperty_) : 0),
    contentLocation_ (s.contentLocation_.get () ? new ::XMLSchema::string< char > (*s.contentLocation_) : 0),
    regulator__ ()
    {
      if (label_.get ()) label_->container (this);
      if (UUID_.get ()) UUID_->container (this);
      if (basePackage_.get ()) basePackage_->container (this);
      if (reference_.get ()) reference_->container (this);
      if (selectRequirement_.get ()) selectRequirement_->container (this);
      if (configProperty_.get ()) configProperty_->container (this);
      if (contentLocation_.get ()) contentLocation_->container (this);
    }

    ::CIAO::Config_Handlers::PackageConfiguration& PackageConfiguration::
    operator= (::CIAO::Config_Handlers::PackageConfiguration const& s)
    {
      if (s.label_.get ()) label (*(s.label_));
      else label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.UUID_.get ()) UUID (*(s.UUID_));
      else UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.basePackage_.get ()) basePackage (*(s.basePackage_));
      else basePackage_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageDescription > (0);

      if (s.reference_.get ()) reference (*(s.reference_));
      else reference_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageReference > (0);

      if (s.selectRequirement_.get ()) selectRequirement (*(s.selectRequirement_));
      else selectRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (0);

      if (s.configProperty_.get ()) configProperty (*(s.configProperty_));
      else configProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.contentLocation_.get ()) contentLocation (*(s.contentLocation_));
      else contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      return *this;
    }


    // PackageConfiguration
    // 
    bool PackageConfiguration::
    label_p () const
    {
      return label_.get () != 0;
    }

    ::XMLSchema::string< char > const& PackageConfiguration::
    label () const
    {
      return *label_;
    }

    ::XMLSchema::string< char >& PackageConfiguration::
    label ()
    {
      return *label_;
    }

    void PackageConfiguration::
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

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    UUID_p () const
    {
      return UUID_.get () != 0;
    }

    ::XMLSchema::string< char > const& PackageConfiguration::
    UUID () const
    {
      return *UUID_;
    }

    ::XMLSchema::string< char >& PackageConfiguration::
    UUID ()
    {
      return *UUID_;
    }

    void PackageConfiguration::
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

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    basePackage_p () const
    {
      return basePackage_.get () != 0;
    }

    ::CIAO::Config_Handlers::ComponentPackageDescription const& PackageConfiguration::
    basePackage () const
    {
      return *basePackage_;
    }

    ::CIAO::Config_Handlers::ComponentPackageDescription& PackageConfiguration::
    basePackage ()
    {
      return *basePackage_;
    }

    void PackageConfiguration::
    basePackage (::CIAO::Config_Handlers::ComponentPackageDescription const& e)
    {
      if (basePackage_.get ())
      {
        *basePackage_ = e;
      }

      else
      {
        basePackage_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageDescription > (new ::CIAO::Config_Handlers::ComponentPackageDescription (e));
        basePackage_->container (this);
      }
    }

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    reference_p () const
    {
      return reference_.get () != 0;
    }

    ::CIAO::Config_Handlers::ComponentPackageReference const& PackageConfiguration::
    reference () const
    {
      return *reference_;
    }

    ::CIAO::Config_Handlers::ComponentPackageReference& PackageConfiguration::
    reference ()
    {
      return *reference_;
    }

    void PackageConfiguration::
    reference (::CIAO::Config_Handlers::ComponentPackageReference const& e)
    {
      if (reference_.get ())
      {
        *reference_ = e;
      }

      else
      {
        reference_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ComponentPackageReference > (new ::CIAO::Config_Handlers::ComponentPackageReference (e));
        reference_->container (this);
      }
    }

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    selectRequirement_p () const
    {
      return selectRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& PackageConfiguration::
    selectRequirement () const
    {
      return *selectRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& PackageConfiguration::
    selectRequirement ()
    {
      return *selectRequirement_;
    }

    void PackageConfiguration::
    selectRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (selectRequirement_.get ())
      {
        *selectRequirement_ = e;
      }

      else
      {
        selectRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (new ::CIAO::Config_Handlers::Requirement (e));
        selectRequirement_->container (this);
      }
    }

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    configProperty_p () const
    {
      return configProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& PackageConfiguration::
    configProperty () const
    {
      return *configProperty_;
    }

    ::CIAO::Config_Handlers::Property& PackageConfiguration::
    configProperty ()
    {
      return *configProperty_;
    }

    void PackageConfiguration::
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

    // PackageConfiguration
    // 
    bool PackageConfiguration::
    contentLocation_p () const
    {
      return contentLocation_.get () != 0;
    }

    ::XMLSchema::string< char > const& PackageConfiguration::
    contentLocation () const
    {
      return *contentLocation_;
    }

    ::XMLSchema::string< char >& PackageConfiguration::
    contentLocation ()
    {
      return *contentLocation_;
    }

    void PackageConfiguration::
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
    // PackageConfiguration
    //

    PackageConfiguration::
    PackageConfiguration (::XSCRT::XML::Element< char > const& e)
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

        else if (n == "basePackage")
        {
          ::CIAO::Config_Handlers::ComponentPackageDescription t (e);
          basePackage (t);
        }

        else if (n == "reference")
        {
          ::CIAO::Config_Handlers::ComponentPackageReference t (e);
          reference (t);
        }

        else if (n == "selectRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          selectRequirement (t);
        }

        else if (n == "configProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          configProperty (t);
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

