// file      : CCF/IDL2/Traversal/ValueType.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_VALUE_TYPE_HPP
#define CCF_IDL2_TRAVERSAL_VALUE_TYPE_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"
#include "CCF/IDL2/Traversal/Operation.hpp"
#include "CCF/IDL2/SemanticGraph/ValueType.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      template <typename T>
      struct ValueTypeTemplate : ScopeTemplate<T>
      {
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
        supports (T&, EdgeDispatcherBase& d);

        virtual void
        supports (T&);

        virtual void
        supports_pre (T&);

        virtual void
        supports_post (T&);

        virtual void
        supports_none (T&);

        virtual void
        post (T&);

        virtual void
        comma (T&);
      };


      typedef
      ValueTypeTemplate<SemanticGraph::ValueType>
      ValueType;

      typedef
      ValueTypeTemplate<SemanticGraph::AbstractValueType>
      AbstractValueType;

      typedef
      ValueTypeTemplate<SemanticGraph::ConcreteValueType>
      ConcreteValueType;

      typedef
      OperationTemplate<SemanticGraph::ValueTypeFactory>
      ValueTypeFactory;
    }
  }
}

#include "CCF/IDL2/Traversal/ValueType.tpp"

#endif  // CCF_IDL2_TRAVERSAL_VALUE_TYPE_HPP
