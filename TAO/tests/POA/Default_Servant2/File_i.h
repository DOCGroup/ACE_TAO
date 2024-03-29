
//=============================================================================
/**
 *  @file     File_i.h
 *
 *    Defines the implementation classes for the File IDL
 *   module
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "FileS.h"

/**
 * @class FileImpl
 *
 * @brief FileImpl class provides the namespace for the File IDL module .
 */
class FileImpl
{
public:
  /**
   * @class Descriptor
   *
   * @brief Descriptor implements the Descriptor interface in the File
   * Module A single Descriptor servant can serve multiple object
   * references
   */
  class Descriptor : public POA_File::Descriptor
  {
  public:
    /// Constructor
    Descriptor (PortableServer::POA_ptr poa);

    /// Destructor
    ~Descriptor ();

    /// Returns the default POA of this object
    PortableServer::POA_ptr _default_POA ();

    /// write buffer to File corresponding to this Descriptor
    virtual CORBA::Long write (const File::Descriptor::DataBuffer &buffer);

    virtual File::Descriptor::DataBuffer *read (CORBA::Long num_bytes);

    /// seek to the offset in file from whence
    virtual CORBA::ULong lseek (CORBA::ULong offset,
                                CORBA::Long whence);

    /// closes the file corresponding to the requested ObjectID
    virtual void destroy ();

  private:
    /// Extracts the ACE_HANDLE from the objectID
    ACE_HANDLE fd ();

    PortableServer::POA_var poa_;
  };

  /**
   * @class System
   *
   * @brief File System implementation class.
   */
  class System : public POA_File::System
  {
  public:
    /// Constructor, Creates a single File Descriptor Servant and
    /// registers it with the POA as the Default Servant
    System (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa);

    ///Destructor
    ~System ();

    ///Returns the default POA of this object
    PortableServer::POA_ptr _default_POA ();

    /// Opens a file ,creates a Descriptor reference with the
    /// ACE_HANDLE and returns that reference
    File::Descriptor_ptr open (const char *file_name,
                               CORBA::Long flags);

    void shutdown ();
  private:
    CORBA::ORB_var orb_;

    PortableServer::POA_var poa_;

    /// The single File Descriptor servant which serves requests for
    /// any Descriptor object under poa_.
    Descriptor fd_servant_;
  };
};
