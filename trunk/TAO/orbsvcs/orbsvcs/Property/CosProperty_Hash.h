/* -*- C++ -*- */

//$Id$

// ===================================================================================================
//
// = FILE
//     CosProperty_Hash.h
//
// = DESCRITION
//     Defines INT_ID and EXT_ID classes for the Hash_Map_Manager to
//     be used in the CosPropertyService. 
//
// = AUTHOR
//     Alexander B. Arulanthu <alex@cs.wustl.edu>
//
// ===================================================================================================

#if !defined (COSPROPERTY_HASH_H)
#define	COSPROPERTY_HASH_H

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "CosPropertyServiceS.h"

class EXT_ID 
{
  // = TITLE
  //     Key for the Hash Table.
public: 
  EXT_ID (void);
  // Default constructor
  
  EXT_ID (const CosPropertyService::PropertyName &name);
  // Costructor that takes PropertyName

  //EXT_ID (const EXT_ID &ext_id);
  // Copy constructor
  
  virtual int operator == (const EXT_ID &ext_id) const;
  // The vital operator for hash binding and "find"ing.

  virtual u_long hash (void) const;
  // The vital function

  virtual ~EXT_ID (void);
  // Destructor

  CosPropertyService::PropertyName_var pname_; 
  // This is still the public variable... 
};

class INT_ID
{
  // = TITLE 
  //    This is the Hash Entry.
public: 
  INT_ID (void);
  // Default constructor 
  
  INT_ID (const CORBA::Any &value);
  // Constructor with Any value

  //INT_ID (const INT_ID &int_id);
  // Copy constructor
  
  virtual ~INT_ID (void);
  // Destructor

  CORBA::Any pvalue_;
  // Public data -- the property value
};

#endif  /* COSPROPERTY_HASH_H */





