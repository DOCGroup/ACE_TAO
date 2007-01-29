

//Author: Stoyan Paunov

This is the README for the PlanGenerator Input Adapter for RACE. This input
adapter has similar functionality to the Interactive Input Adapter, however
instead of parsing the descriptors on disk, it takes the provided by the
injector package name, retrieves the corresponding PackageConfiguration, and
finally build a DeploymentPlan in memory. Due to the fact that the XML parsing
is avoided, the operation takes a lot less time than its counterpart in the
Interactive Input Adapter. An added benefit is that the implementation arti-
facts in the constructed DeploymentPlan to point to the HTTP server collocated
with the RepositoryManager.


