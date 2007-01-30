// file      : CCF/IDL2/Traversal/String.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/String.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // BoundedString
      //

      void BoundedString::
      traverse (Type& s)
      {
        pre (s);
        arguments_with_value (s);
        name (s);
        post (s);
      }

      void BoundedString::
      pre (Type&)
      {
      }

      void BoundedString::
      arguments_with_value (Type& s, EdgeDispatcherBase& d)
      {
        d.traverse (**s.arguments_begin ());
      }

      void BoundedString::
      arguments_with_value (Type& s)
      {
        arguments_with_value (s, edge_traverser ());
      }

      void BoundedString::
      name (Type&)
      {
      }

      void BoundedString::
      post (Type&)
      {
      }


      // BoundedWideString
      //

      void BoundedWideString::
      traverse (Type& s)
      {
        pre (s);
        arguments_with_value (s);
        name (s);
        post (s);
      }

      void BoundedWideString::
      pre (Type&)
      {
      }

      void BoundedWideString::
      arguments_with_value (Type& s, EdgeDispatcherBase& d)
      {
        d.traverse (**s.arguments_begin ());
      }

      void BoundedWideString::
      arguments_with_value (Type& s)
      {
        arguments_with_value (s, edge_traverser ());
      }

      void BoundedWideString::
      name (Type&)
      {
      }

      void BoundedWideString::
      post (Type&)
      {
      }
    }
  }
}
