// $Id$
#include "ccd.hpp"

namespace Deployment
{
  // ComponentInterfaceDescription
  // 

  ComponentInterfaceDescription::
  ComponentInterfaceDescription ()
  :
  regulator__ ()
  {
  }

  ComponentInterfaceDescription::
  ComponentInterfaceDescription (::Deployment::ComponentInterfaceDescription const& s)
  :
  label_ (s.label_.get () ? new ::XMLSchema::string< char > (*s.label_) : 0),
  UUID_ (s.UUID_.get () ? new ::XMLSchema::string< char > (*s.UUID_) : 0),
  specificType_ (s.specificType_.get () ? new ::XMLSchema::string< char > (*s.specificType_) : 0),
  contentLocation_ (s.contentLocation_.get () ? new ::XMLSchema::string< char > (*s.contentLocation_) : 0),
  regulator__ ()
  {
    if (label_.get ()) label_->container (this);
    if (UUID_.get ()) UUID_->container (this);
    if (specificType_.get ()) specificType_->container (this);
    supportedType_.reserve (s.supportedType_.size ());
    {
      for (supportedType_const_iterator i (s.supportedType_.begin ());
      i != s.supportedType_.end ();
      ++i) add_supportedType (*i);
    }

    idlFile_.reserve (s.idlFile_.size ());
    {
      for (idlFile_const_iterator i (s.idlFile_.begin ());
      i != s.idlFile_.end ();
      ++i) add_idlFile (*i);
    }

    configProperty_.reserve (s.configProperty_.size ());
    {
      for (configProperty_const_iterator i (s.configProperty_.begin ());
      i != s.configProperty_.end ();
      ++i) add_configProperty (*i);
    }

    port_.reserve (s.port_.size ());
    {
      for (port_const_iterator i (s.port_.begin ());
      i != s.port_.end ();
      ++i) add_port (*i);
    }

    property_.reserve (s.property_.size ());
    {
      for (property_const_iterator i (s.property_.begin ());
      i != s.property_.end ();
      ++i) add_property (*i);
    }

    infoProperty_.reserve (s.infoProperty_.size ());
    {
      for (infoProperty_const_iterator i (s.infoProperty_.begin ());
      i != s.infoProperty_.end ();
      ++i) add_infoProperty (*i);
    }

    if (contentLocation_.get ()) contentLocation_->container (this);
  }

  ::Deployment::ComponentInterfaceDescription& ComponentInterfaceDescription::
  operator= (::Deployment::ComponentInterfaceDescription const& s)
  {
    if (s.label_.get ()) label (*(s.label_));
    else label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    if (s.UUID_.get ()) UUID (*(s.UUID_));
    else UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    if (s.specificType_.get ()) specificType (*(s.specificType_));
    else specificType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    supportedType_.clear ();
    supportedType_.reserve (s.supportedType_.size ());
    {
      for (supportedType_const_iterator i (s.supportedType_.begin ());
      i != s.supportedType_.end ();
      ++i) add_supportedType (*i);
    }

    idlFile_.clear ();
    idlFile_.reserve (s.idlFile_.size ());
    {
      for (idlFile_const_iterator i (s.idlFile_.begin ());
      i != s.idlFile_.end ();
      ++i) add_idlFile (*i);
    }

    configProperty_.clear ();
    configProperty_.reserve (s.configProperty_.size ());
    {
      for (configProperty_const_iterator i (s.configProperty_.begin ());
      i != s.configProperty_.end ();
      ++i) add_configProperty (*i);
    }

    port_.clear ();
    port_.reserve (s.port_.size ());
    {
      for (port_const_iterator i (s.port_.begin ());
      i != s.port_.end ();
      ++i) add_port (*i);
    }

    property_.clear ();
    property_.reserve (s.property_.size ());
    {
      for (property_const_iterator i (s.property_.begin ());
      i != s.property_.end ();
      ++i) add_property (*i);
    }

    infoProperty_.clear ();
    infoProperty_.reserve (s.infoProperty_.size ());
    {
      for (infoProperty_const_iterator i (s.infoProperty_.begin ());
      i != s.infoProperty_.end ();
      ++i) add_infoProperty (*i);
    }

    if (s.contentLocation_.get ()) contentLocation (*(s.contentLocation_));
    else contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    return *this;
  }


