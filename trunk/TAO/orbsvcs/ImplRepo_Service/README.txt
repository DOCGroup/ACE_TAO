/** -*- HTML-Helper -*- $Id$

$Id$

@mainpage Implementation Repository

@section intro Introduction

The purpose of the Implementation Repository (ImR) in TAO is for the automatic
activation of a TAO server when a method is invoked but the server is not
running. It does this by working with the server to keep track of when it is
activated and stores information on how to reactivate it. Method
invocations on the server will actually be sent to the Implementation
Repository, which will then be able to start the server process if it is not
already running and forward the invocation to the real server.

@section resources Resources

The ImR in TAO was first based on a paper on the subject by
<a href="http://www.triodia.com/staff/michi-henning.html">Michi Henning</a>
called <a href="http://www.triodia.com/staff/michi/cacm.pdf">Binding,
Migration, and Scalability in CORBA</a>. Mr. Henning later went on to
coauthor the book
<a href="http://cseng.awl.com/bookdetail.qry?ISBN=0-201-37927-9&amp;ptype=0">
Advanced CORBA Programming in C++</a> and included a chapter on the
Implementation Repository. The description of the ImR in this chapter is used
as the specification for TAO's ImR.

@ref usersguide - Overall documentation on how to use the ImR in your programs

@ref future - Future Work

@ref ntservice - Running the ImR as a NT Service

@ref imrandnaming - Using the Naming Service with the Implmentation Repository

@ref movefromoldImR - Moving from IMR as in TAO 1.2.2 to the present version.

@section authors Authors

The guy who first worked on ImR and provided a base for the current
ImR is Darrell Brunsch
@<<a href="mailto:brunsch@cs.wustl.edu">brunsch@cs.wustl.edu</a>@>.
The current version is developed by Priyanka Gontla
@<<a href="mailto:gontla_p@ociweb.com">gontla_p@ociweb.com</a>@>.
You can reach us by either email (which is better), or through the
<a href="http://www.cs.wustl.edu/~schmidt/ACE-mail.html">ACE mailing list</a>
@<<a href="mailto:ace-useres@cs.wustl.edu">ace-users@cs.wustl.edu</a>@>,
or through the <a href="news:comp.soft-sys.ace">comp.soft-sys.ace</a>
newsgroup. The newsgroup mirrors the mailing list, but not the other way.
Also, if you use the mailing list, it is best if you join it before sending
a question since responses are usually just replied back to the group.

The NT Service part of the ImR was developed by Jeff Parsons @<<a
href="mailto:parsons@cs.wustl.edu">parsons@cs.wustl.edu</a>@>.  He can
also be reached via the same channels.
It was later extended and enhanced by Justin Michel <michel_j@ociweb.com>

*/

/**
@page future Future Work

As with any program, there is always a wishlist of things to do.

@subsection ort Use Object Reference Template (ORT) Features

   Use ORT so that the ImR-ified IORs doesnt have any information
about the ImR_Activators. Right now, even though, the ImplRepo_Service is
the one that is exposed to the client, the IOR will have a reference
to the ImR_Activator. By using ORT features, we want the IOR to refer
to the ImplRepo_Service rather than the underlying ImR_Activator. That way
the ImR_Activator is totally blocked from the client using the ImR
service.

@subsection ort_design Design Considerations

   For the above mentioned purposes, we can say that the ImplRepo_Service
acts as the gateway which receives requests on behalf of the actual
ImR_Activator.

   The ImplRepo_Service will now need a new IDL method 'create_object'. The
create_object method will take the interface_repository_id of the
object that has to be gatewayed (ImR_Activator's) and the
CORBA::Object_ptr of the gatewayed object. And, using these two will
create an object which will point to the ImplRepo_Service and will also
know that the ultimate receiver is the ImR_Activator.

   So, this is how it works.

   As before, we will first run the ImplRepo_Service. And, then run an
ImR_Activator. We will use interceptors and in the post_init method,
we will add an ior interceptor which will have the ImplRepo_Service
object. By this, the IOR that is generated will actually point to the
ImplRepo_Service rather than ImR_Activator. So, the IOR which is visible to
the client will show that the IOR actually points to the ImplRepo_Service.

   When a client sends a request to the ImplRepo_Service, the request
will come to ImplRepo_Service which will send the request to the actual
ImR_Activator.

@subsection AMH Use AMH Features

   Use the AMH features to improve the TAO IMR performance.

@subsection logicalnames Logical Server names

In the IDL interface, but not used on the server side or fully implemented in
tao_ImR.

@subsection shutdown Server Shutdown

Only cooperative shutdown implemented right now.  Since we keep track of the
Process ID, then we could kill the server if it doesn't cooperate.

@subsection optimization Client-side Optimizations

Nothing yet.

@subsection security Server Security

Nothing really here.  There is two main things I have in mind.

First, the security service would be useful to restrict access to some
of the Administration methods

Second, without a security service we should provide some sort of flag to
the ImR that disables the Administration methods.  That way, the database
can be set up with the servers in a controlled run of the ImR.  Then the
ImR could be run again reading in the database without Admin access.

@subsection federations Federations

Nothing yet.

@subsection dllserver DLL servers

Nothing yet.

@subsection XML Database Support

  As of now, the support is only to be able to have the information
	about a registered server written to an XML file. Have to support
retrieving information from the XML file to be able to do any actions
on the registered servers.

@subsection Remove ImR_Activator

  We can now successfully register an ImR_Activator. But, we have to
yet provide support to gracefully unregister the ImR_Activator from
the ImplRepo_Service. The ImplRepo_Service then has to try to transfer the
servers that were registered with this instance to other activators.
*/

