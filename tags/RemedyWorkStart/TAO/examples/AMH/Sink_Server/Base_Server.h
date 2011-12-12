// -*- C++ -*-

// =========================================================================
/**
 *  @file  Base_Server.h
 *
 *  $Id$
 *
 *  @desc  Implements common CORBA server ORB and POA initialisations.
 *
 *  @author Mayur Deshpande <mayur@ics.uci.edu>
 *
 */
// =========================================================================


#ifndef BASE_SERVER_H
#define BASE_SERVER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

#include "tao/ORB.h"

// Forward declaration of the kind of servant we are willing to handle
// in this server
class AMH_Servant;

/**
 Class that performs all 'dirty' initialisation work that is common to
 all the AMH servers and 'hides' all the common ORB functions.
 */
class Base_Server
{
public:
  Base_Server (int &argc, ACE_TCHAR **argv);

  //    Is there any reason to make the destructor virtual?  Will you
  //    ever be destroying a derived class through a pointer to
  //    Base_Server?
  //
  // Mayur: I like to code 'defensively': This class already has
  // virtual methods (it is a base class after all!).  Making the
  // destructor won't add any additional overhead (other than one more
  // entry in the vtbl) and if, by chance, someone does delete a
  // derived-class though this classes ptr, we are still safe.  The
  // short answer to your question is : I don't know. :)
  virtual ~Base_Server (void);

  // Just parse the command line for the output-file (-o) option.
  // return -1 if -o option is not present
  virtual int parse_args (void);

  /// try and schedule using FIFO
  void try_RT_scheduling (void);

  /// ORB inititalisation stuff
  int start_orb_and_poa (void);

  int shutdown_orb_and_poa (void);

  /// register the servant with the poa
  virtual void register_servant (AMH_Servant *servant);

  /// orb-perform_work () abstraction
  virtual void run_event_loop (void);

public:
  // @@ Mayur, please put inlined methods in a separate `.inl' file,
  //    as detailed in the ACE/TAO coding/style guidelines, and as per
  //    our conventions.
  // Mayur: A seperate file for just one method? AAAGH. OK.

  /// Accesor method (for servants) to the initialised ORB
  CORBA::ORB_ptr orb (void);

protected:
  int &argc_;
  ACE_TCHAR **argv_;
  const ACE_TCHAR *ior_output_file_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

private:
  /// Write servant IOR to file specified with the '-o' option
  int write_ior_to_file (const char * ior);
};

#if defined (__ACE_INLINE__)
# include "Base_Server.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* BASE_SERVER_H */
