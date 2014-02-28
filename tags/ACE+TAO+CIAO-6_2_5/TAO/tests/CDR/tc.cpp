
//=============================================================================
/**
 *  @file    tc.cpp
 *
 *  $Id$
 *
 * Verify that the basic typecodes are able to interpret their CDR
 * buffers.
 *
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/ORB.h"
#include "tao/SystemException.h"
#include "tao/debug.h"

#include "ace/Log_Msg.h"

// In this version of TAO typecodes are based on CDR, we have to
// verify that CDR offers the services needed for Typecode...
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

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

          CORBA::TCKind k = tc->kind ();

          switch (k)
            {
            case CORBA::tk_objref:
            case CORBA::tk_struct:
            case CORBA::tk_union:
            case CORBA::tk_enum:
            case CORBA::tk_alias:
            case CORBA::tk_except:
              {
                const char *id = tc->id ();

                const char *name = tc->name ();

                CORBA::ULong length = 0;
                // tc->length (TAO_TRY_ENV);

                if (TAO_debug_level > 0)
                  {
                    ACE_DEBUG ((LM_DEBUG,
                                "ID = '%C'\n"
                                "%{%{ NAME = %C%$"
                                " KIND = %d%$"
                                " LENGTH = %d"
                                "%}%}\n",
                                (id?id:"empty ID"),
                                (name?name:"empty name"),
                                k,
                                length));
                  }
                break;
              }
            default:
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              "basic type: %d\n",
                              k));
                }
              break;
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("TC");
      return 1;
    }

  return 0;
}
