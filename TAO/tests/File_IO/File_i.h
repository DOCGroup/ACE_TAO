
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
    explicit Descriptor (PortableServer::POA_ptr poa);
    Descriptor (Descriptor const &) = delete;
    Descriptor (Descriptor &&) = delete;
    Descriptor &operator= (Descriptor const &) = delete;
    Descriptor &operator= (Descriptor &&) = delete;

    /// Destructor
    ~Descriptor () override = default;

    /// Returns the default POA of this object
    PortableServer::POA_ptr _default_POA () override;

    /// write buffer to File corresponding to this Descriptor
    CORBA::Long write (File::Descriptor::DataBuffer const &buffer) override;

    File::Descriptor::DataBuffer *read (CORBA::Long num_bytes) override;

    /// seek to the offset in file from whence
    CORBA::ULong lseek (CORBA::ULong offset,
                        CORBA::Long whence) override;

    /// closes the file corresponding to the requested ObjectID
    void destroy () override;

  private:
    /// Extracts the ACE_HANDLE from the objectID
    ACE_HANDLE fd ();

    PortableServer::POA_var poa_ {};
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
    explicit System (PortableServer::POA_ptr poa);
    System (System const &) = delete;
    System (System &&) = delete;
    System &operator= (System const &) = delete;
    System &operator= (System &&) = delete;

    ///Destructor
    ~System () override = default;

    ///Returns the default POA of this object
    PortableServer::POA_ptr _default_POA () override;

    /// Opens a file ,creates a Descriptor reference with the
    /// ACE_HANDLE and returns that reference
    File::Descriptor_ptr open (char const *file_name,
                               CORBA::Long flags) override;

  private:
    PortableServer::POA_var poa_ {};

    /// The single File Descriptor servant which serves requests for
    /// any Descriptor object under poa_.
    Descriptor fd_servant_;
  };
};
