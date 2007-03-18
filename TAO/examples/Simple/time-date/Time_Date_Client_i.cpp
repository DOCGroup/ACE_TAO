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


  try
    {
      CORBA::Long l;

      // Get the time & date in binary format.
      client_->bin_date (l);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Binary time_date = %d\n",
                  l));

      // Get the time & date in string format.
      CORBA::String_var str_var;
      client_->str_date (str_var.out());

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) String time_date = %s\n",
                  str_var.in()));

      client_.shutdown ();
    }
  catch (const CORBA::UserException& range_ex)
    {
      range_ex._tao_print_exception ("\tFrom get and set time_date");
      return -1;
    }
  catch (const CORBA::SystemException& memex)
    {
      memex._tao_print_exception ("Cannot make time_date");
      return -1;
    }

  return 0;
}

