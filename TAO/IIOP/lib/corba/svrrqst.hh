// @(#)svrrqst.hh	1.5 95/09/25
//
// Header file for Win32 C/C++/COM interface to CORBA's Dynamic
// Server Skeleton Interface's "Server Request" type.
//
// XXX this is currently in an intermediate state; this is not
// supposed to be IIOP-specific, or to expose quite so many
// implementation details, but right now it is.
//

typedef class CORBA_ServerRequest *CORBA_ServerRequest_ptr;
typedef class CORBA_ORB *CORBA_ORB_ptr;
typedef class TOA *TOA_ptr;

void                              CORBA_release (CORBA_ServerRequest_ptr req);
CORBA_Boolean                     CORBA_is_nil (CORBA_ServerRequest_ptr req);

extern const IID                  IID_CORBA_ServerRequest;

class _EXPCLASS CORBA_ServerRequest : public IUnknown
{
  public:
    //
    // Implementation uses this to provide the ORB with the operation's
    // parameter list ... on return, their values are available; the list
    // fed in has typecodes and (perhap) memory assigned.
    //
    virtual void __stdcall	params (
				    CORBA_NVList_ptr	list,
				    CORBA_Environment	&env
				) = 0;

    //
    // Implementation uses this to provide the operation result ... illegal
    // if exception() was called or params() was not called.
    //
    // XXX Implementation should be able to assume response has been sent
    // when this returns, and reclaim memory it allocated.
    //
    virtual void __stdcall	result (
				    CORBA_Any_ptr	value,
				    CORBA_Environment       &env
				) = 0;

    //
    // Implementation uses this to provide the exception value which is the
    // only result of this particular invocation.
    //
    // XXX Implementation should be able to assume response has been sent
    // when this returns, and reclaim memory it allocated.
    //
    virtual void __stdcall	exception (
				    CORBA_ExceptionType	type,
				    CORBA_Any_ptr           value,
				    CORBA_Environment       &env
				) = 0;

    //
    // Get various universal call attributes:  who made the call, the
    // target of the call, what ORB and OA that target object uses.
    //
    // NOTE:  none of these report exceptions; unavailability of any of
    // this stuff is a catastrophic error since this is all part of
    // the basic CORBA Object Model.
    //
    // XXX should not be not assuming all OAs implement the TOA API !!
    //
    virtual CORBA_Principal_ptr __stdcall	caller () = 0;
    virtual CORBA_Object_ptr __stdcall		target () = 0;
    virtual CORBA_String __stdcall		op_name () = 0;
    virtual TOA_ptr __stdcall			oa () = 0;
    virtual CORBA_ORB_ptr __stdcall		orb () = 0;
};
