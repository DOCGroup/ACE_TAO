// file      : CCF/CIDL/Traversal/Composition.tpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

namespace CCF
{
  namespace CIDL
  {
    namespace Traversal
    {

      template <typename T>
      void CompositionTemplate<T>::
      traverse (T& c)
      {
        pre (c);
        name (c);
        names (c);
        post (c);
      }

      template <typename T>
      void CompositionTemplate<T>::
      pre (T&)
      {
      }

      template <typename T>
      void CompositionTemplate<T>::
      name (T&)
      {
      }

      template <typename T>
      void CompositionTemplate<T>::
      post (T&)
      {
      }
    }
  }
}
