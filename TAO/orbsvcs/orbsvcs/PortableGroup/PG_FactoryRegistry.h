// -*- C++ -*-
//=============================================================================
/**
 *  @file    PG_FactoryRegistry.h
 *
 *  $Id$
 *
 *  This file declares the implementation of PortableGroup::FactoryRegistry.
 *  Eventually this should be folded into the Fault Tolerance ReplicationManager
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PG_FACTORYREGISTRY_H_
#define TAO_PG_FACTORYREGISTRY_H_
#include /**/ <ace/pre.h>
#include <ace/ACE.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//////////////////////////////////
// Classes declared in this header
namespace TAO
{
  class PG_FactoryRegistry;
}

/////////////////////////////////
// Includes needed by this header
#include <orbsvcs/PortableGroupS.h>
#include <ace/Hash_Map_Manager.h>


/////////////////////
// Forward references

namespace TAO
{
  /**
   * Implement the PortableGroup::FactoryRegistry interface
   * Note FactoryRegistry is not part of the OMG standard.  It was added
   * as part of the TAO implementation of Fault Tolerant CORBA
   */
  class TAO_PortableGroup_Export PG_FactoryRegistry : public virtual POA_PortableGroup::FactoryRegistry
  {
    /* <DESIGN> originally I used FactoryInfos_vars rather than FactoryInfos *,
        but it actually made memory management harder. DLW </DESIGN> */
    typedef ACE_Null_Mutex MapMutex;
    typedef ACE_Hash_Map_Manager <ACE_CString, PortableGroup::FactoryInfos *, MapMutex>  RegistryType;
    typedef ACE_Hash_Map_Entry <ACE_CString, PortableGroup::FactoryInfos *> RegistryType_Entry;
    typedef ACE_Hash_Map_Iterator <ACE_CString, PortableGroup::FactoryInfos *, MapMutex> RegistryType_Iterator;

    //////////////////////
    // non-CORBA interface
  public:
    /// Constructor
    PG_FactoryRegistry (const char * name = "FactoryRegistry");

    /// virtual Destructor
    virtual ~PG_FactoryRegistry (void);

    /**
     * Parse command line arguments.
     * @param argc traditional C argc
     * @param argv traditional C argv
     * @return zero for success; nonzero is process return code for failure.
     */
    int parse_args (int argc, char * argv[]);

    /**
     * Initialize this object.
     * @param orbManager our ORB -- we keep var to it.
     * @return zero for success; nonzero is process return code for failure.
     */
    int init (CORBA::ORB_var & orb  ACE_ENV_ARG_DECL);

    /**
     * Prepare to exit.
     * @return zero for success; nonzero is process return code for failure.
     */
    int fini (ACE_ENV_SINGLE_ARG_DECL);

    /**
     * Processing to happen when the ORB's event loop is idle.
     * @param result is a place to return status to be returned by the process
     * @returns 0 to continue.  1 to quit.
     */
    int idle(int & result);

    /**
     * Identify this object.
     * @return a string to identify this object for logging/console message purposes.
     */
    const char * identity () const;

    /**
     * An object reference to the this object.
     * Duplicated by the call so it may (and probably should) be assigned to a _var..
     */
    ::PortableGroup::FactoryRegistry_ptr reference();

    ////////////////////////////////
    // override servant base methods
    virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

    //////////////////
    // CORBA interface
    // See IDL for documentation

    virtual void register_factory (
        const char * type_id,
        const PortableGroup::FactoryInfo & factory_info
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((CORBA::SystemException, PortableGroup::MemberAlreadyPresent));

    virtual void unregister_factory (
      const char * type_id,
      const PortableGroup::Location & location
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException, PortableGroup::MemberNotFound));

    virtual void unregister_factory_by_type (
      const char * type_id
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void unregister_factory_by_location (
      const PortableGroup::Location & location
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::PortableGroup::FactoryInfos * list_factories_by_type (
      const char * type_id
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    virtual ::PortableGroup::FactoryInfos * list_factories_by_location (
      const PortableGroup::Location & location
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException));

    /////////////////////////
    // Implementation methods
  private:
    /**
     * Write this factory's IOR to a file
     */
    int write_ior_file (const char * outputFile, const char * ior);

    ///////////////
    // Data Members
  private:

    /**
     * A human-readable string to distinguish this from other Notifiers.
     */
    ACE_CString identity_;

    /**
     * Protect internal state.
     * Mutex should be locked by corba methods, or by
     * external (public) methods before calling implementation
     * methods.
     * Implementation methods should assume the mutex is
     * locked if necessary.
     */
    ACE_Mutex internals_;
    typedef ACE_Guard<ACE_Mutex> InternalGuard;

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

    /**
     * This objects identity as a CORBA object.
     */
    CORBA::Object_var this_obj_;

    /**
     * IOR of this object as assigned by poa.
     */
    CORBA::String_var ior_;

    /**
     * A file to which the factory's IOR should be written.
     */
    const char * ior_output_file_;

    /**
     * A name to be used to register the factory with the name service.
     */
    const char * ns_name_;

    CosNaming::NamingContext_var naming_context_;

    CosNaming::Name this_name_;

    /**
     * Quit on idle flag.
     */
    int quit_on_idle_;

    /**
     * State of the quit process
     */
    enum {LIVE, DEACTIVATED, GONE} quit_state_;

    int linger_;

    RegistryType registry_;

  };
} // namespace TAO

#include /**/ <ace/post.h>

#endif // TAO_PG_FACTORYREGISTRY_H_
