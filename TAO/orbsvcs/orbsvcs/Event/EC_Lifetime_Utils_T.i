// $Id$

// Helper.
template <typename T>
ACE_INLINE typename T::_var_type
my_narrow_until_carlos_gets_jeff_to_fix_the_idl_compiler (
                                       T *,
                                       CORBA::Object_ptr obj
                                       ACE_ENV_ARG_DECL)
{
  return T::_narrow (obj ACE_ENV_ARG_PARAMETER);
}


//***************************************************************************

template <class X>
ACE_INLINE
TAO_EC_Auto_Command<X>::TAO_EC_Auto_Command (void)
  : command_ ()
  , allow_command_ (0)
{
}

template <class X>
ACE_INLINE
TAO_EC_Auto_Command<X>::TAO_EC_Auto_Command (const X & command)
  : command_ (command)
  , allow_command_ (1)
{
}

template <class X>
ACE_INLINE
TAO_EC_Auto_Command<X>::~TAO_EC_Auto_Command (void)
{
  this->execute ();
}

template <class X>
ACE_INLINE void
TAO_EC_Auto_Command<X>::set_command (const X & command)
{
  this->command_ = command;
  this->allow_command_ = 1;
}

template <class X>
ACE_INLINE void
TAO_EC_Auto_Command<X>::set_command (TAO_EC_Auto_Command<X> & auto_command)
{
  if (this == &auto_command)
    return;

  this->command_ = auto_command.command_;
  this->allow_command_ = auto_command.allow_command_;
  auto_command.allow_command_ = 0;
}

template <class X>
ACE_INLINE void
TAO_EC_Auto_Command<X>::execute (void)
{
  if (this->allow_command_)
    {
      this->allow_command_ = 0;

      ACE_TRY_NEW_ENV
        {
          this->command_.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // ignore.
        }
      ACE_ENDTRY;
    }
}

template <class X>
ACE_INLINE void
TAO_EC_Auto_Command<X>::allow_command (void)
{
  this->allow_command_ = 1;
}

template <class X>
ACE_INLINE void
TAO_EC_Auto_Command<X>::disallow_command (void)
{
  this->allow_command_ = 0;
}


//***************************************************************************

template <class TARGET>
ACE_INLINE
TAO_EC_Shutdown_Command<TARGET>::TAO_EC_Shutdown_Command (void)
  : target_ ()
{
}

template <class TARGET>
ACE_INLINE
TAO_EC_Shutdown_Command<TARGET>::TAO_EC_Shutdown_Command (TARGET target)
  : target_ (target)
{
}

template <class TARGET>
ACE_INLINE void
TAO_EC_Shutdown_Command<TARGET>::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->target_.in ())
    {
      this->target_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

//***************************************************************************


// Life would be much easier if _add_ref() and _remove_ref()
// had throw specs of "throw ()"

#include <algorithm>

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(T * p)
  : ptr_(p)
{
}

// If _add_ref throws, this object will not be completely constructed
// so the destructor will not be called.
template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(TAO_EC_Servant_Var<T> const & rhs)
  : ptr_(rhs.ptr_)
{
  if (ptr_)
  {
    ptr_->_add_ref();
  }
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(TAO_EC_Servant_Var<T> const & rhs)
{
  TAO_EC_Servant_Var<T> tmp(rhs);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(T * p)
{
  TAO_EC_Servant_Var<T> tmp(p);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
~TAO_EC_Servant_Var()
{
  // Unfortunately, there is no throw spec on _remove_ref, so we
  // can't assume that it will not throw.  If it does, then we are in
  // trouble.  In any event, we can't let the exception escape our
  // destructor.
  if (ptr_ != 0)
  {
    ACE_TRY_NEW_ENV
    {
      ptr_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHALL
    {
    }
    ACE_ENDTRY;
  }
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(Y * p)
  : ptr_(p)
{
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T>::
TAO_EC_Servant_Var(TAO_EC_Servant_Var<Y> const & rhs)
  : ptr_(rhs.in())
{
  if (ptr_)
  {
    ptr_->_add_ref();
  }
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(TAO_EC_Servant_Var<Y> const & rhs)
{
  TAO_EC_Servant_Var<T> tmp(rhs);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}

template <class T> template <class Y>
ACE_INLINE TAO_EC_Servant_Var<T> & TAO_EC_Servant_Var<T>::
operator=(Y * p)
{
  TAO_EC_Servant_Var<T> tmp(p);

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return *this;
}
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

template <class T>
ACE_INLINE T const * TAO_EC_Servant_Var<T>::
operator->() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
operator->()
{
  return ptr_;
}

template <class T>
ACE_INLINE T const & TAO_EC_Servant_Var<T>::
operator*() const
{
  return *ptr_;
}

template <class T>
ACE_INLINE T & TAO_EC_Servant_Var<T>::
operator*()
{
  return *ptr_;
}

template <class T>
ACE_INLINE TAO_EC_Servant_Var<T>::
operator void const * () const
{
  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
in() const
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO_EC_Servant_Var<T>::
inout()
{
  return ptr_;
}

template <class T>
ACE_INLINE T *& TAO_EC_Servant_Var<T>::
out()
{
  TAO_EC_Servant_Var<T> tmp;

  // std::swap(tmp.ptr_, ptr_);
  T * swap_temp = tmp.ptr_;
  tmp.ptr_ = ptr_;
  ptr_ = swap_temp;

  return ptr_;
}

template <class T>
ACE_INLINE T * TAO_EC_Servant_Var<T>::
_retn()
{
  T * rval = ptr_;
  ptr_ = 0;
  return rval;
}

template <class X, class Y>
ACE_INLINE bool
operator==(TAO_EC_Servant_Var<X> const & x,
           TAO_EC_Servant_Var<Y> const & y)
{
  return x.in() == y.in();
}

template <class X, class Y>
ACE_INLINE bool
operator!=(TAO_EC_Servant_Var<X> const & x,
           TAO_EC_Servant_Var<Y> const & y)
{
  return x.in() != y.in();
}
