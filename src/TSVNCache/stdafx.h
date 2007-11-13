// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WIN32_IE 0x600
#define _WIN32_WINNT 0x0600

#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <WinSock2.h>

#include <atlbase.h>
#include <atlstr.h>

#include <conio.h>

#define CSTRING_AVAILABLE


using namespace ATL;

#pragma warning(push)
#pragma warning(disable: 4702)	// Unreachable code warnings in xtree
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <deque>
#pragma warning(pop)

#include "svn_wc.h"
#include "svn_client.h"
#include "svn_path.h"
#include "svn_pools.h"
#include "svn_utf.h"


typedef CComCritSecLock<CComAutoCriticalSection> AutoLocker;

// Temporary fix for people not using the latest SDK
#ifndef PROCESS_MODE_BACKGROUND_BEGIN
#define PROCESS_MODE_BACKGROUND_BEGIN 0x00100000
#endif
