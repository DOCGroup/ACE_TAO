# Server-only periodic idle scanning

The server enables idle age Y=2 seconds and scan interval X=2 seconds.
The client explicitly disables expiry. Server settings live in server.conf,
not the automatically loaded svc.conf.

The client makes six pings separated by one-second reactor waits. This keeps
the transport active across multiple scans and beyond the first Y+X window.
Before each subsequent ping it checks its cache, so transparent reconnection
cannot hide premature server closure.

It then stops sending, processes closure notifications for up to Y+X+1 seconds,
and requires an empty cache. A subsequent ping must reconnect successfully.

Generate projects using MPC, build, and run `perl run_test.pl`.
Use `-debug` or `-cdebug` for ORB logging.
