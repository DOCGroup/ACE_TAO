//$Id$

#include "DD_servant_i.h"
#include "DD_servantC.h"

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/High_Res_Timer.h"
#include "ace/Timer_Queue.h"
#include "ace/Reactor.h"
/* add by Yfzhang */
#include "tao/Messaging/Messaging.h"
/* end of add */
#include <unistd.h>

#include "tao/RTScheduling/RTScheduler_Manager.h"
#include "EDF_Scheduler.h"
#include "orbsvcs/orbsvcs/Time_Utilities.h"
#include "Task_Stats.h"
#include "cpuload.h"

#include "chain_config.h"
#include "chain_dsui_families.h"
#include <dsui.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using namespace std;

ACE_RCSID(MT_Server, client, "client.cpp,v 1.2 2003/10/08 13:26:32 venkita Exp")

const int TOTAL_EXEC_TIME = 400;	/* sec */
const int SYNC_TIME = 60;		/* sec */

const int FILE_SZ = 128;
typedef struct DD_TASK_ {
	int task_id;

	int prev_node;		/* -1 means no previous node, head of a chain */
	char in_file[FILE_SZ];
	char in_file_name[FILE_SZ];
	int prev_bdone;

	int next_node;		/* -1 means no next node, tail of a chain */
	char out_file[FILE_SZ];
	int next_bdone;

	int exec_time;
	int period;
	int phase;
	int iteration;
} DD_TASK;

const int TASK_NUM = 16;
DD_TASK gdd_task[TASK_NUM];
int gdd_task_num = 0;

int gnode_id = 0;
const char *machine[5] = {"", "norbert", "hermione", "harry", "ron"};

void parse_configure_file(const char *cfg_file)
{
	fstream fs;
	string read;	
	
	fs.open(cfg_file, fstream::in);

	getline(fs, read);
	sscanf(read.c_str(), "node_id %d", &gnode_id);
	ACE_DEBUG ((LM_DEBUG, "[parse_configure_file] node_id %d\n", gnode_id));
	assert(gnode_id > 0);

	int task_id, prev_node, next_node, exec_time, period, phase, iteration, rtn;
	while(!fs.eof()) {
		getline(fs, read);
		if(read.length() == 0) break;

		if(*read.c_str() == '#') continue;

		gdd_task_num++;

		sscanf(read.c_str(), "%d %d %d %d %d %d %d", &task_id, &prev_node, &next_node, &exec_time, &period, &phase, &iteration);
		ACE_DEBUG ((LM_DEBUG, "[parse_configure_file] task_id %d, prev_node %d, next_node %d, exec_time %d, period %d, phase %d, iteration %d\n", 
			task_id, prev_node, next_node, exec_time, period, phase, iteration));
		gdd_task[gdd_task_num].task_id = task_id;

		gdd_task[gdd_task_num].prev_node = prev_node;
		gdd_task[gdd_task_num].prev_bdone = 1;
		rtn = 0;
		gdd_task[gdd_task_num].in_file[rtn] = 0;
		gdd_task[gdd_task_num].in_file_name[rtn] = 0;
		if(prev_node > 0) {
			rtn = sprintf(gdd_task[gdd_task_num].in_file, "file:///home/ron/yfzhang/iors/task_%d_%d.ior", task_id, prev_node);
			gdd_task[gdd_task_num].in_file[rtn] = 0;
			rtn = sprintf(gdd_task[gdd_task_num].in_file_name, "/home/ron/yfzhang/iors/task_%d_%d.ior", task_id, prev_node);
			gdd_task[gdd_task_num].in_file_name[rtn] = 0;
			gdd_task[gdd_task_num].prev_bdone = 0;
		}

		gdd_task[gdd_task_num].next_node = next_node;
		gdd_task[gdd_task_num].next_bdone = 1;
		rtn = 0;
		if(next_node > 0) {
			rtn = sprintf(gdd_task[gdd_task_num].out_file, "/home/ron/yfzhang/iors/task_%d_%d.ior", 
				task_id, gnode_id);
			gdd_task[gdd_task_num].next_bdone = 0;
		}
		gdd_task[gdd_task_num].out_file[rtn] = 0;
		if(rtn != 0) ACE_DEBUG ((LM_DEBUG, "output file name %s\n", gdd_task[gdd_task_num].out_file));
		  
		gdd_task[gdd_task_num].exec_time = exec_time;
		gdd_task[gdd_task_num].period = period;
		gdd_task[gdd_task_num].phase = phase;
		gdd_task[gdd_task_num].iteration = iteration;
	}
	
	fs.close();
}

