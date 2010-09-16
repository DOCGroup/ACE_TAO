// $Id$

This test exists of 3 components.
-Master component  which sends asynchronous and synchronous calls to a Sender.
-Sender which recieves asynchronous and synchronous calls from the Master and replies them. Also the
Sender sends asynchronous and synchronous calls ( uses multiple) to 3 Receivers. So the Sender has 
'uses' and 'provides' ports.
-Receiver which recieves asynchronous and synchronous calls from the Sender and replies them.
