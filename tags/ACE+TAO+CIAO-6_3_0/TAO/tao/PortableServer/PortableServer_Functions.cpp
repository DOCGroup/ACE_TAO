// -*- C++ -*-
// $Id$

#include "tao/PortableServer/PortableServer_Functions.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  PortableServer::ObjectId *
  string_to_ObjectId (const char *string)
  {
    // Size of string
    //
    // We DO NOT include the zero terminator, as this is simply an
    // artifact of the way strings are stored in C.
    //
    CORBA::ULong buffer_size = static_cast <CORBA::ULong>
                                           (ACE_OS::strlen (string));

    // Create the buffer for the Id
    CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

    // Copy the contents
    ACE_OS::memcpy (buffer, string, buffer_size);

    // Create and return a new ID
    PortableServer::ObjectId *id = 0;
    ACE_NEW_RETURN (id,
                    PortableServer::ObjectId (buffer_size,
                                              buffer_size,
                                              buffer,
                                              1),
                    0);

    return id;
  }

  char *
  ObjectId_to_string (const PortableServer::ObjectId &id)
  {
    // Create space
    char * string = CORBA::string_alloc (id.length ());

    // Copy the data
    ACE_OS::memcpy (string, id.get_buffer (), id.length ());

    // Null terminate the string
    string[id.length ()] = '\0';

    // Return string
    return string;
  }
}


TAO_END_VERSIONED_NAMESPACE_DECL