int do_shutdown = 0;
int enable_dynamic_scheduling = 1;
int enable_yield = 1;
int enable_rand = 0;
int niteration = 3;
int workload = 50;
int period = 200;
int left_work = 50;
int count = 0;
int nthreads = 1;
int importance=0;

int ID_BEGIN = 1;

class Worker : public ACE_Task_Base
{
public:
      	Worker (CORBA::ORB_ptr orb,
	    	DD_Servant_ptr servant_ptr,
		RTScheduling::Current_ptr current,
		EDF_Scheduler* scheduler,
		long importance,
		CORBA::Long server_load,
		CORBA::Long period,
		CORBA::Long niteration,
		int sleep_time);

	virtual int svc (void);
	// The thread entry point.

	void sync_clock();

private:
	CORBA::ORB_var orb_;
	// The orb

	DD_Servant_ptr servant_ptr_;
	RTScheduling::Current_var scheduler_current_;
	EDF_Scheduler* scheduler_;
	long importance_;
	CORBA::Long server_load_;
	CORBA::Long period_;
	CORBA::Long niteration_;
	int sleep_time_;	/* in msec */
	unsigned int local_;
};

class Worker_s : public ACE_Task_Base
{
public:
      	Worker_s (CORBA::ORB_ptr orb);

	virtual int svc (void);

private:
	CORBA::ORB_var orb_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "i:sn:w:p:r");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'i':
        // ior_input_file1 = get_opts.opt_arg ();
        break;

      case 's':
        enable_yield = 0;
        break;

      case 'n':
        niteration = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'p':
        period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'r':
        enable_rand = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <ior> "
                           "-s (disable yield)"
			   "-n <niterations>"
                           "-w <workload>"
                           "-p <period>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

class DD_Node {
public:
	DD_Node();
	~DD_Node();
	int does_file_exist(const char *file);
	int simulate();
	int init_node(int argc, char *argv[]);
	int shutdown();

private:	
	int max_prio_;
	long flags_;
	EDF_Scheduler *scheduler_;
	RTScheduling::Current_var sch_current_;
	CORBA::ORB_var orb_;
	PortableServer::POAManager_var poa_manager_;
	Task_Stats task_stats_;
	ACE_High_Res_Timer non_dsui_timer_;
	ACE_Sched_Params::Policy  sched_policy_;
	int sched_scope_;
	ds_control *pds_cntrl_;
	vector<DD_Servant_i *> vpservant_i_;
	vector<Worker_s *> vpworker_s_;
	vector<Worker *> vpworker_;
};

DD_Node::DD_Node()
{
	pds_cntrl_ = NULL;
        vpservant_i_.clear();
        vpworker_s_.clear();
        vpworker_.clear();
}

DD_Node::~DD_Node()
{
	for(int i = 0; i < (int)vpservant_i_.size(); i++)
		delete vpservant_i_[i];
	for(int i = 0; i < (int)vpworker_s_.size(); i++)
		delete vpworker_s_[i];
	for(int i = 0; i < (int)vpworker_.size(); i++)
		delete vpworker_[i];
	if(pds_cntrl_)
		delete pds_cntrl_;
}

int DD_Node::does_file_exist(const char *file)
{
	assert(file);

	if(strlen(file) == 0) return 0;

	struct stat buf;
	int rtn;
    	rtn = stat(file, &buf);
	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [CHECK FILE] file %s, rtn %d, size %d\n", file, rtn, buf.st_size));
	if(rtn < 0) return 0;

	return 1;
}

