// file      : CCF/IDL3/Traversal/EventType.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      template <typename T>
      void EventTypeTemplate<T>::
      traverse (T& et)
      {
        pre (et);
        name (et);
        inherits (et);
        supports (et);
        names (et);
        post (et);
      }

      template <typename T>
      void EventTypeTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits (T& et, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (et.inherits_begin (),
                              et.inherits_end (),
                              d);

      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits (T& et)
      {
        typename T::InheritsIterator
          b (et.inherits_begin ()), e (et.inherits_end ());

        if (b != e)
        {
          inherits_pre (et);
          iterate_and_traverse (b,
                                e,
                                this->edge_traverser (),
                                *this,
                                &EventTypeTemplate::comma,
                                et);
          inherits_post (et);
        }
        else
        {
          inherits_none (et);
        }
      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits_pre (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits_post (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits_none (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports (T& et, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (et.supports_begin (),
                              et.supports_end (),
                              d);
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports (T& et)
      {
        typename T::SupportsIterator
          b (et.supports_begin ()), e (et.supports_end ());

        if (b != e)
        {
          supports_pre (et);
          iterate_and_traverse (b,
                                e,
                                this->edge_traverser (),
                                *this,
                                &EventTypeTemplate::comma,
                                et);
          supports_post (et);
        }
        else
        {
          supports_none (et);
        }
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports_pre (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports_post (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports_none (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      post (T&)
      {
      }

      template <typename T>
      void EventTypeTemplate<T>::
      comma (T&)
      {
      }
    }
  }
}
