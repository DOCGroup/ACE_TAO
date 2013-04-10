// -*- C++ -*-

//=============================================================================
/**
 *  @file   ORB_Manager.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland <cleeland@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ORB_MANAGER_H
#define TAO_ORB_MANAGER_H
#include /**/ "ace/pre.h"

#include "tao/Utils/utils_export.h"
#include "tao/PortableServer/PortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ORB_Manager
 *
 * @brief Helper class for simple ORB/POA initialization and
 * registering servants with the POA.
 *
 * This class is a TAO extension that makes it easier to write
 * CORBA applications.  It's just a wrapper and doesn't do
 * anything special within the ORB itself.
 */
class TAO_UTILS_Export TAO_ORB_Manager
{
public:
  // = Initialization and termination methods.
  /** Constructor.
   *
   *  @param orb  pointer to an ORB which is duplicated an stored
   *              internally in an ORB_var.  If pointer is 0,
   *              a new ORB pointer is created internally in the init()
   *              call.
   *
   *  @param poa  pointer to a POA which is duplicated and stored
   *              internally in a POA_var.  If pointer is 0,
   *              a pointer to the Root POA is obtained from the ORB.
   *
   *  @param poa_manager pointer to a POA Manager which is duplicated
   *                     and stored internally in a POAManager_var.
   *                     If pointer is 0, a new POAManager is created
   *                     internally in the init() call.
   */
  TAO_ORB_Manager (CORBA::ORB_ptr orb = CORBA::ORB::_nil(),
                   PortableServer::POA_ptr poa = PortableServer::POA::_nil(),
                   PortableServer::POAManager_ptr poa_manager =
                      PortableServer::POAManager::_nil());

  /**
   * Initialize the ORB/root POA, using the supplied command line
   * arguments or the default ORB components.
   *
   * @retval -1 Failure
   * @retval 0 Success
   */
  int init (int &argc,
            ACE_TCHAR *argv[],
            const char *orb_name = 0);

#if !defined (CORBA_E_MICRO)
  /**
   * Creates a child poa under the root poa with PERSISTENT and
   * USER_ID policies.  Call this if you want a @a child_poa with the
   * above policies, otherwise call init.
   *
   * @retval -1 Failure
   * @retval 0 Success
   */
  int init_child_poa (int &argc,
                      ACE_TCHAR *argv[],
                      const char *poa_name,
                      const char *orb_name = 0);
#endif /* CORBA_E_MICRO */

  /**
   * Shut down.  Invoke the destroy() methods on the orb and poa.
   *
   * @retval -1 Failure
   * @retval 0 Success
   */
  int fini (void);

  /// Destructor.
  ~TAO_ORB_Manager (void);

  // = Accessor methods.

  /**
   *  Put POA manager into the <Active> state, so that incoming corba
   *  requests are processed.  This method is useful for clients,
   *  which are not going to enter "orb->run" loop, yet may want to
   *  service incoming requests while waiting for a result of CORBA
   *  call on a server.
   *
   * @retval -1 Failure
   * @retval 0 Success
   */
  int activate_poa_manager (void);

  /**
   * Activate <servant>, using the POA <activate_object> call.  Users
   * can call this method multiple times to activate multiple objects.
   *
   * @return 0 on failure, a string representation of the object ID if
   *         successful.  Caller of this method is responsible for
   *         memory deallocation of the string.
   */
  char *activate (PortableServer::Servant servant);

  /** Deactivate object in RootPOA.
   *
   *  @param id  A string representation of the Object ID
   *             of the servant to deactivate in the POA
   */
  void deactivate (const char *id);

#if !defined (CORBA_E_MICRO)
  /**
   * Precondition: init_child_poa has been called.  Activate <servant>
   * using the POA <activate_object_with_id> created from the string
   * <object_name>. Users should call this to activate objects under
   * the child_poa.
   *
   * @param object_name  String name which will be used to create
   *                     an Object ID for the servant.
   * @param servant  The servant to activate under the child POA.
   *
   * @return 0 on failure, a string representation of the object ID if
   *           successful.  Caller of this method is responsible for
   *           memory deallocation of the string.
   */
  char *activate_under_child_poa (const char *object_name,
                                  PortableServer::Servant servant);

  /**
   * Deactivate object in child POA.
   *
   * @param id  string representation of the object ID, which represents
   *            the object to deactivate in the POA
   */
  void deactivate_under_child_poa (const char *id);
#endif /* CORBA_E_MICRO */

  /**
   * Run the ORB event loop with the specified @a tv time value.
   *
   * @param tv  the time interval for how long to run the ORB event loop.
   * @retval -1 Failure
   * @retval 0 Success
   */
  int run (ACE_Time_Value &tv);

  /**
   * Run the ORB event loop.
   */
  int run (void);

  /**
   * Accessor which returns the ORB pointer.  Following the normal
   * CORBA memory management rules of return values from functions,
   * this function duplicates the orb return value before returning
   * it.
   *
   * @return ORB pointer which has been duplicated, so caller
   *         must release pointer when done.
   */
  CORBA::ORB_ptr orb (void);

  /**
   * Accessor which returns the root poa. Following the normal CORBA
   * memory management rules of return values from functions, this
   * function duplicates the poa return value before returning it.
   *
   * @return Root POA pointer which has been duplicated.  Caller
   *         must release pointer when done.
   */
  PortableServer::POA_ptr root_poa (void);

  /**
   * Accessor which returns the child poa. Following the normal CORBA
   * memory management rules of return values from functions, this
   * function duplicates the poa return value before returning it.
   *
   * @return Child POA pointer which has been duplicated.  Caller
   *         must release pointer when done.
   */
  PortableServer::POA_ptr child_poa (void);

  /**
   * Accessor which returns the poa manager. Following the normal
   * CORBA memory management rules of return values from functions,
   * this function duplicates the poa manager return value before
   * returning it.
   *
   * @return POAManager pointer which has been duplicated.  Caller
   *         must release pointer when done.
   */
  PortableServer::POAManager_ptr poa_manager (void);

protected:
  /// The ORB.
  CORBA::ORB_var orb_;

  /// The POA for this ORB.
  PortableServer::POA_var poa_;

  /// Child poa under the root POA.
  PortableServer::POA_var child_poa_;

  /// The POA manager of poa_.
  PortableServer::POAManager_var poa_manager_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ORB_MANAGER_H */
