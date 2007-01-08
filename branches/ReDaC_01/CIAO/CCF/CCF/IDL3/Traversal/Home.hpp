// file      : CCF/IDL3/Traversal/Home.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL3_TRAVERSAL_HOME_HPP
#define CCF_IDL3_TRAVERSAL_HOME_HPP

#include "CCF/IDL3/SemanticGraph/Home.hpp"

#include "CCF/IDL2/Traversal/Operation.hpp"
#include "CCF/IDL3/Traversal/Elements.hpp"

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {

      //
      //
      //
      struct Manages : Edge<SemanticGraph::Manages>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.managee ());
        }
      };

      //
      //
      //
      struct Home : ScopeTemplate<SemanticGraph::Home>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        name (Type&);

        virtual void
        inherits (Type&, EdgeDispatcherBase&);

        virtual void
        inherits (Type&);

        virtual void
        inherits_pre (Type&);

        virtual void
        inherits_post (Type&);

        virtual void
        inherits_none (Type&);

        virtual void
        supports (Type&, EdgeDispatcherBase&);

        virtual void
        supports (Type&);

        virtual void
        supports_pre (Type&);

        virtual void
        supports_post (Type&);

        virtual void
        supports_none (Type&);

        virtual void
        manages (Type&, EdgeDispatcherBase&);

        virtual void
        manages (Type&);

        virtual void
        manages_pre (Type&);

        virtual void
        manages_post (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };


      //
      //
      //
      /*
      struct HomeFactory : Node<SemanticGraph::HomeFactory>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        returns (Type&, EdgeDispatcherBase&);

        virtual void
        returns (Type&);

        virtual void
        name (Type&);

        virtual void
        receives (Type&, EdgeDispatcherBase&);

        virtual void
        receives (Type&);

        virtual void
        receives_pre (Type&);

        virtual void
        receives_post (Type&);

        virtual void
        raises (Type&, EdgeDispatcherBase&);

        virtual void
        raises (Type&);

        virtual void
        raises_pre (Type&);

        virtual void
        raises_post (Type&);

        virtual void
        raises_none (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };
      */

      typedef
      OperationTemplate<SemanticGraph::HomeFactory>
      HomeFactory;

      //
      //
      //
      /*
      struct HomeFinder : Node<SemanticGraph::HomeFinder>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        returns (Type&, EdgeDispatcherBase&);

        virtual void
        returns (Type&);

        virtual void
        name (Type&);

        virtual void
        receives (Type&, EdgeDispatcherBase&);

        virtual void
        receives (Type&);

        virtual void
        receives_pre (Type&);

        virtual void
        receives_post (Type&);

        virtual void
        raises (Type&, EdgeDispatcherBase&);

        virtual void
        raises (Type&);

        virtual void
        raises_pre (Type&);

        virtual void
        raises_post (Type&);

        virtual void
        raises_none (Type&);

        virtual void
        post (Type&);

        virtual void
        comma (Type&);
      };
      */

      typedef
      OperationTemplate<SemanticGraph::HomeFinder>
      HomeFinder;
    }
  }
}

#endif  // CCF_IDL3_TRAVERSAL_HOME_HPP
