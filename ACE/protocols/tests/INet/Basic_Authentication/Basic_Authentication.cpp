/// @file Basic_Authentication.cpp

#include "ace/INet/HTTP_BasicAuthentication.h"
#include "ace/INet/HTTP_Request.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE::HTTP::Request request;
  ACE::HTTP::BasicAuthentication const original ("user", "password");
  original.set_credentials (request);
  ACE::HTTP::BasicAuthentication const decoded (request);
  if (decoded.user () != "user" || decoded.password () != "password")
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Basic authentication round trip failed\n")), 1);
    }

  request.set_credentials ("Basic", "A");
  ACE::HTTP::BasicAuthentication const invalid (request);
  if (!invalid.user ().empty () || !invalid.password ().empty ())
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid credentials were accepted\n")), 1);
    }
  return 0;
}
