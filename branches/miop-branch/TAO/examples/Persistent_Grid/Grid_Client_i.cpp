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
  ACE_Get_Opt get_opts (argc, argv, "df:nk:xw:h:v:");

  int c = 0;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'w':
        this->width_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
        this->height_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'v':
        this->value_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      }

  if (width_ == 0 )
    width_ = Grid::DEFAULT_SIZE;
  if (height_ == 0 )
    height_ = Grid::DEFAULT_SIZE;
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

  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Make the Grid.

      Grid_ptr grid = client->make_grid (width_,
                                         height_
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid succesfully\n"));

      for (CORBA::Short index_ = 0; index_ < width_; index_++)
        {
          for (CORBA::Short ctr = 0; ctr < height_; ctr++)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Grid value [%d][%d] =  %d \n",index_, ctr,value_+ctr));
              // Set a value on the grid
              grid->set (index_,
                         ctr,
                         (value_ + ctr)
                         TAO_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the grid\n"));

      ACE_TRY_CHECK;

      if (client.shutdown () == 1) {
        client->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }

    }
  ACE_CATCH (CORBA::UserException, range_ex)
    {
      ACE_PRINT_EXCEPTION (range_ex,
                           "\tFrom get and set grid");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, memex)
    {
      ACE_PRINT_EXCEPTION (memex,
                           "Cannot make grid as Memory exhausted");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Client<Grid_Factory,Grid_Factory_var>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Client<Grid_Factory,Grid_Factory_var>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
