// file      : CCF/IDL2/Traversal/Translation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_TRANSLATION_HPP
#define CCF_IDL2_TRAVERSAL_TRANSLATION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Translation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Some edges.
      //
      //
      struct ContainsPrincipal : Edge<SemanticGraph::ContainsPrincipal>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };

      struct ContainsImplied : Edge<SemanticGraph::ContainsImplied>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };

      struct ContainsRoot : Edge<SemanticGraph::ContainsRoot>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };

      struct Includes : Edge<SemanticGraph::Includes>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };

      struct QuoteIncludes : Edge<SemanticGraph::QuoteIncludes>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };

      struct BracketIncludes : Edge<SemanticGraph::BracketIncludes>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.element ());
        }
      };


      //
      //
      //
      typedef
      ScopeTemplate <SemanticGraph::Root>
      Root;


      //
      //
      //
      struct TranslationRegion : Node<SemanticGraph::TranslationRegion>
      {
        virtual void
        traverse (SemanticGraph::TranslationRegion&);
      };


      //
      //
      //
      struct TranslationUnit : Node<SemanticGraph::TranslationUnit>
      {
        virtual void
        traverse (SemanticGraph::TranslationUnit&);
      };

    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_TRANSLATION_HPP