/* three main blocks: setup connections, run tasks and wait */
int DD_Node::simulate()
{
	ACE_DEBUG ((LM_DEBUG, "[simulate] begin\n"));

	DD_Servant_var servant[16];

	/* setup links */
	int bdone = 0;
	while(1) {
		bdone = 1;
		for(int i = 1; i <= gdd_task_num; i++) {
			if(!gdd_task[i].prev_bdone) {
				bdone = 0;
				if(does_file_exist(gdd_task[i].in_file_name)) {
					/* previous ior file exists, read */
				  	CORBA::Object_var object =
						orb_->string_to_object(gdd_task[i].in_file ACE_ENV_ARG_PARAMETER);
					ACE_TRY_CHECK;

					servant[i] =
						DD_Servant::_narrow(object.in() ACE_ENV_ARG_PARAMETER);
					ACE_TRY_CHECK;

					assert(!CORBA::is_nil(servant[i].in()));
					gdd_task[i].prev_bdone = 1; 
					
					ACE_DEBUG ((LM_DEBUG, "[simulate] read in_file done!\n"));
				}
			}

			if(!gdd_task[i].prev_bdone) continue;

			if(!gdd_task[i].next_bdone) {
				bdone = 0;
				{ /* create out_file */
					DD_Servant_var local_servant;
					DD_Servant_i *pservant_impl = NULL;

					/*
					switch(i) {
						// case 1:
						default:
							// DD_Servant_i servant_impl_1(orb_.in(), sch_current_.in (), servant[i].in(), task_stats_, enable_yield);
							// local_servant = servant_impl_1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
							pservant_impl = new DD_Servant_i(orb_.in(), sch_current_.in (), servant[i].in(), task_stats_, enable_yield);
							local_servant = pservant_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
							ACE_TRY_CHECK;
							break;	
					}
					*/

					DD_Servant_ptr _servant_ptr = NULL;
					if(gdd_task[i].prev_node > 0) {
						/* end of a task chain */
						_servant_ptr = servant[i].in();
						assert(!CORBA::is_nil(_servant_ptr));
					}

					pservant_impl = new DD_Servant_i(orb_.in(), sch_current_.in (), _servant_ptr, 
						task_stats_, enable_yield, gdd_task[i].exec_time);
					assert(pservant_impl);
					vpservant_i_.push_back(pservant_impl);
					local_servant = pservant_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
					ACE_TRY_CHECK;

					CORBA::String_var ior =
						orb_->object_to_string(local_servant.in() ACE_ENV_ARG_PARAMETER);
					ACE_TRY_CHECK;

					ACE_DEBUG((LM_DEBUG, "Activated as <%s>\n", ior.in()));

					// If the ior_output_file exists, output the ior to it
					assert(strlen(gdd_task[i].out_file));
					FILE *output_file= ACE_OS::fopen(gdd_task[i].out_file, "w");
					assert(output_file);
					ACE_OS::fprintf(output_file, "%s", ior.in());
					ACE_OS::fclose(output_file);

					ACE_DEBUG ((LM_DEBUG, "[simulate] write out_file done\n"));
				}

				gdd_task[i].next_bdone = 1;
			} // if(!gdd_task[i].next_bdone)
		}
		if(!bdone) sleep(5);
		else break;
	}

	poa_manager_->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
	ACE_TRY_CHECK;

	ACE_DEBUG ((LM_DEBUG, "[simulate] about to run task\n"));
	/* run task */
	Worker *pworker[16]; Worker_s *pworker_s[16];
	for(int i = 1; i <= gdd_task_num; i++) {
		if(gdd_task[i].next_node <= 0) {
			/* independent node, not in a task chain */
			DD_Servant_ptr servant_ptr = NULL;
			if(gdd_task[i].prev_node > 0) {
				/* end of a task chain */
				servant_ptr = servant[i].in();
				assert(!CORBA::is_nil(servant_ptr));
			}
			/* start Worker */
			ACE_DEBUG ((LM_DEBUG, "[simulate] start worker\n"));
		  	Worker *pworker_one = new Worker(orb_.in(),
				servant_ptr, /* servant[i].in(), */
				sch_current_.in(),
				scheduler_,
				importance,
				gdd_task[i].exec_time, /* workload2, */
				gdd_task[i].period, /* period2, */
				gdd_task[i].iteration, /* niteration, */
				gdd_task[i].phase /* sleep_time */);
			ACE_DEBUG ((LM_DEBUG, "[simulate] about to activate\n"));
			if(pworker_one->activate (flags_, 1, 0, max_prio_) != 0)
			{
				ACE_ERROR ((LM_ERROR, "(%t|%T) cannot activate worker thread.\n"));
			}
			ACE_DEBUG ((LM_DEBUG, "[simulate] activate done\n"));
			pworker[i] = pworker_one;
			vpworker_.push_back(pworker_one);
		}
		else {
			/* start Worker_s */
			ACE_DEBUG ((LM_DEBUG, "[simulate] start worker_s\n"));
		   	Worker_s *pworker_s_one = new Worker_s(orb_.in());
			if(pworker_s_one->activate (flags_,
						nthreads,
						0,
						ACE_Sched_Params::priority_max(sched_policy_, sched_scope_)) != 0)
			{
				ACE_ERROR ((LM_ERROR,
					"Cannot activate threads in RT class.",
					"Trying to activate in non-RT class\n"));

				/* local flags */
				long flags = THR_NEW_LWP | THR_JOINABLE | THR_BOUND;
				if (pworker_s_one->activate (flags, nthreads) != 0)
				{
					ACE_ERROR_RETURN ((LM_ERROR,
						"Cannot activate server threads\n"),
						-1);
				}
			}
			pworker_s[i] = pworker_s_one;
			vpworker_s_.push_back(pworker_s_one);
		}
	}

	ACE_DEBUG ((LM_DEBUG, "[simulate] about to wait\n"));
	// sleep(0);

	/* wait */
	for(int i = 1; i <= gdd_task_num; i++) {
		if(gdd_task[i].next_node <= 0) {
			pworker[i]->wait(); 
		}
		else if(gdd_task[i].prev_node <= 0 && gdd_task[i].next_node <= 0) {
			/* start Worker_c */
		}
		else {
			pworker_s[i]->wait(); 
		}
	}

	ACE_DEBUG ((LM_DEBUG, "[simulate] exit\n"));
	return 1;
}


