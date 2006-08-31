#ifndef _GLOBAL_DATA_H
#define _GLOBAL_DATA_H

#include <vector>
#include <string>
#include "PICML/PICML.h"

/**
 * In BGML, a Task Group Data identifes all data present within a task group. 
 * In particular, a Task group will have a set of tasks such that:
 * 1: All the tasks in a group will have the same rate and priority of 
      invoking operations
 * 2: The operations invoked in a task might be different than the main
      operation that is going to be benchmarked.
 */
struct BGML_Task_Group_Data
{
	__int64 task_rate;
	__int64 task_priority;
	__int64 size;
	std::vector<PICML::OperationBase> background_operations;
};

struct BGML_Data
{
	__int64 benchmark_priority;
	__int64 benchmark_rate;
	// Benchmarking Rate and priority 
	// for the tasks
	
	std::vector<BGML_Task_Group_Data> task_group_data;
	/// Task information, their rates, 
	/// priorities and number of tasks

	std::string file_name;
	// A File to dump data to
	
	__int64 warmup_iterations;
	// Number of warmup iterations
	
	__int64 benchmark_iterations;
	// Number of actual benchmarking iterations
	// required

	std::string output_path;
	// Directory path were the generated data should
	// be written out

	std::string resolution;
	// Resolution of the data samples generated in the results

	PICML::BenchmarkType benchmark_type;
	// The type of benchmark
};

#endif /* _GLOBAL_DATA_H */
