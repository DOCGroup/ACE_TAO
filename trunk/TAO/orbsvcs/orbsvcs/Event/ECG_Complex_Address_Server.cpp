// $Id$

#include "ECG_Complex_Address_Server.h"
#include "ace/SString.h"

#if !defined(__ACE_INLINE__)
#include "ECG_Complex_Address_Server.i"
#endif /* __ACE_INLINE__ */

TAO_ECG_Complex_Address_Server::TAO_ECG_Complex_Address_Server (
                                              int is_source_mapping)
  : is_source_mapping_ (is_source_mapping)
{
}

TAO_ECG_Complex_Address_Server::~TAO_ECG_Complex_Address_Server (void)
{
}

int
TAO_ECG_Complex_Address_Server::init (const char *arg)
{
  ACE_CString key_string;
  ACE_CString mcast_string;

  // Our position in parsing initialization string.
  const char * data = arg;

  // Parse initialization string until we reach the end.
  while (*data != '\0')
    {
      // Extract lookup value (it is followed by '@').
      const char * location = 0;
      location = ACE_OS::strchr (data, '@');
      if (!location)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                                        "Unable to initialize address "
                                        "server: cannot find <@> separator "
                                        "in initialization string "
                                        "as expected"),
                            -1);
        }
      size_t len = location - data;
      key_string.set (data, len, 1);
      data += len + 1;

      // Extract mcast address to be mapped to just extracted lookup
      // value.
      location = 0;
      location = ACE_OS::strchr (data, ' ');
      if (location)
        {
          len = location - data;
          mcast_string.set (data, len, 1);
          data += len + 1;
        }
      else
        {
          // This must be the last entry in the mapping.
          len = ACE_OS::strlen (data);
          mcast_string.set (data, len, 1);
          data += len;
        }

      // Add new entry to the mapping.
      if (this->add_entry (key_string.c_str (),
                           mcast_string.c_str ()) == -1)
        return -1;
    }
  return 0;
}

int
TAO_ECG_Complex_Address_Server::add_entry (const char * key,
                                           const char * mcast_addr)
{
  // Check whether this is the default mcast address.
  if (ACE_OS::strlen (key) == 1
      && *key == '*')
    {
      if (this->default_addr_.set (mcast_addr) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to initialize: invalid "
                                     "mcast address specified: %s.",
                           mcast_addr),
                          -1);
      return 0;
    }

  // Convert strings to values.
  char * endptr = 0;
  CORBA::Long header_value = ACE_OS::strtol (key, &endptr, 0);
  if (*endptr != '\0')
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to initialize: invalid "
                                   "source/type specified: %s.",
                         key),
                        -1);
    }

  ACE_INET_Addr addr;
  if (addr.set (mcast_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to initialize: invalid "
                                   "mcast address specified: %s.",
                         mcast_addr),
                         -1);
    }

  if (this->mcast_mapping_.bind (header_value, addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to initialize: error adding "
                                   "new entry to the mapping."),
                        -1);
    }

  return 0;
}


void
TAO_ECG_Complex_Address_Server::get_addr (
                         const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr_out addr
                         ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Long key;
  if (this->is_source_mapping_)
    key = header.source;
  else
    key = header.type;

  MAP::ENTRY * mapping_entry = 0;
  if (this->mcast_mapping_.find (key, mapping_entry) == -1)
    {
      // Key was not found in the mapping.  Use default.
      addr.ipaddr = this->default_addr_.get_ip_address ();
      addr.port = this->default_addr_.get_port_number ();
    }
  else
    {
      addr.ipaddr = mapping_entry->int_id_.get_ip_address ();
      addr.port = mapping_entry->int_id_.get_port_number ();
    }
}

void
TAO_ECG_Complex_Address_Server::dump_content (void)
{
  cout << "Default address: "
       << this->default_addr_.get_host_addr ()
       << ":"
       << this->default_addr_.get_port_number ()
       << endl;

  for (MAP::iterator iter = this->mcast_mapping_.begin ();
       iter != this->mcast_mapping_.end ();
       iter++)
    {
      MAP::ENTRY & entry = *iter;
      cout << entry.ext_id_
           << " --> "
           << entry.int_id_.get_host_addr ()
           << ":"
           << entry.int_id_.get_port_number ()
           << endl;
    }
}
