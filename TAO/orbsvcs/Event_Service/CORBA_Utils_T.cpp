// $Id$

#if !defined (ACE_CORBA_UTILS_C)
#define ACE_CORBA_UTILS_C

#include "CORBA_Utils_T.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "CORBA_Utils_T.i"
#endif /* __ACE_INLINE__ */

template <class TYPE>
ACE_CORBA_Sequence<TYPE>::ACE_CORBA_Sequence (const ACE_CORBA_Sequence<TYPE> &source)
  : maximum_ (0),
    length_ (0),
    buffer_ (0),
    release_ (0)
{
  (*this) = source;
}

template <class TYPE>
ACE_CORBA_Sequence<TYPE>::ACE_CORBA_Sequence (void)
  : maximum_ (0),
    length_ (0),
    buffer_ (0),
    release_ (0)
{
}

template <class TYPE>
ACE_CORBA_Sequence<TYPE>::~ACE_CORBA_Sequence (void)
{
  if (release_)
    {
      this->delete_array (buffer_, maximum_);
    }
}

template <class TYPE>
ACE_CORBA_Sequence<TYPE>::ACE_CORBA_Sequence (CORBA::ULong max)
  : maximum_ (max),
    length_ (0),
    buffer_ (0),

    release_ (0)
{
  if (maximum_ > 0)
    {
      buffer_ = this->new_array (maximum_);
      release_ = 1;
    }
}

template <class TYPE> TYPE *
ACE_CORBA_Sequence<TYPE>::new_array (size_t len)
{
  return new TYPE[len];
}

template <class TYPE> void
ACE_CORBA_Sequence<TYPE>::delete_array (TYPE *buf, size_t)
{
  delete [] buf;
}

template <class TYPE>
ACE_CORBA_Sequence<TYPE>::ACE_CORBA_Sequence (CORBA::ULong max, 
					      CORBA::ULong length, 
					      TYPE* data,
					      CORBA::Boolean release)
  : maximum_ (max),
    length_ (length),
    buffer_ (data),
    release_ (release)
{
  if ((buffer_ == 0) && (max > 0))
    {
      // @@ What should we do here?
      errno = ENOMEM;
      ACE_ERROR ((LM_ERROR, "No memory.\n"));
    }
}

// @@ This makes a deep copy, dig?
template <class TYPE> ACE_CORBA_Sequence<TYPE> &
ACE_CORBA_Sequence<TYPE>::operator= (const ACE_CORBA_Sequence<TYPE> &source)
{
  if (source.length () > 0)
    {
      // If our buffer is too small, release it and allocate one just big
      // enough.  If buffer_ == 0, this works fine.
      if (this->maximum () < source.length ())
	{
	  if (release_)
	    {
	      this->delete_array (buffer_, maximum_);
	    }
	  maximum_ = source.length ();
	  buffer_ = this->new_array (maximum_);
	  release_ = 1;
	}

      // Copy each of the items from the source.
      for (CORBA::ULong index=0; index < source.length (); index++)
	(*this)[index] = source[index];

      this->length (source.length ());
    }

  return *this;
}

template <class TYPE> void
ACE_CORBA_Sequence<TYPE>::length (CORBA::ULong len)
{
  if (len > maximum_)
    {
      // Allocate the space that we need.
      TYPE* tmp = this->new_array (len);
      // Copy over the old sequence.
      for (CORBA::ULong i = 0; i < maximum_; ++i)
	{
	  tmp[i] = buffer_[i];
	}
      if (release_)
	{
	  this->delete_array (buffer_, maximum_);
	}
      buffer_ = tmp;
      release_ = 1;
      maximum_ = len;
      // The destructor of -old- will release the old buffer_ if
      // necessary.
    }

  length_ = len;
}


// g++ can't handle these operator [] functions if they're inline
template <class TYPE> TYPE&
ACE_CORBA_Sequence<TYPE>::operator [] (CORBA::ULong i)
{
  // @@ Should we do bounds checking?
  if (i >= maximum_)
    {
      ACE_ERROR ((LM_ERROR, "Trying to write past maximum.\n"));
      return buffer_[maximum_ - 1];
    }
  else
    return buffer_[i];
}

template <class TYPE> const TYPE&
ACE_CORBA_Sequence<TYPE>::operator [] (CORBA::ULong i) const
{
  if (i >= length_)
    {
      ACE_ERROR ((LM_ERROR, "Trying to read past length.\n"));
      return buffer_[length_ - 1];
    }
    
  return buffer_[i];
}

// ********************

