// file      : CCF/IDL2/Traversal/Operation.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_OPERATION_HPP
#define CCF_IDL2_TRAVERSAL_OPERATION_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/SemanticGraph/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      //
      //
      //
      struct Receives : Edge<SemanticGraph::Receives>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.parameter ());
        }
      };


      //
      //
      //
      struct Returns : Edge<SemanticGraph::Returns>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.type ());
        }
      };

      //
      //
      //
      struct Raises : Edge<SemanticGraph::Raises>
      {
        virtual void
        traverse (Type& e)
        {
          node_traverser ().traverse (e.exception ());
        }
      };

      //
      //
      //
      template<typename T>
      struct ParameterTemplate : Node<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        belongs (T&, EdgeDispatcherBase&);

        virtual void
        belongs (T&);

        virtual void
        name (T&);

        virtual void
        post (T&);
      };

      typedef
      ParameterTemplate<SemanticGraph::Parameter>
      Parameter;

      typedef
      ParameterTemplate<SemanticGraph::InParameter>
      InParameter;

      typedef
      ParameterTemplate<SemanticGraph::InOutParameter>
      InOutParameter;

      typedef
      ParameterTemplate<SemanticGraph::OutParameter>
      OutParameter;

      //
      //
      //
      template <typename T>
      struct OperationTemplate : Node<T>
      {
        /* GCC#13590/DR#39
        using Node<T>::edge_traverser;
        */

        virtual void
        traverse (T&);

        virtual void
        pre (T&);

        virtual void
        returns (T&, EdgeDispatcherBase&);

        virtual void
        returns (T&);

        virtual void
        name (T&);

        virtual void
        receives (T&, EdgeDispatcherBase&);

        virtual void
        receives (T&);

        virtual void
        receives_pre (T&);

        virtual void
        receives_post (T&);

        virtual void
        receives_none (T&);

        virtual void
        raises (T&, EdgeDispatcherBase&);

        virtual void
        raises (T&);

        virtual void
        raises_pre (T&);

        virtual void
        raises_post (T&);

        virtual void
        raises_none (T&);

        virtual void
        post (T&);

        virtual void
        comma (T&);
      };

      typedef
      OperationTemplate<SemanticGraph::Operation>
      Operation;

      typedef
      OperationTemplate<SemanticGraph::OneWayOperation>
      OneWayOperation;

      typedef
      OperationTemplate<SemanticGraph::TwoWayOperation>
      TwoWayOperation;
    }
  }
}

#include "CCF/IDL2/Traversal/Operation.tpp"

#endif  // CCF_IDL2_TRAVERSAL_OPERATION_HPP
