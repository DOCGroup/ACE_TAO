// $Id$

// ============================================================================
//
// = LIBRARY
//    
//
// = FILENAME
//    clnt.cpp
//
// = DESCRIPTION
//
// = AUTHORS
//   Michael Kircher
//
// ============================================================================

#include "tao/corba.h"
#include "clnt.h"
#include "NavWeapC.h"

ACE_RCSID(DOVEMIB, clnt, "$Id$")

Any_Test_Client::Any_Test_Client ()
{

}

Any_Test_Client::~Any_Test_Client ()
{
    CORBA::release(this->any_test_ptr_);
}

int
Any_Test_Client::init (int argc, char *argv [])
{
  
  ACE_TRY_NEW_ENV 
  {
     this->argc_ = argc;
     this->argv_ = argv;
            
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
				                            this->argv_,
				                            "internet",
				                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (this->argc_ != 2)
	    { 
	      ACE_ERROR_RETURN ((LM_ERROR, "Expected an IOR as parameter\n"),-1);
	    }

      CORBA::Object_var any_test_object_ =  this->orb_->string_to_object (argv_[1], ACE_TRY_ENV);
      ACE_TRY_CHECK;  

      this->any_test_ptr_ = Any_Test::_narrow (any_test_object_.in(), ACE_TRY_ENV);	
      ACE_TRY_CHECK;

      if (CORBA::is_nil (this->any_test_ptr_))
	    {
	      ACE_ERROR_RETURN ((LM_ERROR,
			                    "Invalid IOR specified.\n"),
			                    -1);
	    }     
      return 0;
  }
  ACE_CATCHANY {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    return 0;
  }
  ACE_ENDTRY;
  return 0;
}

int
Any_Test_Client::run ()
{
  ACE_TRY_NEW_ENV
    {
      CORBA::Any data; 

      navigation_.position_latitude = 1;
      navigation_.position_longitude = 2;
      navigation_.altitude = 3;
      navigation_.heading = 4;
      navigation_.roll = 5;
      navigation_.pitch = 6;

      navigation_.utilization = 1;
      navigation_.overhead = 2;
      navigation_.arrival_time = 3;
      navigation_.deadline_time = 4;
      navigation_.completion_time = 5;
      navigation_.computation_time = 6;

      data.replace (_tc_Navigation, &navigation_, 0, ACE_TRY_ENV);

      ACE_DEBUG ((LM_DEBUG,"Starting test with Any: Navigation\n")); 
      //any_test_ptr_->try_an_any (data, TAO_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,"Ending test with Any: Navigation\n"));   


/*      CORBA::Short x_ = 8;
      data <<= x_;

      ACE_DEBUG ((LM_DEBUG,"Starting test with Any: Navigation\n")); 
      any_test_ptr_->try_an_any (data, TAO_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,"Ending test with Any: Navigation\n"));   
*/      
      
      weapons_.number_of_weapons = 5;
      weapons_.weapon1_identifier = CORBA::string_alloc (20);
      strcpy (weapons_.weapon1_identifier.out (), "A");
      weapons_.weapon1_status = 1;
      weapons_.weapon2_identifier = CORBA::string_alloc (20);
      strcpy (weapons_.weapon2_identifier.out (), "B");
      weapons_.weapon2_status = 2;
      weapons_.weapon3_identifier = CORBA::string_alloc (20);
      strcpy (weapons_.weapon3_identifier.out (), "C");
      weapons_.weapon3_status = 4;
      weapons_.weapon4_identifier = CORBA::string_alloc (20);
      strcpy (weapons_.weapon4_identifier.out (), "D");
      weapons_.weapon4_status = 8;
      weapons_.weapon5_identifier = CORBA::string_alloc (20);
      strcpy (weapons_.weapon5_identifier.out (), "E");
      weapons_.weapon5_status = 16;
      
      weapons_.utilization = 1;
      weapons_.overhead = 2;
      weapons_.arrival_time = 3;
      weapons_.deadline_time = 4;
      weapons_.completion_time = 5;
      weapons_.computation_time = 6;
      
      data.replace (_tc_Weapons, &weapons_, 0, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      ACE_DEBUG ((LM_DEBUG,"Starting test with Any: Weapons\n")); 
      any_test_ptr_->try_an_any (data, ACE_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,"Ending test with Any: Weapons\n"));   	
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Error in Any_Test_Client::run");
    }
  ACE_ENDTRY;

      
  return 0;
}



// function main

int
main (int argc, char *argv [])
{

  ACE_TRY_NEW_ENV
    {
      Any_Test_Client any_test_client_;
      ACE_TRY_CHECK;

      if (any_test_client_.init (argc, argv) == -1)
	return 1;
      ACE_TRY_CHECK;

      return any_test_client_.run ();
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "SYS_EX");
    }
  ACE_ENDTRY;


  return 0;
}

