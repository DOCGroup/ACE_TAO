

//Author: Stoyan Paunov

This is the README for the LocationUpdater Input Adapter for RACE. This input
adapter has similar functionality to the Interactive Input Adapter, however
instead of using only the descriptors on disk, it uses the UUID from the
parsed PackageConfiguration to retrieve its counterpart from the Repository
Manager and updates the location of the implementation artifact to point to
the HTTP server. This way the files can then be retrieved by DAnCE at
deployment.

