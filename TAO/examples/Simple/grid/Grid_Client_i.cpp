//$Id$

#include "Grid_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

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
  ACE_Get_Opt get_opts (argc, argv, "w:h:p:q:v:");

  int c = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p': // A horizontal position of the grid where
                // the value is stored
        setx_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'q': // A vertical position of the grid where a
                // value is stored
        sety_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'w':
        width_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'h':
        height_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'v':
        value_ = (u_int) ACE_OS::atoi (get_opts.optarg);
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
Grid_Client_i::run (char *name,
                    int argc,
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name,argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  ACE_TRY_NEW_ENV
    {
      // Make the Grid.
      
      Grid_ptr grid = client->make_grid (width_,
                                         height_,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid succesfully\n"));

      // Set a value on the grid
      grid->set (setx_,
                 sety_,
                 value_,
                 ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the grid\n"));

      CORBA::Long ret_val = grid->get (setx_,
                                       sety_,
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_ASSERT (ret_val == value_);
  
      if (client.shutdown () == 1)
        client->shutdown (ACE_TRY_ENV);
      ACE_UNUSED_ARG (ret_val);
    }
  ACE_CATCH (CORBA::UserException, range_ex)
    {
      ACE_UNUSED_ARG (range_ex);
      ACE_TRY_ENV.print_exception ("\tFrom get and set grid");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, memex)
    {
      ACE_UNUSED_ARG (memex);
      ACE_TRY_ENV.print_exception (" Cannot make grid as Memory exhausted");
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Grid_Factory,Grid_Factory_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Grid_Factory,Grid_Factory_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
