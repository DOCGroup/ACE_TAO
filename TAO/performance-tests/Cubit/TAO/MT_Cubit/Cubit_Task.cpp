// $Id$

#include "ace/Sched_Params.h"
#include "Globals.h"
#include "Cubit_Task.h"
#include "tao/TAO_Internal.h"

Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const char *args,
                        const char *orbname,
                        u_int num_of_objs,
                        ACE_Thread_Manager *thr_mgr,
                        u_int task_id)
  : ACE_Task<ACE_SYNCH> (thr_mgr),
    key_ ("Cubit"),
    orbname_ ((char *) orbname),
    orbargs_ ((char *) args),
    num_of_objs_ (num_of_objs),
    servants_ (0),
    servants_iors_ (0),
    task_id_ (task_id)
{
}

int
Cubit_Task::svc (void)
{
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  // thr_getprio () on the current thread should never fail.

  if (ACE_OS::thr_getprio (thr_handle, prio) == -1)
    return -1;

  //   char buffer[BUFSIZ];
  //   sprintf (buffer, "server%d.log", this->task_id_);
  //   ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::OSTREAM);
  //   ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::STDERR);
  //   ACE_Log_Msg::instance()->clr_flags (ACE_Log_Msg::LOGGER);
  //  ofstream log (buffer);
  // log.setf (ios::unitbuf);
  //  log.rdbuf()->unbuffered (1);
  //ACE_Log_Msg::instance()->msg_ostream (&log);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Beginning Cubit task with args = '%s' and priority %d\n",
              orbargs_,
              prio));
  int result = this->initialize_orb ();

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ORB initialization failed\n"),
                      -1);
  result = this->create_servants ();
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Create Servants failed.\n"),
                      -1);
  TAO_TRY
    {
      GLOBALS::instance ()->barrier_->wait ();

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      if (this->orb_manager_.run (TAO_TRY_ENV) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "run"),
                          -1);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("poa->destroy()");
    }
  TAO_ENDTRY;

  // Need to clean up and do a CORBA::release on everything we've
  // created!

  for (u_int i = 0; i < num_of_objs_; i++)
    delete this->servants_[i];

  return 0;
}

int
Cubit_Task::initialize_orb (void)
{
  TAO_TRY
    {
      ACE_ARGV args (this->orbargs_);

      int argc = args.argc ();
      char **argv = args.argv ();

      if (this->orb_manager_.init_child_poa (argc,
                                             argv,
                                             "persistent_poa",
                                             TAO_TRY_ENV) == -1)
        return -1;

      TAO_CHECK_ENV;

      this->orb_ = this->orb_manager_.orb ();

      // Do the argument parsing.
      if (this->task_id_ == 0)
        {
          if (GLOBALS::instance ()->parse_args (argc,
                                                argv) == -1)
            return -1;
          ACE_NEW_RETURN (GLOBALS::instance ()->barrier_,
                          ACE_Barrier (GLOBALS::instance ()->num_of_objs + 1),
                          -1);
          ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ready_mon, GLOBALS::instance ()->ready_mtx_, 1));
          GLOBALS::instance ()->ready_ = 1;
          GLOBALS::instance ()->ready_cnd_.broadcast ();
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("orb_init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

CORBA::String
Cubit_Task::get_servant_ior (u_int index)
{
  if (index >= num_of_objs_)
    return 0;
  else
    return ACE_OS::strdup (this->servants_iors_[index]);
}

int
Cubit_Task::create_servants (void)
{
  TAO_TRY
    {
      // Create the array of cubit implementations.
      ACE_NEW_RETURN (this->servants_,
                      Cubit_i *[this->num_of_objs_],
                      -1);

      // Create the array of strings.
      ACE_NEW_RETURN (this->servants_iors_,
                      CORBA::String [this->num_of_objs_],
                      -1);

      char *buffer;
      // Length of the string is the length of the key + 2 char id of
      // the servant + null space.
      int len = ACE_OS::strlen (this->key_) + 3;

      ACE_NEW_RETURN (buffer,
                      char[len],
                      -1);

      // This loop creates multiple servants, and prints out their
      // IORs.
      for (u_int i = 0;
           i < this->num_of_objs_;
           i++)
        {
          ACE_OS::sprintf (buffer,
                           "%s%02d",
                           (char *) this->key_,
                           this->task_id_);

          ACE_NEW_RETURN (this->servants_[i],
                          Cubit_i (this->orb_.in ()),
                          -1);

          if (this->servants_[i] == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to create "
                               "implementation object #%d\n",
                               i),
                              2);

          this->orb_manager_.activate_under_child_poa
            (buffer,
             this->servants_[i],
             TAO_TRY_ENV);
          TAO_CHECK_ENV;

          // Stringify the objref we'll be implementing, and print it
          // to stdout.  Someone will take that string and give it to
          // some client.  Then release the object.

          Cubit_var cubit =
            this->servants_[i]->_this (TAO_TRY_ENV);
          TAO_CHECK_ENV;

          CORBA::String_var str =
            this->orb_->object_to_string (cubit.in (),
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->servants_iors_[i] =
            ACE_OS::strdup (str.in ());
        }

      delete [] buffer;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("print IOR");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}
