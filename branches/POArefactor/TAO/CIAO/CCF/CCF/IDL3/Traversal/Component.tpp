// file      : CCF/IDL3/Traversal/Component.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL3
  {
    namespace Traversal
    {
      //
      //
      //
      template <typename T>
      void PortTemplate<T>::
      traverse (T& p)
      {
        this->edge_traverser ().traverse (p.belongs ());
      }


      // PortAccessorTemplate
      //
      //
      template <typename T>
      void PortAccessorTemplate<T>::
      traverse (T& p)
      {
        pre (p);
        returns (p);
        name (p);
        receives (p);
        raises (p);
        post (p);
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      returns (T& p, EdgeDispatcherBase& d)
      {
        d.traverse (p.belongs ());
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      returns (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      receives (T& p, EdgeDispatcherBase& d)
      {
        // @@ Still thinking about the way of making
        //    fake parameter.
        //
        d.traverse (p.belongs ());
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      receives (T& p)
      {
        receives_pre (p);
        receives_post (p);

      }

      template <typename T>
      void PortAccessorTemplate<T>::
      receives_pre (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      receives_post (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      raises (T&, EdgeDispatcherBase&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      raises (T& p)
      {
        raises_pre (p);
        raises_post (p);
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      raises_pre (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      raises_post (T&)
      {
      }

      template <typename T>
      void PortAccessorTemplate<T>::
      post (T&)
      {
      }

      // PortGetTemplate
      //
      //
      template <typename T>
      void PortGetTemplate<T>::
      returns (T& p)
      {
        PortAccessorTemplate<T>::returns (p, this->edge_traverser ());
      }


      // PortSetTemplate
      //
      //
      template <typename T>
      void PortSetTemplate<T>::
      receives (T& p)
      {
        receives_pre (p);
        PortAccessorTemplate<T>::receives (p, this->edge_traverser ());
        receives_post (p);
      }


      // PortDataTemplate
      //
      //
      template <typename T>
      void PortDataTemplate<T>::
      traverse (T& p)
      {
        pre (p);
        belongs (p);
        name (p);
        post (p);
      }

      template <typename T>
      void PortDataTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void PortDataTemplate<T>::
      belongs (T& p, EdgeDispatcherBase& d)
      {
        d.traverse (p.belongs ());
      }

      template <typename T>
      void PortDataTemplate<T>::
      belongs (T& p)
      {
        belongs (p, this->edge_traverser ());
      }

      template <typename T>
      void PortDataTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void PortDataTemplate<T>::
      post (T&)
      {
      }
    }
  }
}
