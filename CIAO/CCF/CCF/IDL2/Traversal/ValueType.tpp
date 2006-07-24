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
      traverse (T& v)
      {
        pre (v);
        name (v);
        inherits (v);
        supports (v);
        names (v);
        post (v);
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
      inherits (T& v, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (v.inherits_begin (),
                              v.inherits_end (),
                              d);
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      inherits (T& v)
      {
        typename T::InheritsIterator
          b (v.inherits_begin ()), e (v.inherits_end ());

        if (b != e)
        {
          inherits_pre (v);
          iterate_and_traverse (b,
                                e,
                                this->edge_traverser (),
                                *this,
                                &ValueTypeTemplate::comma,
                                v);
          inherits_post (v);
        }
        else
        {
          inherits_none (v);
        }
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
      supports (T& v, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (v.supports_begin (),
                              v.supports_end (),
                              d);
      }

      template <typename T>
      void ValueTypeTemplate<T>::
      supports (T& v)
      {
        typename T::SupportsIterator
          b (v.supports_begin ()), e (v.supports_end ());

        if (b != e)
        {
          supports_pre (v);
          iterate_and_traverse (b,
                                e,
                                this->edge_traverser (),
                                *this,
                                &ValueTypeTemplate::comma,
                                v);
          supports_post (v);
        }
        else
        {
          supports_none (v);
        }
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
