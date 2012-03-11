/* soapStub.h
   Generated by gSOAP 2.8.0 from DumpUploader.h
   Copyright(C) 2000-2010, Robert van Engelen, Genivia Inc. All Rights Reserved.
   The generated code is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapStub_H
#define soapStub_H
#include <vector>
#define SOAP_NAMESPACE_OF_ns1	"https://www.crash-server.com/"
#define SOAP_NAMESPACE_OF_ns2	"http://microsoft.com/wsdl/types/"
#include "stdsoap2.h"

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_ns1__Architecture
#define SOAP_TYPE_ns1__Architecture (24)
/* ns1:Architecture */
enum ns1__Architecture {ns1__Architecture__x32 = 0, ns1__Architecture__x64 = 1};
#endif

#ifndef SOAP_TYPE_ns1__ResponseType
#define SOAP_TYPE_ns1__ResponseType (25)
/* ns1:ResponseType */
enum ns1__ResponseType {ns1__ResponseType__HaveSolution = 0, ns1__ResponseType__NeedMiniDump = 1, ns1__ResponseType__NeedMoreInfo = 2, ns1__ResponseType__Stop = 3, ns1__ResponseType__Error = 4};
#endif

#ifndef SOAP_TYPE_ns1__SolutionType
#define SOAP_TYPE_ns1__SolutionType (26)
/* ns1:SolutionType */
enum ns1__SolutionType {ns1__SolutionType__Url = 0, ns1__SolutionType__Exe = 1};
#endif

#ifndef SOAP_TYPE_ns1__AdditionalInfoType
#define SOAP_TYPE_ns1__AdditionalInfoType (27)
/* ns1:AdditionalInfoType */
enum ns1__AdditionalInfoType {ns1__AdditionalInfoType__None = 0, ns1__AdditionalInfoType__FullDump = 1, ns1__AdditionalInfoType__Info = 2};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE__xop__Include
#define SOAP_TYPE__xop__Include (8)
/* Base64 schema type: */
struct _xop__Include
{
public:
	unsigned char *__ptr;
	int __size;
	char *id;	/* optional element of type xsd:string */
	char *type;	/* optional element of type xsd:string */
	char *options;	/* optional element of type xsd:string */
};
typedef struct _xop__Include _xop__Include;
#endif

#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE_ns1__ClientLib
#define SOAP_TYPE_ns1__ClientLib (12)
/* ns1:ClientLib */
class SOAP_CMAC ns1__ClientLib
{
public:
	unsigned short v1;	/* required element of type xsd:unsignedShort */
	unsigned short v2;	/* required element of type xsd:unsignedShort */
	unsigned short v3;	/* required element of type xsd:unsignedShort */
	unsigned short v4;	/* required element of type xsd:unsignedShort */
	enum ns1__Architecture arch;	/* required element of type ns1:Architecture */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 12; } /* = unique id SOAP_TYPE_ns1__ClientLib */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__ClientLib(): soap(NULL) { ns1__ClientLib::soap_default(NULL); }
	virtual ~ns1__ClientLib() { }
};
#endif

#ifndef SOAP_TYPE_ns1__Application
#define SOAP_TYPE_ns1__Application (13)
/* ns1:Application */
class SOAP_CMAC ns1__Application
{
public:
	std::wstring applicationGUID;	/* required element of type ns2:guid */
	unsigned short v1;	/* required element of type xsd:unsignedShort */
	unsigned short v2;	/* required element of type xsd:unsignedShort */
	unsigned short v3;	/* required element of type xsd:unsignedShort */
	unsigned short v4;	/* required element of type xsd:unsignedShort */
	unsigned short hotfix;	/* required element of type xsd:unsignedShort */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 13; } /* = unique id SOAP_TYPE_ns1__Application */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__Application(): soap(NULL) { ns1__Application::soap_default(NULL); }
	virtual ~ns1__Application() { }
};
#endif

#ifndef SOAP_TYPE_ns1__Response
#define SOAP_TYPE_ns1__Response (14)
/* ns1:Response */
class SOAP_CMAC ns1__Response
{
public:
	enum ns1__ResponseType type;	/* required element of type ns1:ResponseType */
	std::wstring *error;	/* optional element of type xsd:string */
	class ns1__Solution *solution;	/* optional element of type ns1:Solution */
	std::wstring *clientID;	/* optional element of type xsd:string */
	int problemID;	/* required element of type xsd:int */
	int dumpGroupID;	/* required element of type xsd:int */
	int dumpID;	/* required element of type xsd:int */
	std::wstring *urlToProblem;	/* optional element of type xsd:string */
	enum ns1__AdditionalInfoType infoType;	/* required element of type ns1:AdditionalInfoType */
	struct _xop__Include *infoModule;	/* optional element of type xop:Include */
	std::wstring *infoModuleCfg;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 14; } /* = unique id SOAP_TYPE_ns1__Response */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__Response(): error(NULL), solution(NULL), clientID(NULL), urlToProblem(NULL), infoModule(NULL), infoModuleCfg(NULL), soap(NULL) { ns1__Response::soap_default(NULL); }
	virtual ~ns1__Response() { }
};
#endif

