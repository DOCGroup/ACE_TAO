// Web_Locator.cpp
// $Id$

#if !defined (ACE_WEB_LOCATOR_C)
#define ACE_WEB_LOCATOR_C

#define ACE_BUILD_DLL
#include "Web_Locator.h"

#if !defined (__ACE_INLINE__)
#include "Web_Locator.i"
#endif /* __ACE_INLINE__ */

ACE_URL_Locator::ACE_URL_Locator (ACE_HANDLE handle,
				  const ACE_Time_Value &timeout)
  : errno_ (0),
    timeout_ (timeout),
    handle_ (handle)
{
}

int
ACE_URL_Locator_Query::query (const ACE_Specified_Property how,
			      const ACE_URL_Property_Seq *pseq,
			      const int how_many,
			      ACE_URL_Offer_Seq *offer)
{
  // First send out request


  // Then, receive offers or error code.

  return -1;
}

ACE_URL_OfferID
ACE_URL_Locator_Register::export (const ACE_URL_Offer *img)
{
  // Set Registration request

  // Receive an ID or an error code.

  return 0;
}


int
ACE_URL_Locator_Register::withdraw (const ACE_URL_OfferID offer)
{
  // Send request

  // Recieve a success/fail result

  return -1;
}

ACE_URL_Offer *
ACE_URL_Locator_Register::describe (const ACE_URL_OfferID offer)
{
  // Send request

  // Receive and construct an offer
  return 0;
}

int
ACE_URL_Locator_Register::modify (const ACE_URL_OfferID offer,
				  const char *url,
				  const ACE_URL_Property_Seq *del,
				  const ACE_URL_Property_Seq *modify)
{
  // Send modification request

  // Receive a succeess/fail result
  return -1;
}

#endif /* ACE_WEB_LOCATOR_C */
