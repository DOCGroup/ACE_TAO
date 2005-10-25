// file      : CCF/IDL2/Traversal/String.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_IDL2_TRAVERSAL_STRING_HPP
#define CCF_IDL2_TRAVERSAL_STRING_HPP

#include "CCF/IDL2/Traversal/Elements.hpp"

#include "CCF/IDL2/SemanticGraph/String.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      struct BoundedString : Node<SemanticGraph::BoundedString>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        arguments_with_value (Type&, EdgeDispatcherBase&);

        virtual void
        arguments_with_value (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };


      struct BoundedWideString : Node<SemanticGraph::BoundedWideString>
      {
        virtual void
        traverse (Type&);

        virtual void
        pre (Type&);

        virtual void
        arguments_with_value (Type&, EdgeDispatcherBase&);

        virtual void
        arguments_with_value (Type&);

        virtual void
        name (Type&);

        virtual void
        post (Type&);
      };
    }
  }
}

#endif  // CCF_IDL2_TRAVERSAL_STRING_HPP
