The computer science club at Hudson County Community College is developing
a custom system for the college's tutoring center.

The system will automate many tasks which are currently being performed by the tutoring center staff, such as matching students with tutors, creating
reports, and bookkeeping.

You will need the following to compile the program:
- The Qt framework (we are using version 4.7.2)
- OpenSSL
- The Poco C++ Libraries

We also use the HighCharts JavaScript library to generate the graphical
reports.

Don't forget to fill in the details for the sender (email address and password) in the file requestfortutorialassistancedialog.cpp.
Later we will change this so that information is found in the settings
dialog.

Also, you will need the following dll files in the program's working directory:
PocoCrypto.dll
PocoFoundation.dll
PocoNet.dll
PocoNetSSL.dll
PocoUtil.dll
PocoXML.dll

These files can be found in the bin folder of the poco library. If you
are compiling the program in debug mode, you will need the debug version
of the same files. They can be identified by an extra d before .dll.
For example, PocoCrypto.dll becomes PocoCryptod.dll.