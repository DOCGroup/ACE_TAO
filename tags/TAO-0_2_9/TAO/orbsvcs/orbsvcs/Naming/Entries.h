/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
// 
// = FILENAME
//   NS_CosNaming.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TAO_ENTRIES_H)
#define TAO_ENTRIES_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"

class TAO_ORBSVCS_Export NS_IntId 
{
  // = TITLE
  //     Stores information a context keeps for each bound name
  //     (object reference and the type of binding).
public:
  // = Initialization and termination methods.
  NS_IntId (void);
  // Constructor.

  NS_IntId (CORBA::Object_ptr obj,
	    CosNaming::BindingType type /* = CosNaming::nobject */);
  // Constructor.
  
  NS_IntId (const NS_IntId & rhs);
  // Copy constructor.

  ~NS_IntId (void);
  // Destructor.

  void operator= (const NS_IntId & rhs);
  // Assignment operator (does copy memory).

  CORBA::Object_ptr ref_;   
  // CORBA object reference of the bound object.

  CosNaming::BindingType type_;  
  // Indicator of whether the object is a NamingContext that should
  // participate in name resolution when compound names are used.
};

class TAO_ORBSVCS_Export NS_ExtId 
{
  // = TITLE
  //    Stores the name to which an object is bound.
public:
  // = Initialization and termination methods.

  NS_ExtId (void);
  // Constructor.

  NS_ExtId (const char *id, 
	    const char *kind);
  // Constructor.
  
  NS_ExtId (const NS_ExtId & rhs);
  // Copy constructor.

  ~NS_ExtId (void);
  // Destructor. 

  void operator= (const NS_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const NS_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const NS_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  ACE_CString kind_;  
  // Any information user wants to store (not used by Naming Service).

  ACE_CString id_;  
  // Any information user wants to store (not used by Naming Service).
};

#endif /* TAO_ENTRIES_H */
