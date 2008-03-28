 /*---------------------------------------------------------------
 EUCON Controller for RACE
 ---------------------------------------------------------------*/

#ifndef __MPC_CONTROLLER__
#define __MPC_CONTROLLER__

#include <string>

#include "liblsq.h"
#include "Domain.h"
#include "Task.h"
#include "Subtask.h"
#include "constant.h"

//using namespace CIAO::RACE;
//using namespace std;

class MPC_Controller{

private:
	//number of tasks in the workload
	int m_task_num;
	//number of processors in the workload
	int m_proc_num;
	//names of all processors in the workload
	std::vector<std::string> proc_names;
	//Desired utilization set-points of all processors(max 1).
	std::vector<double> set_points;


	//Flag to indicate whether the compiler-generated library is initialized or not
	int lib_flag;

	int m_subtask_num;

	//data members used by liblsq solver
	double* m_rate_chgs;
	double* rccPtr;
	double* m_Xptr, *m_Uptr, *m_Rptr, *m_Fptr;
	double* Hptr;
	double* Yptr;
	double* utilBptr;

	mwArray m_X;
	mwArray m_U;
	mwArray m_R;
	mwArray m_F;
	mwArray m_R_Chgs;
	mwArray Y;
	mwArray H0,H,HW;
	mwArray A;
	mwArray B;
	mwArray BB;
	mwArray E,E0,E1;
        mwArray utilB;
        mwArray consB, consB_temp;
        mwArray consLeft, consRight;

public:
	MPC_Controller();
	~MPC_Controller();

	//initialize the domain(e.g., processor names) information for the current workload
	int init_domain(CIAO::RACE::Domain dom);
	//initialize the matlab environment and the data information for lsqlin solver
	int init(std::vector<CIAO::RACE::Task> tasks);
	//control action (invoked periodically)
	std::vector<CIAO::RACE::Task> control_period(CIAO::RACE::Domain dom, std::vector<CIAO::RACE::Task> tasks);

	void rowmajor_to_colmajor(double* matrix, int row, int col);
	//in1 = in1 - in2;
	void array_minus(double* in1,double* in2, int len);
	void array_plus(double* in1, double* in2, int len);
	void new_matrix_multiple(double* out,double* in1,double* in2, int r1,int c1,int r2,int c2);
	mwArray matrix_multiple(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2);
	mwArray matrix_plus(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2);
	mwArray matrix_minus(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2);
	mwArray matrix_row_join(mwArray in1,mwArray in2,int r1,int c1,int r2,int c2);
};

#endif
