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
#include "ace/Get_Opt.h"
#include "ace/IOStream.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/ORBInitializer_Registry.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/LocalObject.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT ("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                           -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestLong\n") );
    if (static_cast<CORBA::Long>( 1 ) != a)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 3 ) != c)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestString\n") );
    if (0 == a)
    {
      //FUZZ: disable check_for_NULL
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter a\n") );
      //FUZZ: enable check_for_NULL
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (1 != ACE_OS::strlen( a ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if ('1' != *a)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (0 == c)
    {
      //FUZZ: disable check_for_NULL
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter c\n") );
      //FUZZ: enable check_for_NULL
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (1 != ACE_OS::strlen( c ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if ('3' != *c)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    Test::MyNonVarStruct
      newret;

    ACE_DEBUG( (LM_INFO, ". in TestNonVarStruct\n") );
    if (static_cast<CORBA::Long>( 1 ) != a.val)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 3 ) != c.val)
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestVarStruct\n") );
    if (0 == a.val.in())
    {
      //FUZZ: disable check_for_NULL
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter a\n") );
      //FUZZ: enable check_for_NULL
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (1 != ACE_OS::strlen( a.val.in() ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if ('1' != *a.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (0 == c.val.in())
    {
      //FUZZ: disable check_for_NULL
      ACE_DEBUG( (LM_INFO, "* Incorrect NULL string given for parameter c\n") );
      //FUZZ: enable check_for_NULL
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (1 != ACE_OS::strlen( c.val.in() ))
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect string length for parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if ('3' != *c.val.in())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    Test::MyNonVarUnion
      newret;

    ACE_DEBUG( (LM_INFO, ". in TestNonVarUnion\n") );
    if (static_cast<CORBA::Short>( 1 ) != a._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 1 ) != a.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Short>( 1 ) != c._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 3 ) != c.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestVarUnion\n") );
    if (static_cast<CORBA::Short>( 1 ) != a._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 1 ) != a.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Short>( 1 ) != c._d())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect type of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 3 ) != c.valLong())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestSeqOfLong\n") );
    if (1u != a.length())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 1 ) != a[0])
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (1u != c.length())
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (static_cast<CORBA::Long>( 3 ) != c[0])
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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
 )
  {
    ACE_DEBUG( (LM_INFO, ". in TestAny\n") );
    CORBA::Long aL;
    CORBA::Long cL;
    if (a >>= aL)
    {
      if (static_cast<CORBA::Long>( 1 ) != aL)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    }
    else
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect any type for parameter a\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
    }
    if (c >>= cL)
    {
      if (static_cast<CORBA::Long>( 3 ) != cL)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    }
    else
    {
      ACE_DEBUG( (LM_INFO, "* Incorrect any type for parameter c\n") );
      throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
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

  Test::MyArray_slice *TestArray(
    const Test::MyArray a,
    Test::MyArray_out b,
    Test::MyArray c)
  {
    ACE_DEBUG( (LM_INFO, ". in TestArray\n") );
    if (a[0].length () != 1)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter a[0]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (a[0][0] != 9)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a[0]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (a[1].length () != 1)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter a[1]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (a[1][0] != 23)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter a[1]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }

    if (c[0].length () != 1)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter c[0]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (c[0][0]++ != 23)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c[0]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (c[1].length () != 1)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect length of parameter c[1]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }
    if (c[1][0]++ != 9)
      {
        ACE_DEBUG( (LM_INFO, "* Incorrect input value of parameter c[1]\n") );
        throw CORBA::BAD_PARAM(0, CORBA::COMPLETED_NO);
      }

    b = Test::MyArray_alloc ();
    CORBA::ULong idx (0);
    b[idx].length (1);
    b[idx][0] = 8;
    ++idx;
    b[idx].length (1);
    b[idx][0] = 22;

    Test::MyArray_var ret = new Test::MyArray;
    idx = 0;
    ret[idx].length (1);
    ret[idx][0] = 7;
    ++idx;
    ret[idx].length (1);
    ret[idx][0] = 21;
    return ret._retn ();
  }

  //-----------------------------------------------------------

  CORBA::Object_ptr TestObject(
    CORBA::Object_ptr a,
    CORBA::Object_out b,
    CORBA::Object_ptr &c
    )
  {
    b = CORBA::Object::_duplicate (a);
    return CORBA::Object::_duplicate (c);
  }

  //-----------------------------------------------------------

  void ShutdownServer(
 )
  {
    ACE_DEBUG( (LM_INFO, ". in ShutdownServer\n") );
    orb->shutdown(0);
  }
};