int DD_Node::init_node(int argc, char *argv[])
{
	ACE_DEBUG ((LM_DEBUG, "[init_node] begin\n"));

	ID_BEGIN = gnode_id*100;

        char ds_file[80];
 	int ret = sprintf(ds_file, "DT_%s", machine[gnode_id]);
        ds_file[ret]=0;
	// ds_control ds_cntrl ("DT_Oneway_Client", "chain_enable.dsui");
	pds_cntrl_ = new ds_control(ds_file, "chain_enable.dsui");
	non_dsui_timer_.calibrate ();
	non_dsui_timer_.start();

	/* MEASURE: Program start time */
	DSTRM_EVENT(MAIN_GROUP_FAM, START,0,0,NULL);

	scheduler_ = NULL;

	int prio;
	sched_policy_ = ACE_SCHED_RR;
	sched_scope_ = ACE_SCOPE_THREAD;

	task_stats_.init (100000);

	if (sched_policy_ == ACE_SCHED_RR) {
		/* MEASURE (DP): Schedule policy */
		flags_ = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_RR;
	}
	else {
		/* MEASURE (DP): Schedule policy */
		flags_ = THR_NEW_LWP | THR_BOUND | THR_JOINABLE | THR_SCHED_FIFO;
	}

	ACE_hthread_t main_thr_handle;
	ACE_Thread::self (main_thr_handle);

	max_prio_ = ACE_Sched_Params::priority_max (sched_policy_, sched_scope_);

	ACE_Sched_Params sched_params (sched_policy_, max_prio_);

	ACE_OS::sched_params (sched_params);

	if (ACE_Thread::getprio (main_thr_handle, prio) == -1)
	{
		if (errno == ENOTSUP)
		{
			ACE_ERROR((LM_ERROR, ACE_TEXT ("getprio not supported\n")));
		}
		else
		{
			ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n")	ACE_TEXT ("thr_getprio failed")));
		}
	}

	ACE_DEBUG ((LM_DEBUG, "main thread prio is %d\n", prio));

	// TAO_debug_level = 1;
	CPULoad::calibrate(5);

	ACE_TRY_NEW_ENV
	{
		orb_ = CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		CORBA::Object_var poa_object =
			orb_->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		if (CORBA::is_nil (poa_object.in ()))
				ACE_ERROR_RETURN ((LM_ERROR,
				" (%P|%t) Unable to initialize the POA.\n"),
				-1);
	
#if 0
		{
			/* TRY MY LUCK */
			CORBA::Object_var _object = orb_->resolve_initial_references("ORBPolicyManager");
			CORBA::PolicyManager_var _policy_manager =
				CORBA::PolicyManager::_narrow (_object.in ());
			_object = orb_->resolve_initial_references("PolicyCurrent");
			CORBA::PolicyCurrent_var _policy_current =
				CORBA::PolicyCurrent::_narrow (_object.in ());


			CORBA::PolicyList _policy_list;
			_policy_list.length (1);

			CORBA::Any _orb_level;
			// _orb_level <<= Messaging::SYNC_WITH_SERVER;
			_orb_level <<= Messaging::SYNC_NONE;
			// _policy_list[0] = orb_->create_policy(Messaging::SYNC_SCOPE_POLICY_TYPE, _orb_level);
			_policy_list[0] = orb_->create_policy(Messaging::SYNC_SCOPE_POLICY_TYPE, _orb_level);
			_policy_manager->set_policy_overrides(_policy_list, CORBA::SET_OVERRIDE);
			/* call func */
			_policy_list[0]->destroy ();
			/* end of TRY MY LUCK */
		}
#endif

		PortableServer::POA_var root_poa =
			PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;

		poa_manager_ = root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
		ACE_TRY_CHECK;

		if (parse_args (argc, argv) != 0)
			return -1;

		if (enable_dynamic_scheduling)
		{
			ACE_DEBUG ((LM_DEBUG, "Dyn Sched enabled\n"));
			CORBA::Object_ptr manager_obj =
				orb_->resolve_initial_references ("RTSchedulerManager"
								ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;

			TAO_RTScheduler_Manager_var manager =
				TAO_RTScheduler_Manager::_narrow (manager_obj
								ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;

			Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type;
			if (enable_yield)
			{
				disp_impl_type = Kokyu::DSRT_CV_BASED;
			}
			else
			{
				disp_impl_type = Kokyu::DSRT_OS_BASED;
			}

			ACE_DEBUG ((LM_DEBUG, "create scheduler\n"));
			ACE_NEW_RETURN (scheduler_,
					EDF_Scheduler (orb_.in (),
					disp_impl_type,
					sched_policy_,
					sched_scope_), -1);

			/* MEASURE: Scheduler start time */
			DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_STARTED, 0, 0, NULL);

			manager->rtscheduler (scheduler_);

			CORBA::Object_var object =
				orb_->resolve_initial_references ("RTScheduler_Current"
								ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;

			sch_current_  =
				RTScheduling::Current::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;

			ACE_DEBUG ((LM_DEBUG, "about to end enable_dynamic_scheduling\n"));
		}
		ACE_DEBUG ((LM_DEBUG, "about to end ACE_TRY\n"));
	}
	ACE_CATCHANY
	{
		ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,	"Exception caught:");
		return -1;
	}
	ACE_ENDTRY;

	ACE_DEBUG ((LM_DEBUG, "[init_node] exit\n"));
	return 1;
}

int DD_Node::shutdown()
{
ACE_TRY_NEW_ENV
{
	if(do_shutdown)
        {
		if (enable_dynamic_scheduling)
		{
			EDF_Scheduling::SchedulingParameter sched_param;
			sched_param.importance = 0;
			ACE_Time_Value deadline_tv = ACE_OS::gettimeofday () + ACE_Time_Value (10,0);
			sched_param.deadline = deadline_tv.sec ()*10000000 + deadline_tv.usec ()*10;
			sched_param.task_id = ID_BEGIN++;
			sched_param.period = 0;
			CORBA::Policy_var sched_param_policy =
				scheduler_->create_scheduling_parameter (sched_param);
			CORBA::Policy_ptr implicit_sched_param = 0;
			sch_current_->begin_scheduling_segment (0,
							sched_param_policy.in (),
							implicit_sched_param
							ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;
		}

		ACE_DEBUG ((LM_DEBUG, "(%t|%T) about to call server shutdown\n"));

		/* MEASURE: Call to shutdown server */
		// char* msg = "(%t): wait for worker threads done in main thread\n";
		// Get thread id
		DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 0, 0, NULL);

		/* NOT QUITE RIGHT YET */
		// server->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
		// ACE_TRY_CHECK;

		/* MEASURE: After call to server shutdown */
		DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 0, 0, NULL);
		ACE_DEBUG ((LM_DEBUG, "after shutdown call in main thread\n"));


		if (enable_dynamic_scheduling)
		{
			sch_current_->end_scheduling_segment(0 ACE_ENV_ARG_PARAMETER);
			ACE_TRY_CHECK;
		}
        }

	/* NOT QUITE RIGHT YET */
	// worker.wait ();

	scheduler_->shutdown ();
	/* MEASURE: Scheduler stop time */
	DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 0, 0, NULL);
	ACE_DEBUG ((LM_DEBUG, "scheduler shutdown done\n"));
}
ACE_CATCHANY
{
	ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught:");
	return -1;
}
ACE_ENDTRY;

	/* MEASURE: Program stop time */
	DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 0, 0, NULL);

	non_dsui_timer_.stop();
	ACE_hrtime_t dsui_ovhd_time;
	non_dsui_timer_.elapsed_time (dsui_ovhd_time);

	ACE_DEBUG ((LM_NOTICE, "(%t|%T) exiting...\n"));
	ACE_OS::printf ( ACE_TEXT ("Elapsed time: %lu (nsec)\n"), dsui_ovhd_time);

	return 1;
}

