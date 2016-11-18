//*---------------------------------------------------------------------------------
//| ODBC Test Tool
//|		Copyright (c) Microsoft, All rights reserved
//|
//| Title:	PARAMIDX.H
//|
//| Purpose:
//|		This file contains constants that represent the parameter number of
//|	a particular field in dlgtmplt.c and usrwndp.c
//*---------------------------------------------------------------------------------
#ifndef PARAMIDX_DEFS
#define PARAMIDX_DEFS


// SQLBindParameter
#define IPAR_BP_HSTMT					0
#define IPAR_BP_IPAR						1
#define IPAR_BP_FPARAMTYPE				2
#define IPAR_BP_FCTYPE					3
#define IPAR_BP_FSQLTYPE				4
#define IPAR_BP_CBCOLDEF				5
#define IPAR_BP_IBSCALE					6
#define IPAR_BP_RGBVALUE				7
#define IPAR_BP_CBVALUEMAX				8
#define IPAR_BP_PCBVALUE				9
#define IPAR_BP_DAE						10
#define IPAR_BP_RS_HDBC					11
#define IPAR_BP_RS_HSTMT				12
#define IPAR_BP_COLUMN					13
#define IPAR_BP_ROW						14
#define IPAR_BP_BUFFERSIZE				15

// SQLBindParam
#define IPAR_BPARAM_HSTMT					0
#define IPAR_BPARAM_IPAR					1
#define IPAR_BPARAM_FCTYPE					2
#define IPAR_BPARAM_FSQLTYPE				3
#define IPAR_BPARAM_CBCOLDEF				4
#define IPAR_BPARAM_IBSCALE				5
#define IPAR_BPARAM_RGBVALUE				6
#define IPAR_BPARAM_PCBVALUE				7
#define IPAR_BPARAM_DAE						8
#define IPAR_BPARAM_RS_HDBC				9
#define IPAR_BPARAM_RS_HSTMT				10
#define IPAR_BPARAM_COLUMN					11
#define IPAR_BPARAM_ROW						12
#define IPAR_BPARAM_BUFFERSIZE			13
#define IPAR_BPARAM_FPARAMTYPE			-1
#define IPAR_BPARAM_CBVALUEMAX			-1

// SQLGetInfo
#define IPAR_GI_HSTMT						0
#define IPAR_GI_FINFOTYPE					1
#define IPAR_GI_RGBINFOVALUE				2
#define IPAR_GI_CBINFOVALUEMAX			3
#define IPAR_GI_PCBINFOVALUE				4
#define IPAR_GI_INFORMATION_VALUE_TYPE	5


// SQLColAttributes
#define IPAR_CAS_HSTMT						0
#define IPAR_CAS_ICOL						1
#define IPAR_CAS_FDESCTYPE					2
#define IPAR_CAS_RGBDESC					3
#define IPAR_CAS_CBDESCMAX					4
#define IPAR_CAS_PCBDESC					5
#define IPAR_CAS_PFDESC						6
#define IPAR_CAS_DESCRIPTION_TYPE		7

// SQLColAttribute
#define IPAR_CA_HSTMT							0
#define IPAR_CA_ICOLUMNNUMBER					1
#define IPAR_CA_FFIELDINDENTIFIER			2
#define IPAR_CA_RGBCHARACTERATTRIBUTE		3
#define IPAR_CA_CBBUFFERLENGTH				4
#define IPAR_CA_PCBSTRINGLENGTH				5
#define IPAR_CA_PVNUMERICATTRIBUTE			6
#define IPAR_CA_FFIELDINDENTIFIER_TYPE		7

// SQLGetConnectOption
#define IPAR_GCO_HDBC							0
#define IPAR_GCO_FOPTION						1
#define IPAR_GCO_PVPARAM						2
#define IPAR_GCO_PARAMETER_TYPE				3

// SQLSetConnectOption
#define IPAR_SCO_HDBC							0
#define IPAR_SCO_FOPTION						1
#define IPAR_SCO_VPARAM							2
#define IPAR_SCO_PARAMETER_TYPE				3

// SQLGetConnectAttr
#define IPAR_GCA_HDBC							0
#define IPAR_GCA_FATTRIBUTE					1
#define IPAR_GCA_RGBVALUE						2
#define IPAR_GCA_CBVALUE						3
#define IPAR_GCA_PCBVALUE						4
#define IPAR_GCA_PARAMETER_TYPE				5

// SQLSetConnectAttr
#define IPAR_SCA_HDBC							0
#define IPAR_SCA_FATTRIBUTE					1
#define IPAR_SCA_RGBVALUE						2
#define IPAR_SCA_CBVALUE						3
#define IPAR_SCA_PARAMETER_TYPE				4

// SQLGetDescField
#define IPAR_GDF_HDESC							0
#define IPAR_GDF_IRECNUM						1
#define IPAR_GDF_IFIELDIDENTIFIER			2
#define IPAR_GDF_RGBVALUE						3
#define IPAR_GDF_CBVALUEMAX					4
#define IPAR_GDF_PCBVALUE						5
#define IPAR_GDF_IFIELDIDENTIFIER_TYPE		6

// SQLSetDescField
#define IPAR_SDF_HDESC							0
#define IPAR_SDF_IRECNUM						1
#define IPAR_SDF_IFIELDIDENTIFIER			2
#define IPAR_SDF_RGBVALUE						3
#define IPAR_SDF_CBBUFFERLENGTH				4
#define IPAR_SDF_IFIELDIDENTIFIER_TYPE		5

