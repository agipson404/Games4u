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
#include <gdiplus.h>
#include <atlimage.h>
#include <string>

#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

#include "Core.hpp"
#include "Loader.hpp"
#include "Screenshot.hpp"