// $Id$

#include "tao/PortableServer/PortableServer_WFunctions.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  PortableServer::ObjectId *
  wstring_to_ObjectId (const CORBA::WChar *string)
  {
    // Size of Id
    //
    // We DO NOT include the zero terminator, as this is simply an
    // artifact of the way strings are stored in C.
    //
    u_int string_length = ACE_OS::wslen (string);

    CORBA::ULong buffer_size = string_length * sizeof (CORBA::WChar);

    // Create the buffer for the Id
    CORBA::Octet *buffer = PortableServer::ObjectId::allocbuf (buffer_size);

    // Copy contents
    ACE_OS::memcpy (buffer, string, buffer_size);

    // Create a new ID
    PortableServer::ObjectId *id = 0;
    ACE_NEW_RETURN (id,
                    PortableServer::ObjectId (buffer_size,
                                              buffer_size,
                                              buffer,
                                              1),
                    0);

    return id;
  }

  CORBA::WChar *
  ObjectId_to_wstring (const PortableServer::ObjectId &id)
  {
    // Compute resulting wide string's length.
    CORBA::ULong string_length =
      id.length () / sizeof (CORBA::WChar);

    // Allocate an extra slot if the id's length is not "aligned" on a
    // CORBA::WChar.
    if (id.length () % sizeof (CORBA::WChar))
      string_length++;

    // Create space - note that this method adds + 1 for the '\0'.
    CORBA::WChar* string = CORBA::wstring_alloc (string_length);

    // Copy the data
    ACE_OS::memcpy (string,
                    id.get_buffer (),
                    id.length ());

    // Null terminate the string
    string[string_length] = '\0';

    // Return string.
    return string;
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
