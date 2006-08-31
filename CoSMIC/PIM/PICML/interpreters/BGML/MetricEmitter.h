#ifndef METRIC_EMITTER_H
#define METRIC_EMITTER_H

#include "PICML/PICML.h"

#include <string>
#include <vector>

#include "BGML/Global_Data.h"

template <class T>
class MetricEmitter
{
public:
	MetricEmitter (PICML::OperationRef &base,
				   T& latency,
			       std::string& metric,
				   BGML_Data &state);
	~MetricEmitter ();

	void generate_benchmark ();

	std::string component_name (PICML::TwowayOperation& op);

	void return_type_signature (PICML::MemberType& mem_type,
								std::string& operation_name,
								int type);

	std::string operation_signature (PICML::TwowayOperation& op);

protected:
	void generate_header_file (std::string& class_name,
							   std::string& component_name,
							   std::string& operation_name,
							   std::vector<string>& arg_list,
							   bool is_main);
	// Generate the header file information

	void create_build_file (std::vector<std::string>& file_list,
							std::string& project_name,
							std::string& dependancy_list);
	// Generate the build file building benchmarking library

	void generate_workload_files (std::string &component_name,
								  std::string &operation_name,
								  std::vector<std::string> &arg_list,
								  std::vector<std::string> &source_file_list,
								  bool is_main,
								  BGML_Task_Group_Data &data);
	// Generate workload .h and .cpp files

	void generate_workload_tasks_that_differ (BGML_Task_Group_Data &task_group_data,
											  std::string &component_name,
											  std::vector<std::string> &source_file_list,
											  bool is_main);
	// Generate workload tasks that benchmark different operation than the main task

private:
	PICML::OperationRef &operation_ref_;
	T& latency_;
	std::string& metric_;
	BGML_Data &bgml_state_;

};

#include "BGML/MetricEmitter.cpp"

#endif /* LATENCY_EMITTER_H */
