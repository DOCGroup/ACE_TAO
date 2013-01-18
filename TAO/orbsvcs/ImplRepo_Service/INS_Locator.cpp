//=============================================================================
/**
*  @file   INS_Locator.cpp
*
*  $Id$
*
*  @brief  Implementation of the ImR's INS Locator class
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include "INS_Locator.h"
#include "ImR_Locator_i.h"
#include "tao/ORB_Constants.h"

INS_Locator::INS_Locator (ImR_Locator_i& loc)
: imr_locator_ (loc)
{
}

char *
INS_Locator::locate (const char* object_key)
{
  ACE_ASSERT (object_key != 0);
  try
    {
      ACE_CString key (object_key);
      ssize_t poaidx = key.find ('/');
      if (poaidx >= 0)
      {
        key = key.substring (0, poaidx);
      }

      if (imr_locator_.debug () > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR: Activating server <%s>.\n", key.c_str ()));

      CORBA::String_var located =
        this->imr_locator_.activate_server_by_object (key.c_str ());

      ACE_CString tmp = located.in ();
      tmp += object_key;

      if (imr_locator_.debug () > 0)
        ACE_DEBUG ((LM_DEBUG, "ImR: Forwarding invocation on <%s> to <%s>\n", key.c_str (), tmp.c_str()));

      return CORBA::string_dup (tmp.c_str ());
    }
  catch (const ImplementationRepository::CannotActivate&)
    {
      throw CORBA::TRANSIENT (
        CORBA::SystemException::_tao_minor_code (
          TAO_IMPLREPO_MINOR_CODE,
          0),
        CORBA::COMPLETED_NO);
    }
  catch (const ImplementationRepository::NotFound&)
    {
      ACE_CString objkey (object_key);

      // check to see if there are more slashes beyond the first one
      ssize_t poaidx2 = objkey.rfind ('/');
      ssize_t poaidx1 = objkey.find ('/');
      if (poaidx1 <= 0 || poaidx2 <= 0 || poaidx1 == poaidx2)
        {
          throw CORBA::TRANSIENT (
            CORBA::SystemException::_tao_minor_code (
              TAO_IMPLREPO_MINOR_CODE,
              0),
            CORBA::COMPLETED_NO);
        }

      // remove the data field beyond the last delimiter
      // which is assumed to be the object id.
      ACE_CString key2 = objkey.substring(0, poaidx2);
      while (poaidx2 > poaidx1)
        {
          try
            {
              if (imr_locator_.debug () > 1)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("ImR: Try activating server using <%s>.\n"),
                    key2.c_str ()));
                }

              CORBA::String_var located =
		this->imr_locator_.activate_server_by_object (key2.c_str ());

              ACE_CString tmp = located.in ();
              tmp += object_key;

              if (imr_locator_.debug () > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
			      ACE_TEXT ("ImR: Forwarding invocation on <%s> ")
			      ACE_TEXT ("to <%s>\n"), key2.c_str (), tmp.c_str()));
                }

              return CORBA::string_dup (tmp.c_str ());
            }
          catch (const ImplementationRepository::CannotActivate&)
            {
              throw CORBA::TRANSIENT (
                CORBA::SystemException::_tao_minor_code (
                  TAO_IMPLREPO_MINOR_CODE,
                  0),
                CORBA::COMPLETED_NO);
            }
          catch (const ImplementationRepository::NotFound&)
            {
              poaidx2 = key2.rfind ('/');
              if (poaidx2 > poaidx1)
                {
                  // continue to try again
                  key2 = key2.substring(0, poaidx2);
                  continue;
                }
              else
                {
                  break;
                }
            }
        } // while()

      //  No match is found
      throw CORBA::TRANSIENT (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_IMPLREPO_MINOR_CODE,
                                0),
                              CORBA::COMPLETED_NO);
    }
}
