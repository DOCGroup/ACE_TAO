// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_Profile.h
//
// = DESCRIPTION
//     IIOP profile specific processing
//
// = AUTHOR
//     Fred Kuhns
//
// ============================================================================

#ifndef TAO_IIOP_PROFILE_H
#define TAO_IIOP_PROFILE_H

#include "ace/Synch.h"
#include "ace/INET_Addr.h"
#include "tao/Pluggable.h"
#include "tao/Object_KeyC.h"

class TAO_Client_Connection_Handler;

// TAO IIOP_Profile concrete Profile definitions
class TAO_Export TAO_IIOP_Profile : public TAO_Profile
{
public:
  enum
    {
      DEF_IIOP_MAJOR = 1,
      DEF_IIOP_MINOR = 0
    };

  static const char *prefix ();
  // return the char string prefix

  TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                    const char *object_key);

  TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                    const Version& version,
                    const char *object_key);

  TAO_IIOP_Profile (const ACE_INET_Addr& addr,
                    const Version& version,
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const char* host,
                    CORBA::UShort port,
                    const TAO_ObjectKey& object_key,
		    const ACE_INET_Addr& addr);

  TAO_IIOP_Profile (const char* host,
                    CORBA::UShort port,
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const char* host,
                    CORBA::UShort port,
                    const Version& version,
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const char* string,
                    CORBA::Environment &env);
  // crate object using string as ior

  TAO_IIOP_Profile (const TAO_IIOP_Profile *pfile);

  TAO_IIOP_Profile (const TAO_IIOP_Profile &pfile);
  
  TAO_IIOP_Profile (const Version& version);

  TAO_IIOP_Profile (void);
  // All of the various creator methods ...

  ~TAO_IIOP_Profile (void);
  // Destructor is to be called only through _decr_refcnt()

  CORBA::ULong tag (void);
  // The tag, each concrete class will have a specific tag value.
  // for example we are TAO_IOP_TAG_INTERNET_IOP

  TAO_Transport* transport (void);
  // return a pointer to the underlying transport object.
  // this will provide access to lower layer protocols
  // and processing.

  int parse (TAO_InputCDR& cdr, 
             CORBA::Boolean& continue_decoding, 
             CORBA::Environment &env);
  // initialize this object using the given CDR octet string

  int parse_string (const char *string, CORBA::Environment &env);
  // initialize this object using the given input string

  CORBA::String to_string (CORBA::Environment &env);
  // return a string representation for this profile.
  // client must deallocate memory.

  const TAO_opaque& body (void) const;
  // Create IIOP_Profile Object from marshalled data.

  CORBA::TypeCode::traverse_status encode (TAO_OutputCDR *&stream,
                                           CORBA::Environment &env);

  const TAO_ObjectKey &object_key (void) const;
  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ depricated
  TAO_ObjectKey *_key (CORBA::Environment &env);

  virtual void fwd_profiles (TAO_MProfile *mprofiles);
  // object will assume ownership for this object!!

  virtual TAO_MProfile *get_fwd_profiles (void);
  // copy of MProfile, user must delete.

  CORBA::Boolean is_equivalent (TAO_Profile *other_profile,
                                CORBA::Environment &env);


  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &env);

  char *addr_to_string(void);

  ACE_Addr &object_addr (const ACE_Addr *addr);
  ACE_Addr &object_addr (void);

  char *host (void);
  char *host (const char *h);

  CORBA::UShort port (void);
  CORBA::UShort port (CORBA::UShort p);

  Version *version (void);
  Version *version (Version *v);

  TAO_Client_Connection_Handler *&hint (void);
  void reset_hint (void);

  TAO_Profile *_nil (void);
  TAO_IIOP_Profile &operator= (const TAO_IIOP_Profile &src);

  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  // @@ Move these to privte when decode is done!
  char *host_;
  // Maybe just a <char*> to reduce memory allocation..
  CORBA::UShort port_;
  // TCP port number

private:
  int set (const ACE_INET_Addr& addr);
  // helper method to set the INET_Addr.

  virtual TAO_MProfile *fwd_profiles (void);
  // this object keeps ownership of this object
  // NOT THREAD SAFE

  void create_body (void);
  // does the work for add_profile.

private:
  CORBA::ULong tag_;
  // The tag, 

  TAO_opaque body_;
  // marshaled profile (CDR).

  Version version_;
  // IIOP version number.

  TAO_ObjectKey object_key_;
  // object_key associated with this profile.

  ACE_INET_Addr object_addr_;
  // Cached instance of <ACE_INET_Addr> for use in making 
  // invocations, etc.
  
  TAO_Client_Connection_Handler *hint_;

  ACE_SYNCH_MUTEX refcount_lock_;
  // Mutex to protect reference count

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  TAO_MProfile *fwd_profiles_;
  // list of profiles which we should try forwarding on.

};

#endif  /* TAO_IIOP_PROFILE_H */
