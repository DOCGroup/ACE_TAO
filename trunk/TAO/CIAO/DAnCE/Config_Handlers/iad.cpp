#include "iad.hpp"
// $Id$

namespace CIAO
{
  namespace Config_Handlers
  {
    // NamedImplementationArtifact
    // 

    NamedImplementationArtifact::
    NamedImplementationArtifact (::XMLSchema::string< char > const& name__,
    ::CIAO::Config_Handlers::ImplementationArtifactDescription const& referencedArtifact__)
    : 
    name_ (new ::XMLSchema::string< char > (name__)),
    referencedArtifact_ (new ::CIAO::Config_Handlers::ImplementationArtifactDescription (referencedArtifact__)),
    regulator__ ()
    {
      name_->container (this);
      referencedArtifact_->container (this);
    }

    NamedImplementationArtifact::
    NamedImplementationArtifact (::CIAO::Config_Handlers::NamedImplementationArtifact const& s)
    :
    XSCRT::Type (), 
    name_ (new ::XMLSchema::string< char > (*s.name_)),
    referencedArtifact_ (new ::CIAO::Config_Handlers::ImplementationArtifactDescription (*s.referencedArtifact_)),
    regulator__ ()
    {
      name_->container (this);
      referencedArtifact_->container (this);
    }

    ::CIAO::Config_Handlers::NamedImplementationArtifact& NamedImplementationArtifact::
    operator= (::CIAO::Config_Handlers::NamedImplementationArtifact const& s)
    {
      name (s.name ());

      referencedArtifact (s.referencedArtifact ());

      return *this;
    }


    // NamedImplementationArtifact
    // 
    ::XMLSchema::string< char > const& NamedImplementationArtifact::
    name () const
    {
      return *name_;
    }

    ::XMLSchema::string< char >& NamedImplementationArtifact::
    name ()
    {
      return *name_;
    }

    void NamedImplementationArtifact::
    name (::XMLSchema::string< char > const& e)
    {
      *name_ = e;
    }

    // NamedImplementationArtifact
    // 
    ::CIAO::Config_Handlers::ImplementationArtifactDescription const& NamedImplementationArtifact::
    referencedArtifact () const
    {
      return *referencedArtifact_;
    }

    ::CIAO::Config_Handlers::ImplementationArtifactDescription& NamedImplementationArtifact::
    referencedArtifact ()
    {
      return *referencedArtifact_;
    }

    void NamedImplementationArtifact::
    referencedArtifact (::CIAO::Config_Handlers::ImplementationArtifactDescription const& e)
    {
      *referencedArtifact_ = e;
    }


    // ImplementationArtifactDescription
    // 

    ImplementationArtifactDescription::
    ImplementationArtifactDescription ()
    : 
    regulator__ ()
    {
    }

    ImplementationArtifactDescription::
    ImplementationArtifactDescription (::CIAO::Config_Handlers::ImplementationArtifactDescription const& s)
    :
    XSCRT::Type (), 
    label_ (s.label_.get () ? new ::XMLSchema::string< char > (*s.label_) : 0),
    UUID_ (s.UUID_.get () ? new ::XMLSchema::string< char > (*s.UUID_) : 0),
    execParameter_ (s.execParameter_.get () ? new ::CIAO::Config_Handlers::Property (*s.execParameter_) : 0),
    deployRequirement_ (s.deployRequirement_.get () ? new ::CIAO::Config_Handlers::Requirement (*s.deployRequirement_) : 0),
    infoProperty_ (s.infoProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.infoProperty_) : 0),
    contentLocation_ (s.contentLocation_.get () ? new ::XMLSchema::string< char > (*s.contentLocation_) : 0),
    regulator__ ()
    {
      if (label_.get ()) label_->container (this);
      if (UUID_.get ()) UUID_->container (this);
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      if (execParameter_.get ()) execParameter_->container (this);
      if (deployRequirement_.get ()) deployRequirement_->container (this);
      dependsOn_.reserve (s.dependsOn_.size ());
      {
        for (dependsOn_const_iterator i (s.dependsOn_.begin ());
        i != s.dependsOn_.end ();
        ++i) add_dependsOn (*i);
      }

      if (infoProperty_.get ()) infoProperty_->container (this);
      if (contentLocation_.get ()) contentLocation_->container (this);
    }

