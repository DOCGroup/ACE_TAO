/* -*- C++ -*- */

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

#include "PortableServer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

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
class TAO_PortableServer_Export TAO_ORB_Manager
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
  TAO_ORB_Manager (CORBA::ORB_ptr orb = 0,
                   PortableServer::POA_ptr poa = 0,
                   PortableServer::POAManager_ptr poa_manager = 0);

  /** Initialize the ORB/root POA, using the supplied command line
   *  arguments or the default ORB components.
   *
   *  @return -1 on failure, 0 on success
   */
  int init (int &argc,
            char *argv[]
            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /** Initialize the ORB/root POA, using the supplied command line
    * arguments or the default ORB components.
    *
    * @return -1 on failure, 0 on success
    */
  int init (int &argc,
            char *argv[],
            const char *orb_name
            ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Creates a child poa under the root poa with PERSISTENT and
   * USER_ID policies.  Call this if you want a <child_poa> with the
   * above policies, otherwise call init.
   *
   * @return -1 on failure, 0 on success
   */
  int init_child_poa (int &argc,
                      char *argv[],
                      const char *poa_name
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Creates a child poa under the root poa with PERSISTENT and
   * USER_ID policies.  Call this if you want a <child_poa> with the
   * above policies, otherwise call init.  Returns -1 on failure.
   */
  int init_child_poa (int &argc,
                      char *argv[],
                      const char *poa_name,
                      const char *orb_name
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /** Shut down.  Invoke the destroy() methods on the orb and poa.
   *
   * @return -1 on failure, 0 on success
   */
  int fini (ACE_ENV_SINGLE_ARG_DECL);

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
   *  @return -1 on failure, 0 on success
   */
  int activate_poa_manager (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * Activate <servant>, using the POA <activate_object> call.  Users
   * can call this method multiple times to activate multiple objects.
   *
   * @return 0 on failure, a string representation of the object ID if
   *         successful.  Caller of this method is responsible for
   *         memory deallocation of the string.
   */
  char *activate (PortableServer::Servant servant
                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /** Deactivate object in RootPOA.
   *
   *  @param id  a string representation of the Object ID
   *             of the servant to deactivate in the POA
   */
  void deactivate (const char *id
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

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
                                  PortableServer::Servant servant
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /** Deactivate object in child POA.
   *
   *  @param id  string representation of the object ID, which represents
   *             the object to deactivate in the POA
   */
  void deactivate_under_child_poa (const char *id
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /** Run the ORB event loop with the specified <tv> time value.
   *
   * @param tv  the time interval for how long to run the ORB event loop.
   * @return -1 on failure, 0 on success
   */
  int run (ACE_Time_Value &tv
           ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * Accessor which returns the ORB pointer.  Following the normal
   * CORBA memory management rules of return values from functions,
   * this function duplicates the orb return value before returning
   * it.
   *
   * @return orb  ORB pointer which has been duplicated, so caller
   *              must release pointer when done.
   */
  CORBA::ORB_ptr orb (void);

  /**
   * Accessor which returns the root poa. Following the normal CORBA
   * memory management rules of return values from functions, this
   * function duplicates the poa return value before returning it.
   *
   * @return poa  Root POA pointer which has been duplicated.  Caller
   *              must release pointer when done.
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


#include /**/ "ace/post.h"
#endif /* TAO_ORB_MANAGER_H */
