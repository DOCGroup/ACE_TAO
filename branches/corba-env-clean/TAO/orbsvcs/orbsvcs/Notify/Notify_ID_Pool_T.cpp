// $Id$

#ifndef TAO_NOTIFY_ID_POOL_T_C
#define TAO_NOTIFY_ID_POOL_T_C

#include "Notify_ID_Pool_T.h"

#if !defined (__ACE_INLINE__)
#include "Notify_ID_Pool_T.i"
#endif /* __ACE_INLINE__ */

template <class ID_TYPE>
TAO_Notify_ID_Pool<ID_TYPE>::TAO_Notify_ID_Pool (void)
  : id_ (0)
{
  // No-Op.
}

template <class ID_TYPE>
TAO_Notify_ID_Pool<ID_TYPE>::~TAO_Notify_ID_Pool ()
{
  // No-Op.
}

template <class ID_TYPE> ID_TYPE
TAO_Notify_ID_Pool<ID_TYPE>::get (void)
{
  return ++this->id_;
}

template <class ID_TYPE> void
TAO_Notify_ID_Pool<ID_TYPE>::put (ID_TYPE id )
{
  this->active_list_.remove (id);
}

template <class ID_TYPE> void
TAO_Notify_ID_Pool<ID_TYPE>::next (void)
{
  this->active_list_.insert (this->id_);
}

template <class ID_TYPE, class ID_TYPE_SEQ>
TAO_Notify_ID_Pool_Ex<ID_TYPE, ID_TYPE_SEQ>::TAO_Notify_ID_Pool_Ex (void)
{
}

template <class ID_TYPE, class ID_TYPE_SEQ>
TAO_Notify_ID_Pool_Ex<ID_TYPE, ID_TYPE_SEQ>::~TAO_Notify_ID_Pool_Ex ()
{
}

template <class ID_TYPE, class ID_TYPE_SEQ> ID_TYPE_SEQ*
TAO_Notify_ID_Pool_Ex<ID_TYPE, ID_TYPE_SEQ>::get_sequence (TAO_ENV_SINGLE_ARG_DECL)
{
  // Figure out the length of the list.
  size_t len = this->active_list_.size ();

  ID_TYPE_SEQ* list;

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    ID_TYPE_SEQ (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  list->length (len);

  ACE_Unbounded_Set_Iterator<ID_TYPE> iter (this->active_list_);
  ID_TYPE* id_ret;

  int i = 0;
  for (iter.first (); iter.next (id_ret) == 1; iter.advance ())
    (*list)[i++] = *id_ret;

  return list;
}

#endif /* TAO_NOTIFY_ID_POOL_T_C */
