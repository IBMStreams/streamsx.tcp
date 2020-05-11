## Details about the com.ibm.streamsx.tcp toolkit and information about testing it

If you use Streams Studio for your development, then you can import the com.ibm.streamsx.tcp 
project and the TcpServerSample project into your workspace. When you import, both of those
projects will be built automatically. If needed, you can right-click on each of those projects and
click the "Build Active Configurations" option to force a build. If no active build configurations
exist, then you can create a Standalone launch configuration for both the TCPSender and
TCPReceiver composites. After that, you can test them by following these steps:

a) Right-click on the TCPReceiver-->Standalone build configuration and click Launch.
b) Right-click on the TCPSender-->Standalone build configuration and click Launch.

c) Both of them will run and you will see a message displayed by the TCPSender that the 
   application ended and the results are available in the data directory. If it is not
   already stopped, you can stop the sender program.
d) Now, you can stop the receiver.
e) You can verify the results by opening the result files created in the data directory.

## You are done verifying the functions of the com.ibm.streamsx.tcp toolkit.

If you don't use Streams Studio and if you use vi, emacs or any other text editor for your 
Streams application development, then you can build and test by following these steps: 

This package contains the following directories:

[All the directory references made below assume that your current directory is the
top-level folder of the repository i.e. streamsx.tcp-XXXXX]

1) com.ibm.streamsx.tcp: This is a toolkit directory that contains the
com.ibm.streamsx.tcp::TCPServer operator - a multi-connection, multi-threaded
source operator that accepts text or binary data from one or more TCP sockets.

Description about this TCPServer operator is well documented in the following web browser friendly HTML file:
com.ibm.streamsx.tcp/doc/spldoc/index.html

You can see this toolkit at work by running the included sample applications (see item 2 below).

To build the toolkit, type:
cd com.ibm.streamsx.tcp/
make

2) Sample: Directory containing test applications that illustrate the use of the TCPServer operator. 

To build the sample, type:
cd samples/TcpServerSample
make

This will build two different applications: 

a) TCPSender application which pumps data via different TCP sockets to be received by another application.
b) TCPReceiver application which invokes multiple TCPServer operator instances to receive the data sent from the sender application.

Start two Linux terminal windows.

At First, to run the TCP receiver application, type this on terminal window 1:
cd samples/TcpServerSample
./output/receiver/bin/standalone

After that, to run the TCP sender application, type this on terminal window 2: 
cd samples/TcpServerSample
./output/sender/bin/standalone 

You can see the results from these tests in the samples/TcpServerSample/data directory. You can stop
these applications after verifying the results by pressing Ctrl C in those terminal windows.
Browse the code for the test applications to understand how the TCPServer operator is used in different ways.

```
For any problems or questions about this toolkit, copy everyone below in an email to get a quick response:

This toolkit was originally designed and developed by Bugra Gedik (bgedik@gmail.com) 


This gem created by Bugra was modified later for enhancements by these members:

Vitali Zoubov  (vzoubov@us.ibm.com)
Senthil Nathan (sen@us.ibm.com)
Frank Scharffenberg (frank.scharffenberg@de.ibm.com)
Leonid Gorelik (leonid.gorelik@il.ibm.com)
```
