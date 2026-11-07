//=============================================================================
/**
 *  @file     Servant_Manager.h
 *
 *   Helper class for <ServantActivator_i> and <ServantLoactor_i>.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef SERVANT_MANAGER_H
#define SERVANT_MANAGER_H

#include "ace/DLL.h"
#include "ace/Containers.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/PortableServer/Active_Object_Map.h"

#include "tao/ORB.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ServantManager_i
 *
 * @brief This class is the helper class for the ServantActivator_i and
 * ServantLocator_i classes.
 *
 * The methods provided by this class are used by the ServantActivator_i
 * and ServantLocator_i classes. This class contains the common methods
 * needed by them.
 */
class ServantManager_i
{
public:
  /// This typedef is used to typecast the void* obtained when finding
  /// a symbol in the DLL.
  using SERVANT_FACTORY = PortableServer::Servant (*) (CORBA::ORB_ptr orb,
                                                       PortableServer::POA_ptr poa);

  /// Initialization.
  explicit ServantManager_i (CORBA::ORB_ptr orb);

  ServantManager_i (ServantManager_i const &) = delete;
  ServantManager_i &operator= (ServantManager_i const &) = delete;
  ServantManager_i (ServantManager_i &&) = delete;
  ServantManager_i &operator= (ServantManager_i &&) = delete;

  /// Destruction.
  ~ServantManager_i ();

  /**
   * Returns an ObjectId when given an DLL name and the factory method
   * to be invoked in the DLL. The application developer can initialise the
   * ServantActivator object by providing the dllname and the factory function.
   */
  PortableServer::ObjectId_var create_dll_object_id (char const *libname,
                                                     char const *factory_function);

  // @@ *done*Kirthika, please explain what this function is USED for, i.e.,
  // who calls it and why?

  /**
   * Obtains a servant on activation by linking and loading the
   * appropriate DLL and creating the servant object.  The <str>
   * argument is the ObjectId that contains the servant DLL name and
   * the factory function name. The <long> argument is an
   * servant-specific argument needed to create the servant for this
   * particular use-case.
   */
  PortableServer::Servant obtain_servant (ACE_TCHAR const *str,
                                          PortableServer::POA_ptr poa);

  /// The servant is destroyed and the DLL that was dynamically linked
  /// is closed.
  void destroy_servant (PortableServer::Servant servant,
                        PortableServer::ObjectId const &oid);

 private:
  /**
   * Parse the string to obtain the DLL name and the factory function
   * symbol that we will used to dynamically obtain the servant
   * pointer.
   */
  bool parse_string (ACE_TCHAR const *s,
                     ACE_TString &dllname,
                     ACE_TString &create_symbol) const;

  /// A reference to the ORB.
  CORBA::ORB_var orb_;

  using SERVANT_MAP = ACE_Hash_Map_Manager_Ex<PortableServer::ObjectId,
                                              ACE_DLL *,
                                              TAO_ObjectId_Hash,
                                              ACE_Equal_To<PortableServer::ObjectId>,
                                              ACE_Null_Mutex>;

  /**
   * The map owns each ACE_DLL pointer associated with an active servant.
   * The DLL must remain loaded until its servant has been destroyed.
   */
  SERVANT_MAP servant_map_;
};
#endif /* SERVANT_MANAGER_H */
