/*---------------------------------------------------------------
  End-to-End MPC control theory based scheduling simulator
  Author: Xiaorui Wang <wang@cse.wustl.edu>
  Note: need shared library(liblsq) created by Matlab R14 for running, Nov, 2005
  ---------------------------------------------------------------*/

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#include "MPC_Controller.h"

#ifdef     WIN32
#pragma warning(disable: 4786)
#endif


MPC_Controller::MPC_Controller()
  : m_task_num(0), m_subtask_num(0), m_proc_num(0), lib_flag (0)
{
  // Call application and library initialization. Perform this
  // initialization before calling any API functions or
  // Compiler-generated libraries by Matlab.
  if (!mclInitializeApplication(NULL,0) || !liblsqInitialize())
    {
      std::cerr << "could not initialize the library properly"
                << std::endl;
    }
  else
    {
      lib_flag = 1;
    }

}

MPC_Controller::~MPC_Controller() {

    // Call the application and Compiler-generated library termination routine
    if(lib_flag == 1){
		liblsqTerminate();
		mclTerminateApplication();
	}
	if(lib_flag)
		delete[] m_rate_chgs;
}


//initialize the domain(e.g., processor names) information for the current workload
int MPC_Controller::init_domain(CIAO::RACE::Domain dom){
	if(dom.nodes.size() == 0){
		std::cout << "Domain is empty" << std::endl;
		return -1;
	}
	m_proc_num = dom.nodes.size();
	//get the proc names and the set points for all procs
	for(int i = 0; i < m_proc_num; i ++){
		proc_names.push_back(dom.nodes[i].label);
		//assume the first element in resources vector is CPU
		set_points.push_back(dom.nodes[i].resources[0].set_point);
	}
	return 0;
}

