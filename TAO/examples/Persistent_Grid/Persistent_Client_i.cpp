#include "Persistent_Client.h"
#include "ace/Get_Opt.h"

// This is the interface program that accesses the remote object

int
Persistent_Client_i::parse_args (int argc,
                                 ACE_TCHAR *argv[])
{
  // Parses some of the options that are specific to this example
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:f:xw:h:r"));

  int c {};
  while ((c = get_opts ()) != -1)
    {
      switch (c)
      {
      case 'w':
        this->width_ = static_cast<CORBA::Short> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'h':
        this->height_ = static_cast<CORBA::Short> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'r':
        this->remove_ = true;
        break;
      }
    }

  return 0;
}

int
Persistent_Client_i::run (char const *name,
                          int argc,
                          ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (this->client.init (name, argc, argv) == -1)
    return -1;

  if (this->parse_args (argc, argv) == -1)
    return -1;
  try
    {
      // Make the Grid.
      Grid_ptr const grid = this->client->make_grid (this->width_,
                                                     this->height_);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Made the grid successfully\n"));
      for (CORBA::Short row {}; row < this->height_; ++row)
        {
          for (CORBA::Short column {}; column < this->width_; ++column)
            {
              CORBA::Long const ret_val = grid->get (row,
                                                     column);

              ACE_DEBUG ((LM_DEBUG,
                          "Grid value [%d][%d] = %d\n",
                          row,
                          column,
                          ret_val));
            }
        }

      if (this->client.shutdown ())
        {
          this->client->shutdown ();
        }

      if (this->remove_)
        {
          this->client->cleanup ();
        }
    }
  catch (CORBA::UserException const &range_ex)
    {
      range_ex._tao_print_exception ("\tFrom get and set grid");
      return -1;
    }
  catch (CORBA::SystemException const &memex)
    {
      memex._tao_print_exception ("Cannot make grid as Memory exhausted");
      return -1;
    }

  return 0;
}

