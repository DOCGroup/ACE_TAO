// file      : CCF/IDL2/Traversal/Translation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $id$

#include "CCF/IDL2/Traversal/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // FileScope
      //
      //

      void FileScope::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          scope (n);
          post (n);
        }
      }

      void FileScope::
      pre (NodePtr const&)
      {
      }

      void FileScope::
      scope (NodePtr const& n)
      {
        delegate_scope (n);
      }

      void FileScope::
      post (NodePtr const&)
      {
      }

      // TranslationRegion
      //
      //

      void TranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void TranslationRegion::
      pre (NodePtr const&)
      {
      }

      void TranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void TranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void TranslationRegion::
      post (NodePtr const&)
      {
      }

      // IncludeTranslationRegion
      //
      //

      void IncludeTranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void IncludeTranslationRegion::
      pre (NodePtr const&)
      {
      }

      void IncludeTranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void IncludeTranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void IncludeTranslationRegion::
      post (NodePtr const&)
      {
      }


      // UserIncludeTranslationRegion
      //
      //

      void UserIncludeTranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void UserIncludeTranslationRegion::
      pre (NodePtr const&)
      {
      }

      void UserIncludeTranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void UserIncludeTranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void UserIncludeTranslationRegion::
      post (NodePtr const&)
      {
      }


      // SysIncludeTranslationRegion
      //
      //

      void SysIncludeTranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void SysIncludeTranslationRegion::
      pre (NodePtr const&)
      {
      }

      void SysIncludeTranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void SysIncludeTranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void SysIncludeTranslationRegion::
      post (NodePtr const&)
      {
      }

      // ImpliedIncludeTranslationRegion
      //
      //

      void ImpliedIncludeTranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void ImpliedIncludeTranslationRegion::
      pre (NodePtr const&)
      {
      }

      void ImpliedIncludeTranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void ImpliedIncludeTranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void ImpliedIncludeTranslationRegion::
      post (NodePtr const&)
      {
      }


      // PrincipalTranslationRegion
      //
      //

      void PrincipalTranslationRegion::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          scope (n);
          post (n);
        }
      }

      void PrincipalTranslationRegion::
      pre (NodePtr const&)
      {
      }

      void PrincipalTranslationRegion::
      content (NodePtr const& n)
      {
        delegate_content (n);
      }

      void PrincipalTranslationRegion::
      scope (NodePtr const& n)
      {
        if (scope_ != 0) scope_->dispatch (n->scope ());
        else dispatch (n->scope ());
      }

      void PrincipalTranslationRegion::
      post (NodePtr const&)
      {
      }

      // TranslationUnit
      //
      //

      void TranslationUnit::
      traverse (NodePtr const& n)
      {
        if (!delegate (n))
        {
          pre (n);
          content (n);
          post (n);
        }
      }


      void TranslationUnit::
      pre (NodePtr const&)
      {
      }

      void TranslationUnit::
      content (NodePtr const& n)
      {
        //cerr << "starting traversal of translation unit content" << endl;
        delegate_content (n);
        //cerr << "finishing traversal of translation unit content" << endl;
      }

      void TranslationUnit::
      post (NodePtr const&)
      {
      }
    }
  }
}