//initialize the matlab environment and the data information for lsqlin solver
int MPC_Controller::init(std::vector<CIAO::RACE::Task> tasks) {

  if (lib_flag != 1)
    {
      return -1;
    }
  int i,j,k;

	m_task_num = tasks.size();


	// ------ initialize the matrices of workload model, input and state ------
	m_X = mwArray(m_proc_num*PREDICTION_HORIZON, 1, mxDOUBLE_CLASS, mxREAL);
	m_Xptr = new double[m_proc_num*PREDICTION_HORIZON];
	m_X.GetData(m_Xptr,m_proc_num*PREDICTION_HORIZON);
	m_U = mwArray(m_task_num, 1, mxDOUBLE_CLASS, mxREAL);
	m_Uptr = new double[m_task_num];
	m_U.GetData(m_Uptr,m_task_num);

	m_R = mwArray(PREDICTION_HORIZON*m_proc_num, 1, mxDOUBLE_CLASS, mxREAL);
	m_Rptr = new double[PREDICTION_HORIZON*m_proc_num];
	m_R.GetData(m_Rptr,PREDICTION_HORIZON*m_proc_num);

	m_R_Chgs = mwArray(m_task_num, 1, mxDOUBLE_CLASS, mxREAL);
	m_rate_chgs = new double[m_task_num];
	rccPtr = new double[m_task_num];
	memset(m_rate_chgs, 0, m_task_num*sizeof(double));
	memset(rccPtr, 0, m_task_num*sizeof(double));

	m_F = mwArray(CONTROL_HORIZON*m_task_num*2, 1, mxDOUBLE_CLASS, mxREAL);
	m_Fptr = new double[CONTROL_HORIZON*m_task_num*2];
	m_F.GetData(m_Fptr,CONTROL_HORIZON*m_task_num*2);

	H0 = mwArray(m_proc_num*PREDICTION_HORIZON, m_task_num*CONTROL_HORIZON, mxDOUBLE_CLASS, mxREAL);
	Hptr = new double[m_proc_num*PREDICTION_HORIZON*m_task_num*CONTROL_HORIZON];
	H0.GetData(Hptr,m_proc_num*PREDICTION_HORIZON*m_task_num*CONTROL_HORIZON);
	HW = mwArray(m_task_num*CONTROL_HORIZON, m_task_num*CONTROL_HORIZON, mxDOUBLE_CLASS, mxREAL);

	H = mwArray(m_proc_num*PREDICTION_HORIZON+m_task_num*CONTROL_HORIZON, m_task_num*CONTROL_HORIZON, mxDOUBLE_CLASS, mxREAL);

	A = mwArray(2*CONTROL_HORIZON*m_task_num,CONTROL_HORIZON*m_task_num,mxDOUBLE_CLASS, mxREAL);


	B = mwArray(CONTROL_HORIZON*m_task_num*2, 1, mxDOUBLE_CLASS, mxREAL);
	BB = mwArray(2*CONTROL_HORIZON*m_task_num,m_task_num, mxDOUBLE_CLASS, mxREAL);

	E = mwArray(PREDICTION_HORIZON*m_proc_num+m_task_num*CONTROL_HORIZON,1,mxDOUBLE_CLASS, mxREAL);
	E0 = mwArray(PREDICTION_HORIZON*m_proc_num, 1, mxDOUBLE_CLASS, mxREAL);
	E1 = mwArray(PREDICTION_HORIZON*m_proc_num, 1, mxDOUBLE_CLASS, mxREAL);
	// ------------------- end of initialization of matrices -----------------

	// ----- below code computes the matrix Y which is a constant matrix ------
	Y = mwArray(m_proc_num*PREDICTION_HORIZON, m_task_num, mxDOUBLE_CLASS, mxREAL);
	Yptr = new double[m_proc_num*PREDICTION_HORIZON*m_task_num];
	Y.GetData(Yptr,m_proc_num*PREDICTION_HORIZON*m_task_num);

	//iterator for proc_names vector
	std::vector<std::string>::iterator it;
	//iterator for tasks vector
	std::vector<CIAO::RACE::Task>::iterator it_t;
	//iterator for subtasks vector
	std::vector<CIAO::RACE::Subtask>::iterator it_s;
	i = 0;
	j = 0;
	for (it = proc_names.begin(); it != proc_names.end(); it++) {
		j = 0;
		for (it_t = tasks.begin(); it_t != tasks.end(); it_t++) {
			for (it_s = (*it_t).subtasks.begin(); it_s != (*it_t).subtasks.end(); it_s++) {
				if ((*it_s).node == i)
					//transfer exec time into ms
					Yptr[i*m_task_num + j] += (*it_s).exec_time.WCET.sec*1000
					 + (double)((*it_s).exec_time.WCET.usec)/1000;
			}
			j++;
		}
		i++;
	}
	for (i=1; i<PREDICTION_HORIZON; i++)
		memcpy((void*)(Yptr + i*m_proc_num*m_task_num), Yptr, m_proc_num*m_task_num*sizeof(double));

	// finalize Yptr
	for (i=1; i<PREDICTION_HORIZON; i++) {
		for (int j=0; j< m_proc_num*m_task_num; j++)
			Yptr[i*m_proc_num*m_task_num+j] *= i+1;
	}

	rowmajor_to_colmajor(Yptr, m_proc_num*PREDICTION_HORIZON, m_task_num);

	//The data is copied in column-major order
    Y.SetData(Yptr, m_proc_num*PREDICTION_HORIZON*m_task_num);
	rowmajor_to_colmajor(Yptr, m_task_num, m_proc_num*PREDICTION_HORIZON);
	// --------------------- end of computation of matrix Y ---------------



	// ---------------------below code computes the matrix H ------------
	int dimension;
	for (i=0; i<PREDICTION_HORIZON; i++)
		for (j=0; j<m_proc_num; j++) {
			if (i+1 < CONTROL_HORIZON)
				dimension = i+1;
			else dimension = CONTROL_HORIZON;
			for (int k=0; k<dimension; k++) {
				memcpy((void*)(Hptr + (i*m_proc_num+j)*m_task_num*CONTROL_HORIZON + k*m_task_num),
					(void*)(Yptr + j*m_task_num), m_task_num*sizeof(double));
				//finalize Hptr
				for (int l=0; l<m_task_num; l++)
					Hptr[(i*m_proc_num+j)*m_task_num*CONTROL_HORIZON + k*m_task_num + l] *= i-k+1;
			}
		}

	rowmajor_to_colmajor(Hptr, m_proc_num*PREDICTION_HORIZON, m_task_num*CONTROL_HORIZON);
	H0.SetData(Hptr,m_proc_num*PREDICTION_HORIZON*m_task_num*CONTROL_HORIZON);
	rowmajor_to_colmajor(Hptr, m_task_num*CONTROL_HORIZON, m_proc_num*PREDICTION_HORIZON);

	// matrix of  weights
	double* HWptr;
	HWptr = new double[m_task_num*CONTROL_HORIZON*m_task_num*CONTROL_HORIZON];
	HW.GetData(HWptr,m_task_num*CONTROL_HORIZON*m_task_num*CONTROL_HORIZON);
	for (i=0;i<CONTROL_HORIZON;i++)
		for (j=0; j<m_task_num; j++)
			HWptr[(i*m_task_num+j)*m_task_num*CONTROL_HORIZON + i*m_task_num+j]
			 = CONTROL_HORIZON-i;

	rowmajor_to_colmajor(HWptr, m_task_num*CONTROL_HORIZON, m_task_num*CONTROL_HORIZON);
	HW.SetData(HWptr,m_task_num*CONTROL_HORIZON*m_task_num*CONTROL_HORIZON);

	H = matrix_row_join(H0,HW,m_proc_num*PREDICTION_HORIZON,m_task_num*CONTROL_HORIZON,m_task_num*CONTROL_HORIZON, m_task_num*CONTROL_HORIZON);
	// ------------------- end of computation of H


    //------------ below code handles the Constraint A on the formulation -----------------
	for(i = 1;i <= CONTROL_HORIZON*m_task_num; i++){
		for( j = 1; j <= CONTROL_HORIZON*m_task_num ; j++){
			if(i != j){
				A(i,j) = 0; A(i+CONTROL_HORIZON*m_task_num,j) = 0;
			}
			else {
				A(i,j) = 1; A(i+CONTROL_HORIZON*m_task_num,j) = -1;
			}
		}
	}

	double temp1,temp2,temp3;
	temp3 = 2.0;
	for (k=0; k<m_task_num; k++)
		for (i=0; i<CONTROL_HORIZON-1; i++)
			for (j=CONTROL_HORIZON-1; j>i; j--) {
				for(int row = 1; row <= 2*CONTROL_HORIZON*m_task_num; row ++ ){
					temp1 = A(row,i*m_task_num+k+1);
					temp2 = A(row,j*m_task_num+k+1);
					A(row,i*m_task_num+k+1) = temp1 + temp3*temp2;
				}
			}

	for(i = 1; i <= 2*CONTROL_HORIZON*m_task_num; i++){
		for(j = 1; j <= m_task_num; j++){
			temp1 = A(i,j);
			BB(i,j) = temp1;
		}
	}
	// ------------------- end of computation of A


	return 0;
}