int
main (int argc, char *argv[])
{
	ACE_LOG_MSG->priority_mask (LM_ERROR | LM_CRITICAL | LM_ALERT | LM_EMERGENCY | LM_NOTICE, ACE_Log_Msg::PROCESS);
	DD_Node dd_node;

	parse_configure_file("dd_node.cfg");
	dd_node.init_node(argc, argv);
	dd_node.simulate();
	dd_node.shutdown();

	return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb,
                DD_Servant_ptr servant_ptr,
                RTScheduling::Current_ptr current,
                EDF_Scheduler* scheduler,
                long importance,
                CORBA::Long server_load,
                CORBA::Long period,
                CORBA::Long niteration,
		int sleep_time)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     servant_ptr_ (servant_ptr),
     /* servant_ (DD_Servant::_duplicate (servant_ptr)), */
     scheduler_current_ (RTScheduling::Current::_duplicate (current)),
     scheduler_ (scheduler),
     importance_ (importance),
     server_load_ (server_load),
     period_(period),
     niteration_(niteration),
     sleep_time_ (sleep_time)
{
	
}

int
Worker::svc (void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] begin\n"));

	sync_clock();
#if 0
	/* initialize policy defaults */
	CORBA::Object_var _object = orb_->resolve_initial_references("ORBPolicyManager");
	CORBA::PolicyManager_var _policy_manager =
		CORBA::PolicyManager::_narrow (_object.in ());
	_object = orb_->resolve_initial_references("PolicyCurrent");
	CORBA::PolicyCurrent_var _policy_current =
		CORBA::PolicyCurrent::_narrow (_object.in ());

	CORBA::PolicyList _policy_list;
	// Disable all default policies.
	_policy_list.length (0);
	_policy_manager->set_policy_overrides(_policy_list, CORBA::SET_OVERRIDE);
	_policy_current->set_policy_overrides(_policy_list, CORBA::SET_OVERRIDE);
	/* end of initialize policy defaults */
