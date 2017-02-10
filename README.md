These projects have been verified to build with platform toolset Visual Studio 2015 (v140).
For details about ODBCTest, https://msdn.microsoft.com/en-us/library/ms712676(v=vs.85).aspx
Happy coding!

The ODBCTest project property under "C/C++ - General - Additional Include Directories" is defined to point the "ODBC-Specification" git repo at the same root level as your ODBCTest git repo. Modify as necessary to point to where your "ODBC-Specification" repo is located.

ODBC 4.0 libs/dlls are not currently available in these repos at the moment. They will be made available upon authorized request. Please update The ODBCTest project property under "Linker - General - Additional Library Directories" to point to the ODBC 4.0 libs