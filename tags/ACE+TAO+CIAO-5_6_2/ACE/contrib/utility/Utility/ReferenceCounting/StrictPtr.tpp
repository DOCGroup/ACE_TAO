// file      : Utility/ReferenceCounting/StrictPtr.tpp
// author    : Boris Kolpackov <boris@kolpackov.net>
// copyright : Copyright (c) 2002-2003 Boris Kolpackov
// license   : http://kolpackov.net/license.html

namespace Utility
{
  namespace ReferenceCounting
  {
    // c-tor's & d-tor

    template <typename T>
    StrictPtr<T>::
    StrictPtr () throw ()
        : ptr_ (0)
    {
    }

    template <typename T>
    StrictPtr<T>::
    StrictPtr (Type* ptr) throw ()
        : ptr_ (ptr)
    {
    }

    template <typename T>
    StrictPtr<T>::
    StrictPtr (StrictPtr<Type> const& s_ptr)
        : ptr_ (add_ref (s_ptr.in ()))
    {
    }

    template <typename T>
    template <typename Other>
    StrictPtr<T>::
    StrictPtr (StrictPtr<Other> const& s_ptr)
        : ptr_ (add_ref (s_ptr.in ()))
    {
    }


    template <typename T>
    StrictPtr<T>::
    ~StrictPtr () throw ()
    {
      // This is an additional catch-all layer to protect from
      // non-conformant Type.
      try
      {
        if (ptr_ != 0) ptr_->remove_ref ();
      }
      catch (...)
      {
      }
    }

    // operator=

    template <typename T>
    StrictPtr<T>&
    StrictPtr<T>::operator= (Type* ptr) throw ()
    {
      if (ptr_ != 0) ptr_->remove_ref ();
      ptr_ = ptr;
      return *this;
    }

    template <typename T>
    StrictPtr<T>& StrictPtr<T>::
    operator= (StrictPtr<Type> const& s_ptr)
    {
      Type* old_ptr (ptr_);
      Type* new_ptr (add_ref (s_ptr.in ())); // this can throw
      if (old_ptr != 0) old_ptr->remove_ref ();

      ptr_ = new_ptr; // commit

      return *this;
    }


    template <typename T>
    template <typename Other>
    StrictPtr<T>& StrictPtr<T>::
    operator= (StrictPtr<Other> const& s_ptr)
    {
      Type*  old_ptr (ptr_);
      Other* new_ptr (add_ref (s_ptr.in ())); // this can throw
      if (old_ptr != 0) old_ptr->remove_ref ();

      ptr_ = new_ptr; // commit

      return *this;
    }

    // comparison

    template <typename T>
    bool StrictPtr<T>::
    operator== (Type* other) const throw ()
    {
      return ptr_ == other;
    }

    template <typename T>
    bool StrictPtr<T>::
    operator!= (Type* other) const throw ()
    {
      return ptr_ != other;
    }

    // accessors

    template <typename T>
    T* StrictPtr<T>::
    operator-> () const
    {
      if (ptr_ == 0)
      {
        throw NotInitialized(
          "Utility::ReferenceCounting::StrictPtr::operator-> : "
          "unable to dereference NULL pointer.");
      }
      return ptr_;
    }

    template <typename T>
    T* StrictPtr<T>::
    in () const throw ()
    {
      return ptr_;
    }

    template <typename T>
    T* StrictPtr<T>::
    retn() throw ()
    {
      Type* ret (ptr_);
      ptr_ = 0;
      return ret;
    }

    // Specialization of add_ref function for StrictPtr<T>
    template <typename T>
    T*
    add_ref (StrictPtr<T> const& ptr)
    {
      // delegate to generic implementation
      return add_ref (ptr.in ());
    }

    // Dynamic type conversion function for StrictPtr's
    template <typename D, typename S>
    StrictPtr<D>
    strict_cast (StrictPtr<S> const& s)
    {
      return StrictPtr<D>(add_ref (dynamic_cast<D*>(s.in ())));
    }
  }
}
//$Id$
