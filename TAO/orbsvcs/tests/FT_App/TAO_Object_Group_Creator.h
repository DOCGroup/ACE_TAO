/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    TAO_Object_Group_Creator.h
 *
 *  $Id$
 *
 *  This file is part of Fault Tolerant CORBA.
 *  Utility to Create Object Group
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_OBJECT_GROUP_CREATOR_H
#define TAO_OBJECT_GROUP_CREATOR_H
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <orbsvcs/FT_ReplicationManagerC.h>
#include <orbsvcs/PortableGroupC.h>
#include <ace/SString.h>
#include <ace/Vector_T.h>

namespace TAO
{
  class Object_Group_Creator
  {
    typedef ACE_Vector<ACE_CString> StringVec;
   public:
    ///////////////////////////
    // construction/destruction
    Object_Group_Creator ();

    ~Object_Group_Creator ();

    /////////////////
    // initialization
    int parse_args (int argc, char *argv[]);

    int init (CORBA::ORB_var & orb ACE_ENV_ARG_DECL);

    ////////////
    // execution
    int run (ACE_ENV_SINGLE_ARG_DECL);

    ////////////
    // shut down
    int fini ();

    /////////////////
    // implementation
  private:
    void usage (ostream & out)const;
    int write_ior_file(const char * outputFile, const char * ior);

    int create_group(const char * role ACE_ENV_ARG_DECL);
    int unregister_role(const char * role ACE_ENV_ARG_DECL);

    ////////////////////
    // forbidden methods
   private:
      Object_Group_Creator (const Object_Group_Creator & rhs);
      Object_Group_Creator & operator = (const Object_Group_Creator & rhs);

    ////////////////
    // Data members
   private:
    CORBA::ORB_var orb_;
    const char * registry_filename_;
    PortableGroup::FactoryRegistry_var registry_;
    StringVec create_roles_;
    StringVec unregister_roles_;


  ::FT::ReplicationManager_var replication_manager_;
    /**
     * bool: true if we have a real replication manager
     */
    int have_replication_manager_;

    /**
     * bool: true if we should create IOGR's
     */
    int make_iogr_;

    /**
     * bool:  true if we should write individual IOR files
     */
    int write_iors_;

    /**
     * sequence number applied to created IOGRs
     */
    unsigned long iogr_seq_;

  };

} // namespace TAO

#endif // TAO_OBJECT_GROUP_CREATOR_H
