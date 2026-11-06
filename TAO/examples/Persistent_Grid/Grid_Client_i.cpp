#include "Grid_Client_i.h"
#include "ace/Get_Opt.h"

// This is the interface program that accesses the remote object

int
Grid_Client_i::parse_args (int argc,
                           ACE_TCHAR *argv[])
{
  // Parses some of the options that are specific to this example
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("df:nk:xw:h:v:"));

  int c {};
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'w':
        this->width_ = static_cast<CORBA::Short> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'h':
        this->height_ = static_cast<CORBA::Short> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      case 'v':
        this->value_ = static_cast<CORBA::Long> (ACE_OS::atoi (get_opts.opt_arg ()));
        break;
      }

  if (this->width_ == 0)
    this->width_ = Grid::DEFAULT_SIZE;
  if (this->height_ == 0)
    this->height_ = Grid::DEFAULT_SIZE;
  if (this->value_ == 0)
    this->value_ = Grid::DEFAULT_VALUE;
  return 0;
}

int
Grid_Client_i::run (char const *name,
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

      for (CORBA::Short row {}; row < this->width_; ++row)
        {
          for (CORBA::Short column {}; column < this->height_; ++column)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "Grid value [%d][%d] = %d\n",
                          row,
                          column,
                          this->value_ + column));
              // Set a value on the grid
              grid->set (row,
                         column,
                         this->value_ + column);
            }
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Setting a value for the grid\n"));
      if (this->client.shutdown ())
        {
          this->client->shutdown ();
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