/**
@page usersguide Implementation Repository User's Guide

In order for a server to make use of the Implementation Repository, it must
communicate with the ImR to keep it up to date on such things as the server's
running status. These functions now are contained within the POA, so when a
Persistent POA is used on a server that has -ORBUseImR specified, it will
communicate with an Implementation Repository, if one is available.

@subsection description The New ImR

	 The new ImR is based on the ImR in TAO 1.2.2 with added
features to help improve throughput and load balancing. The work to be
performed by the Implementation Repository is distributed between two
entities (ImplRepo_Service and ImR_Activator) to help achieve the goal of
better throughput and load balance.

@subsection locator ImplRepo_Service

	The ImplRepo_Service acts as the main server which is visible to
the application intending to use the ImR. It receives requests sent
via tao_ImR and distributes the work to the registered ImR_Activators.
It is stateless and does not maintain any information except about the
ImR_Activators that are registered with it. Its job is to act as
a mediator between the application and the actual ImR_Activator that does
the real work. As of now, we only support one ImplRepo_Service to be running at
any time. ImplRepo_Service can be reached through the usual methods of
-ORBInitRef and -ORBDefaultInitRef and multicast.

Commandline Arguments that can be passed to ImplRepo_Service

-d  debug information
-m  support multicast discovery.
-o  generate the ior.
-x  support persistence to the ImplRepo_Service. We use XML to support
    persistence. Names of the activators registered with the locator,
	their IORs, and the servers registered with each of the activators are
	saved to the xml file. Use this option to pass the name of the file
	where the data has to be saved.

	And, ofcourse, the ORB Options.


@subsection activator ImR_Activator

	ImR_Activators, as you might have guessed, do the real work of
activating servers or shutting them down and maintaining the information
about servers related to them. Only one instance of an ImR_Activator
can be run on one host. The ImR_Activator is not exposed at all to the
application. Only the ImplRepo_Service needs to and it is the only one that
can contact the ImR_Activator.

	An instance of ImR_Activator first registers itself with the
ImplRepo_Service so that it can begin to receive requests. When registering
with the ImplRepo_Service, it passes the hostname where it is being run and
its IOR to the ImplRepo_Service. And, the ImplRepo_Service reaches it using the
same information.

The Commandline paramters that are valid for ImR_Activator are

-c:	     Run the Service command.
-d:number    Debug Information
-l	     lock the database.
-o 	     Generate the IOR to a file (just in case some one wants
	     to read the IOR)
-r	     Enable Win32 regsitry implementation.
-s	     Run as a service.
-t 	     Set a timeout value.
-h 	     Prints the help.

        When Persistence of an ImR_Activator is required, we will save
the information about the server's that this ImR_Activator is related
to in a file (being the easy form of a persistent database). The
information about each server include its startup options, location,
working directory which are needed to execute the requests that can
passed by tao_imr with regards to the server.

	There are two ways in which you can store data in the
file. One way is to use ACE_Configuration_Heap to save all
the information to the file. To do this, we have to pass the '-p' option.

-p	     Pass the ImplRepo service a filename to use for the
             backing store. Uses ACE_Configuration_Heap.

	The second way is to save in XML-ized format.

-x	     Pass the filename where the repository information should
             be saved. Use XML format.


@subsection work So how does the whole thing work?

	The first thing to do is to have an ImplRepo_Service running. Once
the ImplRepo_Service is running, we can instantiate one or more ImR_Activators
as needed per the application. As mentioned already, the
ImR_Activators, upon instantiation, register with the ImplRepo_Service to
be able to receive requests.

	When a new server has to be added or any change has to the
done to an existing server, a request is to be sent to the ImplRepo_Service
via the tao_imr utility. Startup commands, the working directory, the
host where the server should be started up and such other information
are passed to the ImplRepo_Service via the TAO_ImR commandline arguments.

	If the host where the server should be started up is passed
while adding a new server, the ImplRepo_Service chooses the ImR_Activator
that is running on that host to be responsible for the server's
activities. Otherwise, an ImR_Activator is chosen based on the Round
robin algorithm. We plan to use better algorithms based on the
existing load for the same purpose in future. Whatever way the
ImR_Activator is chosen for a server, once an ImR_Activator is chosen,
that ImR_Activator remains reponsible for the server throughout the
server's lifetime.

	After an ImR_Activator is chosen, the ImplRepo_Service passes the
request to the chosen ImR_Activator. The ImR_Activator acts on the request
and updates its database to reflect the new state of the server.

@subsection run How is the ImR run?

<ol>
  <li>First run the <b>ImplRepo_Service</b>
    <em>Example:<code> </code> </em><code> ImplRepo_Service -o locator.ior<br></code>
  <li>Run the <b>Activator</b>
    <em>Example:<code> </code> </em><code> ImR_Activator -ORBInitRef ImplRepoService=file://locator.ior<br></code>

@subsection use How is the ImR used?

The main steps for the lifetime of a server that uses the ImR are generally
the following:

<ol>
  <li>Register name and startup commands with the ImR using <b>tao_ImR<br>
    </b><em>Example:<code>  </code> </em><code> tao_ImR -ORBInitRef
    ImplRepoService=file://locator.ior add plane -c &quot;airplane_server -i
    -ORBInitRef ImplRepoService=file://locator.ior&quot;<br></code>
    <br>
    Note that the name used to register the server is the name of the POA
    which the objects are created in. So in this example, the airplane_server
    creates a POA called &quot;plane&quot; and activates its servants under
    it.<br>
  <li>Start the server once to generate an ImR-ified IOR<br>
  <li>Start clients and pass them the above IOR<br>
  <li>Clients will use the IOR, which will automatically go through the ImR<br>
  <li>The ImR will start the server if it is not already running<br>
  <li>At any time when the server is not currently in use by a client, it can be
      shut down using <strong>tao_ImR<br></strong><em>Example:</em><code> tao_ImR
      -ORBInitRef ImplRepoService=file://locator.ior shutdown plane<br>
    </code>
  <li>After the server isn't needed anymore, it can be removed from the ImR database
      using <strong>tao_ImR<br></strong><em>Example:<code> </em>tao_ImR -ORBInitRef
      ImplRepoService=file://locator.ior remove plane</code>
</ol>

@subsection serverresponsibilities So what does the server need to do?

As of TAO 1.0.9, the Implementation Repository support on the server-side has
been integrated into the POA. Previously, the IR_Helper class needed to be
used explicitly. Now only the &quot;-ORBUseImR 1&quot; command line
argument needs to be used.

There are a couple of restrictions on the server though. Only objects within
a persistent POA will be supported by the ImR. Also the Implementation
Repository will key its entries on POA name. What this means for the server
is that each server must have unique persistent POA names.

@subsection defaultinitref Use of -ORBDefaultInitRef with the ImR

As mentioned in the INS documentation (in TAO/docs/INS.html), a base IOR
can be passed to the ORB.  Then when resolve_initial_reference is called,
the ORB can append the service name to the base IOR to form a full IOR.

When used with the ImR, this can be a powerful feature.  If the ImR's endpoint
is used as the base IOR, then the ImR can be used to provide many services via
the resolve_initial_reference functionality.

For example, let's say the ImR service is running on doriath on port 5555 and
the Name Service is already registered with the ImR (in that the ImR knows how
to start the Name Service).

Now we should be able to run some client that uses the Name Service like this:

<code>client -ORBDefaultInitRef corbaloc:iiop:doriath:5555/</code>

When the client calls resolve_initial_reference("NameService"), the ORB will
resolve that to "corbaloc:iiop:doriath:5555/NameService".  The ImR
recognizes this IOR as a pointer to the NameService, and will then
start it up if necessary.  Finally, it will forward the client to the
Name Service.

Services used in this way have two requirements:

- The server must be registered as the request_initial_reference name.  For
  example, the Name Service is registered as the "NameService", and thus also
  contains a POA with the name "NameService".
- The server must also be able to handle the INS name
  "corbaloc:iiop:machine:port/poa_name", where the poa_name is the
  same name as above.

@subsection activationmodes What are activation modes

Each server can have one of three different types of activation modes:

<ul>
  <li>NORMAL is the default.  The server can be started via tao_ImR,
      the command line, and with client requests.
  <li>MANUAL specifies that the server shouldn't be activated with a
      client request but can be activated through tao_ImR or via the
      command line.
  <li>PER_CLIENT specifies that each request to the ImplRepo will
      result in a new server process started up.  Because clients
      cache the forwarded reference, there is one server per client
      (more or less).  There are some exceptions, such as if the
      original IOR is used in different threads (each thread would get
      a different server).  <b>Note:</b> The Implementation Repository
      doesn't store any information about the started servers in this
      mode, so it cannot be used to shut down the servers.  So the
      servers must have an alternative way of shutting down.
  <li>AUTO_START specifies that a server should be activated when the
      Implementation Repository is started.  tao_ImR also has an
      autostart command to activate all servers marked AUTO_START.
</ul>

@subsection taoImRior Using the tao_ImR ior command

First, some background.

For the longest time, there was no way with TAO's Implementation Repository
to register a server and then start using the client immediately.  The server
had to be started once just for the purpose of creating an IOR for the client
to use.  The problem was that it was very difficult to create an IOR without
running the server.

It would be nice to be able to  generate a valid IOR without requiring the
program to be run. A valid IOR in this case requires two major things.  First
it requires the endpoint of the ImR. This is relatively easy to get, since it
is encoded in the ImR's IOR.  Second it also requires an object key.  At the
least, this involves both the POA hierarchy and the object name.

So if we knew the POA and object names, then we should be able to create an
IOR for the server. One possibility would be to have tao_ImR ask for both the
POA and the object, and then create the POA hierarchy to generate an IOR.
Doing the generation is this manner (letting the POA create the reference)
shields us from changes in the IOR generation schemes.  Since we are using
the same code that the server would use, our IORs would be up to date.

It ends up there is an easier way to do this.  The Interoperable Naming
Service is intended to be used in situations where an IOR could be created by
hand.  Using the same information as above, it is not difficult to take the
endpoint information from the ImR and attach the POA name.  For example,
let's say that we are running the ImR on ringil.ece.uci.edu at port 5000.
The endpoint would be
&quot;corbaloc:iiop:1.2@ringil.ece.uci.edu:5000&quot;.  If we are
creating an IOR for the nestea server, we'd just need to attach
&quot;/nestea_server&quot; to the end of the endpoint.  Now we have an
IOR.

So what does this mean for the server?

The main issue here is that the server must be changed to support the
simplified name. This can be done by using the IORTable like this:

<CODE>
      CORBA::Object_var table_object =
        this->orb_->resolve_initial_references ("IORTable",
                                                ACE_TRY_ENV);
      ACE_TRY_CHECK;

      IORTable::Table_var adapter =
        IORTable::Table::_narrow (table_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
      if (CORBA::is_nil (adapter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
        }
      else
        {
          adapter->bind (poa_name, server_str.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }
</CODE>

These lines, as taken from the nestea_server example, just uses the same
poa_name as registered with the ImR and associates it with the server_obj
object in the IOR table.  Because the ImR will be able to handle the
simplified name (if it uses the POA name scheme) then this IOR will work.

Just one more thing, each object that needs an IOR needs to be registered
with the IOR table.  But this raises the problem of uniqueness; they all
can't have the same name.  The ImR will actually only look at the name part
of the simplified IOR up to the first &quot;/&quot;.  So both
&quot;corbaloc:iiop:1.2@ringil:5000/nestea_server/foo&quot; and
&quot;corbaloc:iiop:1.2@ringil:5000/nestea_server/bar&quot; will be treated by
the ImR as objects in the &quot;nestea_server&quot; server.

@subsection persistence Persistent Implementation Repository

Tao's Implementation Repository can be made persistent by doing two things:

<ul>
<li>
Always start up the Implementation Repository on the same port. This ensures that
the Implementation Repository will not have to re-ImR-ify the IORs of every server
registered to it each time it starts up. The way to accomplish this is to add<br>
-ORBEndpoint iiop://(hostname):(portnumber)<br>

 to the ImR_Activator's startup command line. The host
name can be obtained portably in C++ code with the lines<br>

	ACE_INET_addr ad;<br>
	char *hostname = ad.get_host_name ();<br>

or in a Perl script by adding<br>

	use Sys::Hostname;<br>
	$hostname = hostname();<br>

There are even specific port numbers, assigned to the OMG by the IANA,
which can be used for this purpose. They are 683 (for IIOP) and 684
(for IIOP over SSL).  For more information about this, see
<a href="http://www.iana.org/">http://www.iana.org/</a>
and <a href="http://www.isi.edu/in-notes/iana/assignments/port-numbers">
http://www.isi.edu/in-notes/iana/assignments/port-numbers</a>.<br><br>
<li>
Pass the ImR a filename to use for the backing store, specified by the
command line option<br>

-p (filename)<br>

This option must be used the first and every subsequent time the
persistent ImR is started up.

</ul>
*/