std::vector<CIAO::RACE::Task>
MPC_Controller::control_period(CIAO::RACE::Domain dom, std::vector<CIAO::RACE::Task> tasks)
{
	if(lib_flag == 0)
		return tasks;
	int num = 0;
	int i,j;

	// ---------- vector X(k) assignment ------------
	//extract current util from domain.
	//Assume the order of the nodes vector in the domain structure is fixed for the whole program
	std::vector<CIAO::RACE::Node>::iterator it;
	for (it = dom.nodes.begin(); it != dom.nodes.end(); it++)
		m_Xptr[num++] = (*it).resources[0].util;

	for (i=1; i<PREDICTION_HORIZON; i++)
		for (j=0; j<m_proc_num; j++)
			m_Xptr[i*m_proc_num+j] = m_Xptr[j];



	//The data is copied in column-major order
    m_X.SetData(m_Xptr, m_proc_num*PREDICTION_HORIZON);

	// -----------vector U(k) assignment ----------------
	memcpy(m_Uptr, m_rate_chgs, m_task_num*sizeof(double));

	std::cout << "U "<<std::endl;
	for (i=0; i<m_task_num; i++)
		std::cout << m_Uptr[i] << std::endl;
	m_U.SetData(m_Uptr,m_task_num);

	//--------------- reference trajectory vector computation----------------
	double lamda = exp(- (double)T_SAMPLING / (double)T_REF);
	double ratio = 1.0;
	double error;
	num = 0;
	for (i=0; i<PREDICTION_HORIZON; i++) {
		ratio = ratio*lamda;
		for (j = 0; j < m_proc_num; j ++) {
			double cpu_util = m_Xptr[j];
			double cpu_util_setpoint = set_points[j];
			error = cpu_util_setpoint - cpu_util;
                        std::cout << "Error in node "
                                  << j << " = " << error
                                  << std::endl;
			m_Rptr[num++] = cpu_util_setpoint - error*ratio;
		}
	}

// 	for (i=0; i<PREDICTION_HORIZON*m_proc_num; i++)
//           std::cout << "Error " << "i = " << m_Rptr[i] << std::endl;
	m_R.SetData(m_Rptr,PREDICTION_HORIZON*m_proc_num);

    //------------ below code handles the Constraint A on the formulation -----------------
	//period in ms
	double l_period, h_period;
	//rate in 1/1000Hz
	double rate;
	std::vector<CIAO::RACE::Task>::iterator it_t;

	num = 0;
	for (it_t = tasks.begin(); it_t != tasks.end(); it_t++) {
		l_period = 1000.0/(*it_t).max_rate;
		rate = (*it_t).curr_rate/1000.0;
		m_Fptr[num] = 1.0/l_period - rate;
		num++;
	}
	for (i=1; i<CONTROL_HORIZON; i++)
		memcpy((void*)(m_Fptr + i*m_task_num), m_Fptr, m_task_num*sizeof(double));

	num = CONTROL_HORIZON*m_task_num;
	for (it_t = tasks.begin(); it_t != tasks.end(); it_t++) {
		h_period = 1000.0/(*it_t).min_rate;
		rate = (*it_t).curr_rate/1000.0;
		m_Fptr[num] = -1.0/h_period + rate;
		num++;
	}
	for (i=CONTROL_HORIZON; i<2*CONTROL_HORIZON; i++)
		memcpy((void*)(m_Fptr + i*m_task_num), (void*)(m_Fptr + CONTROL_HORIZON*m_task_num),
			m_task_num*sizeof(double));


	m_F.SetData(m_Fptr,CONTROL_HORIZON*m_task_num*2);


    //-------------- least square optimiztion computation --------------
	E0 = matrix_minus(m_R,m_X,PREDICTION_HORIZON*m_proc_num,1,PREDICTION_HORIZON*m_proc_num,1);
	E1 = matrix_multiple(Y,m_U,m_proc_num*PREDICTION_HORIZON, m_task_num,m_task_num,1);
	E0 = matrix_minus(E0,E1,PREDICTION_HORIZON*m_proc_num,1,PREDICTION_HORIZON*m_proc_num,1);


	double temp;
	for (i = 1; i <= PREDICTION_HORIZON*m_proc_num; i++){
		temp = E0(i,1);
		E(i,1) = temp;
	}
	for (i = PREDICTION_HORIZON*m_proc_num + 1; i <= PREDICTION_HORIZON*m_proc_num + m_task_num*CONTROL_HORIZON;i++){
		E(i,1) = 0;
	}

	B = matrix_multiple(BB,m_U,2*CONTROL_HORIZON*m_task_num,m_task_num,m_task_num,1);
	B = matrix_minus(m_F,B,2*CONTROL_HORIZON*m_task_num,1,2*CONTROL_HORIZON*m_task_num,1);


	// call lsqlin_simple2 function from liblsq.dll(compiler-generated library) to compute the least square optimiztion
	lsqlin_simple2(1,m_R_Chgs,H,E,A,B);

	//Get the resulted new period
	m_R_Chgs.GetData(rccPtr,m_task_num);
	for(i = 0; i < m_task_num; i++){
		m_rate_chgs[i] += rccPtr[i];
		//std::cout<< rccPtr[i] << std::endl;
	}
	i = 0;
	for (it_t = tasks.begin(); it_t != tasks.end(); it_t++) {
		//transfer rate change back to Hz and store it in the tasks vector
		(*it_t).delta_rate = 1000.0*m_rate_chgs[i++];
	}

	return tasks;

}

