/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_TestReplica_i.h
 *
 *  $Id$
 *
 *  This file declares an implementation of CORBA interface TestReplica.
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
//
#ifndef FT_TESTREPLICA_I_H_
#define FT_TESTREPLICA_I_H_
#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <ace/pre.h>
#include "FT_TestReplicaS.h"

////////////////////
// Forward Reference
class FT_ReplicaFactory_i;

/**
 * Implement the TestReplica IDL interface.
 *
 * Persistent storage simulated by storing the counter
 * in Persistent.dat.
 */
class FT_TestReplica_i : public virtual POA_FT_TEST::TestReplica
{
public:
  FT_TestReplica_i (FT_ReplicaFactory_i * factory, const char * name, unsigned long factory_id);
  virtual ~FT_TestReplica_i ();

  /**
   * parse command line arguments.
   * remove any that are recognized, adjusting argc accordingly.
   * @param argc classic C argc
   * @param argv classic C argv
   * @return 0 if ok, otherwise process exit code.
   */
  int parse_args (int argc, char *argv[]);

  /**
   * provide information to appear in a "usage" display.
   * caller will display:
   *  usage: [program] &lt;usageMessage()&gt; [other usage]
   * @returns c-style string containing "bare" usage options.
   */
  const char * usage_options();

  /**
   * Initialize this object.
   * @param orbManager our ORB -- we keep var to it.
   * @return zero for success; nonzero is process return code for failure.
   */
  int init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL);

  /**
   * Prepare to exit.
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * idle time activity.
   * @param result [out] status code to return from process
   * @returns 0 to continue; nonzero to quit
   */
  int idle(int &result);

  void request_quit();


  unsigned long factory_id()const;

  ::FT_TEST::TestReplica_ptr object_reference();

  PortableServer::ObjectId object_id()const;

  //////////////////////////////////////////
  // Override CORBA servant virtual methods
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL);

  virtual void FT_TestReplica_i::_remove_ref (ACE_ENV_SINGLE_ARG_DECL);

private:
  ///////////////////////////
  // override Replica methods
  virtual void set (CORBA::Long value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC (( CORBA::SystemException));

  virtual CORBA::Long increment (CORBA::Long delta
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Long get (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Long counter (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void counter (CORBA::Long counter
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void die (FT_TEST::TestReplica::Bane when
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ///////////////////////////
  // override PullMonitorable

  virtual CORBA::Boolean is_alive (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ///////////////////////////
  // override Updatable

  virtual ::FT::State * get_update ()
    ACE_THROW_SPEC ((CORBA::SystemException, FT::NoUpdateAvailable));

  virtual void set_update (const FT::State & s)
    ACE_THROW_SPEC ((CORBA::SystemException, FT::InvalidUpdate));

  virtual ::FT::State * get_state ()
    ACE_THROW_SPEC ((CORBA::SystemException, FT::NoStateAvailable));

  virtual void set_state (const FT::State & s)
    ACE_THROW_SPEC ((CORBA::SystemException, FT::InvalidState));

  /////////////////
  // implementation
private:
  /**
   * Load the persistent data.
   * returns the data loaded.
   */
  long load();
  /**
   * Store the persistent data.
   * @param the data to be stored.
   */
  void store(long value);

  void suicide(const char *);

  ///////////////
  // data members
private:
  /**
   * a bane code for when we should die.
   */
  FT_TEST::TestReplica::Bane death_pending_;

  /**
   * verbosity level, settable by client.
   */
  int verbose_;

  /**
   * who am I?
   */
  ACE_CString name_;

  /**
   * The ID number assigned by the factory
   */
  unsigned long factory_id_;

  /**
   * the factory that created thsi replica
   */
  FT_ReplicaFactory_i * factory_;

  /**
   * The orb
   */
  CORBA::ORB_var orb_;

  /**
   * The POA used to activate this object.
   */
  PortableServer::POA_var poa_;

  /**
   * The CORBA object id assigned to this object.
   */
  PortableServer::ObjectId_var object_id_;

};

#include /**/ "ace/post.h"

#endif /* FT_TESTREPLICA_I_H_  */
