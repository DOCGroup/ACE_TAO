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

#include "ace/pre.h"
#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"

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
    Base_Server (int *argc, char **argv);
    virtual ~Base_Server ();

    // Just parse the command line for the output-file (-o) option.
    // return -1 if -o option is not present
    virtual int parse_args (void);

    /// try and schedule using FIFO
    void try_RT_scheduling (void);

    /// ORB inititalisation stuff
    int start_orb_and_poa (void);

    /// register the servant with the poa
    virtual void register_servant (AMH_Servant *servant);

    /// orb-perform_work () abstraction
    virtual void run_event_loop ();

 public:
    /// Accesor method (for servants) to the initialised ORB
    CORBA::ORB_ptr orb () { return this->orb_.in (); }

 protected:
  int *argc_;
  char **argv_;
  char *ior_output_file_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;

 private:
    /// Write servant IOR to file specified with the '-o' option
    int write_ior_to_file (CORBA::String_var ior);
};

#include "ace/post.h"

#endif /* BASE_SERVER_H */
