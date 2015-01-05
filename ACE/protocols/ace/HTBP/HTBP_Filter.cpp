// $Id$

#include "ace/Log_Msg.h"

#include "HTBP_Session.h"
#include "HTBP_Filter.h"

#if !defined (__ACE_INLINE__)
#include "HTBP_Filter.inl"
#endif



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE::HTBP::Filter::Filter (void)
  :http_code_ (0)
{
}

ACE::HTBP::Filter::~Filter (void)
{
}

char *
ACE::HTBP::Filter::header_complete (ACE::HTBP::Channel *ch)
{
  if (ch->leftovers().length() == 0)
    {
      return 0;
    }
  if (ch->leftovers().space() > 0)
    *ch->leftovers().wr_ptr() = '\0';
  char *start = ch->leftovers().rd_ptr();
  char *nl = ACE_OS::strchr (start,'\n');
  if (this->http_code_ == 0)
    {
      char *code = ACE_OS::strstr (start,"HTTP/1.");
      if (code && code < nl)
        {
          code += 9; // skip past "HTTP/1.1 "
          this->http_code_ = ACE_OS::strtol(code,0,10);
        }
    }

  while (nl)
    {
      if ((nl == start) ||
          (nl == start +1 && *start == '\r'))
        return nl + 1;
      start = nl + 1;
      nl = ACE_OS::strchr (start,'\n');
    }
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
