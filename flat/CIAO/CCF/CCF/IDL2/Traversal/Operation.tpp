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
        belongs (p, this->edge_traverser ());
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


      // OperationTemplate
      //
      //
      template<typename T>
      void OperationTemplate<T>::
      traverse (T& o)
      {
        pre (o);
        returns (o);
        name (o);
        receives (o);
        raises (o);
        post (o);
      }

      template<typename T>
      void OperationTemplate<T>::
      pre (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      returns (T& o, EdgeDispatcherBase& d)
      {
        d.traverse (o.returns ());
      }

      template<typename T>
      void OperationTemplate<T>::
      returns (T& o)
      {
        returns (o, this->edge_traverser ());
      }

      template<typename T>
      void OperationTemplate<T>::
      name (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      receives (T& o, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (o.receives_begin (), o.receives_end (), d);
      }

      template<typename T>
      void OperationTemplate<T>::
      receives (T& o)
      {
        typename T::ReceivesIterator
          b (o.receives_begin ()), e (o.receives_end ());

        if (b != e)
        {
          receives_pre (o);
          iterate_and_traverse (
            b, e, this->edge_traverser (), *this, &OperationTemplate<T>::comma, o);
          receives_post (o);
        }
        else
        {
          receives_none (o);
        }
      }

      template<typename T>
      void OperationTemplate<T>::
      receives_pre (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      receives_post (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      receives_none (T&)
      {
      }


      template<typename T>
      void OperationTemplate<T>::
      raises (T& o, EdgeDispatcherBase& d)
      {
        iterate_and_traverse (o.raises_begin (), o.raises_end (), d);
      }

      template<typename T>
      void OperationTemplate<T>::
      raises (T& o)
      {
        typename T::RaisesIterator b (o.raises_begin ()), e (o.raises_end ());

        if (b != e)
        {
          raises_pre (o);
          iterate_and_traverse (
            b, e, this->edge_traverser (), *this, &OperationTemplate<T>::comma, o);
          raises_post (o);
        }
        else
        {
          raises_none (o);
        }
      }

      template<typename T>
      void OperationTemplate<T>::
      raises_pre (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      raises_post (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      raises_none (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      post (T&)
      {
      }

      template<typename T>
      void OperationTemplate<T>::
      comma (T&)
      {
      }
    }
  }
}
