// $Id$
//===================================================================
//
// = FILENAME
//     File_i.h
//
// = DESCRIPTION
//      Defines the implementation classes for the File IDL
//     module
//
// = AUTHOR
//     Irfan Pyarali
//
//====================================================================



#include "FileS.h"

class FileImpl
// FileImpl class provides the namespace for the File IDL module .
{
public:
  class Descriptor : public POA_File::Descriptor
    // Descriptor implements the Descriptor interface in the File Module
    // A single Descriptor servant can serve multiple object references
  {
  public:
    //Constructor
    Descriptor (PortableServer::POA_ptr poa);

    //Destructor
    ~Descriptor (void);

    // Returns the default POA of this object
    PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

    // write buffer to File corresponding to this Descriptor
    virtual CORBA::Long write (const File::Descriptor::DataBuffer &buffer,
                               CORBA::Environment &env);

    // Reads num_bytes from the file and returns it
    virtual File::Descriptor::DataBuffer *read (CORBA::Long num_bytes,
                                                CORBA::Environment &env);
    // seek to the offset in file from whence
    virtual CORBA::ULong lseek (CORBA::ULong offset,
                                CORBA::Long whence,
                                CORBA::Environment &env);

    // closes the file corresponding to the requested ObjectID
    virtual void destroy (CORBA::Environment &env);

  private:

    // Extracts the ACE_HANDLE from the objectID
    ACE_HANDLE fd (CORBA::Environment &env);

    PortableServer::POA_var poa_;
  };

  class System : public POA_File::System
    // File System implementation class
  {
  public:
    // Constructor, Creates a single File Descriptor Servant and
    // registers it with the POA as the Default Servant
    System (PortableServer::POA_ptr poa);

    //Destructor
    ~System (void);

    //Returns the default POA of this object
    PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

    //Opens a file ,creates a Descriptor reference with the ACE_HANDLE
    // and returns that reference
    File::Descriptor_ptr open (const char *file_name,
                               CORBA::Long flags,
                               CORBA::Environment &env);

  private:
    PortableServer::POA_var poa_;

    // The single File Descriptor servant which serves requests for any
    // Descriptor object under poa_.
    Descriptor fd_servant_;
  };
};