/*
template <class TYPE> void 
dump (const ACE_CORBA_Sequence<TYPE> &seq)
{
  for (CORBA::ULong index=0; index < seq.length (); index++)
    dump (seq[index]);
}
*/

/*
template <class TYPE> ACE_INLINE void
operator += (ACE_CORBA_Sequence<TYPE> &seq,
	     const TYPE &item)
{
  CORBA::ULong length = seq.length ();
  seq.length (length + 1);
  seq[length] = item;
}
*/

template <class TYPE> ACE_INLINE void
operator += (ACE_CORBA_Sequence<TYPE> &dest,
	     const ACE_CORBA_Sequence<TYPE> &source)
{
  int old_length = dest.length ();
  int new_length = old_length + source.length ();
  dest.length (new_length);

  int difference = new_length - old_length;
  
  for (int x=0; x < difference; x++)
    dest[old_length + x] = source[x];
}

/*
template <class TYPE> ACE_INLINE void
remove_item (TYPE &seq, CORBA::ULong index)
{
  int new_length = seq.length () - 1;

  // Shift the set back one, starting at <index>.
  for (int x = index; x < new_length; x++)
    seq[x] = seq[x+1];

  // Set the new length.
  seq.length (new_length);
}
*/

// ************************************************************
// ************************************************************
// ************************************************************

template<class TYPE>
ACE_CORBA_var<TYPE>::ACE_CORBA_var (const ACE_CORBA_var<TYPE> &source)
{
  if (source.me_ != 0)
    me_ = (TYPE *) source.me_->_duplicate ();
  // CORBA::duplicate (source.me_);
}

template<class TYPE>  
ACE_CORBA_var<TYPE>::ACE_CORBA_var (void)
  : me_ (0)
{
}

template<class TYPE>
ACE_CORBA_var<TYPE>::ACE_CORBA_var (TYPE *source)
{
  me_ = (TYPE *) source->_duplicate ();
  // CORBA::duplicate (source);
}

template<class TYPE>
ACE_CORBA_var<TYPE>::~ACE_CORBA_var (void)
{
  if (me_ != 0)
    me_->_release ();
  //CORBA::release (me_);
}

template<class TYPE> ACE_CORBA_var<TYPE> &
ACE_CORBA_var<TYPE>::operator= (TYPE *source)
{
  if (me_ != source &&
      me_ != 0)
    me_->_release ();
  //CORBA::release (me_);

  me_ = (TYPE *) source->_duplicate ();
  // CORBA::duplicate (source);
  return *this;
}

template<class TYPE> ACE_CORBA_var<TYPE> & 
ACE_CORBA_var<TYPE>::operator= (const ACE_CORBA_var<TYPE> &source)
{
  if (me_ != source.me_ &&
      me_ != 0)
    me_->_release ();
  // CORBA::release (me_);

  me_ = (TYPE *) source.me_->_duplicate ();
  // CORBA::duplicate (source.me_);
  return *this;
}

template<class TYPE> TYPE *
ACE_CORBA_var<TYPE>::operator->(void)
{
  return me_;
}

template<class TYPE>
ACE_CORBA_var<TYPE>::operator TYPE *(void) const
{
  return me_;
}

/*
template<class TYPE>
ACE_CORBA_var<TYPE>::operator TYPE *&(void)
{
  return me_;
}
*/

template<class TYPE>
ACE_CORBA_var<TYPE>::operator TYPE &(void) const
{
  return *me_;
}

// ************************************************************
// ************************************************************

/*
ACE_CORBA_Object_Ref<IMPL>::ACE_CORBA_Object_Ref (void)
  : impl_ (0)
{
}

ACE_CORBA_Object_Ref<IMPL>::ACE_CORBA_Object_Ref (IMPL *impl)
  : impl_ (impl)
{
}

ACE_CORBA_Object_Ref<IMPL>::~ACE_CORBA_Object_Ref (void)
  : impl_ (0)
{
}

ACE_CORBA_Object_Ref<IMPL> &
ACE_CORBA_Object_Ref<IMPL>::operator= (const ACE_CORBA_Object_Ref<IMPL> &source)
{
  return *this;
}

IMPL *
ACE_CORBA_Object_Ref<IMPL>::operator->(void)
{
  return impl_;
}

IMPL *
ACE_CORBA_Object_Ref<IMPL>::operator IMPL *(void)
{
  return impl_;
}

ACE_CORBA_Object_Ref<IMPL> *
ACE_CORBA_Object_Ref<IMPL>::_duplicate (ACE_CORBA_Object_Ref<IMPL> *source)
{
  source->ref_count_++;
  return source;
}

*/
#endif /* ACE_CORBA_UTILS_C */
