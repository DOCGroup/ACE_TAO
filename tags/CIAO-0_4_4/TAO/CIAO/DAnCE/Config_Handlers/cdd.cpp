//$Id$
#include "cdd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // Domain
    // 

    Domain::
    Domain ()
    :
    label_ (new ::XMLSchema::string< char > ()),
    UUID_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      label_->container (this);
      UUID_->container (this);
    }

    Domain::
    Domain (::CIAO::Config_Handlers::Domain const& s)
    :
    label_ (new ::XMLSchema::string< char > (*s.label_)),
    UUID_ (new ::XMLSchema::string< char > (*s.UUID_)),
    sharedResource_ (s.sharedResource_.get () ? new ::CIAO::Config_Handlers::SharedResource (*s.sharedResource_) : 0),
    infoProperty_ (s.infoProperty_.get () ? new ::CIAO::Config_Handlers::Property (*s.infoProperty_) : 0),
    regulator__ ()
    {
      label_->container (this);
      UUID_->container (this);
      if (sharedResource_.get ()) sharedResource_->container (this);
      node_.reserve (s.node_.size ());
      {
        for (node_const_iterator i (s.node_.begin ());
        i != s.node_.end ();
        ++i) add_node (*i);
      }

      if (infoProperty_.get ()) infoProperty_->container (this);
    }

    ::CIAO::Config_Handlers::Domain& Domain::
    operator= (::CIAO::Config_Handlers::Domain const& s)
    {
      label (s.label ());

      UUID (s.UUID ());

      if (s.sharedResource_.get ()) sharedResource (*(s.sharedResource_));
      else sharedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SharedResource > (0);

      node_.clear ();
      node_.reserve (s.node_.size ());
      {
        for (node_const_iterator i (s.node_.begin ());
        i != s.node_.end ();
        ++i) add_node (*i);
      }

      if (s.infoProperty_.get ()) infoProperty (*(s.infoProperty_));
      else infoProperty_ = ::std::auto_ptr< ::CIAO::Config_Handlers::Property > (0);

      return *this;
    }


    // Domain
    // 
    ::XMLSchema::string< char > const& Domain::
    label () const
    {
      return *label_;
    }

    ::XMLSchema::string< char >& Domain::
    label ()
    {
      return *label_;
    }

    void Domain::
    label (::XMLSchema::string< char > const& e)
    {
      *label_ = e;
    }

    // Domain
    // 
    ::XMLSchema::string< char > const& Domain::
    UUID () const
    {
      return *UUID_;
    }

    ::XMLSchema::string< char >& Domain::
    UUID ()
    {
      return *UUID_;
    }

    void Domain::
    UUID (::XMLSchema::string< char > const& e)
    {
      *UUID_ = e;
    }

    // Domain
    // 
    bool Domain::
    sharedResource_p () const
    {
      return sharedResource_.get () != 0;
    }

    ::CIAO::Config_Handlers::SharedResource const& Domain::
    sharedResource () const
    {
      return *sharedResource_;
    }

    ::CIAO::Config_Handlers::SharedResource& Domain::
    sharedResource ()
    {
      return *sharedResource_;
    }

    void Domain::
    sharedResource (::CIAO::Config_Handlers::SharedResource const& e)
    {
      if (sharedResource_.get ())
      {
        *sharedResource_ = e;
      }

      else
      {
        sharedResource_ = ::std::auto_ptr< ::CIAO::Config_Handlers::SharedResource > (new ::CIAO::Config_Handlers::SharedResource (e));
        sharedResource_->container (this);
      }
    }

    // Domain
    // 
    Domain::node_iterator Domain::
    begin_node ()
    {
      return node_.begin ();
    }

    Domain::node_iterator Domain::
    end_node ()
    {
      return node_.end ();
    }

    Domain::node_const_iterator Domain::
    begin_node () const
    {
      return node_.begin ();
    }

    Domain::node_const_iterator Domain::
    end_node () const
    {
      return node_.end ();
    }

    void Domain::
    add_node (::CIAO::Config_Handlers::Node const& e)
    {
      if (node_.capacity () < node_.size () + 1)
      {
        ::std::vector< ::CIAO::Config_Handlers::Node > v;
        v.reserve (node_.size () + 1);

        while (node_.size ())
        {
          //@@ VC6
          ::CIAO::Config_Handlers::Node& t = node_.back ();
          t.container (0);
          v.push_back (t);
          v.back ().container (this);
          node_.pop_back ();
        }

        node_.swap (v);
      }

      node_.push_back (e);
      node_.back ().container (this);
    }

    // Domain
    // 
    bool Domain::
    infoProperty_p () const
    {
      return infoProperty_.get () != 0;
    }

    ::CIAO::Config_Handlers::Property const& Domain::
    infoProperty () const
    {
      return *infoProperty_;
    }

    ::CIAO::Config_Handlers::Property& Domain::
    infoProperty ()
    {
      return *infoProperty_;
    }

    void Domain::
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
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    // Domain
    //

    Domain::
    Domain (::XSCRT::XML::Element< char > const& e)
    :
    Base__ (e),
    label_ (new ::XMLSchema::string< char > ()),
    UUID_ (new ::XMLSchema::string< char > ()),
    regulator__ ()
    {
      label_->container (this);
      UUID_->container (this);

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

        else if (n == "sharedResource")
        {
          ::CIAO::Config_Handlers::SharedResource t (e);
          sharedResource (t);
        }

        else if (n == "node")
        {
          ::CIAO::Config_Handlers::Node t (e);
          add_node (t);
        }

        else if (n == "infoProperty")
        {
          ::CIAO::Config_Handlers::Property t (e);
          infoProperty (t);
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

