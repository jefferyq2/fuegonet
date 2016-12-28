# Fuegonet

Feugonet is an Open Source projects which provide your Servers the ability to share System and service information status
with other servers which enables multi to multi load balancing 

## Getting Started


everything that fuegonet needs is in the glibc package

### Installing

Now lets download and install fuegonet


    $ git clone https://github.com/ooichman/fuegonet.git
    $ cd fuegonet
    $ make
    $ make install

- On Redhat/Centos/Fedora you can use the fuegonet.spec file by tar the directory and rpmbuild it:
    $ tar -zcvf 0.1.0.tar.gz fuegonet/ && rpmbuild -ta 0.1.0.tar.gz
 
now you have a service daemon you need to run named **fuegonetd** and a set of tools to
test your multicast/unicast network configuration.