// Here is our Regression test class that actually tests the problem with
// the interceptor's arguments.

class AnInterceptor : public PortableInterceptor::ServerRequestInterceptor
{
public:
  char *name( )
  {
    return const_cast<char *>("");
  }

  void destroy( )
  {
  }

  void receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr
 )
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
    Test::MyArray_forany       arr;
    CORBA::Object_var          obj;

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
        ACE_DEBUG( (LM_INFO, "String %C", vU->valString()));
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
    else if (arg >>= arr)
    {
      ACE_DEBUG( (LM_INFO, "MyArray (") );
      for (CORBA::ULong a_idx = 0; a_idx < 2; ++a_idx)
        {
          CORBA::ULong length = arr[a_idx].length ();
          ACE_DEBUG( (LM_INFO, "[%u].length () == %u  ", a_idx, length));
        }
    }
    else if (arg >>= CORBA::Any::to_object(obj))
    {
      ACE_DEBUG( (LM_INFO, "CORBA::Object (") );
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
 )
  {
    ACE_DEBUG( (LM_INFO, "AnInterceptor::receive_request\n") );
    Dynamic::ParameterList
      *pArgs= ri->arguments( );
    display_arg_list( pArgs );
  }

  // This send_reply() method would cause the problem due to it's
  // ri->arguments() call. When the returned arguments list was
  // deleted (due to the display_arg_list( Dynamic::ParameterList_var ))
  // going out of scope, the "Owned" out pointer 'Argument B' would
  // be premiturely deleted before being sent back to the client.
  void send_reply(
    PortableInterceptor::ServerRequestInfo_ptr ri
 )
  {
    ACE_DEBUG( (LM_INFO, "AnInterceptor::send_reply\n") );
    Dynamic::ParameterList
      *pArgs= ri->arguments( );
    display_arg_list( pArgs );
    ACE_DEBUG( (LM_INFO, "  result is an ") );
    CORBA::Any
      *pAny= ri->result( );
    display_any( CORBA::Any_var( pAny ).in() );
    ACE_DEBUG( (LM_INFO, "\n") );
  }

  void send_exception(
    PortableInterceptor::ServerRequestInfo_ptr
  )
  {
  }
  void send_other(
    PortableInterceptor::ServerRequestInfo_ptr
 )
  {
  }
};

class Initialiser
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual CORBA::LocalObject
{
public:
  Initialiser( AnInterceptor* interceptor )
  {
    this->interceptor_= interceptor;
  }

  void pre_init(
    PortableInterceptor::ORBInitInfo_ptr
 )
  {
  }

  void post_init(
     PortableInterceptor::ORBInitInfo_ptr info
 )
   {
     info->add_server_request_interceptor( interceptor_ );
   }

private:
  AnInterceptor *interceptor_;
};

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
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

    orb= CORBA::ORB_init (argc, argv);
    CORBA::Object_var
      Object = orb->resolve_initial_references( "RootPOA" );
    PortableServer::POA_var rootPOA=
      PortableServer::POA::_narrow( Object.in() );
    PortableServer::POAManager_var
      rootPOAMgr = rootPOA->the_POAManager( );

    if (parse_args (argc, argv) != 0)
      return 1;

    FooImpl
      phooey;
    PortableServer::ObjectId_var
      phooeyId= rootPOA->activate_object( &phooey );
    CORBA::Object_var
      phooeyObj= rootPOA->id_to_reference( phooeyId.in() );
    CORBA::String_var
      stringifiedObj= orb->object_to_string( phooeyObj.in() );
    ofstream file( ACE_TEXT_ALWAYS_CHAR(ior_output_file) );
    file << stringifiedObj;
    file.close();

    rootPOAMgr->activate( );

    orb->run( 0 );

    orb->destroy( );
  }
  catch (const CORBA::SystemException& exception)
  {
    exception._tao_print_exception ("CORBA::SystemException: ");
    return -1;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("CORBA::Exception: ");
    return -1;
  }
  catch (...)
  {
    ACE_DEBUG( (LM_ERROR, "Unexpected general exception.\n") );
    return -1;
  }

  ACE_DEBUG( (LM_INFO, "Server stopped\n") );
  return 0;
}
