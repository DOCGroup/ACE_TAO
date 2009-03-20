// file      : CCF/IDL2/Traversal/Array.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Array.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      void Array::
      traverse (Type& a)
      {
        pre (a);
        arguments_with_type (a);
        arguments_with_value (a);
        name (a);
        post (a);
      }

      void Array::
      pre (Type&)
      {
      }

      void Array::
      arguments_with_type (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (**a.arguments_begin ());
      }

      void Array::
      arguments_with_type (Type& a)
      {
        arguments_with_type (a, edge_traverser ());
      }

      void Array::
      arguments_with_value (Type& a, EdgeDispatcherBase& d)
      {
        Type::ArgumentsIterator
          b (a.arguments_begin ()), e (a.arguments_end ());

        ++b; // Bounds start afterthe type.

        iterate_and_traverse (b,
                              e,
                              d,
                              *this,
                              &Array::comma,
                              a);
      }

      void Array::
      arguments_with_value (Type& a)
      {
        arguments_with_value (a, edge_traverser ());
      }

      void Array::
      comma (Type&)
      {
      }

      void Array::
      name (Type&)
      {
      }

      void Array::
      post (Type&)
      {
      }
    }
  }
}
