// $Id$

#ifndef ACE_VECTOR_T_C
#define ACE_VECTOR_T_C

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector_T.h"

#if !defined (__ACE_INLINE__)
#include "ace/Vector_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Vector_T, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Vector)

template <class T, const size_t DEFAULT_SIZE>
void ACE_Vector<T, DEFAULT_SIZE>::resize (const size_t new_size,
					  const T& t)
{
  ACE_Array<T>::size (new_size);
  if (new_size > length_)
    for (size_t i = length_ - 1; i < new_size; ++i)
      (*this)[i]=t;

  curr_max_size_ = this->max_size ();
  length_ = new_size;
}

template <class T, const size_t DEFAULT_SIZE>
void ACE_Vector<T, DEFAULT_SIZE>::push_back (const T& elem)
{
  if (length_ == curr_max_size_)
    {
      ACE_Array<T>::size (curr_max_size_ * 2);
      curr_max_size_ = this->max_size ();
    }
  ++length_;
  (*this)[length_-1] = elem;
}

template <class T, const size_t DEFAULT_SIZE>
void ACE_Vector<T, DEFAULT_SIZE>::dump (void) const
{
  for (size_t i = 0; i < this->size (); ++i)
    (*this)[i].dump ();
}

#if 0
template<class T>
bool compare(const ACE_Vector<T>& v1,
	     const ACE_Vector<T>& v2,
             const size_t from_ndx,
	     const size_t to_ndx)
{
  size_t last1 = v1.size () - 1;
  size_t last2 = v2.size () - 1;
  if(last1 < from_ndx || last1 < to_ndx)
    {
      return false;
    }
  if (last2 < from_ndx || last2 < to_ndx)
    {
      return false;
    }
  if (last1 != last2)
    {
      return false;
    }

  //  cout<<"compare() <================="<<endl;
  for (size_t i = from_ndx; i <= to_ndx; ++i)
    {
      //     cout<<"V1["<<i<<"]="<<v1[i];
      //     cout<<", V2["<<i<<"]="<<v2[i];
      //     cout<<": NOT EQUAL == "<<(v1[i]!=v2[i])<<endl;
      if (v1[i] != v2[i])
	{
	  return false;
	}
    }
  //  cout<<"compare() ====================>"<<endl;
  return true;
}

template<class T>
bool partial_compare(const ACE_Vector<T>& v1,
		     const ACE_Vector<T>& v2,
                     const size_t from_ndx,
		     const size_t to_ndx)
{
  size_t last1 = v1.size () - 1;
  size_t last2 = v2.size () - 1;
  if (last1 < from_ndx || last1 < to_ndx)
    {
      return false;
    }
  if (last2 < from_ndx || last2 < to_ndx)
    {
      return false;
    }
  //  cout<<"partial_compare() <================="<<endl;
  for (size_t i = from_ndx; i <= to_ndx; ++i)
    {
      //     cout<<"V1["<<i<<"]="<<v1[i];
      //     cout<<", V2["<<i<<"]="<<v2[i];
      //     cout<<": NOT EQUAL == "<<(v1[i]!=v2[i])<<endl;
      if (v1[i] != v2[i])
	{
	  return false;
	}
    }
  //  cout<<"partial_compare() ====================>"<<endl;
  return true;
}
#endif

#endif /* ACE_VECTOR_T_C */

