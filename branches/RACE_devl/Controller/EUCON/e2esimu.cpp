/*---------------------------------------------------------------
  Main file to test EUCON controller
  ---------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

#ifdef     WIN32
#pragma warning(disable: 4786)
#endif

#include "MPC_Controller.h"

int main(int argc, char *argv[])
{
	MPC_Controller m_con;
	CIAO::RACE::Domain dom;
	CIAO::RACE::Node n1,n2;
	CIAO::RACE::Resource cpu1,cpu2;
	cpu1.label = "cpu1";
	cpu1.set_point = 0.828427;
	cpu1.util = 0.91;

	cpu2.label = "cpu2";
	cpu2.set_point = 0.828427;
	cpu2.util = 0.836;

	n1.label = "p1";
	n1.resources.push_back(cpu1);
	n2.label = "p2";
	n2.resources.push_back(cpu2);
	
	dom.nodes.push_back(n1);
	dom.nodes.push_back(n2);

	m_con.init_domain(dom);

	
	CIAO::RACE::time_val t1,t2;
	t1.sec = 0;
	t1.usec = 35*1000;
	t2.sec = 0;
	t2.usec = 45*1000;
	
	CIAO::RACE::Execution_Time exec1,exec2;
	exec1.BCET = t1;
	exec1.WCET = t1;
	exec2.BCET = t2;
	exec2.WCET = t2;
	
	CIAO::RACE::Subtask s11,s21,s22,s31;
	s11.exec_time = exec1;
	s11.node = 0;

	s21.exec_time = exec1;
	s21.node = 0;

	s22.exec_time = exec1;
	s22.node = 1;

	s31.exec_time = exec2;
	s31.node = 1;

	CIAO::RACE::Task T1,T2,T3;
	T1.max_rate = (double)1000/35;
	T1.min_rate = (double)1000/700;
	T1.curr_rate = 15.4285;//(double)1000/60;
	T1.subtasks.push_back(s11);

	T2.max_rate = (double)1000/35;
	T2.min_rate = (double)1000/700;
	T2.curr_rate = 10.2772;//(double)1000/90;
	T2.subtasks.push_back(s21);
	T2.subtasks.push_back(s22);

	T3.max_rate = (double)1000/45;
	T3.min_rate = (double)1000/900;
	T3.curr_rate = 10.5198;//(double)1000/100;
	T3.subtasks.push_back(s31);

	std::vector<CIAO::RACE::Task> tasks;
	tasks.push_back(T1);
	tasks.push_back(T2);
	tasks.push_back(T3);

	// Initialize the controller.
	m_con.init(tasks);	
	//for (size_t j = 0; j < 5; ++j)
//	{
		tasks = m_con.control_period(dom,tasks);
		std::cout << "Rate change:"<< std::endl;
        for(size_t i = 0; i < tasks.size(); i ++)
		{
			std::cout << tasks[i].curr_rate << " " << tasks[i].delta_rate << std::endl;
		//	tasks [i].curr_rate = tasks [i].curr_rate + tasks[i].delta_rate;
		}
//	}
	return 0;

}
