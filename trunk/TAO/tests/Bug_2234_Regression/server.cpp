// $Id$

// Regression test Bug 2234
//
// The bug actually manifested itself in class AnInterceptor::receive_request()
// where any OUT parameters of variable length items (as they are held as
// addresses of NULL pointers)
//
// The bug is caused by the arguments() call which should be
// creating a read-only COPY of the parameters to the call. These are held as
// a sequence of ANYs, whith the code for the insertion into which is created
// by the TAO_IDL compiler when their user types are compiled.
// It is also manifested by the result() call in the same way.

#include "TestS.h"
#include "ace/IOStream.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PortableServer/Root_POA.h"

CORBA::ORB_ptr orb;

// The test case code for the server always sends back to the client:
// b(out)= a(in) *2
// c(inout)= c(inout) + 1
// return= 7
// Strings are single numerical digits, longs are the numbers themselves
// and sequences are always single long values.
// Parameter 'a' must be received from the client as 1, and 'c' as 3.

class FooImpl : public POA_Test::Foo
{
public:
  FooImpl() {}
  ~FooImpl() {}

  //-----------------------------------------------------------

  CORBA::Long TestLong(
    CORBA::Long a,
    CORBA::Long_out b,
    CORBA::Long &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException ) )
  {
    ACE_DEBUG( (LM_INFO, ". in TestLong\n") );
    if (static_cast<CORBA::Long>( 1 ) != a)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Long>( 3 ) != c)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }

    b= static_cast<CORBA::Long>(  a << 1 );

    c+= static_cast<CORBA::Long>( 1 );
    return static_cast<CORBA::Long>( 7 );
  }

  //-----------------------------------------------------------

  char *TestString(
    const char *a,
    CORBA::String_out b,
    char *&c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    ACE_DEBUG( (LM_INFO, ". in TestString\n") );
    if (0 == a)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (1 != ACE_OS::strlen( a ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if ('1' != *a)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (1 != ACE_OS::strlen( c ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (0 == c)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if ('3' != *c)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }

    b= CORBA::string_dup( "0" ); // Create a one character output buffer
    *b= a[0] + 1;
    *c+= 1;
    return CORBA::string_dup( "7" );
  }

  //-----------------------------------------------------------

  Test::MyNonVarStruct TestNonVarStruct(
    const Test::MyNonVarStruct &a,
    Test::MyNonVarStruct_out b,
    Test::MyNonVarStruct &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    Test::MyNonVarStruct
      newret;

    ACE_DEBUG( (LM_INFO, ". in TestNonVarStruct\n") );
    if (static_cast<CORBA::Long>( 1 ) != a.val)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }
    if (static_cast<CORBA::Long>( 3 ) != c.val)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }

    newret.val= static_cast<CORBA::Long>( 7 );

    Test::MyNonVarStruct *newval_p;
    ACE_NEW_RETURN( newval_p, Test::MyNonVarStruct, newret );
    Test::MyNonVarStruct_var
      newval= newval_p;

    newval->val= a.val << 1;
    c.val+= 1;

    b= newval._retn();
    return newret;
  }

  //-----------------------------------------------------------

  Test::MyVarStruct *TestVarStruct(
    const Test::MyVarStruct &a,
    Test::MyVarStruct_out b,
    Test::MyVarStruct &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC(( CORBA::SystemException ))
  {
    ACE_DEBUG( (LM_INFO, ". in TestVarStruct\n") );
    if (0 == a.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (1 != ACE_OS::strlen( a.val.in() ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if ('1' != *a.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (0 == c.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if (1 != ACE_OS::strlen( c.val.in() ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }
    if ('3' != *c.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0);
    }

    char ca[2]= {0};
    ca[0]= ('0' + ((*a.val.in() -'0') << 1));

    Test::MyVarStruct_var
      newval_p,
      newret_p;
    ACE_NEW_RETURN( newval_p, Test::MyVarStruct(), 0 );
    Test::MyVarStruct_var
      newval= newval_p;
    ACE_NEW_RETURN( newret_p, Test::MyVarStruct(), 0 );
    Test::MyVarStruct_var
      newret= newret_p;

    newval->val= CORBA::string_dup( ca );

    *c.val.inout()+= 1;

    newret->val= CORBA::string_dup( "7" );

    b= newval._retn();
    return newret._retn();
  }

  //-----------------------------------------------------------

  Test::MyNonVarUnion TestNonVarUnion(
    const Test::MyNonVarUnion &a,
    Test::MyNonVarUnion_out b,
    Test::MyNonVarUnion &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    Test::MyNonVarUnion
      newret;

    ACE_DEBUG( (LM_INFO, ". in TestNonVarUnion\n") );
    if (static_cast<CORBA::Short>( 1 ) != a._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }
    if (static_cast<CORBA::Long>( 1 ) != a.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }
    if (static_cast<CORBA::Short>( 1 ) != c._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }
    if (static_cast<CORBA::Long>( 3 ) != c.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), newret );
    }

    Test::MyNonVarUnion *newval_p;
    ACE_NEW_RETURN( newval_p, Test::MyNonVarUnion(), newret );
    Test::MyNonVarUnion_var
      newval= newval_p;

    newval->valLong( a.valLong() << 1 );
    c.valLong( c.valLong() + 1 );

    newret.valLong( static_cast<CORBA::Long>( 7 ) );

    b= newval._retn();
    return newret;
  }

  //-----------------------------------------------------------

  Test::MyVarUnion *TestVarUnion(
    const Test::MyVarUnion &a,
    Test::MyVarUnion_out b,
    Test::MyVarUnion &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC (( CORBA::SystemException ))
  {
    ACE_DEBUG( (LM_INFO, ". in TestVarUnion\n") );
    if (static_cast<CORBA::Short>( 1 ) != a._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Long>( 1 ) != a.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Short>( 1 ) != c._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Long>( 3 ) != c.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }

    Test::MyVarUnion *newval_p;
    ACE_NEW_RETURN( newval_p, Test::MyVarUnion(), 0 );
    Test::MyVarUnion_var
      newval= newval_p;

    newval->valLong( a.valLong() << 1 );
    c.valLong( c.valLong() + 1 );

    Test::MyVarUnion_var
      newret_p;
    ACE_NEW_RETURN( newret_p, Test::MyVarUnion(), 0 );
    Test::MyVarUnion_var
      newret= newret_p;
    newret->valLong( static_cast<CORBA::Short>( 7 ) );

    b= newval._retn();
    return newret._retn();
  }

  //-----------------------------------------------------------

  Test::MySeqOfLong *TestSeqOfLong (
    const Test::MySeqOfLong &a,
    Test::MySeqOfLong_out b,
    Test::MySeqOfLong &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    ACE_DEBUG( (LM_INFO, ". in TestSeqOfLong\n") );
    if (1u != a.length())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Long>( 1 ) != a[0])
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (1u != c.length())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (static_cast<CORBA::Long>( 3 ) != c[0])
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }

    Test::MySeqOfLong *newval_p;
    ACE_NEW_RETURN( newval_p, Test::MySeqOfLong(1), 0 );
    Test::MySeqOfLong_var
      newval= newval_p;

    newval->length(1);
    newval[0]= a[0] << 1;
    c[0]+= 1;

    Test::MySeqOfLong *newret_p;
    ACE_NEW_RETURN( newret_p, Test::MySeqOfLong(1), 0 );
    Test::MySeqOfLong_var
      newret= newret_p;
    newret->length( 1 );
    newret[0]= static_cast<CORBA::Long>( 7 );

    b= newval._retn();
    return newret._retn();
  }

  //-----------------------------------------------------------

  CORBA::Any *TestAny(
    const CORBA::Any &a,
    CORBA::Any_out b,
    CORBA::Any &c
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    ACE_DEBUG( (LM_INFO, ". in TestAny\n") );
    CORBA::Long aL;
    CORBA::Long cL;
    if (a >>= aL)
    {
      if (static_cast<CORBA::Long>( 1 ) != aL)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
        ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
      }
    }
    else
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect any type for parameter a\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }
    if (c >>= cL)
    {
      if (static_cast<CORBA::Long>( 3 ) != cL)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
        ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
      }
    }
    else
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect any type for parameter c\n") );
      ACE_THROW_RETURN( CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO), 0 );
    }

    CORBA::Any *newval_p;
    ACE_NEW_RETURN( newval_p, CORBA::Any(), 0 );
    CORBA::Any_var
      newval= newval_p;

    newval<<= aL << 1;
    c<<= cL + 1;

    CORBA::Any *newret_p;
    ACE_NEW_RETURN( newret_p, CORBA::Any(), 0 );
    CORBA::Any_var
      newret= newret_p;
    newret<<= static_cast<CORBA::Long>( 7 );

    b= newval._retn();
    return newret._retn();
  }

  //-----------------------------------------------------------

  void ShutdownServer(
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    ACE_DEBUG( (LM_INFO, ". in ShutdownServer\n") );
    orb->shutdown(0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
};

// Here is our Regression test class that actually tests the problem with
// the interceptor's arguments.

class AnInterceptor : public PortableInterceptor::ServerRequestInterceptor
{
public:
  char *name( ACE_ENV_SINGLE_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    return const_cast<char *>("");
  }

  void destroy( ACE_ENV_SINGLE_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
  }

  void receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC( (CORBA::SystemException,
                     PortableInterceptor::ForwardRequest) )
  {
  }

  // Note this helper only deals with the types inserted into
  // the any that we defined for this test.
  static void display_any( const CORBA::Any &arg )
  {
    const CORBA::Any           *holding;
    const Test::MyVarStruct    *vS;
    const Test::MyNonVarStruct *fS;
    const char                 *pString;
    CORBA::Long                theLong;
    const Test::MyVarUnion     *vU;
    const Test::MyNonVarUnion  *fU;
    const Test::MySeqOfLong    *sL;

    if (arg >>= vS)
    {
      ACE_DEBUG( (LM_INFO, "MyVarStruct (") );
      if (vS)
        ACE_DEBUG( (LM_INFO, vS->val.in()) );
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= fS)
    {
      ACE_DEBUG( (LM_INFO, "MyNonVarStruct (") );
      if (fS)
        ACE_DEBUG( (LM_INFO, "%d", fS->val) );
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= pString)
    {
      ACE_DEBUG( (LM_INFO, "String (") );
      if (pString)
        ACE_DEBUG( (LM_INFO, pString) );
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= theLong)
      ACE_DEBUG( (LM_INFO, "Long (%d", theLong) );
    else if (arg >>= fU)
    {
      ACE_DEBUG( (LM_INFO, "MyNonVarUnion (") );
      if (fU) switch (fU->_d())
      {
      case 1:
        ACE_DEBUG( (LM_INFO, "Long %d", fU->valLong()) );
        break;

      case 2:
        ACE_DEBUG( (LM_INFO, "Short %d", fU->valShort()) );
        break;

      default:
        ACE_DEBUG( (LM_INFO, "*Unknown*") );
      }
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= vU)
    {
      ACE_DEBUG( (LM_INFO, "MyVarUnion (") );
      if (vU) switch (vU->_d())
      {
      case 1:
        ACE_DEBUG( (LM_INFO, "Long %d", vU->valLong()) );
        break;

      case 2:
        ACE_DEBUG( (LM_INFO, "String %s", vU->valString()) );
        break;

      default:
        ACE_DEBUG( (LM_INFO, "*Unknown*") );
      }
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= sL)
    {
      ACE_DEBUG( (LM_INFO, "MySeqOfLong (") );
      if (sL)
      {
        if (0u < sL->length())
        {
          for (unsigned int i= 0u; i < sL->length() - 1u; ++i)
            ACE_DEBUG( (LM_INFO, "%d, ", (*sL)[ i ]) );
          ACE_DEBUG( (LM_INFO, "%d", (*sL)[ sL->length() - 1u ]) );
        }
        else
          ACE_DEBUG( (LM_INFO, "*Empty*") );
      }
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else if (arg >>= holding)
    {
      ACE_DEBUG( (LM_INFO, "Any (") );
      if (holding)
      {
        if (*holding >>= theLong)
          ACE_DEBUG( (LM_INFO, "Long %d", theLong) );
        else
          ACE_DEBUG( (LM_INFO, "*Not Long*") );
      }
      else
        ACE_DEBUG( (LM_INFO, "*Null*") );
    }
    else
      ACE_DEBUG( (LM_INFO, "Unknown (") );
    ACE_DEBUG( (LM_INFO, ") parameter\n") );
  }

  // Useful parameter dumping helper method.-----------| Note this VAR
  // which will automatically delete after the call!   V
  static void display_arg_list( Dynamic::ParameterList_var param_list )
  {
    for (unsigned int i= 0u; i < param_list->length(); ++i)
    {
      ACE_DEBUG( (LM_INFO, "  param %d is an ", i) );
      switch( (*param_list)[i].mode )
      {
      case CORBA::PARAM_IN:
        ACE_DEBUG( (LM_INFO, "in ") );
        break;

      case CORBA::PARAM_OUT:
        ACE_DEBUG( (LM_INFO, "out ") );
        break;

      case CORBA::PARAM_INOUT:
        ACE_DEBUG( (LM_INFO, "inout ") );
        break;

      default:
        ACE_DEBUG( (LM_INFO, "non-directional ") );
        break;
      }

      display_any( (*param_list)[i].argument );
    }
  }

  void receive_request(
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException,
                     PortableInterceptor::ForwardRequest) )
  {
    ACE_DEBUG( (LM_INFO, "AnInterceptor::receive_request\n") );
    Dynamic::ParameterList
      *pArgs= ri->arguments( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_CHECK;
    display_arg_list( pArgs );
  }

  // This send_reply() method would cause the problem due to it's
  // ri->arguments() call. When the returned arguments list was
  // deleted (due to the display_arg_list( Dynamic::ParameterList_var ))
  // going out of scope, the "Owned" out pointer 'Argument B' would
  // be premiturely deleted before being sent back to the client.
  void send_reply(
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL_WITH_DEFAULTS )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
    ACE_DEBUG( (LM_INFO, "AnInterceptor::send_reply\n") );
    Dynamic::ParameterList
      *pArgs= ri->arguments( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_CHECK;
    display_arg_list( pArgs );
    ACE_DEBUG( (LM_INFO, "  result is an ") );
    CORBA::Any
      *pAny= ri->result( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_CHECK;
    display_any( CORBA::Any_var( pAny ).in() );
    ACE_DEBUG( (LM_INFO, "\n") );
  }

  void send_exception(
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED  )
    ACE_THROW_SPEC( (CORBA::SystemException,
                     PortableInterceptor::ForwardRequest) )
  {
  }

  void send_other(
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC( (CORBA::SystemException,
                     PortableInterceptor::ForwardRequest) )
  {
  }
};

class Initialiser : public  PortableInterceptor::ORBInitializer
{
public:
  Initialiser( AnInterceptor* interceptor )
  {
    this->interceptor_= interceptor;
  }

  void pre_init(
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED )
    ACE_THROW_SPEC( (CORBA::SystemException) )
  {
  }

  void post_init(
     PortableInterceptor::ORBInitInfo_ptr info
     ACE_ENV_ARG_DECL_NOT_USED )
     ACE_THROW_SPEC( (CORBA::SystemException) )
   {
     info->add_server_request_interceptor( interceptor_ );
   }

private:
  AnInterceptor *interceptor_;
};

int main( int argc, char *argv[] )
{
  ACE_TRY_NEW_ENV
  {
    ACE_DEBUG( (LM_INFO, "Server start\n") );
    AnInterceptor
      interceptor;
    PortableInterceptor::ORBInitializer_ptr
      initialiser_p;
    ACE_NEW_RETURN( initialiser_p, Initialiser( &interceptor ), -1 );
    PortableInterceptor::ORBInitializer_var
      initialiser= initialiser_p;
    PortableInterceptor::register_orb_initializer( initialiser.in() );

    orb= CORBA::ORB_init( argc, argv, 0 ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    CORBA::Object_var
      Object = orb->resolve_initial_references( "RootPOA" ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    PortableServer::POA_var rootPOA=
      PortableServer::POA::_narrow( Object.in() ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    PortableServer::POAManager_var
      rootPOAMgr = rootPOA->the_POAManager( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;

    FooImpl
      phooey;
    PortableServer::ObjectId_var
      phooeyId= rootPOA->activate_object( &phooey ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    CORBA::Object_var
      phooeyObj= rootPOA->id_to_reference( phooeyId.in() ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    CORBA::String_var
      stringifiedObj= orb->object_to_string( phooeyObj.in() ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;
    ofstream file( "server.ior" );
    file << stringifiedObj;
    file.close();

    rootPOAMgr->activate( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;

    orb->run( 0 ACE_ENV_ARG_PARAMETER );
    ACE_TRY_CHECK;

    orb->destroy( ACE_ENV_SINGLE_ARG_PARAMETER );
    ACE_TRY_CHECK;
  }
  ACE_CATCH( CORBA::SystemException, exception )
  {
    ACE_PRINT_EXCEPTION( exception, "CORBA::SystemException: " );
    return -1;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION( ACE_ANY_EXCEPTION, "CORBA::Exception: " );
    return -1;
  }
  ACE_CATCHALL
  {
    ACE_DEBUG( (LM_ERROR, "Unexpected general exception.\n") );
    return -1;
  }
  ACE_ENDTRY;

  ACE_DEBUG( (LM_INFO, "Server stopped\n") );
  return 0;
}
