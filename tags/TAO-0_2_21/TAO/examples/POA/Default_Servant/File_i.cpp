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

ACE_RCSID(Default_Servant, File_i, "$Id$")

// IDL File::System constructor
FileImpl::System::System (PortableServer::POA_ptr poa)
 : poa_ (PortableServer::POA::_duplicate (poa)),
   // Create the Default Descriptor Servant
    fd_servant_ (poa)
{
  CORBA::Environment env;
  // set the default servant of the POA
  poa->set_servant (&this->fd_servant_, env);
  ACE_ASSERT (env.exception () == 0);
}

FileImpl::System::~System (void)
{
}

PortableServer::POA_ptr
FileImpl::System::_default_POA (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

File::Descriptor_ptr
FileImpl::System::open (const char *file_name,
                        CORBA::Long flags,
                        CORBA::Environment &env)
{
  // Do an ACE_OS::open
  ACE_HANDLE file_descriptor = ACE_OS::open (file_name,
                                             flags);

  if (file_descriptor == ACE_INVALID_HANDLE)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }

  char file_descriptor_buffer[BUFSIZ];

  // convert ACE_HANDLE to a string
  ACE_OS::sprintf (file_descriptor_buffer,
                   "%ld",
                   (CORBA::Long) file_descriptor);

  //Create an objectID from the ACE_HANDLE string
  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (file_descriptor_buffer);

  // create an object reference with the specified ObjectID got
  // from ACE_HANDLE string
  CORBA::Object_var obj =
    this->poa_->create_reference_with_id (oid.in (),
                                          "IDL:File/Descriptor:1.0",
                                          env);
  if (env.exception () != 0)
    return File::Descriptor::_nil ();

  // Narrow the object reference to a File Descriptor
  File::Descriptor_var fd =
    File::Descriptor::_narrow (obj.in (), env);

  if (env.exception () != 0)
    return File::Descriptor::_nil ();

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
FileImpl::Descriptor::_default_POA (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

//Extracts the ACE_HANDLE from the passed object reference
ACE_HANDLE
FileImpl::Descriptor::fd (CORBA::Environment &env)
{
  // Get a reference to myself
  File::Descriptor_var me = this->_this (env);

  if (env.exception () != 0)
    return ACE_INVALID_HANDLE;

  // Get the ObjectId from the reference
  PortableServer::ObjectId_var oid =
    this->poa_->reference_to_id (me.in (), env);

  if (env.exception () != 0)
    return ACE_INVALID_HANDLE;

  // Convert the ObjectId to a string
  CORBA::String_var s =
    PortableServer::ObjectId_to_string (oid.in ());

  // Get the ACE_HANDLE from the string
  return (ACE_HANDLE) ::atol (s.in ());
}

CORBA::Long
FileImpl::Descriptor::write (const File::Descriptor::DataBuffer &buffer,
                             CORBA::Environment &env)
{
  ACE_HANDLE file_descriptor = this->fd (env);

  if (env.exception () != 0)
    return 0;

  const CORBA::Octet *data = &buffer[0];

  ssize_t len = ACE_OS::write (file_descriptor,
                               data,
                               buffer.length ());
  if (len > 0)
    return len;
  else
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }
}

File::Descriptor::DataBuffer *
FileImpl::Descriptor::read (CORBA::Long num_bytes,
                            CORBA::Environment &env)
{
  ACE_HANDLE file_descriptor = this->fd (env);

  if (env.exception () != 0)
    return 0;

  CORBA::Octet *buffer = File::Descriptor::DataBuffer::allocbuf (num_bytes);
  int length = ACE_OS::read (file_descriptor, buffer, num_bytes);

  if (length > 0)
    return new File::Descriptor::DataBuffer (length,
                                             length,
                                             buffer,
                                             1);
  else
    {
      File::Descriptor::DataBuffer::freebuf (buffer);
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }
}

CORBA::ULong
FileImpl::Descriptor::lseek (CORBA::ULong offset,
                             CORBA::Long whence,
                             CORBA::Environment &env)
{
  ACE_HANDLE file_descriptor = this->fd (env);

  if (env.exception () != 0)
    return 0;

  CORBA::Long result = (CORBA::Long) ACE_OS::lseek (file_descriptor,
                                                    offset,
                                                    whence);
  if (result == -1)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }
  else
    return (CORBA::ULong) result;
}

void
FileImpl::Descriptor::destroy (CORBA::Environment &env)
{
  // Get the ACE_HANDLE for this object reference
  ACE_HANDLE file_descriptor = this->fd (env);

  if (env.exception () != 0)
    return;

  //close the file corresponding to this object reference
  int result = ACE_OS::close (file_descriptor);

  if (result != 0)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return;
    }
}
