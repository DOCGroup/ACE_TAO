// $Id$

#include "ace/Sched_Params.h"
#include "Globals.h"
#include "Cubit_Task.h"
#include "tao/TAO_Internal.h"
#include "ace/Barrier.h"

Cubit_Task::Cubit_Task (void)
{
  // No-op.
}

Cubit_Task::Cubit_Task (const ACE_TCHAR *args,
                        const char *orbname,
                        u_int num_of_objs,
                        ACE_Thread_Manager *thr_mgr,
                        u_int task_id)
  : ACE_Task<ACE_SYNCH> (thr_mgr),
    key_ ("Cubit"),
    orbname_ (orbname),
    orbargs_ (args),
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

  try
    {
      GLOBALS::instance ()->barrier_->wait ();

      // Handle requests for this object until we're killed, or one of
      // the methods asks us to exit.
      int r = this->orb_manager_.run ();

      if (r == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "run"),
                          -1);

      CORBA::ORB_var orb =
        this->orb_manager_.orb ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("poa->destroy()");
    }

  // Need to clean up and do a CORBA::release on everything we've
  // created!

  for (u_int i = 0; i < num_of_objs_; i++)
    delete this->servants_[i];

  return 0;
}

int
Cubit_Task::initialize_orb (void)
{
  try
    {
      ACE_ARGV args (this->orbargs_);

      int argc = args.argc ();
      ACE_TCHAR **argv = args.argv ();

      char orb_name[BUFSIZ];
      ACE_OS::sprintf (orb_name,
                       "ORB %d",
                       this->task_id_);

      int r = this->orb_manager_.init_child_poa (argc,
                                                 argv,
                                                 "persistent_poa",
                                                 orb_name);
      if (r == -1)
        return -1;

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
          ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ready_mon,
                                    GLOBALS::instance ()->ready_mtx_, 1));
          GLOBALS::instance ()->ready_ = 1;
          GLOBALS::instance ()->ready_cnd_.broadcast ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("orb_init");
      return -1;
    }

  return 0;
}

char*
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
  try
    {
      CORBA::Object_var obj =
        this->orb_->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());

      PortableServer::POAManager_var manager =
        poa->the_POAManager();

      manager->activate();

      // Create the array of cubit implementations.
      ACE_NEW_RETURN (this->servants_,
                      Cubit_i *[this->num_of_objs_],
                      -1);

      // Create the array of strings.
      ACE_NEW_RETURN (this->servants_iors_,
                      char* [this->num_of_objs_],
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
                           this->key_,
                           this->task_id_);

          ACE_NEW_RETURN (this->servants_[i],
                          Cubit_i (this->orb_.in (),
                                   poa.in ()),
                          -1);

          if (this->servants_[i] == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               " (%P|%t) Unable to create "
                               "implementation object #%d\n",
                               i),
                              2);

          // Stringify the objref we'll be implementing, and print it
          // to stdout.  Someone will take that string and give it to
          // some client.  Then release the object.

          PortableServer::ObjectId_var id_act =
            poa->activate_object (this->servants_[i]);

          CORBA::Object_var object_act = poa->id_to_reference (id_act.in ());

          Cubit_var cubit =
            Cubit::_narrow (object_act.in ());


          CORBA::String_var str =
            this->orb_->object_to_string (cubit.in ());

          this->servants_iors_[i] =
            ACE_OS::strdup (str.in ());
        }

      delete [] buffer;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Cubit_Task::create_servants");
      return -1;
    }
  return 0;
}
