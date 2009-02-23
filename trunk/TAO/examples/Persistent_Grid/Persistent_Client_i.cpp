//$Id$
#include "Persistent_Client.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

// This is the interface program that accesses the remote object

// Constructor.
Persistent_Client_i::Persistent_Client_i (void)
  : height_ (0),
    width_ (0)
{
  //no-op
}

//Destructor.
Persistent_Client_i::~Persistent_Client_i (void)
{
  //no-op
}

int
Persistent_Client_i::parse_args (int argc,
                                 ACE_TCHAR *argv[])
{
  // Parses some of the options that are specific to this example
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:f:xw:h:r"));

  int c = 0;
  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
      case 'w':
        this->width_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'h':
        this->height_ = (u_int) ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'r':
        this->remove_ = 1;
        break;
      }
    }

  return 0;
}

int
Persistent_Client_i::run (const char *name,
                          int argc,
                          ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;


  try
    {
      // Make the Grid.
      Grid_ptr grid = client->make_grid (width_,
                                         height_);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid succesfully\n"));


      for (CORBA::Short index_ = 0; index_ < height_; index_++)
        {
          for (CORBA::Short ctr = 0; ctr < width_; ctr++)
            {
              CORBA::Long ret_val = grid->get (index_,
                                               ctr);

              ACE_DEBUG ((LM_DEBUG,
                          "Grid value [%d][%d] =  %d\n",index_, ctr,ret_val));
            }
        }

      if (client.shutdown () == 1) {
        client->shutdown ();
      }

      if (this->remove_ == 1) {
        client->cleanup ();
      }
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