// SQLGetDescRec
#define IPAR_GDR_HDESC							0
#define IPAR_GDR_IRECNUM						1
#define IPAR_GDR_SZNAME							2
#define IPAR_GDR_CBNAME							3
#define IPAR_GDR_PCBNAME						4
#define IPAR_GDR_FTYPE							5
#define IPAR_GDR_FSUBTYPE						6
#define IPAR_GDR_PCOLDEF						7
#define IPAR_GDR_PPRECISION					8
#define IPAR_GDR_PSCALE							9
#define IPAR_GDR_PFNULLABLE					10

// SQLSetDescRec
#define IPAR_SDR_HDESC							0
#define IPAR_SDR_IRECNUM						1
#define IPAR_SDR_FTYPE							2
#define IPAR_SDR_FSUBTYPE						3
#define IPAR_SDR_CBLENGTH						4
#define IPAR_SDR_CBCOLDEF						5
#define IPAR_SDR_IBSCALE						6
#define IPAR_SDR_RGBVALUE						7
#define IPAR_SDR_PCBSTRINGLENGTH				8
#define IPAR_SDR_PFINDICATOR					9

// SQLGetDiagField
#define IPAR_GDF_FHANDLETYPE					0
#define IPAR_GDF_HHANDLE						1
#define IPAR_GDF_IRECRECNUMBER				2
#define IPAR_GDF_FDIAGIDENTIFIER				3
#define IPAR_GDF_RGBDIAGINFO					4
#define IPAR_GDF_CBBUFFERLENGTH				5
#define IPAR_GDF_PCBSTRINGLENGTH				6
#define IPAR_GDF_FDIAGIDENTIFIER_TYPE		7

#define IPAR_GDR_FHANDLETYPE					0
#define IPAR_GDR_HHANDLE						1
#define IPAR_GDR_IRECRECNUMBER				2
#define IPAR_GDR_SZSQLSTATE					3
#define IPAR_GDR_PFNATIVEERROR				4
#define IPAR_GDR_SZMESSAGETEXT				5
#define IPAR_GDR_CBBUFFERLENGTH				6
#define IPAR_GDR_PCBTEXTLENGTH				7

// SQLEndTran
#define IPAR_ET_FHANDLETYPE					0
#define IPAR_ET_HHANDLE							1
#define IPAR_ET_FCOMPLETIONTYPE				2

// SQLFetchScroll
#define IPAR_FS_HSTMT							0
#define IPAR_FS_FFETCHORIENTATION			1
#define IPAR_FS_FFETCHOFFSET					2
#define IPAR_FS_DISPLAY_ROWSET				3

// SQLFetch
#define IPAR_F_HSTMT								0
#define IPAR_F_DISPLAY_ROWSET					1

// SQLExtendedFetch
#define IPAR_EF_HSTMT							0
#define IPAR_EF_FFETCHTYPE						1
#define IPAR_EF_IROW								2
#define IPAR_EF_PCROW							3
#define IPAR_EF_RGFROWSTATUS					4
#define IPAR_EF_DISPLAY_ROW					5

// SQLCloseCursor
#define IPAR_CC_HSTMT							0

// SQLFreeHandle
#define IPAR_FH_FHANDLETYPE					0
#define IPAR_FH_INPUTHANDLE					1

// SQLGetEnvAttr
#define IPAR_GEA_HENV							0
#define IPAR_GEA_FATTRIBUTE					1
#define IPAR_GEA_RGBVALUE						2
#define IPAR_GEA_CBVALUE						3
#define IPAR_GEA_PCBVALUE						4
#define IPAR_GEA_FATTRIBUTE_TYPE				5

// SQLSetEnvAttr
#define IPAR_SEA_HENV							0
#define IPAR_SEA_FATTRIBUTE					1
#define IPAR_SEA_RGBVALUE						2
#define IPAR_SEA_CBVALUE						3
#define IPAR_SEA_FATTRIBUTE_TYPE				4

// SQLSetStmtAttr
#define IPAR_SSA_HSTMT							0
#define IPAR_SSA_FATTRIBUTE					1
#define IPAR_SSA_RGBVALUE						2
#define IPAR_SSA_CBVALUE						3
#define IPAR_SSA_PARAMETER_TYPE				4

// SQLGetStmtAttr
#define IPAR_GSA_HSTMT							0
#define IPAR_GSA_FATTRIBUTE					1
#define IPAR_GSA_RGBVALUE						2
#define IPAR_GSA_CBVALUE						3
#define IPAR_GSA_PCBVALUE						4
#define IPAR_GSA_PARAMETER_TYPE				5

// SQLSetStmtOption
#define IPAR_SSO_HSTMT						  0
#define IPAR_SSO_FOPTION					  1
#define IPAR_SSO_VPARAM						  2
#define IPAR_SSO_PARAMETER_TYPE			  3

// SQLGetStmtOption
#define IPAR_GSO_HSTMT						  0
#define IPAR_GSO_FOPTION					  1
#define IPAR_GSO_PVPARAM					  2
#define IPAR_GSO_PARAMETER_TYPE			  3

// SQLBulkOperations
#define IPAR_BO_HSTMT							0
#define IPAR_BO_OPERATION						1

#endif

