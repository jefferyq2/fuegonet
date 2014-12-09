fuegonet
========

the fuego-net is an Open Source project which is an implementation
of the dtpp (Data Transaction Priority Protocal) for sending
information between the applications API

what does it do ?
=================

the application provides a backbone for sending data between any
API without a central bottleneck server by talking to the API 
locally and sending the data in binary transfer over the network
to the destination fuegonet which recieve the data will locally 
send the data to it's APIs in speed and high quality with the 
ability to priorities the destination and send the data to the 
fastest API. 

fuegonet components 
===================

there are 3 components for the application:

1) the udp layer which provides a realtime information about the 
   application members.

2) the tcp layer which is the message carier over the network in
   a peer to peer mode to the destination server.

3) the library stack which enable the application to use the 
   fuegonet ability to their use.

Priority control
================

each massage with in the application has it's own priority which
can be set by the sender API which will tell the client to give a
higher priority to this message over the others in the queue is
this option is set

the priority also allow the ability to built up more the one API 
where all the APIs are equal without the need of them knowing 
about each other (without cluster awere).

The fuego-net application is writen in C and it is protected by
the GPLv2 Open Source License.