  // ComponentInterfaceDescription
  // 
  bool ComponentInterfaceDescription::
  label_p () const
  {
    return label_.get () != 0;
  }

  ::XMLSchema::string< char > const& ComponentInterfaceDescription::
  label () const
  {
    return *label_;
  }

  ::XMLSchema::string< char >& ComponentInterfaceDescription::
  label ()
  {
    return *label_;
  }

  void ComponentInterfaceDescription::
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

  // ComponentInterfaceDescription
  // 
  bool ComponentInterfaceDescription::
  UUID_p () const
  {
    return UUID_.get () != 0;
  }

  ::XMLSchema::string< char > const& ComponentInterfaceDescription::
  UUID () const
  {
    return *UUID_;
  }

  ::XMLSchema::string< char >& ComponentInterfaceDescription::
  UUID ()
  {
    return *UUID_;
  }

  void ComponentInterfaceDescription::
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

  // ComponentInterfaceDescription
  // 
  bool ComponentInterfaceDescription::
  specificType_p () const
  {
    return specificType_.get () != 0;
  }

  ::XMLSchema::string< char > const& ComponentInterfaceDescription::
  specificType () const
  {
    return *specificType_;
  }

  ::XMLSchema::string< char >& ComponentInterfaceDescription::
  specificType ()
  {
    return *specificType_;
  }

  void ComponentInterfaceDescription::
  specificType (::XMLSchema::string< char > const& e)
  {
    if (specificType_.get ())
    {
      *specificType_ = e;
    }

    else
    {
      specificType_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
      specificType_->container (this);
    }
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::supportedType_iterator ComponentInterfaceDescription::
  begin_supportedType ()
  {
    return supportedType_.begin ();
  }

  ComponentInterfaceDescription::supportedType_iterator ComponentInterfaceDescription::
  end_supportedType ()
  {
    return supportedType_.end ();
  }

  ComponentInterfaceDescription::supportedType_const_iterator ComponentInterfaceDescription::
  begin_supportedType () const
  {
    return supportedType_.begin ();
  }

  ComponentInterfaceDescription::supportedType_const_iterator ComponentInterfaceDescription::
  end_supportedType () const
  {
    return supportedType_.end ();
  }

  void ComponentInterfaceDescription::
  add_supportedType (::XMLSchema::string< char > const& e)
  {
    if (supportedType_.capacity () < supportedType_.size () + 1)
    {
      ::std::vector< ::XMLSchema::string< char > > v;
      v.reserve (supportedType_.size () + 1);

      while (supportedType_.size ())
      {
        //@@ VC6
        ::XMLSchema::string< char >& t = supportedType_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        supportedType_.pop_back ();
      }

      supportedType_.swap (v);
    }

    supportedType_.push_back (e);
    supportedType_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::idlFile_iterator ComponentInterfaceDescription::
  begin_idlFile ()
  {
    return idlFile_.begin ();
  }

  ComponentInterfaceDescription::idlFile_iterator ComponentInterfaceDescription::
  end_idlFile ()
  {
    return idlFile_.end ();
  }

  ComponentInterfaceDescription::idlFile_const_iterator ComponentInterfaceDescription::
  begin_idlFile () const
  {
    return idlFile_.begin ();
  }

  ComponentInterfaceDescription::idlFile_const_iterator ComponentInterfaceDescription::
  end_idlFile () const
  {
    return idlFile_.end ();
  }

  void ComponentInterfaceDescription::
  add_idlFile (::XMLSchema::string< char > const& e)
  {
    if (idlFile_.capacity () < idlFile_.size () + 1)
    {
      ::std::vector< ::XMLSchema::string< char > > v;
      v.reserve (idlFile_.size () + 1);

      while (idlFile_.size ())
      {
        //@@ VC6
        ::XMLSchema::string< char >& t = idlFile_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        idlFile_.pop_back ();
      }

      idlFile_.swap (v);
    }

    idlFile_.push_back (e);
    idlFile_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::configProperty_iterator ComponentInterfaceDescription::
  begin_configProperty ()
  {
    return configProperty_.begin ();
  }

  ComponentInterfaceDescription::configProperty_iterator ComponentInterfaceDescription::
  end_configProperty ()
  {
    return configProperty_.end ();
  }

  ComponentInterfaceDescription::configProperty_const_iterator ComponentInterfaceDescription::
  begin_configProperty () const
  {
    return configProperty_.begin ();
  }

  ComponentInterfaceDescription::configProperty_const_iterator ComponentInterfaceDescription::
  end_configProperty () const
  {
    return configProperty_.end ();
  }

  void ComponentInterfaceDescription::
  add_configProperty (::Deployment::Property const& e)
  {
    if (configProperty_.capacity () < configProperty_.size () + 1)
    {
      ::std::vector< ::Deployment::Property > v;
      v.reserve (configProperty_.size () + 1);

      while (configProperty_.size ())
      {
        //@@ VC6
        ::Deployment::Property& t = configProperty_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        configProperty_.pop_back ();
      }

      configProperty_.swap (v);
    }

    configProperty_.push_back (e);
    configProperty_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::port_iterator ComponentInterfaceDescription::
  begin_port ()
  {
    return port_.begin ();
  }

  ComponentInterfaceDescription::port_iterator ComponentInterfaceDescription::
  end_port ()
  {
    return port_.end ();
  }

  ComponentInterfaceDescription::port_const_iterator ComponentInterfaceDescription::
  begin_port () const
  {
    return port_.begin ();
  }

  ComponentInterfaceDescription::port_const_iterator ComponentInterfaceDescription::
  end_port () const
  {
    return port_.end ();
  }

  void ComponentInterfaceDescription::
  add_port (::Deployment::ComponentPortDescription const& e)
  {
    if (port_.capacity () < port_.size () + 1)
    {
      ::std::vector< ::Deployment::ComponentPortDescription > v;
      v.reserve (port_.size () + 1);

      while (port_.size ())
      {
        //@@ VC6
        ::Deployment::ComponentPortDescription& t = port_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        port_.pop_back ();
      }

      port_.swap (v);
    }

    port_.push_back (e);
    port_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::property_iterator ComponentInterfaceDescription::
  begin_property ()
  {
    return property_.begin ();
  }

  ComponentInterfaceDescription::property_iterator ComponentInterfaceDescription::
  end_property ()
  {
    return property_.end ();
  }

  ComponentInterfaceDescription::property_const_iterator ComponentInterfaceDescription::
  begin_property () const
  {
    return property_.begin ();
  }

  ComponentInterfaceDescription::property_const_iterator ComponentInterfaceDescription::
  end_property () const
  {
    return property_.end ();
  }

  void ComponentInterfaceDescription::
  add_property (::Deployment::ComponentPropertyDescription const& e)
  {
    if (property_.capacity () < property_.size () + 1)
    {
      ::std::vector< ::Deployment::ComponentPropertyDescription > v;
      v.reserve (property_.size () + 1);

      while (property_.size ())
      {
        //@@ VC6
        ::Deployment::ComponentPropertyDescription& t = property_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        property_.pop_back ();
      }

      property_.swap (v);
    }

    property_.push_back (e);
    property_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  ComponentInterfaceDescription::infoProperty_iterator ComponentInterfaceDescription::
  begin_infoProperty ()
  {
    return infoProperty_.begin ();
  }

  ComponentInterfaceDescription::infoProperty_iterator ComponentInterfaceDescription::
  end_infoProperty ()
  {
    return infoProperty_.end ();
  }

  ComponentInterfaceDescription::infoProperty_const_iterator ComponentInterfaceDescription::
  begin_infoProperty () const
  {
    return infoProperty_.begin ();
  }

  ComponentInterfaceDescription::infoProperty_const_iterator ComponentInterfaceDescription::
  end_infoProperty () const
  {
    return infoProperty_.end ();
  }

  void ComponentInterfaceDescription::
  add_infoProperty (::Deployment::Property const& e)
  {
    if (infoProperty_.capacity () < infoProperty_.size () + 1)
    {
      ::std::vector< ::Deployment::Property > v;
      v.reserve (infoProperty_.size () + 1);

      while (infoProperty_.size ())
      {
        //@@ VC6
        ::Deployment::Property& t = infoProperty_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        infoProperty_.pop_back ();
      }

      infoProperty_.swap (v);
    }

    infoProperty_.push_back (e);
    infoProperty_.back ().container (this);
  }

  // ComponentInterfaceDescription
  // 
  bool ComponentInterfaceDescription::
  contentLocation_p () const
  {
    return contentLocation_.get () != 0;
  }

  ::XMLSchema::string< char > const& ComponentInterfaceDescription::
  contentLocation () const
  {
    return *contentLocation_;
  }

  ::XMLSchema::string< char >& ComponentInterfaceDescription::
  contentLocation ()
  {
    return *contentLocation_;
  }

  void ComponentInterfaceDescription::
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

namespace Deployment
{
  // ComponentInterfaceDescription
  //

  ComponentInterfaceDescription::
  ComponentInterfaceDescription (::XSCRT::XML::Element< char > const& e)
  :Base__ (e), regulator__ ()
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

      else if (n == "specificType")
      {
        ::XMLSchema::string< char > t (e);
        specificType (t);
      }

      else if (n == "supportedType")
      {
        ::XMLSchema::string< char > t (e);
        add_supportedType (t);
      }

      else if (n == "idlFile")
      {
        ::XMLSchema::string< char > t (e);
        add_idlFile (t);
      }

      else if (n == "configProperty")
      {
        ::Deployment::Property t (e);
        add_configProperty (t);
      }

      else if (n == "port")
      {
        ::Deployment::ComponentPortDescription t (e);
        add_port (t);
      }

      else if (n == "property")
      {
        ::Deployment::ComponentPropertyDescription t (e);
        add_property (t);
      }

      else if (n == "infoProperty")
      {
        ::Deployment::Property t (e);
        add_infoProperty (t);
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

namespace Deployment
{
}

#include <XMLSchema/TypeInfo.hpp>

namespace Deployment
{
  namespace
  {
    ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

    struct ComponentInterfaceDescriptionTypeInfoInitializer
    {
      ComponentInterfaceDescriptionTypeInfoInitializer ()
      {
        ::XSCRT::TypeId id (typeid (ComponentInterfaceDescription));
        ::XSCRT::ExtendedTypeInfo nf (id);

        nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
        ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
      }
    };

    ComponentInterfaceDescriptionTypeInfoInitializer ComponentInterfaceDescriptionTypeInfoInitializer_;
  }
}

namespace Deployment
{
  namespace Traversal
  {
    // ComponentInterfaceDescription
    //
    //

    void ComponentInterfaceDescription::
    traverse (Type& o)
    {
      pre (o);
      if (o.label_p ()) label (o);
      else label_none (o);
      if (o.UUID_p ()) UUID (o);
      else UUID_none (o);
      if (o.specificType_p ()) specificType (o);
      else specificType_none (o);
      supportedType (o);
      idlFile (o);
      configProperty (o);
      port (o);
      property (o);
      infoProperty (o);
      if (o.contentLocation_p ()) contentLocation (o);
      else contentLocation_none (o);
      post (o);
    }

    void ComponentInterfaceDescription::
    traverse (Type const& o)
    {
      pre (o);
      if (o.label_p ()) label (o);
      else label_none (o);
      if (o.UUID_p ()) UUID (o);
      else UUID_none (o);
      if (o.specificType_p ()) specificType (o);
      else specificType_none (o);
      supportedType (o);
      idlFile (o);
      configProperty (o);
      port (o);
      property (o);
      infoProperty (o);
      if (o.contentLocation_p ()) contentLocation (o);
      else contentLocation_none (o);
      post (o);
    }

    void ComponentInterfaceDescription::
    pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    label (Type& o)
    {
      dispatch (o.label ());
    }

    void ComponentInterfaceDescription::
    label (Type const& o)
    {
      dispatch (o.label ());
    }

    void ComponentInterfaceDescription::
    label_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    label_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    UUID (Type& o)
    {
      dispatch (o.UUID ());
    }

    void ComponentInterfaceDescription::
    UUID (Type const& o)
    {
      dispatch (o.UUID ());
    }

    void ComponentInterfaceDescription::
    UUID_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    UUID_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    specificType (Type& o)
    {
      dispatch (o.specificType ());
    }

    void ComponentInterfaceDescription::
    specificType (Type const& o)
    {
      dispatch (o.specificType ());
    }

    void ComponentInterfaceDescription::
    specificType_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    specificType_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::supportedType_iterator b (o.begin_supportedType()), e (o.end_supportedType());

      if (b != e)
      {
        supportedType_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) supportedType_next (o);
        }

        supportedType_post (o);
      }

      else supportedType_none (o);
    }

    void ComponentInterfaceDescription::
    supportedType (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::supportedType_const_iterator b (o.begin_supportedType()), e (o.end_supportedType());

      if (b != e)
      {
        supportedType_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) supportedType_next (o);
        }

        supportedType_post (o);
      }

      else supportedType_none (o);
    }

    void ComponentInterfaceDescription::
    supportedType_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    supportedType_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::idlFile_iterator b (o.begin_idlFile()), e (o.end_idlFile());

      if (b != e)
      {
        idlFile_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) idlFile_next (o);
        }

        idlFile_post (o);
      }

      else idlFile_none (o);
    }

    void ComponentInterfaceDescription::
    idlFile (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::idlFile_const_iterator b (o.begin_idlFile()), e (o.end_idlFile());

      if (b != e)
      {
        idlFile_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) idlFile_next (o);
        }

        idlFile_post (o);
      }

      else idlFile_none (o);
    }

