#include "FileS.h"

class FileImpl
{
public:
  class Descriptor : public POA_File::Descriptor
  {
  public:
    Descriptor (PortableServer::POA_ptr poa);

    ~Descriptor (void);

    PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

    virtual CORBA::Long write (const File::Descriptor::DataBuffer &buffer, 
                               CORBA::Environment &env);

    virtual File::Descriptor::DataBuffer *read (CORBA::Long num_bytes, 
                                                CORBA::Environment &env);
    
    virtual CORBA::ULong lseek (CORBA::ULong offset, 
                                CORBA::Long whence,  
                                CORBA::Environment &env);

    virtual void destroy (CORBA::Environment &env);

  private:
    ACE_HANDLE fd (CORBA::Environment &env);

    PortableServer::POA_var poa_;
  };

  class System : public POA_File::System
  {
  public:
    System (PortableServer::POA_ptr poa);

    ~System (void);

    PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

    File::Descriptor_ptr open (const char *file_name, 
                               CORBA::Long flags,
                               CORBA::Environment &env);
    
  private:
    PortableServer::POA_var poa_;
    
    Descriptor fd_servant_;
  };
};