void MPC_Controller::rowmajor_to_colmajor(double* matrix, int row, int col)
{
	int i,j;
	double* temp;
	int num = 0;

	temp = new double[row*col];
	memcpy(temp, matrix, row*col*sizeof(double));
	for (i=0; i<col; i++)
		for (j=0; j<row; j++)
			matrix[num++] = temp[j*col+i];
	delete[] temp;
}


void MPC_Controller::new_matrix_multiple(double* out,double* in1,double* in2, int r1,int c1,int r2,int c2){
	if(c1 != r2){
		std::cout << "dimensions of in1 and in2 do not match " <<std::endl;
		return;

	}
	int i,j,k;
	int num = 0;
	double a,b,c;
	for(i = 0; i < r1; i++){
		for(j = 0; j < c2; j++){
			c = 0;
			for(k = 0; k < c1; k ++){
				a = in1[i*c1+k];
				b = in2[k*c2+j];
				c += a*b;
			}
			out[num++] = c;
		}
	}
}

//caculate out = in1(r1,c1) * in2(r2,c2);
mwArray MPC_Controller::matrix_multiple(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2){
	if(c1 != r2){
		std::cout << "dimensions of in1 and in2 do not match " <<std::endl;
		return mwArray();

	}
	int i,j,k;
	double a,b,c;
	mwArray out(r1,c2,mxDOUBLE_CLASS, mxREAL);;
	for(i = 1; i <= r1; i++){
		for(j = 1; j <= c2; j++){
			c = 0;
			for(k = 1; k <= c1; k ++){
				a = in1(i,k);
				b = in2(k,j);
				c += a*b;
			}
			out(i,j) = c;
		}
	}
	return out;
}

