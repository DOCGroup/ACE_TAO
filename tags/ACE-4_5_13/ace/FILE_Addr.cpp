// $Id$

#define ACE_BUILD_DLL
#include "ace/FILE_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_FILE_Addr)

ACE_FILE_Addr::ACE_FILE_Addr (void)
  : ACE_Addr (AF_FILE, sizeof this->filename_)
{
  (void) ACE_OS::memset ((void *) &this->filename_,
                         0,
                         sizeof this->filename_);
}

int
ACE_FILE_Addr::set (const ACE_FILE_Addr &sa)
{
  this->base_set (sa.get_type (), sa.get_size ());

  if (sa.get_type () == AF_ANY)
    (void) ACE_OS::memset ((void *) &this->filename_,
                           0,
                           sizeof this->filename_);
  else
    (void) ACE_OS::memcpy ((void *) &this->filename_,
                           (void *) &sa.filename_,
                           sa.get_size ());
  return 0;
}

// Copy constructor.

ACE_FILE_Addr::ACE_FILE_Addr (const ACE_FILE_Addr &sa)
{
  this->set (sa);
}

ACE_FILE_Addr &
ACE_FILE_Addr::operator= (const ACE_FILE_Addr &sa)
{
  if (this != &sa)
    {
      size_t size = sa.get_size ();
      (void) ACE_OS::memcpy ((void *) &this->filename_,
                             (void *) &sa.filename_,
                             size);
      if (size < MAXNAMLEN + 1)
        this->filename_[size] = '\0';
    }
  return *this;
}

void
ACE_FILE_Addr::set (LPCTSTR filename)
{
  this->ACE_Addr::base_set (AF_FILE,
                            ACE_OS::strlen (filename) );
  (void) ACE_OS::strcpy (this->filename_,
                         filename);
}

// Create a ACE_Addr from a ACE_FILE pathname.

ACE_FILE_Addr::ACE_FILE_Addr (LPCTSTR filename)
{
  this->set (filename);
}

// Transform the current address into string format.

#if defined (UNICODE)
int
ACE_FILE_Addr::addr_to_string (wchar_t * s, size_t len) const
{
  ACE_OS::strncpy (s, this->filename_, len);
  return 0;
}
#endif /* UNICODE */

int
ACE_FILE_Addr::addr_to_string (char *s, size_t len) const
{
  ACE_OS::strncpy (s,
                   ACE_MULTIBYTE_STRING (this->filename_),
                   len);
  return 0;
}

void
ACE_FILE_Addr::dump (void) const
{
  ACE_TRACE ("ACE_FILE_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("filename_ = %s"), this->filename_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
