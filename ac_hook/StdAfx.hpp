#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include "atlbase.h"
#include "Psapi.h"
#include <winternl.h>
#include <tlhelp32.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include "Shlwapi.h"

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std;

#include "NtDll.hpp"
#include "Core.hpp"
#include "Scan.hpp"
#include "WMI.hpp"