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

    void
    Base_Handler::populate_string (const ::XMLSchema::string <char> &str,
                                   CORBA::String_out &tofill)
    {
      tofill =
        CORBA::string_dup (str.c_str ());
    }

    void
    Base_Handler::populate_string_seq (const ::XMLSchema::string <char> &str,
                                       CORBA::StringSeq &tofill)
    {
      tofill.length (tofill.length () + 1);
      tofill[tofill.length () - 1] =
        CORBA::string_dup (str.c_str ());
    }

    bool
    Base_Handler::bind_ref (ACE_CString& id, size_t index)
    {
      int retval =
        this->idref_map_.bind (id, index);

      if (return < 0)
        return false;

      return true;
    }

    bool
    Base_Handler::find_ref (ACE_CString& id, size_t val)
    {
      int retval =
        this->idref_map_.find (id, val);

      if (return < 0)
        return false;

      return true;
    }

  }
}