#ifndef SOAP_TYPE_ns1__Solution
#define SOAP_TYPE_ns1__Solution (15)
/* ns1:Solution */
class SOAP_CMAC ns1__Solution
{
public:
	bool askConfirmation;	/* required element of type xsd:boolean */
	enum ns1__SolutionType type;	/* required element of type ns1:SolutionType */
	std::wstring *url;	/* optional element of type xsd:string */
	struct _xop__Include *exe;	/* optional element of type xop:Include */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 15; } /* = unique id SOAP_TYPE_ns1__Solution */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         ns1__Solution(): url(NULL), exe(NULL), soap(NULL) { ns1__Solution::soap_default(NULL); }
	virtual ~ns1__Solution() { }
};
#endif

#ifndef SOAP_TYPE__ns1__Hello
#define SOAP_TYPE__ns1__Hello (16)
/* ns1:Hello */
class SOAP_CMAC _ns1__Hello
{
public:
	ns1__ClientLib *client;	/* optional element of type ns1:ClientLib */
	ns1__Application *app;	/* optional element of type ns1:Application */
	std::wstring *mainModule;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 16; } /* = unique id SOAP_TYPE__ns1__Hello */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__Hello(): client(NULL), app(NULL), mainModule(NULL), soap(NULL) { _ns1__Hello::soap_default(NULL); }
	virtual ~_ns1__Hello() { }
};
#endif

#ifndef SOAP_TYPE__ns1__HelloResponse
#define SOAP_TYPE__ns1__HelloResponse (17)
/* ns1:HelloResponse */
class SOAP_CMAC _ns1__HelloResponse
{
public:
	ns1__Response *HelloResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type ns1:Response */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 17; } /* = unique id SOAP_TYPE__ns1__HelloResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__HelloResponse(): HelloResult(NULL), soap(NULL) { _ns1__HelloResponse::soap_default(NULL); }
	virtual ~_ns1__HelloResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadMiniDump
#define SOAP_TYPE__ns1__UploadMiniDump (18)
/* ns1:UploadMiniDump */
class SOAP_CMAC _ns1__UploadMiniDump
{
public:
	ns1__ClientLib *client;	/* optional element of type ns1:ClientLib */
	ns1__Application *app;	/* optional element of type ns1:Application */
	std::wstring *mainModule;	/* optional element of type xsd:string */
	int PCID;	/* required element of type xsd:int */
	int submitterID;	/* required element of type xsd:int */
	struct _xop__Include *dump;	/* optional element of type xop:Include */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 18; } /* = unique id SOAP_TYPE__ns1__UploadMiniDump */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadMiniDump(): client(NULL), app(NULL), mainModule(NULL), dump(NULL), soap(NULL) { _ns1__UploadMiniDump::soap_default(NULL); }
	virtual ~_ns1__UploadMiniDump() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadMiniDumpResponse
#define SOAP_TYPE__ns1__UploadMiniDumpResponse (19)
/* ns1:UploadMiniDumpResponse */
class SOAP_CMAC _ns1__UploadMiniDumpResponse
{
public:
	ns1__Response *UploadMiniDumpResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type ns1:Response */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 19; } /* = unique id SOAP_TYPE__ns1__UploadMiniDumpResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadMiniDumpResponse(): UploadMiniDumpResult(NULL), soap(NULL) { _ns1__UploadMiniDumpResponse::soap_default(NULL); }
	virtual ~_ns1__UploadMiniDumpResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadAdditionalInfo
#define SOAP_TYPE__ns1__UploadAdditionalInfo (20)
/* ns1:UploadAdditionalInfo */
class SOAP_CMAC _ns1__UploadAdditionalInfo
{
public:
	ns1__ClientLib *client;	/* optional element of type ns1:ClientLib */
	ns1__Application *app;	/* optional element of type ns1:Application */
	std::wstring *mainModule;	/* optional element of type xsd:string */
	int miniDumpID;	/* required element of type xsd:int */
	struct _xop__Include *info;	/* optional element of type xop:Include */
	enum ns1__AdditionalInfoType infoType;	/* required element of type ns1:AdditionalInfoType */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 20; } /* = unique id SOAP_TYPE__ns1__UploadAdditionalInfo */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadAdditionalInfo(): client(NULL), app(NULL), mainModule(NULL), info(NULL), soap(NULL) { _ns1__UploadAdditionalInfo::soap_default(NULL); }
	virtual ~_ns1__UploadAdditionalInfo() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadAdditionalInfoResponse
#define SOAP_TYPE__ns1__UploadAdditionalInfoResponse (21)
/* ns1:UploadAdditionalInfoResponse */
class SOAP_CMAC _ns1__UploadAdditionalInfoResponse
{
public:
	ns1__Response *UploadAdditionalInfoResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type ns1:Response */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 21; } /* = unique id SOAP_TYPE__ns1__UploadAdditionalInfoResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadAdditionalInfoResponse(): UploadAdditionalInfoResult(NULL), soap(NULL) { _ns1__UploadAdditionalInfoResponse::soap_default(NULL); }
	virtual ~_ns1__UploadAdditionalInfoResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadSymbol
#define SOAP_TYPE__ns1__UploadSymbol (22)
/* ns1:UploadSymbol */
class SOAP_CMAC _ns1__UploadSymbol
{
public:
	ns1__ClientLib *client;	/* optional element of type ns1:ClientLib */
	ns1__Application *app;	/* optional element of type ns1:Application */
	bool publish;	/* required element of type xsd:boolean */
	struct _xop__Include *sym;	/* optional element of type xop:Include */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 22; } /* = unique id SOAP_TYPE__ns1__UploadSymbol */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadSymbol(): client(NULL), app(NULL), sym(NULL), soap(NULL) { _ns1__UploadSymbol::soap_default(NULL); }
	virtual ~_ns1__UploadSymbol() { }
};
#endif

#ifndef SOAP_TYPE__ns1__UploadSymbolResponse
#define SOAP_TYPE__ns1__UploadSymbolResponse (23)
/* ns1:UploadSymbolResponse */
class SOAP_CMAC _ns1__UploadSymbolResponse
{
public:
	std::wstring *UploadSymbolResult;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 23; } /* = unique id SOAP_TYPE__ns1__UploadSymbolResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns1__UploadSymbolResponse(): UploadSymbolResult(NULL), soap(NULL) { _ns1__UploadSymbolResponse::soap_default(NULL); }
	virtual ~_ns1__UploadSymbolResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns3__Hello
#define SOAP_TYPE___ns3__Hello (43)
/* Operation wrapper: */
struct __ns3__Hello
{
public:
	_ns1__Hello *ns1__Hello;	/* optional element of type ns1:Hello */
};
#endif

#ifndef SOAP_TYPE___ns3__UploadMiniDump
#define SOAP_TYPE___ns3__UploadMiniDump (47)
/* Operation wrapper: */
struct __ns3__UploadMiniDump
{
public:
	_ns1__UploadMiniDump *ns1__UploadMiniDump;	/* optional element of type ns1:UploadMiniDump */
};
#endif

#ifndef SOAP_TYPE___ns3__UploadAdditionalInfo
#define SOAP_TYPE___ns3__UploadAdditionalInfo (51)
/* Operation wrapper: */
struct __ns3__UploadAdditionalInfo
{
public:
	_ns1__UploadAdditionalInfo *ns1__UploadAdditionalInfo;	/* optional element of type ns1:UploadAdditionalInfo */
};
#endif

#ifndef SOAP_TYPE___ns3__UploadSymbol
#define SOAP_TYPE___ns3__UploadSymbol (55)
/* Operation wrapper: */
struct __ns3__UploadSymbol
{
public:
	_ns1__UploadSymbol *ns1__UploadSymbol;	/* optional element of type ns1:UploadSymbol */
};
#endif

#ifndef SOAP_TYPE___ns4__Hello
#define SOAP_TYPE___ns4__Hello (57)
/* Operation wrapper: */
struct __ns4__Hello
{
public:
	_ns1__Hello *ns1__Hello;	/* optional element of type ns1:Hello */
};
#endif

#ifndef SOAP_TYPE___ns4__UploadMiniDump
#define SOAP_TYPE___ns4__UploadMiniDump (59)
/* Operation wrapper: */
struct __ns4__UploadMiniDump
{
public:
	_ns1__UploadMiniDump *ns1__UploadMiniDump;	/* optional element of type ns1:UploadMiniDump */
};
#endif

#ifndef SOAP_TYPE___ns4__UploadAdditionalInfo
#define SOAP_TYPE___ns4__UploadAdditionalInfo (61)
/* Operation wrapper: */
struct __ns4__UploadAdditionalInfo
{
public:
	_ns1__UploadAdditionalInfo *ns1__UploadAdditionalInfo;	/* optional element of type ns1:UploadAdditionalInfo */
};
#endif

#ifndef SOAP_TYPE___ns4__UploadSymbol
#define SOAP_TYPE___ns4__UploadSymbol (63)
/* Operation wrapper: */
struct __ns4__UploadSymbol
{
public:
	_ns1__UploadSymbol *ns1__UploadSymbol;	/* optional element of type ns1:UploadSymbol */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (64)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (65)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (67)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
	char *__any;
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (70)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (71)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE_ns2__guid
#define SOAP_TYPE_ns2__guid (29)
typedef std::wstring ns2__guid;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */
