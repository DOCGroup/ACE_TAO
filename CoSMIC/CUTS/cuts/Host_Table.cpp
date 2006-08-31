// $Id$

#include "cuts/Host_Table.h"
#include "ace/String_Base.h"

//
// CUTS_Host_Table
//
CUTS_Host_Table::CUTS_Host_Table (void)
{

}

//
// ~CUTS_Host_Table
//
CUTS_Host_Table::~CUTS_Host_Table (void)
{

}

//
// bind
//
int CUTS_Host_Table::bind (const ACE_CString & ipaddr,
                           const ACE_CString & hostname)
{
  // Place the IP address on its map.
  IP_Address_Map::ENTRY * ipaddr_entry = 0;
  int result = this->ipaddr_map_.bind (ipaddr, 0, ipaddr_entry);

  if (result == 0)
  {
    // Place the hostname on its map.
    Hostname_Map::ENTRY * hostname_entry = 0;
    result = this->host_map_.bind (hostname, 0, hostname_entry);

    // Map the IP-address to the hostname.
    ipaddr_entry->int_id_ = hostname_entry;
    hostname_entry->int_id_ = ipaddr_entry;
  }

  return result;
}

//
// find
//
int CUTS_Host_Table::find_by_name (const ACE_CString & hostname,
                                   ACE_CString & ipaddr)
{
  // Locate the <entry> in the <host_map_>, whose <int_id_> is
  // of type <void *>.
  IP_Address_Map::ENTRY * entry = 0;
  int result = this->find_by_name_i (hostname, entry);

  if (result == 0 && entry != 0)
  {
    ipaddr = entry->ext_id_;
  }

  return result;
}

//
// find
//
int CUTS_Host_Table::find_by_addr (const ACE_CString & ipaddr,
                                   ACE_CString & hostname)
{
  // Locate the <entry> in the <host_map_>, whose <int_id_> is
  // of type <void *>.
  Hostname_Map::ENTRY * entry = 0;
  int result = this->find_by_addr_i (ipaddr, entry);

  if (result == 0 && entry != 0)
  {
    hostname = entry->ext_id_;
  }

  return result;
}

//
// unbind
//
void CUTS_Host_Table::unbind_by_addr (const ACE_CString & ipaddr)
{
  // Locate the <ipaddr> in the map.
  Hostname_Map::ENTRY * entry = 0;
  int result = this->find_by_addr_i (ipaddr, entry);

  if (result == 0 && entry != 0)
  {
    // Remove both entries from their respective maps.
    this->ipaddr_map_.unbind (ipaddr);
    this->host_map_.unbind (entry);
  }
}

//
// unbind
//
void CUTS_Host_Table::unbind_by_name (const ACE_CString & hostname)
{
  // Locate the IP-address entry.
  IP_Address_Map::ENTRY * entry = 0;
  int result = this->find_by_name_i (hostname, entry);

  if (result == 0 && entry != 0)
  {
    // Remove both entries from their respective maps.
    this->host_map_.unbind (hostname);
    this->ipaddr_map_.unbind (entry);
  }
}

//
// find_by_name_i
//
int CUTS_Host_Table::find_by_name_i (const ACE_CString & hostname,
                                     IP_Address_Map::ENTRY * & entry)
{
  void * temp = 0;
  int result = this->host_map_.find (hostname, temp);

  if (result == 0)
  {
    entry = reinterpret_cast <IP_Address_Map::ENTRY *> (temp);
  }

  return result;
}

//
// find_by_addr_i
//
int CUTS_Host_Table::find_by_addr_i (const ACE_CString & ipaddr,
                                     Hostname_Map::ENTRY * & entry)
{
  void * temp = 0;
  int result = this->ipaddr_map_.find (ipaddr, temp);

  if (result == 0)
  {
    entry = reinterpret_cast <Hostname_Map::ENTRY *> (temp);
  }

  return result;
}