/**
@page ntservice Running as an NT service

The ImplRepo_Service.exe can now also function as a Windows NT
Service.  The -c option can be used to install and remove the service
(this requires Administrator access on the machine).

@note When using the ImplRepo_Service as a service, it must have all
of its required ACE/TAO DLL's in the path or in the same directory.
For example, the run_test.pl copies ImplRepo_Service.exe to the
ACE_wrappers@\bin directory before using "-c install".
Alternatively, You can set the usual ACE_ROOT, TAO_ROOT, and PATH environment
variables on a system wide basis.

The service can be then started either from the Windows NT "Services"
Admin Tool or via the "net" program on the command line:

<CODE>net start "TAO Implementation Repository Locator"</CODE>
<CODE>net start "TAO Implementation Repository Activator"</CODE>

The Implementation Repository supports start and stop but not pause.

When the Activator is installed using ImR_Activator -c install, it is added
with a dependency on a locator service. If you don't wish to also install
the locator on the same machine, then you must use the -c install_no_imr
option instead.

@subsection serviceopts Service Options

Any options that are specified along with -c install, will be saved in
the registry under
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\TAOIMRActivator\Parameters and
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\TAOIMRLocator\Parameters.
For example:
ImR_Activator -c install -d 3 -l -m -o activator.ior -x persist.xml -t 30 -orbendpoint iiop://:9988 -orbdebuglevel 1

The order of arguments makes no difference, but you must run
-c remove and then -c install if you want to change the parameters.

You can also manually change these using the typical regedit utility.

*/

