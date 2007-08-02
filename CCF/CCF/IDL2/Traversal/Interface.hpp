// file      : CCF/IDL2/Traversal/Interface.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_INTERFACE_HPP
#define CCF_IDL2_TRAVERSAL_INTERFACE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/Interface.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct Supports : Edge<SemanticGraph::Supports>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.supportee ());
        }
      };


      template <typename T>
      struct InterfaceTemplate : ScopeTemplate<T>
      {
        /* GCC#13590/DR#39
        using ScopeTemplate<T>::edge_traverser;
        */

        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        name (T&);

        virtual void
        inherits (T&, EdgeDispatcherBase& d);

        virtual void
        inherits (T&);

        virtual void
        inherits_pre (T&);

        virtual void
        inherits_post (T&);

        virtual void
        inherits_none (T&);

        virtual void
        post (T&);

        virtual void
        comma (T&);
      };


      typedef
      InterfaceTemplate<SemanticGraph::Interface>
      Interface;


      typedef
      InterfaceTemplate<SemanticGraph::AbstractInterface>
      AbstractInterface;

      typedef
      InterfaceTemplate<SemanticGraph::LocalInterface>
      LocalInterface;

      typedef
      InterfaceTemplate<SemanticGraph::UnconstrainedInterface>
      UnconstrainedInterface;
    }
  }
}

#include "CCF/IDL2/Traversal/Interface.tpp"

#endif  // CCF_IDL2_TRAVERSAL_INTERFACE_HPP
