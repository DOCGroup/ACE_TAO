// $Id$

#include "Options_Parser.h"

ACE_RCSID(lib, TAO_Options_Parser, "$id$")

#include "tao/debug.h"
#include "orbsvcs/NotifyExtC.h"

TAO_NS_Options_Parser::TAO_NS_Options_Parser (void)
{
}

TAO_NS_Options_Parser::~TAO_NS_Options_Parser ()
{
}

void
TAO_NS_Options_Parser::execute (CosNotification::EventTypeSeq& added, CosNotification::EventTypeSeq& removed, ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR* current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      current_arg = arg_shifter.get_current ();
      arg_shifter.consume_arg ();

      if (current_arg[0] == '+')
        {
          // create 1 more space.
          int seq_ln =  added.length ();
          added.length (seq_ln + 1);

          added[seq_ln].domain_name = CORBA::string_dup ("*");
          added[seq_ln].type_name = CORBA::string_dup (++current_arg); // Skip the '+' sign.
        }
      else if (current_arg[0] == '-')
        {
          // create 1 more space.
          int seq_ln =  removed.length ();
          removed.length (seq_ln + 1);

          removed[seq_ln].domain_name = CORBA::string_dup ("*");
          removed[seq_ln].type_name = CORBA::string_dup (++current_arg); // Skip the '-' sign.
        }
    }
}

void
TAO_NS_Options_Parser::execute (CosNotification::QoSProperties& qos, ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;
  int default_priority = ACE_DEFAULT_THREAD_PRIORITY;

  if (arg_shifter.cur_arg_strncasecmp ("-ThreadPool") == 0) // -ThreadPool [-Threads static_threads] [-Priority default_priority]
    {
      arg_shifter.consume_arg ();

      int static_threads = 1;

      if (arg_shifter.cur_arg_strncasecmp ("-Threads") == 0)
        {
          arg_shifter.consume_arg ();

          current_arg = arg_shifter.get_current ();

          static_threads = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      if (arg_shifter.cur_arg_strncasecmp ("-Priority") == 0)
        {
          arg_shifter.consume_arg ();

          current_arg = arg_shifter.get_current ();

          default_priority = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      NotifyExt::ThreadPoolParams tp_params
        = { NotifyExt::CLIENT_PROPAGATED, default_priority,
            0, static_threads, 0, default_priority, 0, 0, 0 };

      qos.length (1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
      qos[0].value <<= tp_params;

    } /* ThreadPool */
  else if (arg_shifter.cur_arg_strncasecmp ("-ThreadPoolLanes") == 0) // -ThreadPoolLanes lane_count -Lane prio static_thr dy_thr
    {
      arg_shifter.consume_arg ();

      current_arg = arg_shifter.get_current ();
      int lanecount = ACE_OS::atoi (current_arg);

      arg_shifter.consume_arg ();

      NotifyExt::ThreadPoolLanesParams tpl_params;

      tpl_params.priority_model = NotifyExt::CLIENT_PROPAGATED;
      tpl_params.server_priority = default_priority,
      tpl_params.stacksize = 0;
      tpl_params.lanes.length (lanecount);
      tpl_params.allow_borrowing = 0;
      tpl_params.allow_request_buffering = 0;
      tpl_params.max_buffered_requests = 0;
      tpl_params.max_request_buffer_size = 0;

      int l_index = 0;
      //parse lane values ...
      while (arg_shifter.is_anything_left ())
        {
          if (arg_shifter.cur_arg_strncasecmp ("-Lane") == 0)
            {
              arg_shifter.consume_arg ();

              // read priority
              tpl_params.lanes[l_index].lane_priority = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();

              // static thread count
              tpl_params.lanes[l_index].static_threads = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();

              // dynamic thread count
              tpl_params.lanes[l_index].dynamic_threads = ACE_OS::atoi (arg_shifter.get_current ());
              arg_shifter.consume_arg ();

              if (TAO_debug_level > 0)
                ACE_DEBUG ((LM_DEBUG, "QoS Parser parsed lane: - %d, %d, %d\n",
                            tpl_params.lanes[l_index].lane_priority, tpl_params.lanes[l_index].static_threads, tpl_params.lanes[l_index].dynamic_threads));
              l_index++;
            }
          else
            break;
        } /* while -- lane values */

      qos.length (1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPoolLanes);
      qos[0].value <<= tpl_params;

    } /* ThreadPoolLane */
}
