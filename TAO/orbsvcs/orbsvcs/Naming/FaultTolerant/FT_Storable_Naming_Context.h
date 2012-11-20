// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_Storable_Naming_Context.h
 *
 *  $Id$
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_FT_STORABLE_NAMING_CONTEXT_H
#define TAO_FT_STORABLE_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"
#include "orbsvcs/orbsvcs/PortableGroupC.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class FT_TAO_Storable_Naming_Context
 *
 * @brief This class specializes the TAO_Storable_Naming_Context
 * 'ConcreteImplementor' in the Bridge pattern architecture of the 
 * CosNaming::NamingContext implementation.
 *
 */
class TAO_FtNaming_Export TAO_FT_Storable_Naming_Context : public TAO_Storable_Naming_Context
{
public:
  // = Initialization and termination methods.

  /**
   * Constructor that takes in preallocated data structure and takes
   * ownership of it.  Derived class from TAO_Persistent_Naming_Context
   * provides specialization of the resolve operation to support 
   * load balancing.
   */
  TAO_FT_Storable_Naming_Context (CORBA::ORB_ptr orb,
                                  PortableServer::POA_ptr poa,
                                  const char *poa_id,
                                  TAO_Naming_Service_Persistence_Factory *factory,
                                  const ACE_TCHAR *persistence_directory,
                                  size_t hash_table_size = ACE_DEFAULT_MAP_SIZE);

  /// Destructor.
  virtual ~TAO_FT_Storable_Naming_Context (void);

  /**
   * Override the resolve operation to support load balancing using
   * the object group manager and associated strategy.
   */
  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n);

  // = Utility methods.
  /**
   * This utility method factors out the code needed to create a new
   * Storable Naming Context servant and activate it under the
   * specified POA with the specified id.  This function is static so
   * that the code can be used, both from inside the class (e.g.,
   * <new_context>), and from outside (e.g., Naming_Utils.cpp).
   */
  static CosNaming::NamingContext_ptr make_new_context (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               TAO_FT_Storable_Naming_Context **new_context);

  // = Methods not implemented in TAO_Hash_Naming_Context.

  static CosNaming::NamingContext_ptr recreate_all(
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              int reentering,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              int use_redundancy);

  // Set the Naming Manager as a static so that it is available for all 
  // naming context implementations.
  static void set_naming_manager (TAO_FT_Naming_Manager *mgr_impl);

  bool is_object_group (const CORBA::Object_ptr obj) const;

protected:
  static TAO_FT_Naming_Manager *naming_manager_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_FT_STORABLE_NAMING_CONTEXT_H */
