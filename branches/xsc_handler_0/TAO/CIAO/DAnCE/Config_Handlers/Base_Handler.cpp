// $Id$

#include "Base_Handler.h"
#include "tao/CORBA_String.h"
#include "tao/StringSeqC.h"

namespace CIAO
{
  namespace Config_Handlers
  {

    Base_Handler::Base_Handler (void)
    {
    }

    Base_Handler::~Base_Handler (void)
    {
    }

    // This should probably be inline.
    // Copies the string provided into the provided CORBA string.
    void 
    Base_Handler::handle_string (const ::XMLSchema::string <char> &str, 
                                 CORBA::String_var &tofill)
    {
      tofill = CORBA::string_dup (str.c_str ());
    }
    
    
    /// Appends the string provided to the provided StringSeq.
    void 
    Base_Handler::handle_string_seq (::XMLSchema::string <char> &str, 
                                     CORBA::StringSeq &tofill)
    {
      tofill.length (tofill.length () + 1);
      tofill[tofill.length () - 1] = 
        CORBA::string_dup (str.c_str ());
    }
    
    
    /// The Deployment spec references elements by 
    /// their position within their parent sequence.
    /// These two methods allow an element's index
    /// to be stored/retrieved.
    void 
    Base_Handler::bind_ref (ACE_TString& id, size_t index)
    {
      idref_map_.bind (id,index); 
    }
    
    int 
    Base_Handler::get_ref (ACE_TString& id, size_t val)
    {
      return idref_map_.find (id,val);
    }

  }
}
