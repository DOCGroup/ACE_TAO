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
//     Alexander Babu Arulanthu
// ================================================================================================

#include "CosProperty_Hash.h"

EXT_ID::EXT_ID ()
{
}

EXT_ID::EXT_ID (const CosPropertyService::PropertyName &name)
  :  pname_ (name)
{
}

int
EXT_ID::operator == (const EXT_ID &ext_id) const   
{
  ACE_DEBUG ( (LM_DEBUG, "EXT_ID::operator == : %s == %s \n", this->pname_.in (), ext_id.pname_.in ()) );
  return ( (ACE_OS::strcmp(this->pname_.in (), ext_id.pname_.in ()) == 0) );
}

long  unsigned int 
EXT_ID::hash (void) const 
{
  unsigned long ret;
  
  ret =  ACE::hash_pjw (this->pname_.in()); 
  ACE_DEBUG ( (LM_DEBUG, "EXT_ID::hash : hasing %s : val : %d \n", this->pname_.in (), ret) ); 
  return ret;
}

EXT_ID::~EXT_ID () 
{
}

INT_ID::INT_ID ()
{
}

INT_ID::INT_ID (const CORBA::Any &anyvalue)
{
  TAO_TRY
    {
      pvalue_.replace (anyvalue.type (), anyvalue.value (), CORBA::B_FALSE, TAO_TRY_ENV);
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
    }
  TAO_ENDTRY;
}

INT_ID::~INT_ID () 
{

}

/*
long unsigned int
Hash_Property_Map::hash (const EXT_ID &ext_id) 
{
  return ACE::hash_pjw (ext_id.pname_.in ());
}


int
Hash_Property_Map::equal (const EXT_ID &id1, const EXT_ID &id2)
{
  return (id1==id2);
}
*/

/*
static const int MAX_HASH=256;

int 
main (int argc, char *argv[]) 
{
  Hash_Property_Map hash (MAX_HASH);
  
  cout << "CosProperty_Hash" << endl;
  
  return 0;
}  

*/








