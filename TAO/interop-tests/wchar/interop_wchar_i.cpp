// -*- C++ -*-
#include "interop_wchar_i.h"

// Implementation skeleton constructor
interop_WChar_Passer_i::interop_WChar_Passer_i (CORBA::ORB_ptr o,
                                                int verbose)
  : orb_(CORBA::ORB::_duplicate (o)),
    ref_ (verbose)
{
}

// Implementation skeleton destructor
interop_WChar_Passer_i::~interop_WChar_Passer_i ()
{
}

char *
interop_WChar_Passer_i::orb_name ()
{
  return CORBA::string_dup ("TAO");
}

CORBA::Boolean
interop_WChar_Passer_i::wchar_to_server (CORBA::WChar test,
                                         CORBA::Short key)
{
  return ref_.match_wchar (key,test);
}

CORBA::WChar
interop_WChar_Passer_i::wchar_from_server (CORBA::Short key)
{
  return ref_.get_wchar (key);
}

CORBA::Boolean
interop_WChar_Passer_i::wstring_to_server (const CORBA::WChar * test,
                                           CORBA::Short key)
{
  return ref_.match_wstring(key,test);
}

CORBA::WChar *
interop_WChar_Passer_i::wstring_from_server (CORBA::Short key)
{
  return CORBA::wstring_dup (ref_.get_wstring(key));
}

CORBA::Boolean
interop_WChar_Passer_i::warray_to_server (const interop::warray test,
                                          CORBA::Short key)
{
  return ref_.match_warray(key,test);
}

interop::warray_slice *
interop_WChar_Passer_i::warray_from_server (CORBA::Short key)
{
  return interop::warray_dup (ref_.get_warray(key));
}

CORBA::Boolean
interop_WChar_Passer_i::wstruct_to_server (const interop::wstruct & test,
                                           CORBA::Short key)
{
  return
    ref_.match_wchar (key,test.st_char) &&
    ref_.match_wstring (key,test.st_string) &&
    ref_.match_warray (key,test.st_array) &&
    this->any_to_server (test.st_any,key);
}

interop::wstruct *
interop_WChar_Passer_i::wstruct_from_server (CORBA::Short key)
{
  interop::wstruct_var ws = new interop::wstruct ();
  ws->st_char = this->wchar_from_server(key);
  ws->st_string = this->wstring_from_server(key);
  ref_.assign_warray (key, ws->st_array);
  ws->st_any <<= ref_.get_wstring(key);
  return ws._retn ();
}

CORBA::Boolean
interop_WChar_Passer_i::wstructseq_to_server (const interop::wstructseq & test,
                                              CORBA::Short key)
{
  CORBA::Boolean result = 1;
  for (CORBA::ULong i = 0; result && i < test.length(); i++)
    {
      result = this->wstruct_to_server(test[i], key);
    }
  return result;
}

interop::wstructseq *
interop_WChar_Passer_i::wstructseq_from_server (CORBA::Short key)
{
  interop::wstructseq_var wsListI = new interop::wstructseq();
  wsListI->length(5);

  for (CORBA::ULong i = 0; i < wsListI->length(); i++)
    {
      wsListI[i].st_char = this->wchar_from_server(key);
      wsListI[i].st_string = this->wstring_from_server(key);
      ref_.assign_warray (key, wsListI[i].st_array);
      // this is to keep Cxx on TRUE64 happy
      // it won't cast a wide character literal (i.e. L"")
      // to a CORBA::WChar *.  +
      // {I wonder if sizeof(wchar_t) != sizeof (CORBA::WChar)?}
      const CORBA::WChar empty_wstring[] = {0};
      wsListI[i].st_any <<= CORBA::wstring_dup(empty_wstring);
    }
  return wsListI._retn();
}

CORBA::Boolean
interop_WChar_Passer_i::wunion_to_server (const interop::wunion & test,
                                          CORBA::Short key)
{
  switch (test._d()) {
  case interop::is_wchar :
    return this->wchar_to_server (test.u_char(),key);
  case interop::is_wstring :
    return this->wstring_to_server (test.u_string(),key);
  case interop::is_warray :
    return this->warray_to_server (test.u_array(),key);
  default:
    /*return 0*/;
  }
  return 0;
}

interop::wunion *
interop_WChar_Passer_i::wunion_from_server (CORBA::Short key,
                                            interop::wchar_types type)
{
  interop::wunion *wu = new interop::wunion ();
  switch (type) {
  case interop::is_wchar :
    wu->u_char (ref_.get_wchar(key));
    break;
  case interop::is_wstring :
    wu->u_string (ref_.get_wstring(key));
    break;
  case interop::is_warray :
    wu->u_array (ref_.get_warray(key));
    break;
  }
  return wu;
}


CORBA::Boolean
interop_WChar_Passer_i::any_to_server (const CORBA::Any &test,
                                       CORBA::Short key)
{
  CORBA::WChar wc;
  const CORBA::WChar *ws;
  CORBA::WString_var wstr;
  interop::warray_forany forany;

  if (test >>= CORBA::Any::to_wchar(wc))
    {
      return this->wchar_to_server(wc,key);
    }
  else if (test >>= ws)
    {
      return this->wstring_to_server (ws,key);
    }
  else if (test >>= forany)
    {
      return this->warray_to_server (forany.in(),key);
    }
  return 0;
}

CORBA::Any*
interop_WChar_Passer_i::any_from_server (CORBA::Short key,
                                         interop::wchar_types type)
{
  CORBA::Any *any = new CORBA::Any;
  switch (type) {
  case interop::is_wchar :
    (*any) <<= CORBA::Any::from_wchar(ref_.get_wchar(key));
    break;
  case interop::is_wstring :
    (*any) <<= ref_.get_wstring(key);
    break;
  case interop::is_warray :
    {
      interop::warray_forany forany(ref_.get_warray(key));
      (*any) <<= forany.in();
      break;
    }
  }
  return any;
}

CORBA::Any *
interop_WChar_Passer_i::any_echo (const CORBA::Any &test)
{
  return new CORBA::Any (test);
}

void
interop_WChar_Passer_i::exception_test ( CORBA::Short key)
{
  throw interop::WChar_Passer::WStringException(ref_.get_except(key),
                                                this->wchar_from_server(key));
}

void
interop_WChar_Passer_i::shutdown ()
{
  this->orb_->shutdown(0);
}
