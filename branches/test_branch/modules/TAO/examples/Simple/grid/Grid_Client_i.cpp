//$Id$

#include "Grid_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"

// This is the interface program that accesses the remote object

// Constructor.
Grid_Client_i::Grid_Client_i (void)
  : height_ (0),
    width_ (0),
    setx_ (0),
    sety_ (0),
    value_ (0)

{
  //no-op
}

//Destructor.
Grid_Client_i::~Grid_Client_i (void)
{
  //no-op
}

int
Grid_Client_i::parse_args (int argc,
                           char *argv[])
{
  // Parses some of the options that are specific to this example
  ACE_Get_Opt get_opts (argc, argv, "df:nk:xw:h:p:q:v:");

  int c = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p': // A horizontal position of the grid where
                // the value is stored
        setx_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'q': // A vertical position of the grid where a
                // value is stored
        sety_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'w':
        width_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
        height_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'v':
        value_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      }

  if (setx_ == 0 )
    setx_ = Grid::DEFAULT_LOCATION;
  if (sety_ == 0 )
    sety_ = Grid::DEFAULT_LOCATION;
  if (value_ == 0 )
    value_ = Grid::DEFAULT_VALUE;
  return 0;
}

int
Grid_Client_i::run (const char *name,
                    int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;


  try
    {
      // Make the Grid.

      Grid_var grid = client->make_grid (width_,
                                         height_);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid succesfully\n"));

      // Set a value on the grid
      grid->set (setx_,
                 sety_,
                 value_);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the grid\n"));

      CORBA::Long ret_val = grid->get (setx_,
                                       sety_);

      ACE_ASSERT (ret_val == value_);

      if (client.shutdown () == 1) {
        client->shutdown ();
      }

      ACE_UNUSED_ARG (ret_val);
    }
  catch (const CORBA::UserException& range_ex)
    {
      range_ex._tao_print_exception ("\tFrom get and set grid");
      return -1;
    }
  catch (const CORBA::SystemException& memex)
    {
      memex._tao_print_exception ("Cannot make grid as Memory exhausted");
      return -1;
    }

  return 0;
}

