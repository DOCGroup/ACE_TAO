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

#if !defined (NS_COSNAMING_H)
#define NS_COSNAMING_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"

class TAO_ORBSVCS_Export NS_IntId 
  // = TITLE
  //     Stores information a context keeps for each bound name
  //     (object reference and the type of binding).
  //
  // = DESCRIPTION
  //
  //
  //
  //
{
public:
  // = Initialization and termination methods.
  NS_IntId (void);
  // default constructor.

  NS_IntId (CORBA::Object_ptr obj,
	    CosNaming::BindingType type = CosNaming::nobject);
  // constructor.
  
  NS_IntId (const NS_IntId & rhs);
  // copy constructor

  ~NS_IntId (void);
  // destructor 

  void operator= (const NS_IntId & rhs);
  // Assignment operator (does copy memory).

  CORBA::Object_ptr ref_;   
  // CORBA object reference of the bound object.

  CosNaming::BindingType type_;  
  // Indicator of whether the object is a NamingContext that should
  // participate in name resolution when compound names are used.
};

class TAO_ORBSVCS_Export NS_ExtId 
  // = TITLE
  //    Stores the name to which an object is bound.
  //
  // = DESCRIPTION
  //
  //
  //
  //
{
public:
  // = Initialization and termination methods.
  NS_ExtId (void);
  // default constructor.

  NS_ExtId (const char *id, 
	    const char *kind);
  // constructor.
  
  NS_ExtId (const NS_ExtId & rhs);
  // copy constructor

  ~NS_ExtId (void);
  // destructor 

  void operator= (const NS_ExtId & rhs);
  // Assignment operator (does copy memory).

  int operator== (const NS_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const NS_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // This class has to have a hash for use with ACE_Hash_Map_Manager.

  ACE_CString kind_;  
  // any information user wants to store (not used by Naming Service).

  ACE_CString id_;  
  // any information user wants to store (not used by Naming Service).
};


#endif /* NS_COSNAMING_H */
