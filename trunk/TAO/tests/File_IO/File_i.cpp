// $Id$

//===================================================================
//
// = FILENAME
//     File_i.cpp
//
// = DESCRIPTION
//      Implementation of the File IDL module and the interfaces
//      Descriptor and System in it.
//
// = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "File_i.h"

ACE_RCSID(Default_Servant, File_i, "File_i.cpp,v 1.14 2001/03/04 19:27:53 irfan Exp")

// IDL File::System constructor
FileImpl::System::System (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    // Create the Default Descriptor Servant
    fd_servant_ (poa)
{
  TAO_ENV_DECLARE_NEW_ENV;
  // set the default servant of the POA
  poa->set_servant (&this->fd_servant_ TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

FileImpl::System::~System (void)
{
}

PortableServer::POA_ptr
FileImpl::System::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

File::Descriptor_ptr
FileImpl::System::open (const char *file_name,
                        CORBA::Long flags
                        TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   File::IOError))
{
  // Do an ACE_OS::open
  ACE_HANDLE file_descriptor = ACE_OS::open (file_name,
                                             flags,
                                             ACE_DEFAULT_FILE_PERMS);

  if (file_descriptor == ACE_INVALID_HANDLE)
    {
      ACE_THROW_RETURN (File::IOError (), 0);
    }

  char file_descriptor_buffer[BUFSIZ];

  // convert ACE_HANDLE to a string
  ACE_OS::sprintf (file_descriptor_buffer,
                   "%ld",
                   (long int) file_descriptor);

  //Create an objectID from the ACE_HANDLE string
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (file_descriptor_buffer);

  // create an object reference with the specified ObjectID got
  // from ACE_HANDLE string
  CORBA::Object_var obj =
    this->poa_->create_reference_with_id (oid.in (),
                                          "IDL:File/Descriptor:1.0"
                                          TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (File::Descriptor::_nil ());

  // Narrow the object reference to a File Descriptor
  File::Descriptor_var fd =
    File::Descriptor::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (File::Descriptor::_nil ());

  return fd._retn ();
}

// IDL File::Descriptor constructor
FileImpl::Descriptor::Descriptor (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

FileImpl::Descriptor::~Descriptor (void)
{
}

PortableServer::POA_ptr
FileImpl::Descriptor::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

//Extracts the ACE_HANDLE from the passed object reference
ACE_HANDLE
FileImpl::Descriptor::fd (TAO_ENV_SINGLE_ARG_DECL)
{
  //
  // One way of getting our id.
  //

  // Get a reference to myself
  File::Descriptor_var me = this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  // Get the ObjectId from the reference
  PortableServer::ObjectId_var oid1 =
    this->poa_->reference_to_id (me.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  //
  // Another way of getting our id.
  //

  PortableServer::ObjectId_var oid2 =
    this->poa_->servant_to_id (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  //
  // Yet another way of getting our id.
  //

  int argc = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc, 0, 0 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  // Get the POA Current object reference
  CORBA::Object_var obj =
    orb->resolve_initial_references ("POACurrent"
                                     TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  // Narrow the object reference to a POA Current reference
  PortableServer::Current_var poa_current =
    PortableServer::Current::_narrow (obj.in ()
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  PortableServer::ObjectId_var oid3 =
    poa_current->get_object_id (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ACE_INVALID_HANDLE);

  ACE_ASSERT (oid1.in () == oid2.in ());
  ACE_ASSERT (oid2.in () == oid3.in ());

  // Convert the ObjectId to a string
  CORBA::String_var s =
    PortableServer::ObjectId_to_string (oid1.in ());

  // Get the ACE_HANDLE from the string
  return (ACE_HANDLE) ::atol (s.in ());
}

CORBA::Long
FileImpl::Descriptor::write (const File::Descriptor::DataBuffer &buffer
                             TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   File::IOError))
{
  ACE_HANDLE file_descriptor = this->fd (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  const CORBA::Octet *data = &buffer[0];

  ssize_t len = ACE_OS::write (file_descriptor,
                               data,
                               buffer.length ());
  if (len > 0)
    return len;

  ACE_THROW_RETURN (File::IOError (), 0);
}

File::Descriptor::DataBuffer *
FileImpl::Descriptor::read (CORBA::Long num_bytes
                            TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   File::IOError))
{
  ACE_HANDLE file_descriptor = this->fd (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Octet *buffer = File::Descriptor::DataBuffer::allocbuf (num_bytes);
  int length = ACE_OS::read (file_descriptor, buffer, num_bytes);

  if (length > 0)
    return new File::Descriptor::DataBuffer (length,
                                             length,
                                             buffer,
                                             1);

  File::Descriptor::DataBuffer::freebuf (buffer);
  ACE_THROW_RETURN (File::IOError (), 0);
}

CORBA::ULong
FileImpl::Descriptor::lseek (CORBA::ULong offset,
                             CORBA::Long whence
                             TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   File::IOError))
{
  ACE_HANDLE file_descriptor = this->fd (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Long result = (CORBA::Long) ACE_OS::lseek (file_descriptor,
                                                    offset,
                                                    whence);
  if (result == -1)
    ACE_THROW_RETURN (File::IOError (), 0);

  return (CORBA::ULong) result;
}

void
FileImpl::Descriptor::destroy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Get the ACE_HANDLE for this object reference
  ACE_HANDLE file_descriptor = this->fd (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Close the file corresponding to this object reference.
  ACE_OS::close (file_descriptor);
}
