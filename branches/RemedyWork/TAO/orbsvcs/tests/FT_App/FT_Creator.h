/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_Creator.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Main wrapped around TAO_Object_Group_Creator
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef FT_CREATOR_H
#define FT_CREATOR_H

#include /**/ "ace/pre.h"

#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_Object_Group_Creator.h"
#include "ace/OS_NS_stdio.h"

namespace FTAPP
{
  class FT_Creator
  {
    typedef ACE_Vector<ACE_CString> StringVec;
   public:
    ///////////////////////////
    // construction/destruction
    FT_Creator ();

    ~FT_Creator ();

    /////////////////
    // initialization
    int parse_args (int argc, ACE_TCHAR *argv[]);

    int init (CORBA::ORB_ptr orb);

    ////////////
    // execution
    int run (void);

    ////////////
    // shut down
    int fini ();

    /////////////////
    // implementation
  private:
    void usage (FILE* out)const;

    ////////////////////
    // forbidden methods
   private:
      FT_Creator (const FT_Creator & rhs);
      FT_Creator & operator = (const FT_Creator & rhs);

    ////////////////
    // Data members
   private:

    TAO::Object_Group_Creator creator_;
    CORBA::ORB_var orb_;
    const ACE_TCHAR * registry_ior_;
    StringVec create_roles_;
    StringVec unregister_roles_;


    CosNaming::NamingContext_var naming_context_;

    ::FT::ReplicationManager_var replication_manager_;
    /**
     * bool: true if we have a real replication manager
     */
    int have_replication_manager_;

    /**
     * bool:  true if we should write individual IOR files
     */
    int write_iors_;

    /**
     * bool:  true if we should write IOGR to a file
     */
    int write_iogr_;

    /**
     * bool:  true if we should write IOGR to a Name Service
     */
    int ns_register_;

    /**
     * sequence number applied to created IOGRs
     */
    size_t iogr_seq_;

    /**
     * prefix for names
     */
    ACE_CString prefix_;


  };

} // namespace TAO

#include /**/ "ace/post.h"

#endif // FT_CREATOR_H
