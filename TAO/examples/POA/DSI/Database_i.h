// $Id$

#include "DatabaseS.h"
#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class DatabaseImpl
{
public:

  typedef ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> MALLOC;

  class Simpler_Malloc : public MALLOC
  {
  public:
    Simpler_Malloc (void);
  };

  typedef ACE_Singleton<Simpler_Malloc, ACE_Null_Mutex> DATABASE;
  typedef ACE_Malloc_Iterator<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex> DATABASE_ITERATOR;

  class Entry : public PortableServer::DynamicImplementation
  {
  public:
    Entry (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
    ~Entry (void);

    virtual void invoke (CORBA::ServerRequest_ptr request,
                         CORBA::Environment &env);
    // The invoke() method receives requests issued to any CORBA
    // object incarnated by the DSI servant and performs the
    // processing necessary to execute the request.

    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &oid,
                                                    PortableServer::POA_ptr poa,
                                                    CORBA::Environment &env);
    // The _primary_interface() method receives an ObjectId value and
    // a POA_ptr as input parameters and returns a valid RepositoryId
    // representing the most-derived interface for that oid.

    virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
    // Returns the default POA for this servant.

    virtual void _is_a (CORBA::ServerRequest_ptr request,
                        CORBA::Environment &env);
    // Handles the _is_a call

  protected:
    CORBA::ORB_var orb_;
    // ORB (auto) pointer

    PortableServer::POA_var poa_;
    // Default POA
  };

  class Agent : public POA_Database::Agent
  {
  public:
    Agent (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
    ~Agent (void);

    virtual Database::Entry_ptr create_entry (const char *key,
                                              const char *entry_type,
                                              const Database::NVPairSequence &initial_attributes,
                                              CORBA::Environment &env);

    virtual Database::Entry_ptr find_entry (const char *key,
                                            const char *entry_type,
                                            CORBA::Environment &env);

    virtual void destroy_entry (const char *key,
                                const char *entry_type,
                                CORBA::Environment &env);

    virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
    // Returns the default POA for this servant.

  protected:
    CORBA::ORB_var orb_;
    // ORB (auto) pointer

    PortableServer::POA_var poa_;
    // Default POA

    Entry common_servant_;
  };

  static char *entry_type_to_repository_id (const char *entry_type);

  class Employee
  {
  public:
    Employee (const char* name,
              CORBA::Long id);

    ~Employee (void);

    const char *name (void) const;
    void name (const char* name);

    CORBA::Long id (void) const;
    void id (CORBA::Long id);

    void *operator new (size_t);
    void operator delete (void *pointer);

  private:
    CORBA::Long id_;
    // Employee ID.

    char *name_;
    // Employee name.
  };
};
