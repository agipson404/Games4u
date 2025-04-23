#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include "atlbase.h"
#include "Psapi.h"
#include <tlhelp32.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include "Shlwapi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <vector>
#include <time.h>
#include <map>
#include <Shlobj.h>
#include <bcrypt.h>
#include <BaseTsd.h>
#include <Dbghelp.h>
#include <io.h>
#include <fcntl.h>
#include <algorithm>
#include <fstream>
#include <wininet.h>
#include <winnt.h>

#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
#pragma warning (disable: 4244)
#pragma warning (disable: 4267)
#pragma warning (disable: 4566)

#include "nt_ddk.hpp"
#include "apiset.hpp"
#include "Core.hpp"
#include "Utilities.hpp"
#include "Cryption.hpp"
#include "Loader.hpp"
#include "Network.hpp"
#include "Client.hpp"
#include "TCPClient.hpp"

