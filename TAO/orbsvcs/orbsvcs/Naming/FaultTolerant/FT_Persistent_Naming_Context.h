// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_Persistent_Naming_Context.h
 *
 *  @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================

#ifndef TAO_FT_PERSISTENT_NAMING_CONTEXT_H
#define TAO_FT_PERSISTENT_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Persistent_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"
#include "orbsvcs/PortableGroupC.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_FT_Persistent_Naming_Context
 *
 * @brief This class specializes the TAO_Persistent_Naming_Context
 * 'ConcreteImplementor' in the Bridge pattern architecture of the
 * CosNaming::NamingContext implementation.
 */
class TAO_FtNaming_Export TAO_FT_Persistent_Naming_Context :
      public TAO_Persistent_Naming_Context
{
public:
  /// Underlying data structure alias for ease of use.
  using HASH_MAP = TAO_Persistent_Naming_Context::HASH_MAP;

  /**
   * Constructor that takes in preallocated data structure and takes
   * ownership of it.  Derived class from TAO_Persistent_Naming_Context
   * provides specialization of the resolve operation to support
   * load balancing.
   */
  TAO_FT_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                    char const *poa_id,
                                    TAO_Persistent_Context_Index *context_index,
                                    HASH_MAP *map = nullptr,
                                    ACE_UINT32 *counter = nullptr);

  /// Destructor.
  ~TAO_FT_Persistent_Naming_Context () override = default;

  /**
   * Override the resolve operation to support load balancing using
   * the object group manager and associated strategy.
   */
  CORBA::Object_ptr resolve (CosNaming::Name const &n) override;

  static void set_naming_manager_impl (TAO_FT_Naming_Manager *mgr_impl);

  CORBA::Boolean is_object_group (CORBA::Object_ptr obj) const;

protected:
  static TAO_FT_Naming_Manager *naming_manager_impl_;

private:
  TAO_FT_Persistent_Naming_Context (TAO_FT_Persistent_Naming_Context const &) = delete;
  TAO_FT_Persistent_Naming_Context (TAO_FT_Persistent_Naming_Context &&) = delete;
  TAO_FT_Persistent_Naming_Context &operator= (TAO_FT_Persistent_Naming_Context const &) = delete;
  TAO_FT_Persistent_Naming_Context &operator= (TAO_FT_Persistent_Naming_Context &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_FT_PERSISTENT_NAMING_CONTEXT_H */