//out = in1(r1,c1) - in2(r2,c2);
mwArray MPC_Controller::matrix_minus(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2){
	if((c1 != c2)&&(r1 != r2)){
		std::cout << "dimensions of in1 and in2 do not match " <<std::endl;
		return mwArray();

	}
	int i,j;
	double a,b,c;
	mwArray out(r1,c1,mxDOUBLE_CLASS, mxREAL);;
	for(i = 1; i <= r1; i++){
		for(j = 1; j <= c1; j++){
			a = in1(i,j);
			b = in2(i,j);
			c = a - b;
			out(i,j) = c;
		}
	}
	return out;
}
void MPC_Controller::array_minus(double* in1,double* in2, int len){
	for(int i = 0; i < len; i ++){
		in1[i] = in1[i] - in2[i];
	}
}
void MPC_Controller::array_plus(double* in1, double* in2, int len){
	for(int i = 0; i < len; i ++){
		in1[i] = in1[i] + in2[i];
	}
}


//out = in1(r1,c1) + in2(r2,c2);
mwArray MPC_Controller::matrix_plus(mwArray in1,mwArray in2, int r1,int c1,int r2,int c2){
	if((c1 != c2)&&(r1 != r2)){
		std::cout << "dimensions of in1 and in2 do not match " <<std::endl;
		return mwArray();
	}
	int i,j;
	double a,b,c;
	mwArray out(r1,c1,mxDOUBLE_CLASS, mxREAL);;
	for(i = 1; i <= r1; i++){
		for(j = 1; j <= c1; j++){
			a = in1(i,j);
			b = in2(i,j);
			c = a + b;
			out(i,j) = c;
		}
	}
	return out;
}

//out = [in1;in2];
mwArray MPC_Controller::matrix_row_join(mwArray in1,mwArray in2,int r1,int c1,int r2,int c2){
	if(c1 != c2){
		std::cout << "dimensions of in1 and in2 do not match " <<std::endl;
		return mwArray();
	}
	int i,j;
	double a,b;
	mwArray out(r1+r2,c1,mxDOUBLE_CLASS,mxREAL);
	for(i = 1; i <= r1; i++){
		for(j = 1; j <= c1; j++){
			a = in1(i,j);
			out(i,j) = a;
		}
	}
	for(i = 1; i <= r2; i++)
		for(j = 1; j <= c2; j++){
			b = in2(i,j);
			out(i+r1,j) = b;
		}
		//std::cout<< "out matrix " << std::endl << out << std::endl;
	return out;

}
