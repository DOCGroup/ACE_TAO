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
                           ACE_TCHAR *argv[])
{
  // Parses some of the options that are specific to this example
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("df:nk:xw:h:p:q:v:"));

  int c = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p': // A horizontal position of the grid where
                // the value is stored
        setx_ = (CORBA::UShort) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'q': // A vertical position of the grid where a
                // value is stored
        sety_ = (CORBA::UShort) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'w':
        width_ = (CORBA::UShort) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
        height_ = (CORBA::UShort) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'v':
        value_ = (CORBA::UShort) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      }

  if (setx_ == 0)
    setx_ = Grid::DEFAULT_LOCATION;
  if (sety_ == 0)
    sety_ = Grid::DEFAULT_LOCATION;
  if (value_ == 0)
    value_ = Grid::DEFAULT_VALUE;
  return 0;
}

int
Grid_Client_i::run (const char *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;

  try
    {
      // Make the Grid.
      Grid_var grid = client_->make_grid (width_,
                                          height_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Made the grid succesfully\n")));

      // Set a value on the grid
      grid->set (setx_,
                 sety_,
                 value_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Setting a value for the grid\n")));

      CORBA::Long ret_val = grid->get (setx_,
                                       sety_);

      ACE_ASSERT (ret_val == value_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Changing grid's width from %i to 5\n"),
                  width_));

      grid->width (5);

      ret_val = grid->get (setx_,
                           sety_);

      ACE_ASSERT (ret_val == value_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Changing grid's height from %d to 5\n"),
                  height_));

      grid->height (5);

      ret_val = grid->get (setx_,
                           sety_);

      ACE_ASSERT (ret_val == value_);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Destroing the grid\n")));

      grid->destroy ();

      try
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Accessing the grid out of range\n")));

          grid->get (setx_,
                     sety_);
        }
      catch (const Grid::RANGE_ERROR &)
        {
          // It's ok.
        }

      if (client_.do_shutdown () == 1)
        client_->shutdown ();

      ACE_UNUSED_ARG (ret_val);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception in Grid_Client_i::run:");
      return -1;
    }

  return 0;
}
