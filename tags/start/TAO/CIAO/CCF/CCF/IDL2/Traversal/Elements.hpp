// file      : CCF/IDL2/Traversal/Elements.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#ifndef CCF_IDL2_TRAVERSAL_ELEMENTS_HPP
#define CCF_IDL2_TRAVERSAL_ELEMENTS_HPP

#include <map>
#include <vector>

#include "CCF/CompilerElements/Introspection.hpp"

#include "CCF/IDL2/SyntaxTree/Elements.hpp"

namespace CCF
{
  namespace IDL2
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
          throw (Ambiguous)
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
        add (Dispatcher* d) throw (Ambiguous)
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
      class ScopeTraverser : public Traverser
      {
      public:
        virtual void
        add_scope_delegate (Dispatcher* d)
        {
          scope_delegates_.push_back (d);
        }

      protected:
        virtual void
        delegate_scope (SyntaxTree::ScopePtr const& s)
        {
          for (SyntaxTree::Scope::Iterator n = s->begin ();
               n != s->end ();
               n++)
          {
            dispatch (*n);

            for (DispatcherList::const_iterator i = scope_delegates_.begin ();
                 i != scope_delegates_.end ();
                 i++)
            {
              (*i)->dispatch (*n);
            }
          }
        }

      protected:
        DispatcherList scope_delegates_;
      };


      //
      //
      //
      struct Comma : Traverser
      {
        typedef
        SyntaxTree::CommaPtr
        NodePtr;

        Comma ()
        {
          map (typeid (SyntaxTree::Comma), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Comma> ());
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
      struct Declaration : Traverser
      {
        typedef
        SyntaxTree::DeclarationPtr
        NodePtr;

        Declaration ()
        {
          map (typeid (SyntaxTree::Declaration), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Declaration> ());
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
      struct Scope : ScopeTraverser
      {
        typedef
        SyntaxTree::ScopePtr
        NodePtr;

        Scope ()
        {
          map (typeid (SyntaxTree::Scope), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::Scope> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct TypeDecl : Traverser
      {
        typedef
        SyntaxTree::TypeDeclPtr
        NodePtr;

        TypeDecl ()
        {
          map (typeid (SyntaxTree::TypeDecl), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TypeDecl> ());
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
      struct TypeDef : Traverser
      {
        typedef
        SyntaxTree::TypeDefPtr
        NodePtr;

        TypeDef ()
        {
          map (typeid (SyntaxTree::TypeDef), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TypeDef> ());
        }

        virtual void
        traverse (NodePtr const& n)
        {
          delegate (n);
        }
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_ELEMENTS_HPP
