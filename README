# Fuegonet

Feugonet is an Open Source projects which provide the abilities to publish the server status or a specified API status and availability.
The Goal of the project is to replace the conservative EAI server with a Smart and Easy Mechanism which removes the Single point of failure the current Solution has.
The way that Fuegonet works is based on DDS or a Publisher/subscriber mechanism which uses multicast to send a string of information over UDP.
With this method Fuegonet is able to send the status over Multicast to all the server (distinguish by group ) that needs to update  the API it provides along with the Server hardware status which enable to client (subscriber) to receive the expected information from multiple servers and mathematically decide which Server is must suitable to serve it.
After the API Server is chosen then the API sends the information to the fuegonet service (subscriber)  on it’s own Server then a binary transfer send the information to the designated Server (publisher) and the fuegonet Service on the receiver ends sends the information to the relevant API.

## For Example
In our example the Organization has 1 ERP API servers , 1 Billing Server and 2 CRM API Servers.
In fuegonet each API will have it’s own Fuegonet Group except the CRM API Server which are in the same group.
All Server are Publishers of their hosted Service and are subscribers to all the other Services so when the ERP wants to sends a Message to the Billing Server it sends the data to it’s fuegonet service , the service sends the data using binary transfer to the fuegonet client on the Billing Server and then to the Billing API.
Is a scenario when the RRP wants to sends data to one of the CRM Servers , it sends the data to it’s feugonet Service, the service knows which CRM Server is more available and sends the data to it’s feugonet service over binary transfer. Once the CRM fuegonet receive the data , it send it to the CRM API.


## Getting Started


everything that fuegonet needs is in the glibc package

### Installing

Now lets download and install fuegonet


    $ git clone https://github.com/ooichman/fuegonet.git
    $ cd fuegonet
    $ ./configure
    $ make
    $ make install

- On Redhat/Centos/Fedora you can use the fuegonet.spec file by tar the directory and rpmbuild it:
    $ tar -zcvf 0.1.0.tar.gz fuegonet/ && rpmbuild -ta 0.1.0.tar.gz
 
now you have a service daemon you need to run named **fuegonetd** and a set of tools to
test your multicast/unicast network configuration.
