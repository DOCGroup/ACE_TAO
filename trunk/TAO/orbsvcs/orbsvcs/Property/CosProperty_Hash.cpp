// $Id$

// ===============================================================================================
//
// = FILE
//     CosProperty_Hash.cpp
//
// = Description
//     Defines equal and hash functions for the Hash_Property_Map class.
//    
// = Author
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ================================================================================================

#include "CosProperty_Hash.h"

CosProperty_Hash_Key::CosProperty_Hash_Key (void)
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const char * &name)
{
  pname_ = CORBA::string_dup (name);
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const CosPropertyService::PropertyName &name)
  : pname_ (CORBA::string_dup (name))
{
}

CosProperty_Hash_Key::CosProperty_Hash_Key (const CosProperty_Hash_Key &src) 
  : pname_ (src.pname_)
{
}

int
CosProperty_Hash_Key::operator == (const CosProperty_Hash_Key &hash_key) const   
{
  ACE_DEBUG ((LM_DEBUG,
              "CosProperty_Hash_Key::operator == : %s == %s \n",
              this->pname_.in (),
              hash_key.pname_.in ()));
  return ACE_OS::strcmp (this->pname_.in (),
                         hash_key.pname_.in ()) == 0;
}

u_long
CosProperty_Hash_Key::hash (void) const 
{
  u_long ret = ACE::hash_pjw (this->pname_.in ());

  ACE_DEBUG ((LM_DEBUG, 
              "CosProperty_Hash_Key::hash : hashing %s : val : %d \n",
              this->pname_.in (),
              ret));
  return ret;
}

CosProperty_Hash_Key::~CosProperty_Hash_Key (void) 
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (void)
{
}

#if 0
CosProperty_Hash_Value::CosProperty_Hash_Value (const CORBA::Any &any)
  : pvalue_ (new CORBA::Any (any))
{
}
#endif /* 0 */

CosProperty_Hash_Value::CosProperty_Hash_Value (const CORBA::Any &any, 
                                                const CosPropertyService::PropertyModeType &mode)
  : pvalue_ (new CORBA::Any (any)),
    pmode_ (mode)
{
}

CosProperty_Hash_Value::CosProperty_Hash_Value (const CosProperty_Hash_Value &src)
  : pvalue_ (src.pvalue_),
    pmode_ (src.pmode_)
{
}

CosProperty_Hash_Value::~CosProperty_Hash_Value (void)
{
}






