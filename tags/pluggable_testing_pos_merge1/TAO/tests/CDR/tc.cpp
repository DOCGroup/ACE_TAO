// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/CDR
//
// = FILENAME
//    tc.cpp
//
// = DESCRIPTION
//   Verify that the basic typecodes are able to interpret their CDR
//   buffers.
//
// = AUTHORS
//    Carlos O'Ryan
//
// ============================================================================

#include "tao/corba.h"

ACE_RCSID(CDR, tc, "$Id$")

// In this version of TAO typecodes are based on CDR, we have to
// verify that CDR offers the services needed for Typecode...

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
					    ACE_TRY_ENV);
      ACE_TRY_CHECK;

      static const CORBA::TypeCode_ptr tcs[]=
      {
        CORBA::_tc_null,
        CORBA::_tc_void,
        CORBA::_tc_short,
        CORBA::_tc_long,
        CORBA::_tc_ushort,
        CORBA::_tc_ulong,
        CORBA::_tc_float,
        CORBA::_tc_double,
        CORBA::_tc_boolean,
        CORBA::_tc_char,
        CORBA::_tc_octet,
        CORBA::_tc_any,
        CORBA::_tc_TypeCode,
        CORBA::_tc_Principal,
        CORBA::_tc_Object,
        // CORBA::_tc_struct,
        // CORBA::_tc_union,
        // CORBA::_tc_enum,
        CORBA::_tc_string,
        // CORBA::_tc_sequence,
        // CORBA::_tc_array,
        // CORBA::_tc_alias,
        // CORBA::_tc_except,
        CORBA::_tc_longlong,
        CORBA::_tc_ulonglong,
        CORBA::_tc_longdouble,
        CORBA::_tc_wchar,
        CORBA::_tc_wstring,
        CORBA::_tc_UNKNOWN,
        CORBA::_tc_BAD_PARAM,
        CORBA::_tc_NO_MEMORY,
        CORBA::_tc_IMP_LIMIT,
        CORBA::_tc_COMM_FAILURE,
        CORBA::_tc_INV_OBJREF,
        CORBA::_tc_OBJECT_NOT_EXIST,
        CORBA::_tc_NO_PERMISSION,
        CORBA::_tc_INTERNAL,
        CORBA::_tc_MARSHAL,
        CORBA::_tc_INITIALIZE,
        CORBA::_tc_NO_IMPLEMENT,
        CORBA::_tc_BAD_TYPECODE,
        CORBA::_tc_BAD_OPERATION,
        CORBA::_tc_NO_RESOURCES,
        CORBA::_tc_NO_RESPONSE,
        CORBA::_tc_PERSIST_STORE,
        CORBA::_tc_BAD_INV_ORDER,
        CORBA::_tc_TRANSIENT,
        CORBA::_tc_FREE_MEM,
        CORBA::_tc_INV_IDENT,
        CORBA::_tc_INV_FLAG,
        CORBA::_tc_INTF_REPOS,
        CORBA::_tc_BAD_CONTEXT,
        CORBA::_tc_OBJ_ADAPTER,
        CORBA::_tc_DATA_CONVERSION,
        CORBA::TypeCode::_tc_Bounds,
        CORBA::TypeCode::_tc_BadKind
      };

      static int n = sizeof (tcs) / sizeof (tcs[0]);

      for (const CORBA::TypeCode_ptr *i = tcs;
           i != tcs + n;
           ++i)
	{
	  CORBA::TypeCode_ptr tc = *i;

	  CORBA::TCKind k = tc->kind (ACE_TRY_ENV);
	  ACE_TRY_CHECK;

	  switch (k)
	    {
	    case CORBA::tk_objref:
	    case CORBA::tk_struct:
	    case CORBA::tk_union:
	    case CORBA::tk_enum:
	    case CORBA::tk_alias:
	    case CORBA::tk_except:
	      {
		const char *id = tc->id (ACE_TRY_ENV);
		ACE_TRY_CHECK;

		const char *name = tc->name (ACE_TRY_ENV);
		ACE_TRY_CHECK;

		CORBA::ULong length = 0;
                // tc->length (TAO_TRY_ENV);
		            // ACE_TRY_CHECK;

		ACE_DEBUG ((LM_DEBUG,
			    "ID = '%s'\n"
			    "%{%{ NAME = %s%$"
			    " KIND = %d%$"
			    " LENGTH = %d"
			    "%}%}\n",
			    (id?id:"empty ID"),
                            (name?name:"empty name"),
                            k,
                            length));
                break;
	      }
	    default:
              ACE_DEBUG ((LM_DEBUG,
                          "basic type: %d\n",
                          k));
              break;
	    }
	}
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("TC");
    }
  ACE_ENDTRY;

  return 0;
}
