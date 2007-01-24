//
// $Id$
//
#include "tao/corba.h"

// Attempts to resolve the NameService.
// Returns:
//    0  if the NameService was resolved
//    1  if the NameService could not be resolved
//    2  if something else went wrong
//
int main( int argc, char *argv[] )
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb = CORBA::ORB_init( argc, argv, 0 ACE_ENV_ARG_PARAMETER ) ;

      ACE_TRY_EX (nested)
        {
           CORBA::Object_var naming =
              orb->resolve_initial_references( "NameService" ACE_ENV_ARG_PARAMETER) ;
           ACE_TRY_CHECK_EX (nested);
           if( CORBA::is_nil( naming.in() ) )
             {
                return 1 ;
             }
        }
      ACE_CATCH (CORBA::Exception, ex)
        {
          ACE_UNUSED_ARG (ex);
          return 1 ;
        }
      ACE_ENDTRY;
    }
  ACE_CATCH ( CORBA::Exception, ex )
    {
       ACE_UNUSED_ARG (ex);
       return 2 ;
    }
  ACE_ENDTRY;

  return 0 ;
}
