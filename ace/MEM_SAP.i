/* -*- C++ -*- */
// $Id$

// MEM_SAP.i

ASYS_INLINE
ACE_MEM_SAP::~ACE_MEM_SAP (void)
{
  // ACE_TRACE ("ACE_MEM_SAP::~ACE_MEM_SAP");
  delete this->shm_malloc_;
}


ASYS_INLINE void *
ACE_MEM_SAP::acquire_buffer (const ssize_t size)
{
  ACE_TRACE ("ACE_MEM_SAP::acquire_buffer");
  if (this->shm_malloc_ == 0)
    return 0;                  // not initialized.

  size_t *lptr = ACE_static_cast (size_t *,
                                  this->shm_malloc_->malloc (sizeof (size_t) + size));

  *lptr = size;
  ++lptr;

  return lptr;
}

ASYS_INLINE int
ACE_MEM_SAP::release_buffer (void *buf)
{
  ACE_TRACE ("ACE_MEM_SAP::release_buffer");
  if (this->shm_malloc_ == 0)
    return -1;                  // not initialized.

  size_t *lptr = ACE_static_cast (size_t *, buf);

  --lptr;
  this->shm_malloc_->free (lptr);
  return 0;
}

ASYS_INLINE off_t
ACE_MEM_SAP::set_buf_len (void *buf, size_t n)
{
  ACE_TRACE ("ACE_MEM_SAP::set_buf_len");
  if (this->shm_malloc_ == 0)
    return -1;

  size_t *lptr = ACE_static_cast (size_t *, buf);
  --lptr;

  if (*lptr >= n)
    *lptr = n;

  return ((char *) lptr - (char *) this->shm_malloc_->memory_pool ().base_addr ());
}

ASYS_INLINE ssize_t
ACE_MEM_SAP::get_buf_len (const off_t off, void *&buf)
{
  ACE_TRACE ("ACE_MEM_SAP::get_buf_len");

  if (this->shm_malloc_ == 0)
    return -1;

  ssize_t retv = 0;

  ACE_SEH_TRY
    {
      size_t *lptr = (size_t*) ((char *) this->shm_malloc_->memory_pool ().base_addr () + off);
      buf = lptr + 1;
      retv = *lptr;
    }
  ACE_SEH_EXCEPT (this->shm_malloc_->memory_pool ().seh_selector (GetExceptionInformation ()))
    {
    }

  return retv;
}

ASYS_INLINE int
ACE_MEM_SAP::remove (void)
{
  ACE_TRACE ("ACE_MEM_SAP::remove");

  return close_shm_malloc (1);
}
