// $Id$

// ============================================================================
//
// = Library
//    Tao/Tests/Param_Test
//
// = Filename
//    Bd_Wstring.Cpp
//
// = Description
//    Tests Bounded Wide Strings
//
// = Authors
//      Jeff Parsons
//
// ============================================================================

#Include "Helper.H"
#Include "Bd_Wstring.H"

Ace_Rcsid(Param_Test, Bd_Wstring, "$id: Bd_Wstring.Cpp,V 1.3 1999/09/17 17:03:22 Parsons Exp $")

// ************************************************************************
//               Test_Bounded_Wstring
// ************************************************************************

Test_Bounded_Wstring::Test_Bounded_Wstring (Void)
  : Opname_ (Corba::String_Dup ("Test_Bounded_Wstring")),
    In_ (0),
    Inout_ (0),
    Out_ (0),
    Ret_ (0)
{
}

Test_Bounded_Wstring::~Test_Bounded_Wstring (Void)
{
  Corba::String_Free (This->Opname_);
  Corba::Wstring_Free (This->In_);
  Corba::Wstring_Free (This->Inout_);
  Corba::Wstring_Free (This->Out_);
  Corba::Wstring_Free (This->Ret_);
  This->Opname_ = 0;
  This->In_ = 0;
  This->Inout_ = 0;
  This->Out_ = 0;
  This->Ret_ = 0;
}

Const Char *
Test_Bounded_Wstring::Opname (Void) Const
{
  Return This->Opname_;
}

Void
Test_Bounded_Wstring::Dii_Req_Invoke (Corba::Request *Req,
                                      Corba::Environment &Ace_Try_Env)
{
  Req->Add_In_Arg ("S1") <<= Corba::Any::From_Wstring (This->In_, 128);
  Req->Add_Inout_Arg ("S2") <<= Corba::Any::From_Wstring (This->Inout_, 128);
  Req->Add_Out_Arg ("S3") <<= Corba::Any::From_Wstring (This->Out_, 128);

  Req->Set_Return_Type (Param_Test::_Tc_Short_Wstring);

  Req->Invoke (Ace_Try_Env);
  Ace_Check;

  Corba::Wchar *Tmp;
  Req->Return_Value () >>= Corba::Any::To_Wstring (Tmp, 128);
  This->Ret_ = Corba::Wstring_Dup (Tmp);

  Corba::Namedvalue_Ptr Arg2 =
    Req->Arguments ()->Item (1, Ace_Try_Env);
  Ace_Check;
  *Arg2->Value () >>= Corba::Any::To_Wstring (Tmp, 128);
  This->Inout_ = Corba::Wstring_Dup (Tmp);

  Corba::Namedvalue_Ptr Arg3 =
    Req->Arguments ()->Item (2, Ace_Try_Env);
  Ace_Check;
  *Arg3->Value () >>= Corba::Any::To_Wstring (Tmp, 128);
  This->Out_ = Corba::Wstring_Dup (Tmp);
}

Int
Test_Bounded_Wstring::Init_Parameters (Param_Test_Ptr,
                                                               Corba::Environment &)
{
  Generator *Gen = Generator::Instance (); // Value Generator

  // Release Any Previously Occupied Values
  Corba::Wstring_Free (This->In_);
  Corba::Wstring_Free (This->Inout_);
  Corba::Wstring_Free (This->Out_);
  Corba::Wstring_Free (This->Ret_);
  This->In_ = 0;
  This->Inout_ = 0;
  This->Out_ = 0;
  This->Ret_ = 0;

  This->In_ = Gen->Gen_Wstring (32);
  This->Inout_ = Corba::Wstring_Dup (This->In_);
  Return 0;
}

Int
Test_Bounded_Wstring::Reset_Parameters (Void)
{
  // Release Any Previously Occupied Values
  Corba::Wstring_Free (This->Inout_);
  Corba::Wstring_Free (This->Out_);
  Corba::Wstring_Free (This->Ret_);
  This->Inout_ = 0;
  This->Out_ = 0;
  This->Ret_ = 0;

  This->Inout_ = Corba::Wstring_Dup (This->In_);
  Return 0;
}

Int
Test_Bounded_Wstring::Run_Sii_Test (Param_Test_Ptr Objref,
                                                            Corba::Environment &Ace_Try_Env)
{
  Ace_Try
    {
      Corba::Wstring_Out Str_Out (This->Out_);

      This->Ret_ = Objref->Test_Bounded_Wstring (This->In_,
                                                 This->Inout_,
                                                 Str_Out,
                                                 Ace_Try_Env);
      Ace_Try_Check;

      Return 0;
    }
  Ace_Catchany
    {
      Ace_Print_Exception (Ace_Any_Exception,
                           "Test_Bounded_Wstring::Run_Sii_Test\N");

    }
  Ace_Endtry;
  Return -1;
}

Corba::Boolean
Test_Bounded_Wstring::Check_Validity (Void)
{
  Corba::Ulong Len = Ace_Os::Wslen (This->In_);

  If (!Ace_Os::Wscmp (This->In_, This->Out_) &&
      !Ace_Os::Wscmp (This->In_, This->Ret_) &&
      Ace_Os::Wslen (This->Inout_) == 2*Len &&
      !Ace_Os::Wsncmp (This->In_, This->Inout_, Len) &&
      !Ace_Os::Wsncmp (This->In_, &This->Inout_[Len], Len))
    Return 1;

  Return 0; // Otherwise
}

Corba::Boolean
Test_Bounded_Wstring::Check_Validity (Corba::Request_Ptr)
{
  // No Need To Retrieve Anything Because, For All The Args And
  // The Return, We Provided The Memory And We Own It.
  Return This->Check_Validity ();
}

Void
Test_Bounded_Wstring::Print_Values (Void)
{
}
