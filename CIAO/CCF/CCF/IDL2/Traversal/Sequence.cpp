// file      : CCF/IDL2/Traversal/Sequence.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Sequence.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {

      // UnboundedSequence
      //

      void UnboundedSequence::
      traverse (Type& us)
      {
        pre (us);
        arguments_with_type (us);
        name (us);
        post (us);
      }

      void UnboundedSequence::
      pre (Type&)
      {
      }

      void UnboundedSequence::
      arguments_with_type (Type& us, EdgeDispatcherBase& d)
      {
        d.traverse (**us.arguments_begin ());
      }

      void UnboundedSequence::
      arguments_with_type (Type& us)
      {
        arguments_with_type (us, edge_traverser ());
      }

      void UnboundedSequence::
      name (Type&)
      {
      }

      void UnboundedSequence::
      post (Type&)
      {
      }


      // BoundedSequence
      //

      void BoundedSequence::
      traverse (Type& bs)
      {
        pre (bs);
        arguments_with_type (bs);
        arguments_with_value (bs);
        name (bs);
        post (bs);
      }

      void BoundedSequence::
      pre (Type&)
      {
      }

      void BoundedSequence::
      arguments_with_type (Type& bs, EdgeDispatcherBase& d)
      {
        d.traverse (**bs.arguments_begin ());
      }

      void BoundedSequence::
      arguments_with_type (Type& bs)
      {
        arguments_with_type (bs, edge_traverser ());
      }

      void BoundedSequence::
      arguments_with_value (Type& bs, EdgeDispatcherBase& d)
      {
        Type::ArgumentsIterator i (bs.arguments_begin ());

        ++i; // Bound is always second to the type.

        d.traverse (**i);
      }

      void BoundedSequence::
      arguments_with_value (Type& bs)
      {
        arguments_with_value (bs, edge_traverser ());
      }

      void BoundedSequence::
      name (Type&)
      {
      }

      void BoundedSequence::
      post (Type&)
      {
      }
    }
  }
}
