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
#include "tao/ORB_Core.h"

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
      CORBA::String_var located =
        this->imr_locator_.activate_server_by_object (object_key);
      return located._retn();
    }
  catch (CORBA::Exception &)
    {
      throw CORBA::TRANSIENT (CORBA::SystemException::_tao_minor_code
                              (TAO_IMPLREPO_MINOR_CODE, 0),
                              CORBA::COMPLETED_NO);
    }
}

void
INS_Locator::async_locate (::IORTable::Locate_ResponseHandler handler,
                           const char* object_key)
{
  ACE_ASSERT (object_key != 0);

  Server_Info_Ptr si;
  ACE_CString key;
  ACE_CString full (object_key);
  if (this->imr_locator_.split_key (full, key, si))
    {
      ImR_ResponseHandler *rh;
      ACE_NEW (rh, INS_Loc_ResponseHandler (key.c_str(), handler));
      this->imr_locator_.activate_server_by_info (si, rh);
    }
  else
    {
      handler->raise_excep (CORBA::TRANSIENT (CORBA::SystemException::_tao_minor_code
                                              (TAO_IMPLREPO_MINOR_CODE, 0),
                                              CORBA::COMPLETED_NO));
    }
}


//----------------------------------------------------------------------------------------
INS_Loc_ResponseHandler::INS_Loc_ResponseHandler (const char *key,
                            ::IORTable::Locate_ResponseHandler handler)
  : key_(key),
    rh_ (handler)
{
}

void
INS_Loc_ResponseHandler::send_ior (const char *pior)
{
  ACE_CString ior = pior;
  ior += key_;
  rh_->forward_ior (ior.c_str(), false);
  delete this;
}

void
INS_Loc_ResponseHandler::send_exception (CORBA::Exception *ex)
{
  delete ex;
  rh_->raise_excep (CORBA::TRANSIENT (CORBA::SystemException::_tao_minor_code
                                              (TAO_IMPLREPO_MINOR_CODE, 0),
                                              CORBA::COMPLETED_NO));
  delete this;
}
