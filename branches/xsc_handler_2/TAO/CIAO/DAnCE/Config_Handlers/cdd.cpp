// $Id$
#include "cdd.hpp"

namespace CIAO
{
  namespace Config_Handlers
  {
    // Domain
    // 

    Domain::
    Domain (::XMLSchema::string< char > const& label__,
    ::XMLSchema::string< char > const& UUID__)
    :
    label_ (new ::XMLSchema::string< char > (label__)),
    UUID_ (new ::XMLSchema::string< char > (UUID__)),
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
    :Base__ (e), regulator__ ()
    {

      ::XSCRT::Parser< char > p (e);

      while (p.more_elements ())
      {
        ::XSCRT::XML::Element< char > e (p.next_element ());
        ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

        if (n == "label")
        {
          label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          label_->container (this);
        }

        else if (n == "UUID")
        {
          UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
          UUID_->container (this);
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

#include <XMLSchema/TypeInfo.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace
    {
      ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

      struct DomainTypeInfoInitializer
      {
        DomainTypeInfoInitializer ()
        {
          ::XSCRT::TypeId id (typeid (Domain));
          ::XSCRT::ExtendedTypeInfo nf (id);

          nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
          ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
        }
      };

      DomainTypeInfoInitializer DomainTypeInfoInitializer_;
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Traversal
    {
      // Domain
      //
      //

      void Domain::
      traverse (Type& o)
      {
        pre (o);
        label (o);
        UUID (o);
        if (o.sharedResource_p ()) sharedResource (o);
        else sharedResource_none (o);
        node (o);
        if (o.infoProperty_p ()) infoProperty (o);
        else infoProperty_none (o);
        post (o);
      }

      void Domain::
      traverse (Type const& o)
      {
        pre (o);
        label (o);
        UUID (o);
        if (o.sharedResource_p ()) sharedResource (o);
        else sharedResource_none (o);
        node (o);
        if (o.infoProperty_p ()) infoProperty (o);
        else infoProperty_none (o);
        post (o);
      }

      void Domain::
      pre (Type&)
      {
      }

      void Domain::
      pre (Type const&)
      {
      }

      void Domain::
      label (Type& o)
      {
        dispatch (o.label ());
      }

      void Domain::
      label (Type const& o)
      {
        dispatch (o.label ());
      }

      void Domain::
      UUID (Type& o)
      {
        dispatch (o.UUID ());
      }

      void Domain::
      UUID (Type const& o)
      {
        dispatch (o.UUID ());
      }

      void Domain::
      sharedResource (Type& o)
      {
        dispatch (o.sharedResource ());
      }

      void Domain::
      sharedResource (Type const& o)
      {
        dispatch (o.sharedResource ());
      }

      void Domain::
      sharedResource_none (Type&)
      {
      }

      void Domain::
      sharedResource_none (Type const&)
      {
      }

      void Domain::
      node (Type& o)
      {
        // VC6 anathema strikes again
        //
        Domain::Type::node_iterator b (o.begin_node()), e (o.end_node());

        if (b != e)
        {
          node_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) node_next (o);
          }

          node_post (o);
        }
      }

      void Domain::
      node (Type const& o)
      {
        // VC6 anathema strikes again
        //
        Domain::Type::node_const_iterator b (o.begin_node()), e (o.end_node());

        if (b != e)
        {
          node_pre (o);
          for (;
           b != e;
          )
          {
            dispatch (*b);
            if (++b != e) node_next (o);
          }

          node_post (o);
        }
      }

      void Domain::
      node_pre (Type&)
      {
      }

      void Domain::
      node_pre (Type const&)
      {
      }

      void Domain::
      node_next (Type&)
      {
      }

      void Domain::
      node_next (Type const&)
      {
      }

      void Domain::
      node_post (Type&)
      {
      }

      void Domain::
      node_post (Type const&)
      {
      }

      void Domain::
      infoProperty (Type& o)
      {
        dispatch (o.infoProperty ());
      }

      void Domain::
      infoProperty (Type const& o)
      {
        dispatch (o.infoProperty ());
      }

      void Domain::
      infoProperty_none (Type&)
      {
      }

      void Domain::
      infoProperty_none (Type const&)
      {
      }

      void Domain::
      post (Type&)
      {
      }

      void Domain::
      post (Type const&)
      {
      }
    }
  }
}

namespace CIAO
{
  namespace Config_Handlers
  {
    namespace Writer
    {
      // Domain
      //
      //

      Domain::
      Domain (::XSCRT::XML::Element< char >& e)
      : ::XSCRT::Writer< char > (e)
      {
      }

      Domain::
      Domain ()
      {
      }

      void Domain::
      traverse (Type const& o)
      {
        Traversal::Domain::traverse (o);
      }

      void Domain::
      label (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("label", top_ ()));
        Traversal::Domain::label (o);
        pop_ ();
      }

      void Domain::
      UUID (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("UUID", top_ ()));
        Traversal::Domain::UUID (o);
        pop_ ();
      }

      void Domain::
      sharedResource (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("sharedResource", top_ ()));
        Traversal::Domain::sharedResource (o);
        pop_ ();
      }

      void Domain::
      node_pre (Type const&)
      {
        push_ (::XSCRT::XML::Element< char > ("node", top_ ()));
      }

      void Domain::
      node_next (Type const& o)
      {
        node_post (o);
        node_pre (o);
      }

      void Domain::
      node_post (Type const&)
      {
        pop_ ();
      }

      void Domain::
      infoProperty (Type const& o)
      {
        push_ (::XSCRT::XML::Element< char > ("infoProperty", top_ ()));
        Traversal::Domain::infoProperty (o);
        pop_ ();
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

