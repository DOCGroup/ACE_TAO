#include "PortableServer_Functions.h"

ACE_RCSID (PortableServer,
           PortableServer_Functions,
           "$Id$")

#include "ace/OS_NS_string.h"

namespace TAO
{
  namespace Portable_Server
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

    PortableServer::ObjectId *
    wstring_to_ObjectId (const CORBA::WChar *string)
    {
      // Size of Id
      //
      // We DO NOT include the zero terminator, as this is simply an
      // artifact of the way strings are stored in C.
      //
      CORBA::ULong string_length = ACE_OS::wslen (string);

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

    CORBA::WChar *
    ObjectId_to_wstring (const PortableServer::ObjectId &id)
    {
      // Compute resulting wide string's length.
      CORBA::ULong string_length =
        id.length () / sizeof (CORBA::WChar) + 1;

      // Allocate an extra slot if the id's length is not "aligned" on a
      // CORBA::WChar.
      if (id.length () % sizeof (CORBA::WChar))
        string_length++;

      // Create space.
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
}

