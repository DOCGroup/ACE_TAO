//
// $Id$
//

#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/IFR_Client/IFR_Client_Adapter_Impl.h"
#include "tao/AnyTypeCode/NVList.h"
#include "tao/ORB.h"
#include "ace/OS_NS_string.h"

int main (int argc, char* argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY_NEW_ENV
   {
      int failed = 0;

      // Initialise ORB.
      //
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Find the Interface Repository.
      //
      CORBA::Object_var ifr_obj = orb->resolve_initial_references( "InterfaceRepository" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Repository_var ifr = CORBA::Repository::_narrow( ifr_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if( CORBA::is_nil( ifr.in() ) )
      {
         ACE_DEBUG((LM_DEBUG, "Nil IFR reference\n"));
         return 1;
      }

      // Add an interface to the repository.
      //
      CORBA::InterfaceDefSeq baseInterfaces(1) ;
      baseInterfaces.length(0) ;
      CORBA::InterfaceDef_var interface =
         ifr->create_interface( "IDL:interface865:1.0",
                                "interface865",
                                "1.0",
                                baseInterfaces  ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;

      // Add an operation to the interface.
      // First get some useful things.
      //
      CORBA::PrimitiveDef_var voidPrimitive =
         ifr->get_primitive( CORBA::pk_void ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;
      CORBA::PrimitiveDef_var charPrimitive =
         ifr->get_primitive( CORBA::pk_char ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;
      CORBA::PrimitiveDef_var longPrimitive =
         ifr->get_primitive( CORBA::pk_long ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;
      CORBA::PrimitiveDef_var shortPrimitive =
         ifr->get_primitive( CORBA::pk_short ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;

      // The operation has three parameters...
      //
      CORBA::ULong numParams = 3 ;
      CORBA::ParDescriptionSeq parameters( numParams ) ;
      parameters.length( numParams ) ;

      // ... which are: in char p1...
      //
      parameters[0].name = CORBA::string_dup("p1") ;
      parameters[0].type_def =
         CORBA::PrimitiveDef::_duplicate( charPrimitive.in() ) ;
      parameters[0].type = charPrimitive->type() ;
      parameters[0].mode = CORBA::PARAM_IN ;

      // ...out long p2...
      //
      parameters[1].name = CORBA::string_dup("p2") ;
      parameters[1].type_def =
         CORBA::PrimitiveDef::_duplicate( longPrimitive.in() ) ;
      parameters[1].type = longPrimitive->type() ;
      parameters[1].mode = CORBA::PARAM_OUT ;

      // ...and inout short p3
      //
      parameters[2].name = CORBA::string_dup("p3") ;
      parameters[2].type_def =
         CORBA::PrimitiveDef::_duplicate( shortPrimitive.in() ) ;
      parameters[2].type = shortPrimitive->type() ;
      parameters[2].mode = CORBA::PARAM_INOUT ;

      // ...and no exceptions...
      //
      CORBA::ExceptionDefSeq exceptions( 1 ) ;
      exceptions.length( 0 ) ;

      // ...and no context ids
      //
      CORBA::ContextIdSeq contextIds( 1 ) ;
      contextIds.length( 0 ) ;

      // Create the operation, called "f".
      //
      CORBA::OperationDef_var operation =
         interface->create_operation( "IDL:interface865/f:1.0",
                                      "f",
                                      "1.0",
                                      voidPrimitive.in(),
                                      CORBA::OP_NORMAL,
                                      parameters,
                                      exceptions,
                                      contextIds ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;



      // Create operation list.
      //
      CORBA::NVList_var opList ;

      ACE_DEBUG((LM_DEBUG, "About to call create_operation_list\n"));

      orb->create_operation_list(operation.in (),
		                 opList.out()
				 ACE_ENV_ARG_PARAMETER) ;
      ACE_TRY_CHECK;

      ACE_DEBUG((LM_DEBUG, "Call to create_operation_list succeeded\n"));

      CORBA::ULong count = opList->count() ;
      ACE_TRY_CHECK;

      if( count != numParams )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed - wrong number of elements n list\n")) ;
         failed = 1 ;
      }

      CORBA::NamedValue_ptr nv = opList->item( 0 ) ;
      if(ACE_OS::strcmp( nv->name(), "p1") != 0 )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 1 wrong name\n"));
         failed = 1 ;
      };

      CORBA::Boolean const eq_char =
        nv->value()->type()->equal (CORBA::_tc_char
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if( !eq_char  )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 1 wrong type\n"));
         failed = 1 ;
      };
      if( nv->flags() != CORBA::ARG_IN )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 1 wrong mode\n"));
         failed = 1 ;
      };

      nv = opList->item( 1 ) ;
      if(ACE_OS::strcmp( nv->name(), "p2") != 0 )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 2 wrong name\n"));
         failed = 1 ;
      };

      CORBA::Boolean const eq_long =
        nv->value()->type()->equal (CORBA::_tc_long
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if( !eq_long  )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 2 wrong type\n"));
         failed = 1 ;
      };
      if( nv->flags() != CORBA::ARG_OUT )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 2 wrong mode\n"));
         failed = 1 ;
      };

      nv = opList->item( 2 ) ;
      if(ACE_OS::strcmp( nv->name(), "p3") != 0 )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 3 wrong name\n"));
         failed = 1 ;
      };

      CORBA::Boolean const eq_short =
        nv->value()->type()->equal (CORBA::_tc_short
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if( !eq_short  )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 3 wrong type\n"));
         failed = 1 ;
      };
      if( nv->flags() != CORBA::ARG_INOUT )
      {
         ACE_DEBUG((LM_DEBUG, "Test failed: param 3 wrong mode\n"));
         failed = 1 ;
      };


     // opList->free();
      //operation->destroy();

      // Finally destroy the interface.
      //
      interface->destroy(ACE_ENV_SINGLE_ARG_PARAMETER) ;

      //orb->destroy();

      if( failed == 1 )
      {
         return 1 ;
      }
   }
   ACE_CATCHANY
   {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception - test failed:\n");
      return 1;
   }
   ACE_CATCHALL
   {
     ACE_DEBUG((LM_DEBUG, "An unknown exception occured - test failed\n"));
     return 1;
   }
   ACE_ENDTRY;

   ACE_CHECK_RETURN(1);

   return 0 ;
}
