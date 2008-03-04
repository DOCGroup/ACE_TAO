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
{
  // = TITLE
  //   FileImpl class provides the namespace for the File IDL module .
public:
  class Descriptor : public POA_File::Descriptor
  {
    // = TITLE
    //   Descriptor implements the Descriptor interface in the File
    //   Module A single Descriptor servant can serve multiple object
    //   references
  public:
    Descriptor (PortableServer::POA_ptr poa);
    // Constructor

    ~Descriptor (void);
    // Destructor

    PortableServer::POA_ptr _default_POA (void);
    // Returns the default POA of this object

    virtual CORBA::Long write (const File::Descriptor::DataBuffer &buffer);
    // write buffer to File corresponding to this Descriptor

    virtual File::Descriptor::DataBuffer *read (CORBA::Long num_bytes);

    virtual CORBA::ULong lseek (CORBA::ULong offset,
                                CORBA::Long whence);
    // seek to the offset in file from whence

    virtual void destroy (void);
    // closes the file corresponding to the requested ObjectID

  private:

    ACE_HANDLE fd (void);
    // Extracts the ACE_HANDLE from the objectID

    PortableServer::POA_var poa_;
  };

  class System : public POA_File::System
  {
    // = TITLE
    //   File System implementation class.
  public:
    System (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa);
    // Constructor, Creates a single File Descriptor Servant and
    // registers it with the POA as the Default Servant

    ~System (void);
    //Destructor

    PortableServer::POA_ptr _default_POA (void);
    //Returns the default POA of this object

    File::Descriptor_ptr open (const char *file_name,
                               CORBA::Long flags);
    // Opens a file ,creates a Descriptor reference with the
    // ACE_HANDLE and returns that reference

    void shutdown (void);
  private:
    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    Descriptor fd_servant_;
    // The single File Descriptor servant which serves requests for
    // any Descriptor object under poa_.
  };
};
