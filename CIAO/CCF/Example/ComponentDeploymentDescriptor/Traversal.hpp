// file      : CCF/Example/ComponentDeploymentDescriptor/Traversal.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CDD_TRAVERSAL_HPP
#define CDD_TRAVERSAL_HPP

#include <map>
#include <vector>

#include "CCF/CompilerElements/Introspection.hpp"

#include "SyntaxTree.hpp"

namespace CDD
{
  namespace Traversal
  {
    //
    //
    //
    class TraverserInterface
    {
    protected:
      virtual
      ~TraverserInterface ()
      {
      }

      virtual void
      traverse (SyntaxTree::NodePtr const& n) = 0;

      friend class Dispatcher;
    };


    //
    //
    //
    class Dispatcher
    {
    public:
      virtual
      ~Dispatcher ()
      {
      }

      virtual void
      dispatch (SyntaxTree::NodePtr const& n);

    protected:

      class Ambiguous {};

      void
      map (Introspection::TypeId id,
           TraverserInterface* t,
           bool suppress_ambiguity = false)
      {
        if (!traversal_map_.insert (std::make_pair (id, t)).second)
        {
          if (suppress_ambiguity)
          {
            traversal_map_[id].suppressed = true;
          }
          else
          {
            throw Ambiguous ();
          }
        }
      }

    private:

      struct TraverserDescriptor
      {
        TraverserDescriptor (TraverserInterface* t = 0)
            : traverser (t),
              suppressed (false)
        {
        }

        TraverserInterface* traverser;
        bool suppressed;
      };

      typedef
      std::map<Introspection::TypeId, TraverserDescriptor>
      TraversalMap;

    public:
      typedef
      TraversalMap::const_iterator
      Iterator;

      Iterator
      begin () const
      {
        return traversal_map_.begin ();
      }

      Iterator
      end () const
      {
        return traversal_map_.end ();
      }

    private:
      TraversalMap traversal_map_;
    };


    //
    //
    //
    class Discriminator : public virtual Dispatcher
    {
    public:
      virtual void
      add (Dispatcher* d)
      {
        for (Iterator i = d->begin (); i != d->end (); i++)
        {
          map (i->first, i->second.traverser, true);
        }
      }
    };


    //
    //
    //
    class Traverser : public TraverserInterface,
                      public virtual Dispatcher
    {
    public:
      virtual void
      add_delegate (Dispatcher* d)
      {
        delegates_.push_back (d);
      }

    protected:
      virtual bool
      delegate (SyntaxTree::NodePtr const& n) const
      {
        if (delegates_.empty ()) return false;

        for (DispatcherList::const_iterator i = delegates_.begin ();
             i != delegates_.end ();
             i++)
        {
          (*i)->dispatch (n);
        }

        return true;
      }

    protected:
      typedef
      std::vector<Dispatcher*>
      DispatcherList;

      DispatcherList delegates_;
    };


    //
    //
    //
    struct PortInstance : Traverser
    {
      typedef
      SyntaxTree::PortInstancePtr
      NodePtr;

      PortInstance ()
      {
        map (typeid (SyntaxTree::PortInstance), this);
      }

      virtual void
      traverse (SyntaxTree::NodePtr const& n)
      {
        traverse (n->dynamic_type<SyntaxTree::PortInstance> ());
      }

      virtual void
      traverse (NodePtr const& n)
      {
        delegate (n);
      }
    };


    //
    //
    //
    struct ComponentInstance : Traverser
    {
    public:

      virtual void
      add_port_delegate (Dispatcher* d)
      {
        port_delegates_.push_back (d);
      }

    public:
      typedef
      SyntaxTree::ComponentInstancePtr
      NodePtr;

      ComponentInstance ()
      {
        map (typeid (SyntaxTree::ComponentInstance), this);
      }

      virtual void
      traverse (SyntaxTree::NodePtr const& n)
      {
        traverse (n->dynamic_type<SyntaxTree::ComponentInstance> ());
      }

      virtual void
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          ports (n);
          post (n);
        }
      }

      virtual void
      pre (NodePtr const&)
      {
      }

      virtual void
      ports (NodePtr const& n)
      {
        delegate_port (n);
      }

      virtual void
      post (NodePtr const&)
      {
      }

    protected:
      virtual void
      delegate_port (NodePtr const& s)
      {
        for (SyntaxTree::ComponentInstance::Iterator n = s->begin ();
             n != s->end ();
             n++)
        {
          dispatch (*n);

          for (DispatcherList::const_iterator i = port_delegates_.begin ();
               i != port_delegates_.end ();
               i++)
          {
            (*i)->dispatch (*n);
          }
        }
      }

    protected:
      DispatcherList port_delegates_;
    };


    //
    //
    //
    struct Connection : Traverser
    {
      typedef
      SyntaxTree::ConnectionPtr
      NodePtr;

      Connection ()
      {
        map (typeid (SyntaxTree::Connection), this);
      }

      virtual void
      traverse (SyntaxTree::NodePtr const& n)
      {
        traverse (n->dynamic_type<SyntaxTree::Connection> ());
      }

      virtual void
      traverse (NodePtr const& n)
      {
        delegate (n);
      }
    };


    //
    //
    //
    struct Descriptor : Traverser
    {
    public:

      virtual void
      add_node_delegate (Dispatcher* d)
      {
        node_delegates_.push_back (d);
      }

    public:
      typedef
      SyntaxTree::DescriptorPtr
      NodePtr;

      Descriptor ()
      {
        map (typeid (SyntaxTree::Descriptor), this);
      }

      virtual void
      traverse (SyntaxTree::NodePtr const& n)
      {
        traverse (n->dynamic_type<SyntaxTree::Descriptor> ());
      }

      virtual void
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          ports (n);
          post (n);
        }
      }

      virtual void
      pre (NodePtr const&)
      {
      }

      virtual void
      ports (NodePtr const& n)
      {
        delegate_node (n);
      }

      virtual void
      post (NodePtr const&)
      {
      }

    protected:
      virtual void
      delegate_node (NodePtr const& s)
      {
        for (SyntaxTree::Descriptor::Iterator n = s->begin ();
             n != s->end ();
             n++)
        {
          dispatch (*n);

          for (DispatcherList::const_iterator i = node_delegates_.begin ();
               i != node_delegates_.end ();
               i++)
          {
            (*i)->dispatch (*n);
          }
        }
      }

    protected:
      DispatcherList node_delegates_;
    };
  }
}

#endif  // CDD_TRAVERSAL_HPP
