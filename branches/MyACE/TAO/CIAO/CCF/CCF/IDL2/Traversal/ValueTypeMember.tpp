// file      : CCF/IDL2/Traversal/ValueTypeMember.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      template <typename T>
      void ValueTypeMemberTemplate<T>::
      traverse (T& m)
      {
        pre (m);
        belongs (m);
        name (m);
        post (m);
      }

      template <typename T>
      void ValueTypeMemberTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void ValueTypeMemberTemplate<T>::
      belongs (T& m, EdgeDispatcherBase& d)
      {
        d.traverse (m.belongs ());
      }

      template <typename T>
      void ValueTypeMemberTemplate<T>::
      belongs (T& m)
      {
        belongs (m, edge_traverser ());
      }

      template <typename T>
      void ValueTypeMemberTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void ValueTypeMemberTemplate<T>::
      post (T&)
      {
      }
    }
  }
}
