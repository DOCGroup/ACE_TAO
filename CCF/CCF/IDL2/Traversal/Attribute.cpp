// file      : CCF/IDL2/Traversal/Attribute.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include "CCF/IDL2/Traversal/Attribute.hpp"

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // Attribute
      //
      //
      void Attribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        post (a);
      }

      void Attribute::
      pre (Type&)
      {
      }

      void Attribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void Attribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void Attribute::
      name (Type&)
      {
      }

      void Attribute::
      post (Type&)
      {
      }

      // ReadAttribute
      //
      //
      void ReadAttribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        get_raises (a);
        post (a);
      }

      void ReadAttribute::
      pre (Type&)
      {
      }

      void ReadAttribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void ReadAttribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void ReadAttribute::
      name (Type&)
      {
      }

      void ReadAttribute::
      get_raises (Type& a, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (a.get_raises_begin (), a.get_raises_end (), d);
      }

      void ReadAttribute::
      get_raises (Type& a)
      {
        Type::GetRaisesIterator
          b (a.get_raises_begin ()),
          e (a.get_raises_end ());

        if (b != e)
        {
          get_raises_pre (a);
          iterate_and_traverse (b,
                                e,
                                edge_traverser (),
                                *this,
                                &ReadAttribute::comma,
                                a);
          get_raises_post (a);
        }
        else
        {
          get_raises_none (a);
        }
      }

      void ReadAttribute::
      get_raises_pre (Type&)
      {
      }

      void ReadAttribute::
      get_raises_post (Type&)
      {
      }

      void ReadAttribute::
      get_raises_none (Type&)
      {
      }

      void ReadAttribute::
      post (Type&)
      {
      }

      void ReadAttribute::
      comma (Type&)
      {
      }


      // ReadWriteAttribute
      //
      //
      void ReadWriteAttribute::
      traverse (Type& a)
      {
        pre (a);
        belongs (a);
        name (a);
        get_raises (a);
        set_raises (a);
        post (a);
      }

      void ReadWriteAttribute::
      pre (Type&)
      {
      }

      void ReadWriteAttribute::
      belongs (Type& a, EdgeDispatcherBase& d)
      {
        d.traverse (a.belongs ());
      }

      void ReadWriteAttribute::
      belongs (Type& a)
      {
        belongs (a, edge_traverser ());
      }

      void ReadWriteAttribute::
      name (Type&)
      {
      }

      void ReadWriteAttribute::
      get_raises (Type& a, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (a.get_raises_begin (), a.get_raises_end (), d);
      }

      void ReadWriteAttribute::
      get_raises (Type& a)
      {
        Type::GetRaisesIterator
          b (a.get_raises_begin ()),
          e (a.get_raises_end ());

        if (b != e)
        {
          get_raises_pre (a);
          iterate_and_traverse (b,
                                e,
                                edge_traverser (),
                                *this,
                                &ReadWriteAttribute::comma,
                                a);
          get_raises_post (a);
        }
        else
        {
          get_raises_none (a);
        }
      }

      void ReadWriteAttribute::
      get_raises_pre (Type&)
      {
      }

      void ReadWriteAttribute::
      get_raises_post (Type&)
      {
      }

      void ReadWriteAttribute::
      get_raises_none (Type&)
      {
      }

      void ReadWriteAttribute::
      set_raises (Type& a, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (a.set_raises_begin (), a.set_raises_end (), d);
      }

      void ReadWriteAttribute::
      set_raises (Type& a)
      {
        Type::SetRaisesIterator
          b (a.set_raises_begin ()),
          e (a.set_raises_end ());

        if (b != e)
        {
          set_raises_pre (a);
          iterate_and_traverse (b,
                                e,
                                edge_traverser (),
                                *this,
                                &ReadWriteAttribute::comma,
                                a);
          set_raises_post (a);
        }
        else
        {
          set_raises_none (a);
        }
      }

      void ReadWriteAttribute::
      set_raises_pre (Type&)
      {
      }

      void ReadWriteAttribute::
      set_raises_post (Type&)
      {
      }

      void ReadWriteAttribute::
      set_raises_none (Type&)
      {
      }

      void ReadWriteAttribute::
      post (Type&)
      {
      }

      void ReadWriteAttribute::
      comma (Type&)
      {
      }
    }
  }
}