#endif
		
  /* MEASURE: Worker start time */
  Object_ID oid = ACE_OBJECT_COUNTER->increment();
  	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] increment done\n"));
  oid.task_id = ID_BEGIN++;
  DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, sizeof(Object_ID), (char*)&oid);
  	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] DSTRM done\n"));

  // Yfzhang: DEBUG ATTEMPT
  // comment out 
  ACE_DECLARE_NEW_CORBA_ENV;
  // enable_dynamic_scheduling = 0; 
  // end of DEBUG ATTEMPT

  const char * name = 0;
  
  ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] about to sleep for %d msec\n", sleep_time_));
  usleep(sleep_time_*1000);
  ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] woke up from sleep for %d msec\n", sleep_time_));

  	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] about to self\n"));

  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

	ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] about to getprio\n"));

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      if (errno == ENOTSUP)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("getprio not supported\n")
                     ));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n")
                      ACE_TEXT ("thr_getprio failed")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] worker activated with prio %d AND iteration is %d\n", prio,niteration_));

  EDF_Scheduling::SchedulingParameter sched_param;
  CORBA::Policy_var sched_param_policy;
  CORBA::Policy_var implicit_sched_param;
  double rand2=0.0;

  int sec_period = period_/1000;
  int usec_period = (period_%1000)*1000;

  if (enable_dynamic_scheduling)
    {
      sched_param.importance = importance_;
      ORBSVCS_Time::Time_Value_to_TimeT (sched_param.deadline,
                                         ACE_OS::gettimeofday () +
                                         ACE_Time_Value (sec_period, usec_period));

      sched_param.period = period_*10000;
      sched_param.task_id = oid.task_id;
      sched_param.id = oid.id;
      sched_param.pid = oid.pid;
      sched_param.tid = oid.tid;
      sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);

      //If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
      implicit_sched_param = sched_param_policy;

      /* MEASURE: Start of scheduling segment */
      oid.task_id = sched_param.task_id;
      DSTRM_EVENT (WORKER_GROUP_FAM, BEGIN_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] before begin_sched_segment\n"));

      scheduler_current_->begin_scheduling_segment (name,
                                                    sched_param_policy.in (),
                                                    implicit_sched_param.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      /* MEASURE: End of scheduling segment */
      DSTRM_EVENT (WORKER_GROUP_FAM, END_SCHED_SEGMENT, 0, sizeof(Object_ID), (char*)&oid);
      ACE_DEBUG ((LM_DEBUG, "(%t|%T) [Worker::svc] after begin_sched_segment\n"));
    }

	ACE_Time_Value start_t, repair_t;
	repair_t=ACE_Time_Value(0,0);

	for(int i=0;i<niteration_;i++)
	{
		ACE_DEBUG((LM_NOTICE, "(%t|%T) [Worker::svc] The Current iter is %d and total is %d\n", i, niteration_));

		if(i>0 && enable_dynamic_scheduling)
		{
			sched_param.deadline = sched_param.deadline+period_*10000;
			sched_param_policy = scheduler_->create_scheduling_parameter (sched_param);
			oid = ACE_OBJECT_COUNTER->increment();
			oid.task_id = sched_param.task_id;
			sched_param.id = oid.id;
			sched_param.tid = oid.tid;
			sched_param.pid = oid.pid;

			//If we do not define implicit_sched_param, the new spawned DT will have the default lowest prio.
			implicit_sched_param = sched_param_policy;
			DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_BEGIN, 0, sizeof(Object_ID), (char*)&oid);
			scheduler_current_->update_scheduling_segment(name,
                                                        sched_param_policy.in (),
                                                        implicit_sched_param.in ()
                                                        ACE_ENV_ARG_PARAMETER);
			ACE_CHECK_RETURN (-1);
			DSTRM_EVENT (WORKER_GROUP_FAM, UPDATE_SCHED_SEGMENT_END, 0, sizeof(Object_ID), (char*)&oid);
		}

		if (i==0)
			start_t =  ACE_OS::gettimeofday ();
		else {
			repair_t = start_t+ACE_Time_Value(sec_period*i,usec_period*i)-ACE_OS::gettimeofday ();
		}

		timeval tv;

		ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] Task id is %d and thread id is %d\n", oid.task_id, oid.tid));
		DSTRM_EVENT (TEST_ONE_FAM, START_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

		tv.tv_sec = server_load_/1000;
		tv.tv_usec = (server_load_%1000)*1000;

		ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] before running the client workload and task_id is %d\n",oid.task_id));
		CPULoad::run(tv);
		ACE_DEBUG((LM_NOTICE,"(%t|%T) [Worker::svc] after running the client workload and task_id is %d\n",oid.task_id));

		DSTRM_EVENT (TEST_ONE_FAM, STOP_SERVICE, 0, sizeof(Object_ID), (char*)&oid);

		if(!CORBA::is_nil(servant_ptr_)) {
			DD_Servant_var servant = DD_Servant::_duplicate(servant_ptr_);
			DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_START, 0, sizeof(Object_ID), (char*)&oid);
		
