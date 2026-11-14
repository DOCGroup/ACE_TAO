#include "DatabaseS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Singleton.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Malloc_T.h"
#include "ace/Null_Mutex.h"

#include <cstddef>
#include <new>

class DatabaseImpl
{
public:
  using DATABASE_MALLOC = ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_Null_Mutex>;

  class Simpler_Database_Malloc : public DATABASE_MALLOC
  {
  public:
    Simpler_Database_Malloc () = default;
    ~Simpler_Database_Malloc ();
  };

  using DATABASE = ACE_Singleton<Simpler_Database_Malloc, ACE_Null_Mutex>;
  class Entry : public TAO_DynamicImplementation
  {
  public:
    Entry (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
    ~Entry () override = default;

    Entry (Entry const &) = delete;
    Entry &operator= (Entry const &) = delete;
    Entry (Entry &&) = delete;
    Entry &operator= (Entry &&) = delete;

    void invoke (CORBA::ServerRequest_ptr request) override;
    // The invoke() method receives requests issued to any CORBA
    // object incarnated by the DSI servant and performs the
    // processing necessary to execute the request.

    CORBA::RepositoryId _primary_interface (PortableServer::ObjectId const &oid,
                                            PortableServer::POA_ptr poa) override;
    // The _primary_interface() method receives an ObjectId value and
    // a POA_ptr as input parameters and returns a valid RepositoryId
    // representing the most-derived interface for that oid.

    PortableServer::POA_ptr _default_POA () override;
    // Returns the default POA for this servant.

    void is_a (CORBA::ServerRequest_ptr request);
    // Handles the _is_a call

  protected:
    CORBA::ORB_var orb_;
    // ORB (auto) pointer

    PortableServer::POA_var poa_;
    // Default POA

    PortableServer::Current_var poa_current_;
    // POA Current.
  };

  class Agent : public POA_Database::Agent
  {
  public:
    Agent (CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);
    ~Agent () override = default;

    Database::Entry_ptr create_entry (char const *key,
                                      char const *entry_type,
                                      Database::NVPairSequence const &initial_attributes) override;

    Database::Entry_ptr find_entry (char const *key,
                                    char const *entry_type) override;

    void destroy_entry (char const *key,
                        char const *entry_type) override;

    void shutdown () override;

    PortableServer::POA_ptr _default_POA () override;
    // Returns the default POA for this servant.

    Agent (Agent const &) = delete;
    Agent &operator= (Agent const &) = delete;
    Agent (Agent &&) = delete;
    Agent &operator= (Agent &&) = delete;

  protected:
    CORBA::ORB_var orb_;
    // ORB (auto) pointer

    PortableServer::POA_var poa_;
    // Default POA

    Entry common_servant_;
  };

  static char *entry_type_to_repository_id (char const *entry_type);

  class Employee
  {
  public:
    Employee (char const *name, CORBA::Long id);

    ~Employee ();

    Employee (Employee const &) = delete;
    Employee &operator= (Employee const &) = delete;
    Employee (Employee &&) = delete;
    Employee &operator= (Employee &&) = delete;

    char const *name () const;
    void name (char const *name);

    CORBA::Long id () const;
    /// Overloaded new operator, nothrow_t variant.
    void *operator new (size_t bytes, std::nothrow_t const &nt);
    void operator delete (void *p, std::nothrow_t const &) noexcept;
    void *operator new (size_t);
    void operator delete (void *pointer);

  private:
    // Employee ID.
    CORBA::Long const id_;

    // Employee name.
    // Allocated from the database memory pool, so std::string cannot own it.
    char *name_ {};
  };
};
