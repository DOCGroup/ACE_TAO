#if !defined (ACE_FREE_LIST_C)
#define ACE_FREE_LIST_C

#include "ace/Free_List.h"

#if !defined (__ACE_INLINE__)
#include "ace/Free_List.i"
#endif /* __ACE_INLINE__ */

// Empty constructor

template <class T>  
ACE_Free_List<T>::~ACE_Free_List (void)
{
  // Nothing
}

// Default constructor that takes in a preallocation number
// (<prealloc>), a low and high water mark (<lwm> and <hwm>) and an
// increment value (<inc>)

template <class T, class ACE_LOCK>  
ACE_Locked_Free_List<T, ACE_LOCK>::ACE_Locked_Free_List (int mode,
						     size_t prealloc, 
                                                     size_t lwm, 
                                                     size_t hwm, 
                                                     size_t inc)
  : mode_ (mode),
    free_list_ (NULL),
    lwm_ (lwm),
    hwm_ (hwm),
    inc_ (inc),
    size_ (0)
{
  this->alloc (prealloc);
}

// Destructor - removes all the elements from the free_list

template <class T, class ACE_LOCK>  
ACE_Locked_Free_List<T, ACE_LOCK>::~ACE_Locked_Free_List (void)
{
  if (this->mode_ != ACE_PURE_FREE_LIST)
    while (this->free_list_ != NULL)
      {
	T *temp = this->free_list_;
	this->free_list_ = this->free_list_->get_next ();
	delete temp;
      }
}

// Allocates <n> extra nodes for the freelist

template <class T, class ACE_LOCK> void 
ACE_Locked_Free_List<T, ACE_LOCK>::alloc (size_t n)
{
  ACE_MT (ACE_GUARD (LOCK, ace_mon, this->mutex_));

  for (; n > 0; n--)
    {
      T *temp;
      ACE_NEW (temp, T);
      temp->set_next (this->free_list_);
      this->free_list_ = temp;
      this->size_++;
    }
}

// Removes and frees <n> nodes from the freelist.

template <class T, class ACE_LOCK> void 
ACE_Locked_Free_List<T, ACE_LOCK>::dealloc (size_t n)
{
  ACE_MT (ACE_GUARD (LOCK, ace_mon, this->mutex_));

  for (; this->free_list_ != NULL && n > 0;
       n--)
    {
      T *temp = this->free_list_;
      this->free_list_ = this->free_list_->get_next ();
      delete temp;
      this->size_--;
    }
}

#endif /* ACE_FREE_LIST_C */
