// file      : CCF/IDL2/Traversal/Operation.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Operation.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Operation
      //
      //
      void Operation::
      traverse (Type& o)
      {
        pre (o);
        returns (o);
        name (o);
        receives (o);
        raises (o);
        post (o);
      }

      void Operation::
      pre (Type&)
      {
      }

      void Operation::
      returns (Type& o, EdgeDispatcherBase& d)
      {
        d.traverse (o.returns ());
      }

      void Operation::
      returns (Type& o)
      {
        returns (o, edge_traverser ());
      }

      void Operation::
      name (Type&)
      {
      }

      void Operation::
      receives (Type& o, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (o.receives_begin (), o.receives_end (), d);
      }

      void Operation::
      receives (Type& o)
      {
        receives_pre (o);
        iterate_and_traverse (o.receives_begin (),
                              o.receives_end (),
                              edge_traverser (),
                              *this,
                              &Operation::comma,
                              o);
        receives_post (o);
      }

      void Operation::
      receives_pre (Type&)
      {
      }

      void Operation::
      receives_post (Type&)
      {
      }

      void Operation::
      raises (Type& o, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (o.raises_begin (), o.raises_end (), d);
      }

      void Operation::
      raises (Type& o)
      {
        Type::RaisesIterator b (o.raises_begin ()), e (o.raises_end ());

        if (b != e)
        {
          raises_pre (o);
          iterate_and_traverse (
            b, e, edge_traverser (), *this, &Operation::comma, o);
          raises_post (o);
        }
        else
        {
          raises_none (o);
        }
      }

      void Operation::
      raises_pre (Type&)
      {
      }

      void Operation::
      raises_post (Type&)
      {
      }

      void Operation::
      raises_none (Type&)
      {
      }

      void Operation::
      post (Type&)
      {
      }

      void Operation::
      comma (Type&)
      {
      }
    }
  }
}
