// file      : CCF/IDL2/Traversal/ValueType.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      template <typename T>
      void ValueTypeTemplate<T>::
      traverse (T& i)
      {
        pre (i);
        name (i);
        inherits (i);
        supports (i);
        names (i);
        post (i);
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits (T& i, EdgeDispatcherBase&)
      {
        //@@ TODO
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits (T& i)
      {
        /*
        typename T::InheritsIterator
          b (i.inherits_begin ()), e (i.inherits_end ());

        if (b != e)
        {
          inherits_pre (i);
          iterate_and_dispatch (b, e, *this, &ValueTypeTemplate::comma, i);
          inherits_post (i);
        }
        */
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits_pre (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits_post (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits_none (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports (T& i, EdgeDispatcherBase&)
      {
        //@@ TODO
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports (T& i)
      {
        /*
        typename T::SupportsIterator
          b (i.supports_begin ()), e (i.supports_end ());

        if (b != e)
        {
          supports_pre (i);
          iterate_and_dispatch (b, e, *this, &ValueTypeTemplate::comma, i);
          supports_post (i);
        }
        */
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports_pre (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports_post (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports_none (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      post (T&)
      {
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      comma (T&)
      {
      }
    }
  }
}
