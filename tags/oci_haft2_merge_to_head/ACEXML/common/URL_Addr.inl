// $Id$

#include "ace/ACE.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"


ACE_INLINE size_t
ACEXML_URL_Addr::calculate_length (int ipaddr_format) const
{
  return ACE_OS::strlen (ipaddr_format == 0 ?
                         this->get_host_name () : this->get_host_addr ())
                     + ACE_OS::strlen ("65536") // Assume the max port number.
                     + ACE_OS::strlen (this->get_path_name ())
                     + sizeof (':')
                     + sizeof ('/')
                     + sizeof ('\0'); // For trailing '\0'.
}

ACE_INLINE void
ACEXML_URL_Addr::operator= (const ACEXML_URL_Addr &addr)
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACEXML_URL_Addr::ACEXML_URL_Addr")));
}

ACE_INLINE u_long
ACEXML_URL_Addr::hash (void) const
{
  u_long result = this->ACE_INET_Addr::hash ()
                  + ACE::hash_pjw (this->get_path_name ());
  return result;
}

ACE_INLINE int
ACEXML_URL_Addr::operator== (const ACEXML_URL_Addr &addr) const
{
  return ACE_OS::strcmp (addr.get_path_name (), this->get_path_name ()) == 0
    && addr.get_port_number () == this->get_port_number ()
    && addr.get_ip_address () == this->get_ip_address ();
}

ACE_INLINE int
ACEXML_URL_Addr::operator!= (const ACEXML_URL_Addr &addr) const
{
  return !(*this == addr);
}

ACE_INLINE const ACEXML_Char *
ACEXML_URL_Addr::get_path_name (void) const
{
  return this->path_name_;
}

ACE_INLINE int
ACEXML_URL_Addr::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}