#if 0	
			{
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] before change policy\n"));
			/* TRY MY LUCK */

			_policy_list.length (2);

			TimeBase::TimeT _relative_expiry = 1.0e-3 * 1.0e8;	/* 10 msec */
		        CORBA::Any _relative_request_timeout_value;
			_relative_request_timeout_value <<= _relative_expiry;
			_policy_list[0] = orb_->create_policy(Messaging::RELATIVE_REQ_TIMEOUT_POLICY_TYPE,
				_relative_request_timeout_value);
			_policy_list[1] = orb_->create_policy(Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
				_relative_request_timeout_value);
			// _policy_manager->set_policy_overrides(_policy_list, CORBA::SET_OVERRIDE);
			_policy_current->set_policy_overrides(_policy_list, CORBA::ADD_OVERRIDE);

			/* call func */
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] before exec_method\n"));
			servant->exec_method (left_work ACE_ENV_ARG_PARAMETER);
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] after exec_method\n"));

			_policy_list[0]->destroy ();

			/* end of TRY MY LUCK */
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] after change policy and exec_method\n"));
			}
#endif

#if 1
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] before exec_method\n"));
			servant->exec_method (left_work ACE_ENV_ARG_PARAMETER);
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] after exec_method\n"));
#endif

			ACE_CHECK_RETURN (-1);
      
			DSTRM_EVENT (WORKER_GROUP_FAM, ONE_WAY_CALL_DONE, 0, sizeof(Object_ID), (char*)&oid);
		}

		if (enable_dynamic_scheduling) {
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] before update_schedule\n"));
			scheduler_->kokyu_dispatcher_->update_schedule (*(scheduler_current_->id ()), Kokyu::BLOCK);
			ACE_DEBUG((LM_DEBUG,"(%t|%T) [Worker::svc] after update_schedule\n"));
		}

		rand2 = 0.1*rand()/RAND_MAX;
		if(enable_rand)
		{
			int sleep_t = period_ * 1000
				- (int)(period_ * rand2 * 1000)
				+ repair_t.sec()*1000000 + repair_t.usec();

			if(sleep_t > 0)
			{
				ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] NOW I AM GOING TO SLEEP FOR %d.\n",
					(int)(period_*1000-period_*rand2*1000)));
				usleep(sleep_t);
			}
			else {
				ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] sleep_t %d is non-positive, do not sleep.\n", sleep_t));
				// ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] NOW I AM GOING TO SLEEP FOR %d\n", 0));
			}
		}
		else {
			ACE_Time_Value current = ACE_OS::gettimeofday ();
			int sleep_t = sched_param.deadline/10-current.sec()*1000000-current.usec();
			if(sleep_t > 0) {
				ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] NOW I AM GOING TO SLEEP FOR %d\n", sleep_t));
				usleep(sleep_t);
			}
			else {
				ACE_DEBUG((LM_DEBUG, "(%t|%T) [Worker::svc] sleep_t %d is non-positive, do not sleep.\n", sleep_t));
			}
		}
	} // for(int i=0;i<niteration_;i++)

	if (enable_dynamic_scheduling)
	{
		scheduler_current_->end_scheduling_segment (name);
		ACE_CHECK_RETURN (-1);
	}

	ACE_DEBUG ((LM_NOTICE, "(%t|%T) [Worker::svc] Worker thread exiting...\n"));

	return 0;
}

