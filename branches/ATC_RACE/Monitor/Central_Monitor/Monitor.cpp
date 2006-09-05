#include "Monitor.h"

namespace CIAO
{
  namespace RACE
  {


    Monitor_i::Monitor_i ()
    {
    }

    Monitor_i::~Monitor_i ()
    {
    }

    void
    Monitor_i::push_delays (const char * id,
                            const Delays & delay)
      throw (CORBA::SystemException)
    {
      ACE_DEBUG ((LM_DEBUG, "Trying to register e-2-e execution times "
                  "for %s\n", id));

      switch (this->delay_map_.bind (id, delay))
        {
          /// Created new entry for id.
          case 0:
            {

              ACE_DEBUG ((LM_DEBUG, "New entry created in the table.\n"));
              break;
            }

            /// id has been already registered. Therefore, update the contents.
          case 1:
            {
              ACE_DEBUG ((LM_DEBUG, "Existing entry found in the table."
                          "Updating the contents...."));
              /// Retrieve the existing entry from the table.
              Delays prev_entry;
              this->delay_map_.find (id, prev_entry);

              /// "Merge" the existing entry and the new enty and store it
              /// back intro the table.
              CORBA::ULong prev_len = prev_entry.length ();
              prev_entry.length (prev_len + delay.length ());
              for (CORBA::ULong itr = prev_len;
                   itr < prev_entry.length ();
                   ++itr)
                {
                  prev_entry [itr] = delay [itr - prev_len];
                }

              if (this->delay_map_.rebind (id, prev_entry)  == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              "ERROR while updating the contents.\n"));
                  break;
                }

              ACE_DEBUG ((LM_DEBUG, "done!\n"));
              break;
            }


            /// This should never happen! -- Unknown error while accessing
            /// the table.
          default:
            {

              ACE_ERROR ((LM_ERROR, "Unknown error while registering e-2-e "
                          "execution times for %s\n.", id));
              break;
            }

        }
      return;
    }

    Delays *
    Monitor_i::get_delays (const char * id)
      throw (::CORBA::SystemException, ::CIAO::RACE::Monitor::IdNotFound)
    {
      Delays temp_delay;
      ACE_DEBUG ((LM_DEBUG, "Searching the table for %s...", id));
      if (this->delay_map_.find (id, temp_delay) != 0)
        {
          ACE_DEBUG ((LM_DEBUG, "Oops! No entry found!\n"));
          throw ::CIAO::RACE::Monitor::IdNotFound ();
        }
      ACE_DEBUG ((LM_DEBUG, "done! Entry found and returned!\n"));

      /// Remove the entry for id from the table.
      this->delay_map_.unbind (id);

      /// Return the delays associated with id.
      Delays_var ret (temp_delay);
      return ret._retn ();
    }


  };
};
