// $Id$

/*
 * University of Kansas Center for Research, Inc.  This software was
 * developed by the Information and Telecommunication Technology
 * Center (ITTC) at the University of Kansas.  This software may be
 * used and distributed according to the terms of the GNU Public
 * License, incorporated herein by reference. ITTC does accept any
 * liability whatsoever for this product.
 *
 * This project was developed under the direction of Dr. Douglas Niehaus.
 *
 */

#define CONFIG_DSTREAM_SCENARIOA
#include <dsui.h>
#include "scenarioa_dsui_table.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <signal.h>
#include <pthread.h>

extern "C" {
#include "include/middleware/group_sched_shared.h"
}
#include "SchedGroup.h"


void my_sleep(int secs){
    char block_state;

    block_state = get_process_block_state(getpid());
    controlled_task_may_block();
    /* This task is going to sleep */
    sleep(secs);
    DSTRM_EVENT(SCENARIOA_FAM, AFTER_UNBLOCK, getpid(), 0, NULL);
    controlled_task_unblocked();
}


void child_process(SchedGroup& grp, const char* process_name)
{
	DSTRM_EVENT(SCENARIOA_FAM, JOINING_GROUP, getpid(), 0, NULL);
	Seq_QoS qos;
	SchedThread thr_member(&grp, qos, process_name);

	if(!thr_member.is_valid()){
    printf("Error! in join_grp\n");
    return;
	}
	

	for (int k = 0; k < 3; ++k) {
	  DSTRM_EVENT(SCENARIOA_FAM,IN_WHILE_LOOP, getpid(), 0, NULL);
    int j;
	  for(int i=0; i<100000; i++)
	    j++;
	
    DSTRM_EVENT(SCENARIOA_FAM, CALLING_SLEEP, getpid(), 0, NULL);
	 	my_sleep(2);
	  DSTRM_EVENT(SCENARIOA_FAM, AFTER_SLEEP, getpid(), 0, NULL);
	}
	
  DSTRM_EVENT(SCENARIOA_FAM, EXITING, getpid(), 0, NULL);
  return;   		
}

int main(int argc, char **argv){
	int pid=0, num_processes=0;

	if(argc!=3){
		printf("usage\n");
		printf("scenarioa <name of process> <number of processes>\n");
		exit(0);
	}

	num_processes=atoi(argv[2]);
	
	Seq_QoS qos;
	
	SchedGroup top_group("SS", qos);
		
	if (top_group.set_as_top() == -1) {
		fprintf (stderr, "Error in setting top group\n");
		exit(1);
	}


	DSUI_INIT("scenarioa", "scenarioa_enable.dsui");

	for(int i=0;i<num_processes;i++) {
	    char    process_name[100];

	    DSTRM_EVENT(SCENARIOA_FAM, CREATING_THREAD, getpid(), 0, NULL);
	    pid=fork();
	    sprintf(process_name,"%s%d", "THREAD",i);

	    if(pid==0){
	      child_process(top_group, process_name);
        exit(0);
      }
	}

	/* Make the main thread sleep for 10 secs */
	sleep(10);
  DSTRM_EVENT(SCENARIOA_FAM, EXITING, getpid(), 0, NULL);

	return 0;
}
