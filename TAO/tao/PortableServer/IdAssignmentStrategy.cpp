#include "tao/PortableServer/IdAssignmentStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    char
    IdAssignmentStrategy::key_type_length () const
    {
      return sizeof (char);
    }

    void
    IdAssignmentStrategy::create_key (
      CORBA::Octet *buffer,
      CORBA::ULong& starting_at)
    {
      // Copy the system id byte.
      buffer[starting_at] = (CORBA::Octet) this->id_assignment_key_type ();
      starting_at += this->key_type_length ();
    }

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
