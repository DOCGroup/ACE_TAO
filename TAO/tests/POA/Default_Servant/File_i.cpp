#include "File_i.h"

FileImpl::System::System (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    fd_servant_ (poa)
{
  CORBA::Environment env;
  poa->set_servant (&this->fd_servant_, env);
  ACE_ASSERT (env.exception () == 0);
}

FileImpl::System::~System (void)
{
}

PortableServer::POA_ptr 
FileImpl::System::_default_POA (CORBA::Environment &env)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

File::Descriptor_ptr 
FileImpl::System::open (const char *file_name, 
                        CORBA::Long flags,
                        CORBA::Environment &env)
{
  ACE_HANDLE file_descriptor = ACE_OS::open (file_name, flags);
  if (file_descriptor == ACE_INVALID_HANDLE)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }

  char file_descriptor_buffer[BUFSIZ];
  ACE_OS::sprintf (file_descriptor_buffer, "%ld", (CORBA::Long) file_descriptor);

  PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId (file_descriptor_buffer);
  CORBA::Object_var obj = this->poa_->create_reference_with_id (oid.in (), 
                                                                this->_interface_repository_id (),
                                                                env);
  if (env.exception () != 0)
    return File::Descriptor::_nil ();

  File::Descriptor_var fd = File::Descriptor::_narrow (obj.in (), env);
  if (env.exception () != 0)
    return File::Descriptor::_nil ();

  return fd._retn ();
}

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
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

ACE_HANDLE
FileImpl::Descriptor::fd (CORBA::Environment &env)
{
  File::Descriptor_var me = this->_this (env);
  if (env.exception () != 0)
    return ACE_INVALID_HANDLE;

  PortableServer::ObjectId_var oid = this->poa_->reference_to_id (me.in (), env);
  if (env.exception () != 0)
    return ACE_INVALID_HANDLE;

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid.in ());
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
  int len = ACE_OS::write (file_descriptor, data, buffer.length ());

  if (len > 0)
    {
      return len;
    }      
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
    {
      return new File::Descriptor::DataBuffer (length, length, buffer, CORBA::B_TRUE);
    }      
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
  
  CORBA::ULong result = ACE_OS::lseek (file_descriptor, offset, whence);
  if (result == -1)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return 0;
    }
  else
    return result;
}

void
FileImpl::Descriptor::destroy (CORBA::Environment &env)
{
  ACE_HANDLE file_descriptor = this->fd (env);
  if (env.exception () != 0)
    return;

  int result = ACE_OS::close (file_descriptor);
  if (result != 0)
    {
      CORBA::Exception *exception = new File::IOError (errno);
      env.exception (exception);
      return;
    }
}