void Worker::sync_clock()
{
	struct timeval cur_tv;
	long sleep_t;	/* usec */
	gettimeofday(&cur_tv, NULL);

	int sec = SYNC_TIME - cur_tv.tv_sec % SYNC_TIME;
	if(sec > 0) sleep_t = sec*1000000 - cur_tv.tv_usec;
	else sleep_t = 0;
	assert(sleep_t >= 0);

  	ACE_DEBUG ((LM_NOTICE, "(%t|%T) [Worker::sync_clock] sleep (%d sec, %d usec) to sync...\n",
		sleep_t/1000000, sleep_t%1000000));
	usleep(sleep_t);
  	ACE_DEBUG ((LM_NOTICE, "(%t|%T) [Worker::sync_clock] sleep to sync done!\n"));
}

Worker_s::Worker_s (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker_s::svc (void)
{
	ACE_DECLARE_NEW_CORBA_ENV;
	ACE_Time_Value tv(TOTAL_EXEC_TIME);

	ACE_TRY
	{
		this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
		ACE_TRY_CHECK;
	}
	ACE_CATCHANY
	{
	}
	ACE_ENDTRY;
	ACE_DEBUG ((LM_NOTICE, "(%t|%T) [Worker::svc] Worker_s thread exiting...\n"));
	return 0;
}
