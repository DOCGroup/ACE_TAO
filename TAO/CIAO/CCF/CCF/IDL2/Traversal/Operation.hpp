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
      struct Operation : Node<SemanticGraph::Operation>
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
    }
  }
}

#include "CCF/IDL2/Traversal/Operation.tpp"

#endif  // CCF_IDL2_TRAVERSAL_OPERATION_HPP
