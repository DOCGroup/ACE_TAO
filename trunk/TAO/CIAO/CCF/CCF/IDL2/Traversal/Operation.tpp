// file      : CCF/IDL2/Traversal/Operation.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace IDL2
  {
    namespace Traversal
    {
      // ParameterTemplate
      //
      //
      template<typename T>
      void ParameterTemplate<T>::
      traverse (T& p)
      {
        pre (p);
        belongs (p);
        name (p);
        post (p);
      }

      template<typename T>
      void ParameterTemplate<T>::
      pre (T&)
      {
      }

      template<typename T>
      void ParameterTemplate<T>::
      belongs (T& p, EdgeDispatcherBase& d)
      {
        d.traverse (p.belongs ());
      }
      
      template<typename T>
      void ParameterTemplate<T>::
      belongs (T& p)
      {
        belongs (p, edge_traverser ());
      }

      template<typename T>
      void ParameterTemplate<T>::
      name (T&)
      {
      }

      template<typename T>
      void ParameterTemplate<T>::
      post (T&)
      {
      }
    }
  }
}
