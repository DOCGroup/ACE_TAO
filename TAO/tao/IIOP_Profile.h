// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#ifndef IIOP_PROFILE_H
#  define IIOP_PROFILE_H

// TAO IIOP_Profile concrete Profile definitions
class TAO_Export TAO_IIOP_Profile : public TAO_Profile
{
public:
  enum
    {
      DEF_IIOP_MAJOR = 1,
      DEF_IIOP_MINOR = 0
    };

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
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const char* host,
                    CORBA::UShort port,
                    const Version& version,
                    const TAO_ObjectKey& object_key);

  TAO_IIOP_Profile (const TAO_IIOP_Profile *pfile);

  TAO_IIOP_Profile (const TAO_IIOP_Profile &pfile);
  
  TAO_IIOP_Profile (const Version& version);

  TAO_IIOP_Profile (void);

  ~TAO_IIOP_Profile (void);

  CORBA::ULong tag (void);
  // The tag, each concrete class will have a specific tag value.

  TAO_Transport* transport (void);

  int parse (TAO_InputCDR& cdr, 
             CORBA::Boolean& continue_decoding, 
             CORBA::Environment &env);

  int parse_string (const char *string, CORBA::Environment &env);

  const TAO_ObjectKey &object_key (void) const;
  TAO_ObjectKey &object_key (TAO_ObjectKey& objkey);
  // @@ depricated
  TAO_ObjectKey *_key (CORBA::Environment &env);

  const TAO_opaque& body (void) const;
  // Create IIOP_Profile Object from marshalled data.


  CORBA::Boolean is_equivalent (TAO_Profile *other_profile,
                                CORBA::Environment &env);


  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &env);

  char *addr_to_string(void);

  ACE_INET_Addr &object_addr (const ACE_INET_Addr *addr);
  ACE_INET_Addr &object_addr (void);

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

  // @@ Move these to privte when decode is done!
  char *host_;
  // Maybe just a <char*> to reduce memory allocation..
  CORBA::UShort port_;
  // TCP port number

private:
  int set (const ACE_INET_Addr& addr);
  // helper method to set the INET_Addr.

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
};

#endif  /* TAO_PROFILE_H */
