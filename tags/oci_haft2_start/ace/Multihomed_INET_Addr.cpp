// $Id$

// Extends ACE_INET_Addr with support for multi-homed addresses.

#include /**/ "ace/Multihomed_INET_Addr.h"
#include /**/ "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include /**/ "ace/Multihomed_INET_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace,
           Multihomed_INET_Addr,
           "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Multihomed_INET_Addr)

// Default constructor

ACE_Multihomed_INET_Addr::ACE_Multihomed_INET_Addr (void)
  : secondaries(0)
{
  ACE_TRACE ("ACE_Multihomed_INET_Addr::ACE_Multihomed_INET_Addr");
}

ACE_Multihomed_INET_Addr::ACE_Multihomed_INET_Addr(u_short port_number,
                                                   const char host_name[],
                                                   int encode,
                                                   int address_family,
                                                   const char *(secondary_host_names[]),
                                                   size_t size){

  // Initialize the primary INET addr
  ACE_INET_Addr::set(port_number, host_name, encode, address_family);

  // check for secondary INET addrs
  if (secondary_host_names && size){
    // we have a non-zero pointer and size
    this->secondaries.size(size); // size the array

    size_t next_empty_slot = 0;
    for (size_t i = 0; i < size; ++i) {
      int ret = this->secondaries[next_empty_slot].set(port_number,
                                                       secondary_host_names[i],
                                                       encode,
                                                       address_family);
      if (ret) {
        ACE_DEBUG ((LM_DEBUG,
                    "Invalid INET addr (%s:%u) will be ignored\n",
                    secondary_host_names[i], port_number));
        this->secondaries.size(this->secondaries.size() - 1);
      }
      else
        ++next_empty_slot;
    }
  }

  return;
}

ACE_Multihomed_INET_Addr::ACE_Multihomed_INET_Addr(u_short port_number,
                                                   ACE_UINT32 primary_ip_addr,
                                                   int encode,
                                                   const ACE_UINT32 *secondary_ip_addrs,
                                                   size_t size){

  // Initialize the primary INET addr
  ACE_INET_Addr::set(port_number, primary_ip_addr, encode);

  // check for secondary INET addrs
  if (secondary_ip_addrs && size){
    // we have a non-zero pointer and size
    this->secondaries.size(size); // size the array

    size_t next_empty_slot = 0;
    for (size_t i = 0; i < size; ++i) {
      int ret = this->secondaries[next_empty_slot].set(port_number,
                                                       secondary_ip_addrs[i],
                                                       encode);

      if (ret) {
        ACE_DEBUG ((LM_DEBUG,
                    "Invalid INET addr (%u:%u) will be ignored\n",
                    secondary_ip_addrs[i], port_number));
        this->secondaries.size(this->secondaries.size() - 1);
      }
      else
        ++next_empty_slot;
    }
  }

  return;
}

// Set implementations  (NEED BETTER COMMENT HERE)
int
ACE_Multihomed_INET_Addr::set (u_short port_number,
                               const char host_name[],
                               int encode,
                               int address_family,
                               const char *(secondary_host_names[]),
                               size_t size)
{
  this->secondaries.size(size);

  for (size_t i = 0; i < size; ++i) {

    int ret = this->secondaries[i].set(port_number,
                                       secondary_host_names[i],
                                       encode,
                                       address_family);
    if (ret) {
      return ret;
    }
  }

  return ACE_INET_Addr::set(port_number, host_name, encode, address_family);
}

int
ACE_Multihomed_INET_Addr::set (u_short port_number,
                               ACE_UINT32 primary_ip_addr,
                               int encode,
                               const ACE_UINT32 *secondary_ip_addrs,
                               size_t size)
{
  this->secondaries.size(size);

  for (size_t i = 0; i < size; ++i) {

    int ret = this->secondaries[i].set(port_number,
                                       secondary_ip_addrs[i],
                                       encode);

    if (ret) {
      return ret;
    }
  }

  return ACE_INET_Addr::set(port_number, primary_ip_addr, encode);
}

int
ACE_Multihomed_INET_Addr::get_secondary_addresses(ACE_INET_Addr *secondary_addrs,
                                                  size_t size) const
{
  size_t top = size < this->secondaries.size() ?
    size : this->secondaries.size();

  for (size_t i = 0; i < top; ++i) {

    int ret = secondary_addrs[i].set(this->secondaries[i]);

    if (ret) {
      return ret;
    }
  }

  return 0;
}

void
ACE_Multihomed_INET_Addr::get_addresses(sockaddr_in *addrs,
                                        size_t size) const
{
  // Copy primary address to the first slot of the user-supplied array
  if (size > 0) {
    addrs[0] = *ACE_reinterpret_cast (sockaddr_in*, this->get_addr());
  }

  // Copy secondary addresses to remaining slots of the user-supplied
  // array.  Secondary address [i] is copied to slot [i+1]

  size_t top = size - 1 < this->secondaries.size() ?
    size - 1 : this->secondaries.size();

  for (size_t i = 0; i < top; ++i) {
    addrs[i+1] = *ACE_reinterpret_cast (sockaddr_in*, this->secondaries[i].get_addr());
  }
}

// Default dtor.
ACE_Multihomed_INET_Addr::~ACE_Multihomed_INET_Addr (void)
{

}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Array_Base<ACE_INET_Addr>;
template class ACE_Array<ACE_INET_Addr>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<ACE_INET_Addr>
#pragma instantiate ACE_Array<ACE_INET_Addr>

#endif /*ACE_HAS_TEMPLATE_INSTANTIATION*/
