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
      inherits (T& i, EdgeDispatcherBase&)
      {
        //@@ TODO
      }

      template <typename T>
      void EventTypeTemplate<T>::
      inherits (T& i)
      {
        /*
        typename T::InheritsIterator
          b (i.inherits_begin ()), e (i.inherits_end ());

        if (b != e)
        {
          inherits_pre (i);
          iterate_and_dispatch (b, e, *this, &EventTypeTemplate::comma, i);
          inherits_post (i);
        }
        */
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
      supports (T& i, EdgeDispatcherBase&)
      {
        //@@ TODO
      }

      template <typename T>
      void EventTypeTemplate<T>::
      supports (T& i)
      {
        /*
        typename T::SupportsIterator
          b (i.supports_begin ()), e (i.supports_end ());

        if (b != e)
        {
          supports_pre (i);
          iterate_and_dispatch (b, e, *this, &EventTypeTemplate::comma, i);
          supports_post (i);
        }
        */
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
