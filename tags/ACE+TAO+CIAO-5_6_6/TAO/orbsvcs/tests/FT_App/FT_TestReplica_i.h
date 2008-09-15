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
#include /**/ "ace/pre.h"

#include <ace/ACE.h>
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

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
  int init (CORBA::ORB_var & orb);

  /**
   * Prepare to exit.
   * @return zero for success; nonzero is process return code for failure.
   */
  int fini (void);

  /**
   * idle time activity.
   * @param result [out] status code to return from process
   * @returns 0 to continue; nonzero to quit
   */
  int idle(int &result);

  void request_quit();

  unsigned long factory_id()const;

  static const char * repository_id();

  ::FT_TEST::TestReplica_ptr object_reference();

  PortableServer::ObjectId object_id()const;

  //////////////////////////////////////////
  // Override CORBA servant virtual methods
  virtual PortableServer::POA_ptr _default_POA (void);

  virtual void _remove_ref (void);

private:
  ///////////////////////////
  // override Replica methods
  virtual void set (CORBA::Long value);

  virtual CORBA::Long increment (CORBA::Long delta);

  virtual CORBA::Long get (void);

  virtual CORBA::Long counter (void);

  virtual void counter (CORBA::Long counter);

  virtual void die (FT_TEST::TestReplica::Bane when);

  virtual void shutdown (void);

  ///////////////////////////
  // override PullMonitorable

  virtual CORBA::Boolean is_alive (void);

  ///////////////////////////
  // override Updatable

  virtual ::FT::State * get_update (void);

  virtual void set_update (const FT::State & s);

  virtual ::FT::State * get_state (void);

  virtual void set_state (const FT::State & s);

  ////////////////
  // Implement TAO_UpdateObjectGroup

  virtual void tao_update_object_group (
      const char * iogr,
      PortableGroup::ObjectGroupRefVersion version,
      CORBA::Boolean is_primary
    );

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
   * @param value the data to be stored.
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
   * The factory that created this replica
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
