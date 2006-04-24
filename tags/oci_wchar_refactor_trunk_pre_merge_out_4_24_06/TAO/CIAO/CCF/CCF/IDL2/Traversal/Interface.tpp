// file      : CCF/IDL2/Traversal/Interface.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      template <typename T>
      void InterfaceTemplate<T>::
      traverse (T& i)
      {
        pre (i);
        name (i);
        inherits (i);
        names (i);
        post (i);
      }

      template <typename T>
      void InterfaceTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      inherits (T& i, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (i.inherits_begin (),
                              i.inherits_end (),
                              d);
      }

      template <typename T>
      void InterfaceTemplate<T>::
      inherits (T& i)
      {
        typename T::InheritsIterator
          b (i.inherits_begin ()), e (i.inherits_end ());

        if (b != e)
        {
          inherits_pre (i);
          iterate_and_traverse (b,
                                e,
                                this->edge_traverser (),
                                *this,
                                &InterfaceTemplate::comma,
                                i);
          inherits_post (i);
        }
        else
        {
          inherits_none (i);
        }

      }

      template <typename T>
      void InterfaceTemplate<T>::
      inherits_pre (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      inherits_post (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      inherits_none (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      post (T&)
      {
      }

      template <typename T>
      void InterfaceTemplate<T>::
      comma (T&)
      {
      }
    }
  }
}
