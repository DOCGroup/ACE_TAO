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
  
  TAO_TRY {
     this->argc_ = argc;
     this->argv_ = argv;
            
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
				                            this->argv_,
				                            "internet",
				                            TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->argc_ != 2)
	    { 
	      ACE_ERROR_RETURN ((LM_ERROR, "Expected an IOR as parameter\n"),-1);
	    }

      CORBA::Object_var any_test_object_ =  this->orb_->string_to_object (argv_[1], TAO_TRY_ENV);
      TAO_CHECK_ENV;  

      this->any_test_ptr_ = Any_Test::_narrow (any_test_object_.in(), TAO_TRY_ENV);	
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->any_test_ptr_))
	    {
	      ACE_ERROR_RETURN ((LM_ERROR,
			                    "Invalid IOR specified.\n"),
			                    -1);
	    }     
      return 0;
  }
  TAO_CATCHANY {
    TAO_TRY_ENV.print_exception ("SYS_EX");
    return 0;
  }
  TAO_ENDTRY;
  return 0;
}

int
Any_Test_Client::run ()
{
  TAO_TRY
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

      data.replace (_tc_Navigation, &navigation_, 0, TAO_TRY_ENV);

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
      
      data.replace (_tc_Weapons, &weapons_, 0, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      ACE_DEBUG ((LM_DEBUG,"Starting test with Any: Weapons\n")); 
      any_test_ptr_->try_an_any (data, TAO_TRY_ENV);
      ACE_DEBUG ((LM_DEBUG,"Ending test with Any: Weapons\n"));   	
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Error in Any_Test_Client::run");
    }
  TAO_ENDTRY;

      
  return 0;
}



// function main

int
main (int argc, char *argv [])
{

  TAO_TRY
    {
      Any_Test_Client any_test_client_;
      TAO_CHECK_ENV;

      if (any_test_client_.init (argc, argv) == -1)
	return 1;
      TAO_CHECK_ENV;

      return any_test_client_.run ();
      TAO_CHECK_ENV;


    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;


  return 0;
}

