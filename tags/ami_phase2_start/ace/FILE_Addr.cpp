// $Id$

#define ACE_BUILD_DLL
#include "ace/FILE_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/FILE_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, FILE_Addr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_FILE_Addr)

ACE_FILE_Addr::ACE_FILE_Addr (void)
  : ACE_Addr (AF_FILE, sizeof this->filename_)
{
  this->filename_[0] = '\0';
}

int
ACE_FILE_Addr::set (const ACE_FILE_Addr &sa)
{
  if (sa.get_type () == AF_ANY)
    {
      // Create a temporary file.
      ACE_OS::strcpy (this->filename_,
                      ACE_DEFAULT_TEMP_FILE);
      ACE_OS::mktemp (this->filename_);
      this->base_set (AF_FILE,
                      ACE_OS::strlen (this->filename_) + 1);
    }
  else
    {
      (void) ACE_OS::strncpy (this->filename_,
                              sa.filename_,
                              sa.get_size ());

      this->base_set (sa.get_type (),
                      sa.get_size ());
    }
  return 0;
}

// Copy constructor.

ACE_FILE_Addr::ACE_FILE_Addr (const ACE_FILE_Addr &sa)
{
  this->set (sa);
}

int
ACE_FILE_Addr::set (LPCTSTR filename)
{
  this->ACE_Addr::base_set (AF_FILE,
                            ACE_OS::strlen (filename) + 1);
  (void) ACE_OS::strncpy (this->filename_,
                          filename,
                          sizeof this->filename_);
  return 0;
}

ACE_FILE_Addr &
ACE_FILE_Addr::operator= (const ACE_FILE_Addr &sa)
{
  if (this != &sa)
    this->set (sa);
  return *this;
}

// Create a ACE_Addr from a ACE_FILE pathname.

ACE_FILE_Addr::ACE_FILE_Addr (LPCTSTR filename)
{
  this->set (filename);
}

int
ACE_FILE_Addr::addr_to_string (char *s, size_t len) const
{
  ACE_OS::strncpy (s,
                   ACE_MULTIBYTE_STRING (this->filename_),
                   len);
  return 0;
}

#if defined (UNICODE)
// Transform the current address into string format.

int
ACE_FILE_Addr::addr_to_string (wchar_t * s, size_t len) const
{
  ACE_OS::strncpy (s, this->filename_, len);
  return 0;
}
#endif /* UNICODE */

void
ACE_FILE_Addr::dump (void) const
{
  ACE_TRACE ("ACE_FILE_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("filename_ = %s"), this->filename_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}
