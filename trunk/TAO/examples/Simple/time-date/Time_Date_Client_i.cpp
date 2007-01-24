//$Id$

#include "Time_Date_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// This is the interface program that accesses the remote object

// Constructor.
Time_Date_Client_i::Time_Date_Client_i (void)
{
  // no-op
}

//Destructor.
Time_Date_Client_i::~Time_Date_Client_i (void)
{
  // no-op
}

int
Time_Date_Client_i::parse_args (int,
                                char *[])
{
  return 0;
}

int
Time_Date_Client_i::run (const char *name,
                         int argc,
                         char *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Long l;

      // Get the time & date in binary format.
      client_->bin_date (l
                         ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Binary time_date = %d\n",
                  l));

      // Get the time & date in string format.
      CORBA::String_var str_var;
      client_->str_date (str_var.out()
                         ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) String time_date = %s\n",
                  str_var.in()));

      client_.shutdown ();
    }
  ACE_CATCH (CORBA::UserException, range_ex)
    {
      ACE_PRINT_EXCEPTION (range_ex,
                           "\tFrom get and set time_date");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, memex)
    {
      ACE_PRINT_EXCEPTION (memex,
                           "Cannot make time_date");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

