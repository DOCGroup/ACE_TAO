// file      : CCF/IDL2/Traversal/Translation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_TRANSLATION_HPP
#define CCF_IDL2_TRAVERSAL_TRANSLATION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SyntaxTree/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      //
      //
      //
      class RegionTraverser : public Traverser
      {
      public:
        virtual void
        add_content_delegate (Dispatcher* d)
        {
          content_delegates_.push_back (d);
        }

      protected:
        RegionTraverser (Dispatcher* scope)
            : scope_ (scope)
        {
        }

        virtual void
        delegate_content (SyntaxTree::TranslationRegionPtr const& tr)
        {
          for (SyntaxTree::TranslationRegion::Iterator n = tr->begin ();
               n != tr->end ();
               n++)
          {
            if (content_delegates_.empty ())
            {
              dispatch (*n);
            }
            else
            {
              for (DispatcherList::const_iterator i =
                     content_delegates_.begin ();
                   i != content_delegates_.end ();
                   i++)
              {
                (*i)->dispatch (*n);
              }
            }
          }
        }

      protected:
        Dispatcher* scope_;
        DispatcherList content_delegates_;
      };


      //
      //
      //
      struct FileScope : ScopeTraverser
      {
        typedef
        SyntaxTree::FileScopePtr
        NodePtr;

        FileScope ()
        {
          map (typeid (SyntaxTree::FileScope), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::FileScope> ());
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
      struct TranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::TranslationRegionPtr
        NodePtr;

        TranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::TranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct IncludeTranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::IncludeTranslationRegionPtr
        NodePtr;

        IncludeTranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::IncludeTranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::IncludeTranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct UserIncludeTranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::UserIncludeTranslationRegionPtr
        NodePtr;

        UserIncludeTranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::UserIncludeTranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (
            n->dynamic_type<SyntaxTree::UserIncludeTranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct SysIncludeTranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::SysIncludeTranslationRegionPtr
        NodePtr;

        SysIncludeTranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::SysIncludeTranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (
            n->dynamic_type<SyntaxTree::SysIncludeTranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct ImpliedIncludeTranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::ImpliedIncludeTranslationRegionPtr
        NodePtr;

        ImpliedIncludeTranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::ImpliedIncludeTranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (
            n->dynamic_type<SyntaxTree::ImpliedIncludeTranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct PrincipalTranslationRegion : RegionTraverser
      {
        typedef
        SyntaxTree::PrincipalTranslationRegionPtr
        NodePtr;

        PrincipalTranslationRegion (Dispatcher* scope = 0)
            : RegionTraverser (scope)
        {
          map (typeid (SyntaxTree::PrincipalTranslationRegion), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::PrincipalTranslationRegion> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        scope (NodePtr const&);

        virtual void
        post (NodePtr const&);
      };


      //
      //
      //
      struct TranslationUnit : Traverser
      {
        typedef
        SyntaxTree::TranslationUnitPtr
        NodePtr;

        TranslationUnit ()
        {
          map (typeid (SyntaxTree::TranslationUnit), this);
        }

        virtual void
        traverse (SyntaxTree::NodePtr const& n)
        {
          traverse (n->dynamic_type<SyntaxTree::TranslationUnit> ());
        }

        virtual void
        traverse (NodePtr const&);

        virtual void
        pre (NodePtr const&);

        virtual void
        content (NodePtr const&);

        virtual void
        post (NodePtr const&);

      public:
        virtual void
        add_content_delegate (Dispatcher* d)
        {
          content_delegates_.push_back (d);
        }

      protected:
        virtual void
        delegate_content (NodePtr const& tu)
        {
          for (SyntaxTree::TranslationRegion::Iterator n = tu->begin ();
               n != tu->end ();
               n++)
          {
            if (content_delegates_.empty ())
            {
              dispatch (*n);
            }
            else
            {
              for (DispatcherList::const_iterator i =
                     content_delegates_.begin ();
                   i != content_delegates_.end ();
                   i++)
              {
                (*i)->dispatch (*n);
              }
            }
          }
        }

      private:
        DispatcherList content_delegates_;
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TRANSLATION_HPP