/**
@page imrandnaming Naming Service

@subsection lowdown The Short Answer

Register the Naming Service with an Implementation Repository using the
POA name "NameService".  So for example:

<CODE>tao_imr add NameService -c "Naming_Service -ORBUseIMR 1"</CODE>

And then an IOR can be created by:

<CODE>tao_imr ior NameService</CODE>

The above commands depend on multicast (since the commands are shorter and
easier put in documents).  You might need to add "-ORBInitRef
ImplRepoService=..." for a more robust solution.

@subsection details Various Details

The Naming Service in TAO contains one persistant POA named "NameService".
If -ORBUseIMR 1 is passed to it, it will communicate with the ImR as any
other persistent POA does.  Also, the Naming Service recognizes the INS
object key "NameService".  This allows you to use the "tao_imr ior"
command to create corbaloc IORs.

NameService was chosen because resolve_initial_references () uses that
name.  And that allows us to do interesting things with ORBDefaultInitRef
(as shown in the @ref defaultinitref section).

*/

/**
@page movefromoldImR Transition from IMR in TAO 1.2.2


While the previous version of ImR has a single focal point (ImplRepo_Service)
for performing all the jobs including receiving various requests,
maintaining the information about all the servers registered with it
and actually activating the servers as needed, in the new IMR, we
distribute the work load between two entities, ImplRepo_Service and
ImR_Activator, to help achieve the goal of better throughput and load
balance.

<b>.</b> The added step that you would have to now do is run the
ImR_Activator. You can just run one instance of it to get the same
behaviour as you were getting before.

If you run the ImplRepo_Service in multicast mode, the
ImR_Activator will be able to get access to it via
multicast. Otherwise, you would have to use one of the reference
initializing methods to give the ImR_Activator access to the
ImplRepo_Service.

<b>.</b> As I mentioned before, the previous commandline parameters
that can be passed to ImplRepoService for the repository purposes will
now be passed to the ImR_Activator since that is the one that is
dealing with the database.

*/
