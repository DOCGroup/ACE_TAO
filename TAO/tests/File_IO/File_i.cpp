
//=============================================================================
/**
 *  @file     File_i.cpp
 *
 *    Implementation of the File IDL module and the interfaces
 *    Descriptor and System in it.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "File_i.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Truncate.h"

// IDL File::System constructor
FileImpl::System::System (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    // Create the Default Descriptor Servant
    fd_servant_ (poa)
{
  // set the default servant of the POA
  poa->set_servant (&this->fd_servant_);
}

PortableServer::POA_ptr
FileImpl::System::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

File::Descriptor_ptr
FileImpl::System::open (char const *file_name,
                        CORBA::Long flags)
{
  // Do an ACE_OS::open
  ACE_HANDLE const file_descriptor = ACE_OS::open (file_name,
                                                   flags,
                                                   ACE_DEFAULT_FILE_PERMS);

  if (file_descriptor == ACE_INVALID_HANDLE)
    {
      throw File::IOError ();
    }

  char file_descriptor_buffer[BUFSIZ] {};

  // convert ACE_HANDLE to a string
  ACE_OS::sprintf (file_descriptor_buffer,
                   "%ld",
                   ACE_Utils::truncate_cast<long int> ((intptr_t)file_descriptor));

  //Create an objectID from the ACE_HANDLE string
  PortableServer::ObjectId_var const oid =
    PortableServer::string_to_ObjectId (file_descriptor_buffer);

  // create an object reference with the specified ObjectID got
  // from ACE_HANDLE string
  CORBA::Object_var const obj =
    this->poa_->create_reference_with_id (oid.in (),
                                          "IDL:File/Descriptor:1.0");

  // Narrow the object reference to a File Descriptor
  File::Descriptor_var fd =
    File::Descriptor::_narrow (obj.in ());


  return fd._retn ();
}

// IDL File::Descriptor constructor
FileImpl::Descriptor::Descriptor (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::POA_ptr
FileImpl::Descriptor::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

//Extracts the ACE_HANDLE from the passed object reference
ACE_HANDLE
FileImpl::Descriptor::fd ()
{
  //
  // One way of getting our id.
  //

  // Get a reference to myself
  File::Descriptor_var const me = this->_this ();

  // Get the ObjectId from the reference
  PortableServer::ObjectId_var const oid1 =
    this->poa_->reference_to_id (me.in ());

  //
  // Another way of getting our id.
  //

  PortableServer::ObjectId_var const oid2 =
    this->poa_->servant_to_id (this);

  //
  // Yet another way of getting our id.
  //

  int argc {};
  ACE_TCHAR **argv {};
  CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

  // Get the POA Current object reference
  CORBA::Object_var const obj =
    orb->resolve_initial_references ("POACurrent");

  // Narrow the object reference to a POA Current reference
  PortableServer::Current_var const poa_current =
    PortableServer::Current::_narrow (obj.in ());

  PortableServer::ObjectId_var const oid3 =
    poa_current->get_object_id ();

  ACE_ASSERT (oid1.in () == oid2.in ());
  ACE_ASSERT (oid2.in () == oid3.in ());

  // Convert the ObjectId to a string
  CORBA::String_var const s =
    PortableServer::ObjectId_to_string (oid1.in ());

  // Get the ACE_HANDLE from the string
  return static_cast<ACE_HANDLE> (static_cast<intptr_t> (ACE_OS::atol (s.in ())));
}

CORBA::Long
FileImpl::Descriptor::write (File::Descriptor::DataBuffer const &buffer)
{
  ACE_HANDLE const file_descriptor = this->fd ();

  CORBA::Octet const *data = buffer.get_buffer ();

  ssize_t const len = ACE_OS::write (file_descriptor,
                                     data,
                                     buffer.length ());
  if (len > 0)
    return len;

  throw File::IOError ();
}

File::Descriptor::DataBuffer *
FileImpl::Descriptor::read (CORBA::Long num_bytes)
{
  ACE_HANDLE const file_descriptor = this->fd ();

  File::Descriptor::DataBuffer_var buffer =
    new File::Descriptor::DataBuffer (num_bytes);
  ssize_t const length = ACE_OS::read (file_descriptor,
                                       buffer->get_buffer (),
                                       num_bytes);

  if (length > 0)
    {
      buffer->length (static_cast<CORBA::ULong> (length));
      return buffer._retn ();
    }

  throw File::IOError ();
}

CORBA::ULong
FileImpl::Descriptor::lseek (CORBA::ULong offset,
                             CORBA::Long whence)
{
  ACE_HANDLE const file_descriptor = this->fd ();

  CORBA::Long const result = static_cast<CORBA::Long> (
    ACE_OS::lseek (file_descriptor, offset, whence));
  if (result == -1)
    throw File::IOError ();

  return static_cast<CORBA::ULong> (result);
}

void
FileImpl::Descriptor::destroy ()
{
  // Get the ACE_HANDLE for this object reference
  ACE_HANDLE const file_descriptor = this->fd ();

  // Close the file corresponding to this object reference.
  ACE_OS::close (file_descriptor);
}