    void ComponentInterfaceDescription::
    idlFile_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    idlFile_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::configProperty_iterator b (o.begin_configProperty()), e (o.end_configProperty());

      if (b != e)
      {
        configProperty_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) configProperty_next (o);
        }

        configProperty_post (o);
      }

      else configProperty_none (o);
    }

    void ComponentInterfaceDescription::
    configProperty (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::configProperty_const_iterator b (o.begin_configProperty()), e (o.end_configProperty());

      if (b != e)
      {
        configProperty_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) configProperty_next (o);
        }

        configProperty_post (o);
      }

      else configProperty_none (o);
    }

    void ComponentInterfaceDescription::
    configProperty_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    configProperty_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    port (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::port_iterator b (o.begin_port()), e (o.end_port());

      if (b != e)
      {
        port_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) port_next (o);
        }

        port_post (o);
      }

      else port_none (o);
    }

    void ComponentInterfaceDescription::
    port (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::port_const_iterator b (o.begin_port()), e (o.end_port());

      if (b != e)
      {
        port_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) port_next (o);
        }

        port_post (o);
      }

      else port_none (o);
    }

    void ComponentInterfaceDescription::
    port_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    port_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    port_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    port_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    port_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    port_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    port_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    port_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    property (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::property_iterator b (o.begin_property()), e (o.end_property());

      if (b != e)
      {
        property_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) property_next (o);
        }

        property_post (o);
      }

      else property_none (o);
    }

    void ComponentInterfaceDescription::
    property (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::property_const_iterator b (o.begin_property()), e (o.end_property());

      if (b != e)
      {
        property_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) property_next (o);
        }

        property_post (o);
      }

      else property_none (o);
    }

    void ComponentInterfaceDescription::
    property_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    property_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    property_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    property_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    property_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    property_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    property_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    property_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty (Type& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::infoProperty_iterator b (o.begin_infoProperty()), e (o.end_infoProperty());

      if (b != e)
      {
        infoProperty_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) infoProperty_next (o);
        }

        infoProperty_post (o);
      }

      else infoProperty_none (o);
    }

    void ComponentInterfaceDescription::
    infoProperty (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ComponentInterfaceDescription::Type::infoProperty_const_iterator b (o.begin_infoProperty()), e (o.end_infoProperty());

      if (b != e)
      {
        infoProperty_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) infoProperty_next (o);
        }

        infoProperty_post (o);
      }

      else infoProperty_none (o);
    }

    void ComponentInterfaceDescription::
    infoProperty_pre (Type&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_pre (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_next (Type&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_next (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_post (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    infoProperty_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    contentLocation (Type& o)
    {
      dispatch (o.contentLocation ());
    }

    void ComponentInterfaceDescription::
    contentLocation (Type const& o)
    {
      dispatch (o.contentLocation ());
    }

    void ComponentInterfaceDescription::
    contentLocation_none (Type&)
    {
    }

    void ComponentInterfaceDescription::
    contentLocation_none (Type const&)
    {
    }

    void ComponentInterfaceDescription::
    post (Type&)
    {
    }

    void ComponentInterfaceDescription::
    post (Type const&)
    {
    }
  }
}

namespace Deployment
{
  namespace Writer
  {
    // ComponentInterfaceDescription
    //
    //

    ComponentInterfaceDescription::
    ComponentInterfaceDescription (::XSCRT::XML::Element< char >& e)
    : ::XSCRT::Writer< char > (e)
    {
    }

    ComponentInterfaceDescription::
    ComponentInterfaceDescription ()
    {
    }

    void ComponentInterfaceDescription::
    traverse (Type const& o)
    {
      Traversal::ComponentInterfaceDescription::traverse (o);
    }

    void ComponentInterfaceDescription::
    label (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("label", top_ ()));
      Traversal::ComponentInterfaceDescription::label (o);
      pop_ ();
    }

    void ComponentInterfaceDescription::
    UUID (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("UUID", top_ ()));
      Traversal::ComponentInterfaceDescription::UUID (o);
      pop_ ();
    }

    void ComponentInterfaceDescription::
    specificType (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("specificType", top_ ()));
      Traversal::ComponentInterfaceDescription::specificType (o);
      pop_ ();
    }

    void ComponentInterfaceDescription::
    supportedType_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("supportedType", top_ ()));
    }

    void ComponentInterfaceDescription::
    supportedType_next (Type const& o)
    {
      supportedType_post (o);
      supportedType_pre (o);
    }

    void ComponentInterfaceDescription::
    supportedType_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    idlFile_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("idlFile", top_ ()));
    }

    void ComponentInterfaceDescription::
    idlFile_next (Type const& o)
    {
      idlFile_post (o);
      idlFile_pre (o);
    }

    void ComponentInterfaceDescription::
    idlFile_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    configProperty_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("configProperty", top_ ()));
    }

    void ComponentInterfaceDescription::
    configProperty_next (Type const& o)
    {
      configProperty_post (o);
      configProperty_pre (o);
    }

    void ComponentInterfaceDescription::
    configProperty_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    port_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("port", top_ ()));
    }

    void ComponentInterfaceDescription::
    port_next (Type const& o)
    {
      port_post (o);
      port_pre (o);
    }

    void ComponentInterfaceDescription::
    port_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    property_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("property", top_ ()));
    }

    void ComponentInterfaceDescription::
    property_next (Type const& o)
    {
      property_post (o);
      property_pre (o);
    }

    void ComponentInterfaceDescription::
    property_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    infoProperty_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("infoProperty", top_ ()));
    }

    void ComponentInterfaceDescription::
    infoProperty_next (Type const& o)
    {
      infoProperty_post (o);
      infoProperty_pre (o);
    }

    void ComponentInterfaceDescription::
    infoProperty_post (Type const&)
    {
      pop_ ();
    }

    void ComponentInterfaceDescription::
    contentLocation (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("contentLocation", top_ ()));
      Traversal::ComponentInterfaceDescription::contentLocation (o);
      pop_ ();
    }
  }
}

namespace Deployment
{
}