    ::CIAO::Config_Handlers::ImplementationArtifactDescription& ImplementationArtifactDescription::
    operator= (::CIAO::Config_Handlers::ImplementationArtifactDescription const& s)
    {
      if (s.label_.get ()) label (*(s.label_));
      else label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      if (s.UUID_.get ()) UUID (*(s.UUID_));
      else UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      location_.clear ();
      location_.reserve (s.location_.size ());
      {
        for (location_const_iterator i (s.location_.begin ());
        i != s.location_.end ();
        ++i) add_location (*i);
      }

      if (s.execParameter_.get ()) execParameter (*(s.execParameter_));
      else execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.deployRequirement_.get ()) deployRequirement (*(s.deployRequirement_));
      else deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (0);

      dependsOn_.clear ();
      dependsOn_.reserve (s.dependsOn_.size ());
      {
        for (dependsOn_const_iterator i (s.dependsOn_.begin ());
        i != s.dependsOn_.end ();
        ++i) add_dependsOn (*i);
      }

      if (s.infoProperty_.get ()) infoProperty (*(s.infoProperty_));
      else infoProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      if (s.contentLocation_.get ()) contentLocation (*(s.contentLocation_));
      else contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

      return *this;
    }


    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    label_p () const
    {
      return label_.get () != 0;
    }

    ::XMLSchema::string< char > const& ImplementationArtifactDescription::
    label () const
    {
      return *label_;
    }

    ::XMLSchema::string< char >& ImplementationArtifactDescription::
    label ()
    {
      return *label_;
    }

    void ImplementationArtifactDescription::
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

    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    UUID_p () const
    {
      return UUID_.get () != 0;
    }

    ::XMLSchema::string< char > const& ImplementationArtifactDescription::
    UUID () const
    {
      return *UUID_;
    }

    ::XMLSchema::string< char >& ImplementationArtifactDescription::
    UUID ()
    {
      return *UUID_;
    }

    void ImplementationArtifactDescription::
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

    // ImplementationArtifactDescription
    // 
    ImplementationArtifactDescription::location_iterator ImplementationArtifactDescription::
    begin_location ()
    {
      return location_.begin ();
    }

    ImplementationArtifactDescription::location_iterator ImplementationArtifactDescription::
    end_location ()
    {
      return location_.end ();
    }

    ImplementationArtifactDescription::location_const_iterator ImplementationArtifactDescription::
    begin_location () const
    {
      return location_.begin ();
    }

    ImplementationArtifactDescription::location_const_iterator ImplementationArtifactDescription::
    end_location () const
    {
      return location_.end ();
    }

    void ImplementationArtifactDescription::
    add_location (::XMLSchema::string< char > const& e)
    {
      if (location_.capacity () < location_.size () + 1)
      {
        ::std::vector< ::XMLSchema::string< char > > v;
        v.reserve (location_.size () + 1);

        while (location_.size ())
        {
          //@@ VC6
          ::XMLSchema::string< char >& t = location_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          location_.pop_back ();
        }

        location_.swap (v);
      }

      location_.push_back (e);
      location_.back ().container (this);
    }

    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    execParameter_p () const
    {
      return execParameter_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& ImplementationArtifactDescription::
    execParameter () const
    {
      return *execParameter_;
    }

    ::CIAO::Config_Handlers::Property& ImplementationArtifactDescription::
    execParameter ()
    {
      return *execParameter_;
    }

    void ImplementationArtifactDescription::
    execParameter (::CIAO::Config_Handlers::Property const& e)
    {
      if (execParameter_.get ())
      {
        *execParameter_ = e;
      }

      else
      {
        execParameter_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (new ::CIAO::Config_Handlers::Property (e));
        execParameter_->container (this);
      }
    }

    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    deployRequirement_p () const
    {
      return deployRequirement_.get () != 0;
    }

    ::CIAO::Config_Handlers::Requirement const& ImplementationArtifactDescription::
    deployRequirement () const
    {
      return *deployRequirement_;
    }

    ::CIAO::Config_Handlers::Requirement& ImplementationArtifactDescription::
    deployRequirement ()
    {
      return *deployRequirement_;
    }

    void ImplementationArtifactDescription::
    deployRequirement (::CIAO::Config_Handlers::Requirement const& e)
    {
      if (deployRequirement_.get ())
      {
        *deployRequirement_ = e;
      }

      else
      {
        deployRequirement_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Requirement > (new ::CIAO::Config_Handlers::Requirement (e));
        deployRequirement_->container (this);
      }
    }

    // ImplementationArtifactDescription
    // 
    ImplementationArtifactDescription::dependsOn_iterator ImplementationArtifactDescription::
    begin_dependsOn ()
    {
      return dependsOn_.begin ();
    }

    ImplementationArtifactDescription::dependsOn_iterator ImplementationArtifactDescription::
    end_dependsOn ()
    {
      return dependsOn_.end ();
    }

    ImplementationArtifactDescription::dependsOn_const_iterator ImplementationArtifactDescription::
    begin_dependsOn () const
    {
      return dependsOn_.begin ();
    }

    ImplementationArtifactDescription::dependsOn_const_iterator ImplementationArtifactDescription::
    end_dependsOn () const
    {
      return dependsOn_.end ();
    }

    void ImplementationArtifactDescription::
    add_dependsOn (::CIAO::Config_Handlers::NamedImplementationArtifact const& e)
    {
      if (dependsOn_.capacity () < dependsOn_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::NamedImplementationArtifact > v;
        v.reserve (dependsOn_.size () + 1);

        while (dependsOn_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::NamedImplementationArtifact& t = dependsOn_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          dependsOn_.pop_back ();
        }

        dependsOn_.swap (v);
      }

      dependsOn_.push_back (e);
      dependsOn_.back ().container (this);
    }

    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    infoProperty_p () const
    {
      return infoProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& ImplementationArtifactDescription::
    infoProperty () const
    {
      return *infoProperty_;
    }

    ::CIAO::Config_Handlers::Property& ImplementationArtifactDescription::
    infoProperty ()
    {
      return *infoProperty_;
    }

    void ImplementationArtifactDescription::
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

    // ImplementationArtifactDescription
    // 
    bool ImplementationArtifactDescription::
    contentLocation_p () const
    {
      return contentLocation_.get () != 0;
    }

    ::XMLSchema::string< char > const& ImplementationArtifactDescription::
    contentLocation () const
    {
      return *contentLocation_;
    }

    ::XMLSchema::string< char >& ImplementationArtifactDescription::
    contentLocation ()
    {
      return *contentLocation_;
    }

    void ImplementationArtifactDescription::
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
    // NamedImplementationArtifact
    //

    NamedImplementationArtifact::
    NamedImplementationArtifact (::XSCRT::XML::Element< char > const& e)
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "name")
        {
          name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          name_->container (this);
        }

        else if (n == "referencedArtifact")
        {
          referencedArtifact_ = ::std::auto_ptr< ::CIAO::Config_Handlers::ImplementationArtifactDescription > (new ::CIAO::Config_Handlers::ImplementationArtifactDescription (e));
          referencedArtifact_->container (this);
        }

        else 
        {
        }
      }
    }

    // ImplementationArtifactDescription
    //

    ImplementationArtifactDescription::
    ImplementationArtifactDescription (::XSCRT::XML::Element< char > const& e)
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

        else if (n == "location")
        {
          ::XMLSchema::string< char > t (e);
          add_location (t);
        }

        else if (n == "execParameter")
        {
          ::CIAO::Config_Handlers::Property t (e);
          execParameter (t);
        }

        else if (n == "deployRequirement")
        {
          ::CIAO::Config_Handlers::Requirement t (e);
          deployRequirement (t);
        }

        else if (n == "dependsOn")
        {
          ::CIAO::Config_Handlers::NamedImplementationArtifact t (e);
          add_dependsOn (t);
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

