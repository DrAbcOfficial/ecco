#include <string>
#include <filesystem>

#include "tcl_loader.h"
#include "tcl_dynamic.h"
#undef DLLEXPORT

#include <extdll.h>
#include <meta_api.h>
#include "meta_utility.h"


#ifdef _WIN32
#define GET_PROC_ADDR GetProcAddress
#define LIB_HANDLE HMODULE
#define LOAD_LIB(path) LoadLibraryA((path).c_str())
#define FREE_LIB FreeLibrary
#else
#define GET_PROC_ADDR dlsym
#define LIB_HANDLE void*
#define LOAD_LIB(path) dlopen((path).c_str(), RTLD_NOW)
#define FREE_LIB dlclose
#endif


static LIB_HANDLE g_TclLibHandle = nullptr;
static LIB_HANDLE g_Zlib1Handle = nullptr;
static LIB_HANDLE g_libtommathHandle = nullptr;

// 辅助宏
#define LOAD_TCL_API(name) \
    name = (decltype(name))GET_PROC_ADDR(g_TclLibHandle, #name);

#pragma region Vars
// Slot 0
PFN_Tcl_PkgProvideEx Tcl_PkgProvideEx;

// Slot 1
PFN_Tcl_PkgRequireEx Tcl_PkgRequireEx;

// Slot 2
PFN_Tcl_Panic Tcl_Panic;

// Slot 3
PFN_Tcl_Alloc Tcl_Alloc;

// Slot 4
PFN_Tcl_Free Tcl_Free;

// Slot 5
PFN_Tcl_Realloc Tcl_Realloc;

// Slot 6
PFN_Tcl_DbCkalloc Tcl_DbCkalloc;

// Slot 7
PFN_Tcl_DbCkfree Tcl_DbCkfree;

// Slot 8
PFN_Tcl_DbCkrealloc Tcl_DbCkrealloc;

// Slot 9
PFN_Tcl_CreateFileHandler Tcl_CreateFileHandler;

// Slot 10
PFN_Tcl_DeleteFileHandler Tcl_DeleteFileHandler;

// Slot 11
PFN_Tcl_SetTimer Tcl_SetTimer;

// Slot 12
PFN_Tcl_Sleep Tcl_Sleep;

// Slot 13
PFN_Tcl_WaitForEvent Tcl_WaitForEvent;

// Slot 14
PFN_Tcl_AppendAllObjTypes Tcl_AppendAllObjTypes;

// Slot 15
PFN_Tcl_AppendStringsToObj Tcl_AppendStringsToObj;

// Slot 16
PFN_Tcl_AppendToObj Tcl_AppendToObj;

// Slot 17
PFN_Tcl_ConcatObj Tcl_ConcatObj;

// Slot 18
PFN_Tcl_ConvertToType Tcl_ConvertToType;

// Slot 19
PFN_Tcl_DbDecrRefCount Tcl_DbDecrRefCount;

// Slot 20
PFN_Tcl_DbIncrRefCount Tcl_DbIncrRefCount;

// Slot 21
PFN_Tcl_DbIsShared Tcl_DbIsShared;

// Slot 22 is reserved

// Slot 23
PFN_Tcl_DbNewByteArrayObj Tcl_DbNewByteArrayObj;

// Slot 24
PFN_Tcl_DbNewDoubleObj Tcl_DbNewDoubleObj;

// Slot 25
PFN_Tcl_DbNewListObj Tcl_DbNewListObj;

// Slot 26 is reserved

// Slot 27
PFN_Tcl_DbNewObj Tcl_DbNewObj;

// Slot 28
PFN_Tcl_DbNewStringObj Tcl_DbNewStringObj;

// Slot 29
PFN_Tcl_DuplicateObj Tcl_DuplicateObj;

// Slot 30
PFN_TclFreeObj TclFreeObj;

// Slot 31 is reserved
// Slot 32 is reserved
// Slot 33 is reserved

// Slot 34
PFN_Tcl_GetDouble Tcl_GetDouble;

// Slot 35
PFN_Tcl_GetDoubleFromObj Tcl_GetDoubleFromObj;

// Slot 36 is reserved

// Slot 37
PFN_Tcl_GetInt Tcl_GetInt;

// Slot 38
PFN_Tcl_GetIntFromObj Tcl_GetIntFromObj;

// Slot 39
PFN_Tcl_GetLongFromObj Tcl_GetLongFromObj;

// Slot 40
PFN_Tcl_GetObjType Tcl_GetObjType;

// Slot 41
PFN_TclGetStringFromObj TclGetStringFromObj;

// Slot 42
PFN_Tcl_InvalidateStringRep Tcl_InvalidateStringRep;

// Slot 43
PFN_Tcl_ListObjAppendList Tcl_ListObjAppendList;

// Slot 44
PFN_Tcl_ListObjAppendElement Tcl_ListObjAppendElement;

// Slot 45
PFN_TclListObjGetElements TclListObjGetElements;

// Slot 46
PFN_Tcl_ListObjIndex Tcl_ListObjIndex;

// Slot 47
PFN_TclListObjLength TclListObjLength;

// Slot 48
PFN_Tcl_ListObjReplace Tcl_ListObjReplace;

// Slot 49 is reserved

// Slot 50
PFN_Tcl_NewByteArrayObj Tcl_NewByteArrayObj;

// Slot 51
PFN_Tcl_NewDoubleObj Tcl_NewDoubleObj;

// Slot 52 is reserved

// Slot 53
PFN_Tcl_NewListObj Tcl_NewListObj;

// Slot 54 is reserved

// Slot 55
PFN_Tcl_NewObj Tcl_NewObj;

// Slot 56
PFN_Tcl_NewStringObj Tcl_NewStringObj;

// Slot 57 is reserved

// Slot 58
PFN_Tcl_SetByteArrayLength Tcl_SetByteArrayLength;

// Slot 59
PFN_Tcl_SetByteArrayObj Tcl_SetByteArrayObj;

// Slot 60
PFN_Tcl_SetDoubleObj Tcl_SetDoubleObj;

// Slot 61 is reserved

// Slot 62
PFN_Tcl_SetListObj Tcl_SetListObj;

// Slot 63 is reserved

// Slot 64
PFN_Tcl_SetObjLength Tcl_SetObjLength;

// Slot 65
PFN_Tcl_SetStringObj Tcl_SetStringObj;

// Slot 66 is reserved
// Slot 67 is reserved

// Slot 68
PFN_Tcl_AllowExceptions Tcl_AllowExceptions;

// Slot 69
PFN_Tcl_AppendElement Tcl_AppendElement;

// Slot 70
PFN_Tcl_AppendResult Tcl_AppendResult;

// Slot 71
PFN_Tcl_AsyncCreate Tcl_AsyncCreate;

// Slot 72
PFN_Tcl_AsyncDelete Tcl_AsyncDelete;

// Slot 73
PFN_Tcl_AsyncInvoke Tcl_AsyncInvoke;

// Slot 74
PFN_Tcl_AsyncMark Tcl_AsyncMark;

// Slot 75
PFN_Tcl_AsyncReady Tcl_AsyncReady;

// Slot 76 is reserved
// Slot 77 is reserved

// Slot 78
PFN_Tcl_BadChannelOption Tcl_BadChannelOption;

// Slot 79
PFN_Tcl_CallWhenDeleted Tcl_CallWhenDeleted;

// Slot 80
PFN_Tcl_CancelIdleCall Tcl_CancelIdleCall;

// Slot 81 is reserved

// Slot 82
PFN_Tcl_CommandComplete Tcl_CommandComplete;

// Slot 83
PFN_Tcl_Concat Tcl_Concat;

// Slot 84
PFN_Tcl_ConvertElement Tcl_ConvertElement;

// Slot 85
PFN_Tcl_ConvertCountedElement Tcl_ConvertCountedElement;

// Slot 86
PFN_Tcl_CreateAlias Tcl_CreateAlias;

// Slot 87
PFN_Tcl_CreateAliasObj Tcl_CreateAliasObj;

// Slot 88
PFN_Tcl_CreateChannel Tcl_CreateChannel;

// Slot 89
PFN_Tcl_CreateChannelHandler Tcl_CreateChannelHandler;

// Slot 90
PFN_Tcl_CreateCloseHandler Tcl_CreateCloseHandler;

// Slot 91
PFN_Tcl_CreateCommand Tcl_CreateCommand;

// Slot 92
PFN_Tcl_CreateEventSource Tcl_CreateEventSource;

// Slot 93
PFN_Tcl_CreateExitHandler Tcl_CreateExitHandler;

// Slot 94
PFN_Tcl_CreateInterp Tcl_CreateInterp;

// Slot 95 is reserved

// Slot 96
PFN_Tcl_CreateObjCommand Tcl_CreateObjCommand;

// Slot 97
PFN_Tcl_CreateChild Tcl_CreateChild;

// Slot 98
PFN_Tcl_CreateTimerHandler Tcl_CreateTimerHandler;

// Slot 99
PFN_Tcl_CreateTrace Tcl_CreateTrace;

// Slot 100
PFN_Tcl_DeleteAssocData Tcl_DeleteAssocData;

// Slot 101
PFN_Tcl_DeleteChannelHandler Tcl_DeleteChannelHandler;

// Slot 102
PFN_Tcl_DeleteCloseHandler Tcl_DeleteCloseHandler;

// Slot 103
PFN_Tcl_DeleteCommand Tcl_DeleteCommand;

// Slot 104
PFN_Tcl_DeleteCommandFromToken Tcl_DeleteCommandFromToken;

// Slot 105
PFN_Tcl_DeleteEvents Tcl_DeleteEvents;

// Slot 106
PFN_Tcl_DeleteEventSource Tcl_DeleteEventSource;

// Slot 107
PFN_Tcl_DeleteExitHandler Tcl_DeleteExitHandler;

// Slot 108
PFN_Tcl_DeleteHashEntry Tcl_DeleteHashEntry;

// Slot 109
PFN_Tcl_DeleteHashTable Tcl_DeleteHashTable;

// Slot 110
PFN_Tcl_DeleteInterp Tcl_DeleteInterp;

// Slot 111
PFN_Tcl_DetachPids Tcl_DetachPids;

// Slot 112
PFN_Tcl_DeleteTimerHandler Tcl_DeleteTimerHandler;

// Slot 113
PFN_Tcl_DeleteTrace Tcl_DeleteTrace;

// Slot 114
PFN_Tcl_DontCallWhenDeleted Tcl_DontCallWhenDeleted;

// Slot 115
PFN_Tcl_DoOneEvent Tcl_DoOneEvent;

// Slot 116
PFN_Tcl_DoWhenIdle Tcl_DoWhenIdle;

// Slot 117
PFN_Tcl_DStringAppend Tcl_DStringAppend;

// Slot 118
PFN_Tcl_DStringAppendElement Tcl_DStringAppendElement;

// Slot 119
PFN_Tcl_DStringEndSublist Tcl_DStringEndSublist;

// Slot 120
PFN_Tcl_DStringFree Tcl_DStringFree;

// Slot 121
PFN_Tcl_DStringGetResult Tcl_DStringGetResult;

// Slot 122
PFN_Tcl_DStringInit Tcl_DStringInit;

// Slot 123
PFN_Tcl_DStringResult Tcl_DStringResult;

// Slot 124
PFN_Tcl_DStringSetLength Tcl_DStringSetLength;

// Slot 125
PFN_Tcl_DStringStartSublist Tcl_DStringStartSublist;

// Slot 126
PFN_Tcl_Eof Tcl_Eof;

// Slot 127
PFN_Tcl_ErrnoId Tcl_ErrnoId;

// Slot 128
PFN_Tcl_ErrnoMsg Tcl_ErrnoMsg;

// Slot 129 is reserved

// Slot 130
PFN_Tcl_EvalFile Tcl_EvalFile;

// Slot 131 is reserved

// Slot 132
PFN_Tcl_EventuallyFree Tcl_EventuallyFree;

// Slot 133
PFN_Tcl_Exit Tcl_Exit;

// Slot 134
PFN_Tcl_ExposeCommand Tcl_ExposeCommand;

// Slot 135
PFN_Tcl_ExprBoolean Tcl_ExprBoolean;

// Slot 136
PFN_Tcl_ExprBooleanObj Tcl_ExprBooleanObj;

// Slot 137
PFN_Tcl_ExprDouble Tcl_ExprDouble;

// Slot 138
PFN_Tcl_ExprDoubleObj Tcl_ExprDoubleObj;

// Slot 139
PFN_Tcl_ExprLong Tcl_ExprLong;

// Slot 140
PFN_Tcl_ExprLongObj Tcl_ExprLongObj;

// Slot 141
PFN_Tcl_ExprObj Tcl_ExprObj;

// Slot 142
PFN_Tcl_ExprString Tcl_ExprString;

// Slot 143
PFN_Tcl_Finalize Tcl_Finalize;

// Slot 144 is reserved

// Slot 145
PFN_Tcl_FirstHashEntry Tcl_FirstHashEntry;

// Slot 146
PFN_Tcl_Flush Tcl_Flush;

// Slot 147 is reserved
// Slot 148 is reserved

// Slot 149
PFN_TclGetAliasObj TclGetAliasObj;

// Slot 150
PFN_Tcl_GetAssocData Tcl_GetAssocData;

// Slot 151
PFN_Tcl_GetChannel Tcl_GetChannel;

// Slot 152
PFN_Tcl_GetChannelBufferSize Tcl_GetChannelBufferSize;

// Slot 153
PFN_Tcl_GetChannelHandle Tcl_GetChannelHandle;

// Slot 154
PFN_Tcl_GetChannelInstanceData Tcl_GetChannelInstanceData;

// Slot 155
PFN_Tcl_GetChannelMode Tcl_GetChannelMode;

// Slot 156
PFN_Tcl_GetChannelName Tcl_GetChannelName;

// Slot 157
PFN_Tcl_GetChannelOption Tcl_GetChannelOption;

// Slot 158
PFN_Tcl_GetChannelType Tcl_GetChannelType;

// Slot 159
PFN_Tcl_GetCommandInfo Tcl_GetCommandInfo;

// Slot 160
PFN_Tcl_GetCommandName Tcl_GetCommandName;

// Slot 161
PFN_Tcl_GetErrno Tcl_GetErrno;

// Slot 162
PFN_Tcl_GetHostName Tcl_GetHostName;

// Slot 163
PFN_Tcl_GetInterpPath Tcl_GetInterpPath;

// Slot 164
PFN_Tcl_GetParent Tcl_GetParent;

// Slot 165
PFN_Tcl_GetNameOfExecutable Tcl_GetNameOfExecutable;

// Slot 166
PFN_Tcl_GetObjResult Tcl_GetObjResult;

// Slot 167
PFN_Tcl_GetOpenFile Tcl_GetOpenFile;

// Slot 168
PFN_Tcl_GetPathType Tcl_GetPathType;

// Slot 169
PFN_Tcl_Gets Tcl_Gets;

// Slot 170
PFN_Tcl_GetsObj Tcl_GetsObj;

// Slot 171
PFN_Tcl_GetServiceMode Tcl_GetServiceMode;

// Slot 172
PFN_Tcl_GetChild Tcl_GetChild;

// Slot 173
PFN_Tcl_GetStdChannel Tcl_GetStdChannel;

// Slot 174 is reserved
// Slot 175 is reserved

// Slot 176
PFN_Tcl_GetVar2 Tcl_GetVar2;

// Slot 177 is reserved
// Slot 178 is reserved

// Slot 179
PFN_Tcl_HideCommand Tcl_HideCommand;

// Slot 180
PFN_Tcl_Init Tcl_Init;

// Slot 181
PFN_Tcl_InitHashTable Tcl_InitHashTable;

// Slot 182
PFN_Tcl_InputBlocked Tcl_InputBlocked;

// Slot 183
PFN_Tcl_InputBuffered Tcl_InputBuffered;

// Slot 184
PFN_Tcl_InterpDeleted Tcl_InterpDeleted;

// Slot 185
PFN_Tcl_IsSafe Tcl_IsSafe;

// Slot 186
PFN_Tcl_JoinPath Tcl_JoinPath;

// Slot 187
PFN_Tcl_LinkVar Tcl_LinkVar;

// Slot 188 is reserved

// Slot 189
PFN_Tcl_MakeFileChannel Tcl_MakeFileChannel;

// Slot 190 is reserved

// Slot 191
PFN_Tcl_MakeTcpClientChannel Tcl_MakeTcpClientChannel;

// Slot 192
PFN_Tcl_Merge Tcl_Merge;

// Slot 193
PFN_Tcl_NextHashEntry Tcl_NextHashEntry;

// Slot 194
PFN_Tcl_NotifyChannel Tcl_NotifyChannel;

// Slot 195
PFN_Tcl_ObjGetVar2 Tcl_ObjGetVar2;

// Slot 196
PFN_Tcl_ObjSetVar2 Tcl_ObjSetVar2;

// Slot 197
PFN_Tcl_OpenCommandChannel Tcl_OpenCommandChannel;

// Slot 198
PFN_Tcl_OpenFileChannel Tcl_OpenFileChannel;

// Slot 199
PFN_Tcl_OpenTcpClient Tcl_OpenTcpClient;

// Slot 200
PFN_Tcl_OpenTcpServer Tcl_OpenTcpServer;

// Slot 201
PFN_Tcl_Preserve Tcl_Preserve;

// Slot 202
PFN_Tcl_PrintDouble Tcl_PrintDouble;

// Slot 203
PFN_Tcl_PutEnv Tcl_PutEnv;

// Slot 204
PFN_Tcl_PosixError Tcl_PosixError;

// Slot 205
PFN_Tcl_QueueEvent Tcl_QueueEvent;

// Slot 206
PFN_Tcl_Read Tcl_Read;

// Slot 207
PFN_Tcl_ReapDetachedProcs Tcl_ReapDetachedProcs;

// Slot 208
PFN_Tcl_RecordAndEval Tcl_RecordAndEval;

// Slot 209
PFN_Tcl_RecordAndEvalObj Tcl_RecordAndEvalObj;

// Slot 210
PFN_Tcl_RegisterChannel Tcl_RegisterChannel;

// Slot 211
PFN_Tcl_RegisterObjType Tcl_RegisterObjType;

// Slot 212
PFN_Tcl_RegExpCompile Tcl_RegExpCompile;

// Slot 213
PFN_Tcl_RegExpExec Tcl_RegExpExec;

// Slot 214
PFN_Tcl_RegExpMatch Tcl_RegExpMatch;

// Slot 215
PFN_Tcl_RegExpRange Tcl_RegExpRange;

// Slot 216
PFN_Tcl_Release Tcl_Release;

// Slot 217
PFN_Tcl_ResetResult Tcl_ResetResult;

// Slot 218
PFN_Tcl_ScanElement Tcl_ScanElement;

// Slot 219
PFN_Tcl_ScanCountedElement Tcl_ScanCountedElement;

// Slot 220 is reserved

// Slot 221
PFN_Tcl_ServiceAll Tcl_ServiceAll;

// Slot 222
PFN_Tcl_ServiceEvent Tcl_ServiceEvent;

// Slot 223
PFN_Tcl_SetAssocData Tcl_SetAssocData;

// Slot 224
PFN_Tcl_SetChannelBufferSize Tcl_SetChannelBufferSize;

// Slot 225
PFN_Tcl_SetChannelOption Tcl_SetChannelOption;

// Slot 226
PFN_Tcl_SetCommandInfo Tcl_SetCommandInfo;

// Slot 227
PFN_Tcl_SetErrno Tcl_SetErrno;

// Slot 228
PFN_Tcl_SetErrorCode Tcl_SetErrorCode;

// Slot 229
PFN_Tcl_SetMaxBlockTime Tcl_SetMaxBlockTime;

// Slot 230 is reserved

// Slot 231
PFN_Tcl_SetRecursionLimit Tcl_SetRecursionLimit;

// Slot 232 is reserved

// Slot 233
PFN_Tcl_SetServiceMode Tcl_SetServiceMode;

// Slot 234
PFN_Tcl_SetObjErrorCode Tcl_SetObjErrorCode;

// Slot 235
PFN_Tcl_SetObjResult Tcl_SetObjResult;

// Slot 236
PFN_Tcl_SetStdChannel Tcl_SetStdChannel;

// Slot 237 is reserved

// Slot 238
PFN_Tcl_SetVar2 Tcl_SetVar2;

// Slot 239
PFN_Tcl_SignalId Tcl_SignalId;

// Slot 240
PFN_Tcl_SignalMsg Tcl_SignalMsg;

// Slot 241
PFN_Tcl_SourceRCFile Tcl_SourceRCFile;

// Slot 242
PFN_TclSplitList TclSplitList;

// Slot 243
PFN_TclSplitPath TclSplitPath;

// Slot 244 is reserved
// Slot 245 is reserved
// Slot 246 is reserved
// Slot 247 is reserved

// Slot 248
PFN_Tcl_TraceVar2 Tcl_TraceVar2;

// Slot 249
PFN_Tcl_TranslateFileName Tcl_TranslateFileName;

// Slot 250
PFN_Tcl_Ungets Tcl_Ungets;

// Slot 251
PFN_Tcl_UnlinkVar Tcl_UnlinkVar;

// Slot 252
PFN_Tcl_UnregisterChannel Tcl_UnregisterChannel;

// Slot 253 is reserved

// Slot 254
PFN_Tcl_UnsetVar2 Tcl_UnsetVar2;

// Slot 255 is reserved

// Slot 256
PFN_Tcl_UntraceVar2 Tcl_UntraceVar2;

// Slot 257
PFN_Tcl_UpdateLinkedVar Tcl_UpdateLinkedVar;

// Slot 258 is reserved

// Slot 259
PFN_Tcl_UpVar2 Tcl_UpVar2;

// Slot 260
PFN_Tcl_VarEval Tcl_VarEval;

// Slot 261 is reserved

// Slot 262
PFN_Tcl_VarTraceInfo2 Tcl_VarTraceInfo2;

// Slot 263
PFN_Tcl_Write Tcl_Write;

// Slot 264
PFN_Tcl_WrongNumArgs Tcl_WrongNumArgs;

// Slot 265
PFN_Tcl_DumpActiveMemory Tcl_DumpActiveMemory;

// Slot 266
PFN_Tcl_ValidateAllMemory Tcl_ValidateAllMemory;

// Slot 267 is reserved
// Slot 268 is reserved

// Slot 269
PFN_Tcl_HashStats Tcl_HashStats;

// Slot 270
PFN_Tcl_ParseVar Tcl_ParseVar;

// Slot 271 is reserved

// Slot 272
PFN_Tcl_PkgPresentEx Tcl_PkgPresentEx;

// Slot 273 is reserved
// Slot 274 is reserved
// Slot 275 is reserved
// Slot 276 is reserved

// Slot 277
PFN_Tcl_WaitPid Tcl_WaitPid;

// Slot 278 is reserved

// Slot 279
PFN_Tcl_GetVersion Tcl_GetVersion;

// Slot 280
PFN_Tcl_InitMemory Tcl_InitMemory;

// Slot 281
PFN_Tcl_StackChannel Tcl_StackChannel;

// Slot 282
PFN_Tcl_UnstackChannel Tcl_UnstackChannel;

// Slot 283
PFN_Tcl_GetStackedChannel Tcl_GetStackedChannel;

// Slot 284
PFN_Tcl_SetMainLoop Tcl_SetMainLoop;

// Slot 285
PFN_Tcl_GetAliasObj Tcl_GetAliasObj;

// Slot 286
PFN_Tcl_AppendObjToObj Tcl_AppendObjToObj;

// Slot 287
PFN_Tcl_CreateEncoding Tcl_CreateEncoding;

// Slot 288
PFN_Tcl_CreateThreadExitHandler Tcl_CreateThreadExitHandler;

// Slot 289
PFN_Tcl_DeleteThreadExitHandler Tcl_DeleteThreadExitHandler;

// Slot 290 is reserved

// Slot 291
PFN_Tcl_EvalEx Tcl_EvalEx;

// Slot 292
PFN_Tcl_EvalObjv Tcl_EvalObjv;

// Slot 293
PFN_Tcl_EvalObjEx Tcl_EvalObjEx;

// Slot 294
PFN_Tcl_ExitThread Tcl_ExitThread;

// Slot 295
PFN_Tcl_ExternalToUtf Tcl_ExternalToUtf;

// Slot 296
PFN_Tcl_ExternalToUtfDString Tcl_ExternalToUtfDString;

// Slot 297
PFN_Tcl_FinalizeThread Tcl_FinalizeThread;

// Slot 298
PFN_Tcl_FinalizeNotifier Tcl_FinalizeNotifier;

// Slot 299
PFN_Tcl_FreeEncoding Tcl_FreeEncoding;

// Slot 300
PFN_Tcl_GetCurrentThread Tcl_GetCurrentThread;

// Slot 301
PFN_Tcl_GetEncoding Tcl_GetEncoding;

// Slot 302
PFN_Tcl_GetEncodingName Tcl_GetEncodingName;

// Slot 303
PFN_Tcl_GetEncodingNames Tcl_GetEncodingNames;

// Slot 304
PFN_Tcl_GetIndexFromObjStruct Tcl_GetIndexFromObjStruct;

// Slot 305
PFN_Tcl_GetThreadData Tcl_GetThreadData;

// Slot 306
PFN_Tcl_GetVar2Ex Tcl_GetVar2Ex;

// Slot 307
PFN_Tcl_InitNotifier Tcl_InitNotifier;

// Slot 308
PFN_Tcl_MutexLock Tcl_MutexLock;

// Slot 309
PFN_Tcl_MutexUnlock Tcl_MutexUnlock;

// Slot 310
PFN_Tcl_ConditionNotify Tcl_ConditionNotify;

// Slot 311
PFN_Tcl_ConditionWait Tcl_ConditionWait;

// Slot 312
PFN_TclNumUtfChars TclNumUtfChars;

// Slot 313
PFN_Tcl_ReadChars Tcl_ReadChars;

// Slot 314 is reserved
// Slot 315 is reserved

// Slot 316
PFN_Tcl_SetSystemEncoding Tcl_SetSystemEncoding;

// Slot 317
PFN_Tcl_SetVar2Ex Tcl_SetVar2Ex;

// Slot 318
PFN_Tcl_ThreadAlert Tcl_ThreadAlert;

// Slot 319
PFN_Tcl_ThreadQueueEvent Tcl_ThreadQueueEvent;

// Slot 320
PFN_Tcl_UniCharAtIndex Tcl_UniCharAtIndex;

// Slot 321
PFN_Tcl_UniCharToLower Tcl_UniCharToLower;

// Slot 322
PFN_Tcl_UniCharToTitle Tcl_UniCharToTitle;

// Slot 323
PFN_Tcl_UniCharToUpper Tcl_UniCharToUpper;

// Slot 324
PFN_Tcl_UniCharToUtf Tcl_UniCharToUtf;

// Slot 325
PFN_TclUtfAtIndex TclUtfAtIndex;

// Slot 326
PFN_TclUtfCharComplete TclUtfCharComplete;

// Slot 327
PFN_Tcl_UtfBackslash Tcl_UtfBackslash;

// Slot 328
PFN_Tcl_UtfFindFirst Tcl_UtfFindFirst;

// Slot 329
PFN_Tcl_UtfFindLast Tcl_UtfFindLast;

// Slot 330
PFN_TclUtfNext TclUtfNext;

// Slot 331
PFN_TclUtfPrev TclUtfPrev;

// Slot 332
PFN_Tcl_UtfToExternal Tcl_UtfToExternal;

// Slot 333
PFN_Tcl_UtfToExternalDString Tcl_UtfToExternalDString;

// Slot 334
PFN_Tcl_UtfToLower Tcl_UtfToLower;

// Slot 335
PFN_Tcl_UtfToTitle Tcl_UtfToTitle;

// Slot 336
PFN_Tcl_UtfToChar16 Tcl_UtfToChar16;

// Slot 337
PFN_Tcl_UtfToUpper Tcl_UtfToUpper;

// Slot 338
PFN_Tcl_WriteChars Tcl_WriteChars;

// Slot 339
PFN_Tcl_WriteObj Tcl_WriteObj;

// Slot 340 is reserved
// Slot 341 is reserved
// Slot 342 is reserved

// Slot 343
PFN_Tcl_AlertNotifier Tcl_AlertNotifier;

// Slot 344
PFN_Tcl_ServiceModeHook Tcl_ServiceModeHook;

// Slot 345
PFN_Tcl_UniCharIsAlnum Tcl_UniCharIsAlnum;

// Slot 346
PFN_Tcl_UniCharIsAlpha Tcl_UniCharIsAlpha;

// Slot 347
PFN_Tcl_UniCharIsDigit Tcl_UniCharIsDigit;

// Slot 348
PFN_Tcl_UniCharIsLower Tcl_UniCharIsLower;

// Slot 349
PFN_Tcl_UniCharIsSpace Tcl_UniCharIsSpace;

// Slot 350
PFN_Tcl_UniCharIsUpper Tcl_UniCharIsUpper;

// Slot 351
PFN_Tcl_UniCharIsWordChar Tcl_UniCharIsWordChar;

// Slot 352
PFN_Tcl_Char16Len Tcl_Char16Len;

// Slot 353 is reserved

// Slot 354
PFN_Tcl_Char16ToUtfDString Tcl_Char16ToUtfDString;

// Slot 355
PFN_Tcl_UtfToChar16DString Tcl_UtfToChar16DString;

// Slot 356
PFN_Tcl_GetRegExpFromObj Tcl_GetRegExpFromObj;

// Slot 357 is reserved

// Slot 358
PFN_Tcl_FreeParse Tcl_FreeParse;

// Slot 359
PFN_Tcl_LogCommandInfo Tcl_LogCommandInfo;

// Slot 360
PFN_Tcl_ParseBraces Tcl_ParseBraces;

// Slot 361
PFN_Tcl_ParseCommand Tcl_ParseCommand;

// Slot 362
PFN_Tcl_ParseExpr Tcl_ParseExpr;

// Slot 363
PFN_Tcl_ParseQuotedString Tcl_ParseQuotedString;

// Slot 364
PFN_Tcl_ParseVarName Tcl_ParseVarName;

// Slot 365
PFN_Tcl_GetCwd Tcl_GetCwd;

// Slot 366
PFN_Tcl_Chdir Tcl_Chdir;

// Slot 367
PFN_Tcl_Access Tcl_Access;

// Slot 368
PFN_Tcl_Stat Tcl_Stat;

// Slot 369
PFN_TclUtfNcmp TclUtfNcmp;

// Slot 370
PFN_TclUtfNcasecmp TclUtfNcasecmp;

// Slot 371
PFN_Tcl_StringCaseMatch Tcl_StringCaseMatch;

// Slot 372
PFN_Tcl_UniCharIsControl Tcl_UniCharIsControl;

// Slot 373
PFN_Tcl_UniCharIsGraph Tcl_UniCharIsGraph;

// Slot 374
PFN_Tcl_UniCharIsPrint Tcl_UniCharIsPrint;

// Slot 375
PFN_Tcl_UniCharIsPunct Tcl_UniCharIsPunct;

// Slot 376
PFN_Tcl_RegExpExecObj Tcl_RegExpExecObj;

// Slot 377
PFN_Tcl_RegExpGetInfo Tcl_RegExpGetInfo;

// Slot 378
PFN_Tcl_NewUnicodeObj Tcl_NewUnicodeObj;

// Slot 379
PFN_Tcl_SetUnicodeObj Tcl_SetUnicodeObj;

// Slot 380
PFN_TclGetCharLength TclGetCharLength;

// Slot 381
PFN_TclGetUniChar TclGetUniChar;

// Slot 382 is reserved

// Slot 383
PFN_TclGetRange TclGetRange;

// Slot 384
PFN_Tcl_AppendUnicodeToObj Tcl_AppendUnicodeToObj;

// Slot 385
PFN_Tcl_RegExpMatchObj Tcl_RegExpMatchObj;

// Slot 386
PFN_Tcl_SetNotifier Tcl_SetNotifier;

// Slot 387
PFN_Tcl_GetAllocMutex Tcl_GetAllocMutex;

// Slot 388
PFN_Tcl_GetChannelNames Tcl_GetChannelNames;

// Slot 389
PFN_Tcl_GetChannelNamesEx Tcl_GetChannelNamesEx;

// Slot 390
PFN_Tcl_ProcObjCmd Tcl_ProcObjCmd;

// Slot 391
PFN_Tcl_ConditionFinalize Tcl_ConditionFinalize;

// Slot 392
PFN_Tcl_MutexFinalize Tcl_MutexFinalize;

// Slot 393
PFN_Tcl_CreateThread Tcl_CreateThread;

// Slot 394
PFN_Tcl_ReadRaw Tcl_ReadRaw;

// Slot 395
PFN_Tcl_WriteRaw Tcl_WriteRaw;

// Slot 396
PFN_Tcl_GetTopChannel Tcl_GetTopChannel;

// Slot 397
PFN_Tcl_ChannelBuffered Tcl_ChannelBuffered;

// Slot 398
PFN_Tcl_ChannelName Tcl_ChannelName;

// Slot 399
PFN_Tcl_ChannelVersion Tcl_ChannelVersion;

// Slot 400
PFN_Tcl_ChannelBlockModeProc Tcl_ChannelBlockModeProc;

// Slot 401 is reserved

// Slot 402
PFN_Tcl_ChannelClose2Proc Tcl_ChannelClose2Proc;

// Slot 403
PFN_Tcl_ChannelInputProc Tcl_ChannelInputProc;

// Slot 404
PFN_Tcl_ChannelOutputProc Tcl_ChannelOutputProc;

// Slot 405 is reserved

// Slot 406
PFN_Tcl_ChannelSetOptionProc Tcl_ChannelSetOptionProc;

// Slot 407
PFN_Tcl_ChannelGetOptionProc Tcl_ChannelGetOptionProc;

// Slot 408
PFN_Tcl_ChannelWatchProc Tcl_ChannelWatchProc;

// Slot 409
PFN_Tcl_ChannelGetHandleProc Tcl_ChannelGetHandleProc;

// Slot 410
PFN_Tcl_ChannelFlushProc Tcl_ChannelFlushProc;

// Slot 411
PFN_Tcl_ChannelHandlerProc Tcl_ChannelHandlerProc;

// Slot 412
PFN_Tcl_JoinThread Tcl_JoinThread;

// Slot 413
PFN_Tcl_IsChannelShared Tcl_IsChannelShared;

// Slot 414
PFN_Tcl_IsChannelRegistered Tcl_IsChannelRegistered;

// Slot 415
PFN_Tcl_CutChannel Tcl_CutChannel;

// Slot 416
PFN_Tcl_SpliceChannel Tcl_SpliceChannel;

// Slot 417
PFN_Tcl_ClearChannelHandlers Tcl_ClearChannelHandlers;

// Slot 418
PFN_Tcl_IsChannelExisting Tcl_IsChannelExisting;

// Slot 419 is reserved
// Slot 420 is reserved

// Slot 421
PFN_Tcl_DbCreateHashEntry Tcl_DbCreateHashEntry;

// Slot 422
PFN_Tcl_CreateHashEntry Tcl_CreateHashEntry;

// Slot 423
PFN_Tcl_InitCustomHashTable Tcl_InitCustomHashTable;

// Slot 424
PFN_Tcl_InitObjHashTable Tcl_InitObjHashTable;

// Slot 425
PFN_Tcl_CommandTraceInfo Tcl_CommandTraceInfo;

// Slot 426
PFN_Tcl_TraceCommand Tcl_TraceCommand;

// Slot 427
PFN_Tcl_UntraceCommand Tcl_UntraceCommand;

// Slot 428
PFN_Tcl_AttemptAlloc Tcl_AttemptAlloc;

// Slot 429
PFN_Tcl_AttemptDbCkalloc Tcl_AttemptDbCkalloc;

// Slot 430
PFN_Tcl_AttemptRealloc Tcl_AttemptRealloc;

// Slot 431
PFN_Tcl_AttemptDbCkrealloc Tcl_AttemptDbCkrealloc;

// Slot 432
PFN_Tcl_AttemptSetObjLength Tcl_AttemptSetObjLength;

// Slot 433
PFN_Tcl_GetChannelThread Tcl_GetChannelThread;

// Slot 434
PFN_TclGetUnicodeFromObj TclGetUnicodeFromObj;

// Slot 435 is reserved
// Slot 436 is reserved

// Slot 437
PFN_Tcl_SubstObj Tcl_SubstObj;

// Slot 438
PFN_Tcl_DetachChannel Tcl_DetachChannel;

// Slot 439
PFN_Tcl_IsStandardChannel Tcl_IsStandardChannel;

// Slot 440
PFN_Tcl_FSCopyFile Tcl_FSCopyFile;

// Slot 441
PFN_Tcl_FSCopyDirectory Tcl_FSCopyDirectory;

// Slot 442
PFN_Tcl_FSCreateDirectory Tcl_FSCreateDirectory;

// Slot 443
PFN_Tcl_FSDeleteFile Tcl_FSDeleteFile;

// Slot 444
PFN_Tcl_FSLoadFile Tcl_FSLoadFile;

// Slot 445
PFN_Tcl_FSMatchInDirectory Tcl_FSMatchInDirectory;

// Slot 446
PFN_Tcl_FSLink Tcl_FSLink;

// Slot 447
PFN_Tcl_FSRemoveDirectory Tcl_FSRemoveDirectory;

// Slot 448
PFN_Tcl_FSRenameFile Tcl_FSRenameFile;

// Slot 449
PFN_Tcl_FSLstat Tcl_FSLstat;

// Slot 450
PFN_Tcl_FSUtime Tcl_FSUtime;

// Slot 451
PFN_Tcl_FSFileAttrsGet Tcl_FSFileAttrsGet;

// Slot 452
PFN_Tcl_FSFileAttrsSet Tcl_FSFileAttrsSet;

// Slot 453
PFN_Tcl_FSFileAttrStrings Tcl_FSFileAttrStrings;

// Slot 454
PFN_Tcl_FSStat Tcl_FSStat;

// Slot 455
PFN_Tcl_FSAccess Tcl_FSAccess;

// Slot 456
PFN_Tcl_FSOpenFileChannel Tcl_FSOpenFileChannel;

// Slot 457
PFN_Tcl_FSGetCwd Tcl_FSGetCwd;

// Slot 458
PFN_Tcl_FSChdir Tcl_FSChdir;

// Slot 459
PFN_Tcl_FSConvertToPathType Tcl_FSConvertToPathType;

// Slot 460
PFN_Tcl_FSJoinPath Tcl_FSJoinPath;

// Slot 461
PFN_TclFSSplitPath TclFSSplitPath;

// Slot 462
PFN_Tcl_FSEqualPaths Tcl_FSEqualPaths;

// Slot 463
PFN_Tcl_FSGetNormalizedPath Tcl_FSGetNormalizedPath;

// Slot 464
PFN_Tcl_FSJoinToPath Tcl_FSJoinToPath;

// Slot 465
PFN_Tcl_FSGetInternalRep Tcl_FSGetInternalRep;

// Slot 466
PFN_Tcl_FSGetTranslatedPath Tcl_FSGetTranslatedPath;

// Slot 467
PFN_Tcl_FSEvalFile Tcl_FSEvalFile;

// Slot 468
PFN_Tcl_FSNewNativePath Tcl_FSNewNativePath;

// Slot 469
PFN_Tcl_FSGetNativePath Tcl_FSGetNativePath;

// Slot 470
PFN_Tcl_FSFileSystemInfo Tcl_FSFileSystemInfo;

// Slot 471
PFN_Tcl_FSPathSeparator Tcl_FSPathSeparator;

// Slot 472
PFN_Tcl_FSListVolumes Tcl_FSListVolumes;

// Slot 473
PFN_Tcl_FSRegister Tcl_FSRegister;

// Slot 474
PFN_Tcl_FSUnregister Tcl_FSUnregister;

// Slot 475
PFN_Tcl_FSData Tcl_FSData;

// Slot 476
PFN_Tcl_FSGetTranslatedStringPath Tcl_FSGetTranslatedStringPath;

// Slot 477
PFN_Tcl_FSGetFileSystemForPath Tcl_FSGetFileSystemForPath;

// Slot 478
PFN_Tcl_FSGetPathType Tcl_FSGetPathType;

// Slot 479
PFN_Tcl_OutputBuffered Tcl_OutputBuffered;

// Slot 480
PFN_Tcl_FSMountsChanged Tcl_FSMountsChanged;

// Slot 481
PFN_Tcl_EvalTokensStandard Tcl_EvalTokensStandard;

// Slot 482
PFN_Tcl_GetTime Tcl_GetTime;

// Slot 483
PFN_Tcl_CreateObjTrace Tcl_CreateObjTrace;

// Slot 484
PFN_Tcl_GetCommandInfoFromToken Tcl_GetCommandInfoFromToken;

// Slot 485
PFN_Tcl_SetCommandInfoFromToken Tcl_SetCommandInfoFromToken;

// Slot 486
PFN_Tcl_DbNewWideIntObj Tcl_DbNewWideIntObj;

// Slot 487
PFN_Tcl_GetWideIntFromObj Tcl_GetWideIntFromObj;

// Slot 488
PFN_Tcl_NewWideIntObj Tcl_NewWideIntObj;

// Slot 489
PFN_Tcl_SetWideIntObj Tcl_SetWideIntObj;

// Slot 490
PFN_Tcl_AllocStatBuf Tcl_AllocStatBuf;

// Slot 491
PFN_Tcl_Seek Tcl_Seek;

// Slot 492
PFN_Tcl_Tell Tcl_Tell;

// Slot 493
PFN_Tcl_ChannelWideSeekProc Tcl_ChannelWideSeekProc;

// Slot 494
PFN_Tcl_DictObjPut Tcl_DictObjPut;

// Slot 495
PFN_Tcl_DictObjGet Tcl_DictObjGet;

// Slot 496
PFN_Tcl_DictObjRemove Tcl_DictObjRemove;

// Slot 497
PFN_TclDictObjSize TclDictObjSize;

// Slot 498
PFN_Tcl_DictObjFirst Tcl_DictObjFirst;

// Slot 499
PFN_Tcl_DictObjNext Tcl_DictObjNext;

// Slot 500
PFN_Tcl_DictObjDone Tcl_DictObjDone;

// Slot 501
PFN_Tcl_DictObjPutKeyList Tcl_DictObjPutKeyList;

// Slot 502
PFN_Tcl_DictObjRemoveKeyList Tcl_DictObjRemoveKeyList;

// Slot 503
PFN_Tcl_NewDictObj Tcl_NewDictObj;

// Slot 504
PFN_Tcl_DbNewDictObj Tcl_DbNewDictObj;

// Slot 505
PFN_Tcl_RegisterConfig Tcl_RegisterConfig;

// Slot 506
PFN_Tcl_CreateNamespace Tcl_CreateNamespace;

// Slot 507
PFN_Tcl_DeleteNamespace Tcl_DeleteNamespace;

// Slot 508
PFN_Tcl_AppendExportList Tcl_AppendExportList;

// Slot 509
PFN_Tcl_Export Tcl_Export;

// Slot 510
PFN_Tcl_Import Tcl_Import;

// Slot 511
PFN_Tcl_ForgetImport Tcl_ForgetImport;

// Slot 512
PFN_Tcl_GetCurrentNamespace Tcl_GetCurrentNamespace;

// Slot 513
PFN_Tcl_GetGlobalNamespace Tcl_GetGlobalNamespace;

// Slot 514
PFN_Tcl_FindNamespace Tcl_FindNamespace;

// Slot 515
PFN_Tcl_FindCommand Tcl_FindCommand;

// Slot 516
PFN_Tcl_GetCommandFromObj Tcl_GetCommandFromObj;

// Slot 517
PFN_Tcl_GetCommandFullName Tcl_GetCommandFullName;

// Slot 518
PFN_Tcl_FSEvalFileEx Tcl_FSEvalFileEx;

// Slot 519 is reserved

// Slot 520
PFN_Tcl_LimitAddHandler Tcl_LimitAddHandler;

// Slot 521
PFN_Tcl_LimitRemoveHandler Tcl_LimitRemoveHandler;

// Slot 522
PFN_Tcl_LimitReady Tcl_LimitReady;

// Slot 523
PFN_Tcl_LimitCheck Tcl_LimitCheck;

// Slot 524
PFN_Tcl_LimitExceeded Tcl_LimitExceeded;

// Slot 525
PFN_Tcl_LimitSetCommands Tcl_LimitSetCommands;

// Slot 526
PFN_Tcl_LimitSetTime Tcl_LimitSetTime;

// Slot 527
PFN_Tcl_LimitSetGranularity Tcl_LimitSetGranularity;

// Slot 528
PFN_Tcl_LimitTypeEnabled Tcl_LimitTypeEnabled;

// Slot 529
PFN_Tcl_LimitTypeExceeded Tcl_LimitTypeExceeded;

// Slot 530
PFN_Tcl_LimitTypeSet Tcl_LimitTypeSet;

// Slot 531
PFN_Tcl_LimitTypeReset Tcl_LimitTypeReset;

// Slot 532
PFN_Tcl_LimitGetCommands Tcl_LimitGetCommands;

// Slot 533
PFN_Tcl_LimitGetTime Tcl_LimitGetTime;

// Slot 534
PFN_Tcl_LimitGetGranularity Tcl_LimitGetGranularity;

// Slot 535
PFN_Tcl_SaveInterpState Tcl_SaveInterpState;

// Slot 536
PFN_Tcl_RestoreInterpState Tcl_RestoreInterpState;

// Slot 537
PFN_Tcl_DiscardInterpState Tcl_DiscardInterpState;

// Slot 538
PFN_Tcl_SetReturnOptions Tcl_SetReturnOptions;

// Slot 539
PFN_Tcl_GetReturnOptions Tcl_GetReturnOptions;

// Slot 540
PFN_Tcl_IsEnsemble Tcl_IsEnsemble;

// Slot 541
PFN_Tcl_CreateEnsemble Tcl_CreateEnsemble;

// Slot 542
PFN_Tcl_FindEnsemble Tcl_FindEnsemble;

// Slot 543
PFN_Tcl_SetEnsembleSubcommandList Tcl_SetEnsembleSubcommandList;

// Slot 544
PFN_Tcl_SetEnsembleMappingDict Tcl_SetEnsembleMappingDict;

// Slot 545
PFN_Tcl_SetEnsembleUnknownHandler Tcl_SetEnsembleUnknownHandler;

// Slot 546
PFN_Tcl_SetEnsembleFlags Tcl_SetEnsembleFlags;

// Slot 547
PFN_Tcl_GetEnsembleSubcommandList Tcl_GetEnsembleSubcommandList;

// Slot 548
PFN_Tcl_GetEnsembleMappingDict Tcl_GetEnsembleMappingDict;

// Slot 549
PFN_Tcl_GetEnsembleUnknownHandler Tcl_GetEnsembleUnknownHandler;

// Slot 550
PFN_Tcl_GetEnsembleFlags Tcl_GetEnsembleFlags;

// Slot 551
PFN_Tcl_GetEnsembleNamespace Tcl_GetEnsembleNamespace;

// Slot 552
PFN_Tcl_SetTimeProc Tcl_SetTimeProc;

// Slot 553
PFN_Tcl_QueryTimeProc Tcl_QueryTimeProc;

// Slot 554
PFN_Tcl_ChannelThreadActionProc Tcl_ChannelThreadActionProc;

// Slot 555
PFN_Tcl_NewBignumObj Tcl_NewBignumObj;

// Slot 556
PFN_Tcl_DbNewBignumObj Tcl_DbNewBignumObj;

// Slot 557
PFN_Tcl_SetBignumObj Tcl_SetBignumObj;

// Slot 558
PFN_Tcl_GetBignumFromObj Tcl_GetBignumFromObj;

// Slot 559
PFN_Tcl_TakeBignumFromObj Tcl_TakeBignumFromObj;

// Slot 560
PFN_Tcl_TruncateChannel Tcl_TruncateChannel;

// Slot 561
PFN_Tcl_ChannelTruncateProc Tcl_ChannelTruncateProc;

// Slot 562
PFN_Tcl_SetChannelErrorInterp Tcl_SetChannelErrorInterp;

// Slot 563
PFN_Tcl_GetChannelErrorInterp Tcl_GetChannelErrorInterp;

// Slot 564
PFN_Tcl_SetChannelError Tcl_SetChannelError;

// Slot 565
PFN_Tcl_GetChannelError Tcl_GetChannelError;

// Slot 566
PFN_Tcl_InitBignumFromDouble Tcl_InitBignumFromDouble;

// Slot 567
PFN_Tcl_GetNamespaceUnknownHandler Tcl_GetNamespaceUnknownHandler;

// Slot 568
PFN_Tcl_SetNamespaceUnknownHandler Tcl_SetNamespaceUnknownHandler;

// Slot 569
PFN_Tcl_GetEncodingFromObj Tcl_GetEncodingFromObj;

// Slot 570
PFN_Tcl_GetEncodingSearchPath Tcl_GetEncodingSearchPath;

// Slot 571
PFN_Tcl_SetEncodingSearchPath Tcl_SetEncodingSearchPath;

// Slot 572
PFN_Tcl_GetEncodingNameFromEnvironment Tcl_GetEncodingNameFromEnvironment;

// Slot 573
PFN_Tcl_PkgRequireProc Tcl_PkgRequireProc;

// Slot 574
PFN_Tcl_AppendObjToErrorInfo Tcl_AppendObjToErrorInfo;

// Slot 575
PFN_Tcl_AppendLimitedToObj Tcl_AppendLimitedToObj;

// Slot 576
PFN_Tcl_Format Tcl_Format;

// Slot 577
PFN_Tcl_AppendFormatToObj Tcl_AppendFormatToObj;

// Slot 578
PFN_Tcl_ObjPrintf Tcl_ObjPrintf;

// Slot 579
PFN_Tcl_AppendPrintfToObj Tcl_AppendPrintfToObj;

// Slot 580
PFN_Tcl_CancelEval Tcl_CancelEval;

// Slot 581
PFN_Tcl_Canceled Tcl_Canceled;

// Slot 582
PFN_Tcl_CreatePipe Tcl_CreatePipe;

// Slot 583
PFN_Tcl_NRCreateCommand Tcl_NRCreateCommand;

// Slot 584
PFN_Tcl_NREvalObj Tcl_NREvalObj;

// Slot 585
PFN_Tcl_NREvalObjv Tcl_NREvalObjv;

// Slot 586
PFN_Tcl_NRCmdSwap Tcl_NRCmdSwap;

// Slot 587
PFN_Tcl_NRAddCallback Tcl_NRAddCallback;

// Slot 588
PFN_Tcl_NRCallObjProc Tcl_NRCallObjProc;

// Slot 589
PFN_Tcl_GetFSDeviceFromStat Tcl_GetFSDeviceFromStat;

// Slot 590
PFN_Tcl_GetFSInodeFromStat Tcl_GetFSInodeFromStat;

// Slot 591
PFN_Tcl_GetModeFromStat Tcl_GetModeFromStat;

// Slot 592
PFN_Tcl_GetLinkCountFromStat Tcl_GetLinkCountFromStat;

// Slot 593
PFN_Tcl_GetUserIdFromStat Tcl_GetUserIdFromStat;

// Slot 594
PFN_Tcl_GetGroupIdFromStat Tcl_GetGroupIdFromStat;

// Slot 595
PFN_Tcl_GetDeviceTypeFromStat Tcl_GetDeviceTypeFromStat;

// Slot 596
PFN_Tcl_GetAccessTimeFromStat Tcl_GetAccessTimeFromStat;

// Slot 597
PFN_Tcl_GetModificationTimeFromStat Tcl_GetModificationTimeFromStat;

// Slot 598
PFN_Tcl_GetChangeTimeFromStat Tcl_GetChangeTimeFromStat;

// Slot 599
PFN_Tcl_GetSizeFromStat Tcl_GetSizeFromStat;

// Slot 600
PFN_Tcl_GetBlocksFromStat Tcl_GetBlocksFromStat;

// Slot 601
PFN_Tcl_GetBlockSizeFromStat Tcl_GetBlockSizeFromStat;

// Slot 602
PFN_Tcl_SetEnsembleParameterList Tcl_SetEnsembleParameterList;

// Slot 603
PFN_Tcl_GetEnsembleParameterList Tcl_GetEnsembleParameterList;

// Slot 604
PFN_TclParseArgsObjv TclParseArgsObjv;

// Slot 605
PFN_Tcl_GetErrorLine Tcl_GetErrorLine;

// Slot 606
PFN_Tcl_SetErrorLine Tcl_SetErrorLine;

// Slot 607
PFN_Tcl_TransferResult Tcl_TransferResult;

// Slot 608
PFN_Tcl_InterpActive Tcl_InterpActive;

// Slot 609
PFN_Tcl_BackgroundException Tcl_BackgroundException;

// Slot 610
PFN_Tcl_ZlibDeflate Tcl_ZlibDeflate;

// Slot 611
PFN_Tcl_ZlibInflate Tcl_ZlibInflate;

// Slot 612
PFN_Tcl_ZlibCRC32 Tcl_ZlibCRC32;

// Slot 613
PFN_Tcl_ZlibAdler32 Tcl_ZlibAdler32;

// Slot 614
PFN_Tcl_ZlibStreamInit Tcl_ZlibStreamInit;

// Slot 615
PFN_Tcl_ZlibStreamGetCommandName Tcl_ZlibStreamGetCommandName;

// Slot 616
PFN_Tcl_ZlibStreamEof Tcl_ZlibStreamEof;

// Slot 617
PFN_Tcl_ZlibStreamChecksum Tcl_ZlibStreamChecksum;

// Slot 618
PFN_Tcl_ZlibStreamPut Tcl_ZlibStreamPut;

// Slot 619
PFN_Tcl_ZlibStreamGet Tcl_ZlibStreamGet;

// Slot 620
PFN_Tcl_ZlibStreamClose Tcl_ZlibStreamClose;

// Slot 621
PFN_Tcl_ZlibStreamReset Tcl_ZlibStreamReset;

// Slot 622
PFN_Tcl_SetStartupScript Tcl_SetStartupScript;

// Slot 623
PFN_Tcl_GetStartupScript Tcl_GetStartupScript;

// Slot 624
PFN_Tcl_CloseEx Tcl_CloseEx;

// Slot 625
PFN_Tcl_NRExprObj Tcl_NRExprObj;

// Slot 626
PFN_Tcl_NRSubstObj Tcl_NRSubstObj;

// Slot 627
PFN_Tcl_LoadFile Tcl_LoadFile;

// Slot 628
PFN_Tcl_FindSymbol Tcl_FindSymbol;

// Slot 629
PFN_Tcl_FSUnloadFile Tcl_FSUnloadFile;

// Slot 630
PFN_Tcl_ZlibStreamSetCompressionDictionary Tcl_ZlibStreamSetCompressionDictionary;

// Slot 631
PFN_Tcl_OpenTcpServerEx Tcl_OpenTcpServerEx;

// Slot 632
PFN_TclZipfs_Mount TclZipfs_Mount;

// Slot 633
PFN_TclZipfs_Unmount TclZipfs_Unmount;

// Slot 634
PFN_TclZipfs_TclLibrary TclZipfs_TclLibrary;

// Slot 635
PFN_TclZipfs_MountBuffer TclZipfs_MountBuffer;

// Slot 636
PFN_Tcl_FreeInternalRep Tcl_FreeInternalRep;

// Slot 637
PFN_Tcl_InitStringRep Tcl_InitStringRep;

// Slot 638
PFN_Tcl_FetchInternalRep Tcl_FetchInternalRep;

// Slot 639
PFN_Tcl_StoreInternalRep Tcl_StoreInternalRep;

// Slot 640
PFN_Tcl_HasStringRep Tcl_HasStringRep;

// Slot 641
PFN_Tcl_IncrRefCount Tcl_IncrRefCount;

// Slot 642
PFN_Tcl_DecrRefCount Tcl_DecrRefCount;

// Slot 643
PFN_Tcl_IsShared Tcl_IsShared;

// Slot 644
PFN_Tcl_LinkArray Tcl_LinkArray;

// Slot 645
PFN_Tcl_GetIntForIndex Tcl_GetIntForIndex;

// Slot 646
PFN_Tcl_UtfToUniChar Tcl_UtfToUniChar;

// Slot 647
PFN_Tcl_UniCharToUtfDString Tcl_UniCharToUtfDString;

// Slot 648
PFN_Tcl_UtfToUniCharDString Tcl_UtfToUniCharDString;

// Slot 649
PFN_TclGetBytesFromObj TclGetBytesFromObj;

// Slot 650
PFN_Tcl_GetBytesFromObj Tcl_GetBytesFromObj;

// Slot 651
PFN_Tcl_GetStringFromObj Tcl_GetStringFromObj;

// Slot 652
PFN_Tcl_GetUnicodeFromObj Tcl_GetUnicodeFromObj;

// Slot 653
PFN_Tcl_GetSizeIntFromObj Tcl_GetSizeIntFromObj;

// Slot 654
PFN_Tcl_UtfCharComplete Tcl_UtfCharComplete;

// Slot 655
PFN_Tcl_UtfNext Tcl_UtfNext;

// Slot 656
PFN_Tcl_UtfPrev Tcl_UtfPrev;

// Slot 657
PFN_Tcl_FSTildeExpand Tcl_FSTildeExpand;

// Slot 658
PFN_Tcl_ExternalToUtfDStringEx Tcl_ExternalToUtfDStringEx;

// Slot 659
PFN_Tcl_UtfToExternalDStringEx Tcl_UtfToExternalDStringEx;

// Slot 660
PFN_Tcl_AsyncMarkFromSignal Tcl_AsyncMarkFromSignal;

// Slot 661
PFN_Tcl_ListObjGetElements Tcl_ListObjGetElements;

// Slot 662
PFN_Tcl_ListObjLength Tcl_ListObjLength;

// Slot 663
PFN_Tcl_DictObjSize Tcl_DictObjSize;

// Slot 664
PFN_Tcl_SplitList Tcl_SplitList;

// Slot 665
PFN_Tcl_SplitPath Tcl_SplitPath;

// Slot 666
PFN_Tcl_FSSplitPath Tcl_FSSplitPath;

// Slot 667
PFN_Tcl_ParseArgsObjv Tcl_ParseArgsObjv;

// Slot 668
PFN_Tcl_UniCharLen Tcl_UniCharLen;

// Slot 669
PFN_Tcl_NumUtfChars Tcl_NumUtfChars;

// Slot 670
PFN_Tcl_GetCharLength Tcl_GetCharLength;

// Slot 671
PFN_Tcl_UtfAtIndex Tcl_UtfAtIndex;

// Slot 672
PFN_Tcl_GetRange Tcl_GetRange;

// Slot 673
PFN_Tcl_GetUniChar Tcl_GetUniChar;

// Slot 674
PFN_Tcl_GetBool Tcl_GetBool;

// Slot 675
PFN_Tcl_GetBoolFromObj Tcl_GetBoolFromObj;

// Slot 676
PFN_Tcl_CreateObjCommand2 Tcl_CreateObjCommand2;

// Slot 677
PFN_Tcl_CreateObjTrace2 Tcl_CreateObjTrace2;

// Slot 678
PFN_Tcl_NRCreateCommand2 Tcl_NRCreateCommand2;

// Slot 679
PFN_Tcl_NRCallObjProc2 Tcl_NRCallObjProc2;

// Slot 680
PFN_Tcl_GetNumberFromObj Tcl_GetNumberFromObj;

// Slot 681
PFN_Tcl_GetNumber Tcl_GetNumber;

// Slot 682
PFN_Tcl_RemoveChannelMode Tcl_RemoveChannelMode;

// Slot 683
PFN_Tcl_GetEncodingNulLength Tcl_GetEncodingNulLength;

// Slot 684
PFN_Tcl_GetWideUIntFromObj Tcl_GetWideUIntFromObj;

// Slot 685
PFN_Tcl_DStringToObj Tcl_DStringToObj;

// Slot 686
PFN_Tcl_UtfNcmp Tcl_UtfNcmp;

// Slot 687
PFN_Tcl_UtfNcasecmp Tcl_UtfNcasecmp;

// Slot 688
PFN_Tcl_NewWideUIntObj Tcl_NewWideUIntObj;

// Slot 689
PFN_Tcl_SetWideUIntObj Tcl_SetWideUIntObj;

// Slot 690
PFN_Tcl_IsEmpty Tcl_IsEmpty;

// Slot 691
PFN_Tcl_GetEncodingNameForUser Tcl_GetEncodingNameForUser;

// Slot 692
PFN_Tcl_ListObjReverse Tcl_ListObjReverse;

// Slot 693
PFN_Tcl_ListObjRepeat Tcl_ListObjRepeat;

// Slot 694
PFN_Tcl_ListObjRange Tcl_ListObjRange;

// Slot 695
PFN_TclUnusedStubEntry TclUnusedStubEntry;
#pragma endregion

bool LoadTCLLibrary() {
    std::filesystem::path tcllib(EccoMetaUtility::GetGameDir());
	tcllib.append("addons/ecco/tcl/");
#ifdef _WIN32
    g_libtommathHandle = LOAD_LIB(tcllib.string() + "libtommath.dll");
    g_Zlib1Handle = LOAD_LIB(tcllib.string() + "zlib1.dll");
    g_TclLibHandle = LOAD_LIB(tcllib.string() + "tcl91.dll");
#else
    g_libtommathHandle = LOAD_LIB(tcllib.string() + "libtommath.so");
    g_Zlib1Handle = LOAD_LIB(tcllib.string() + "zlib1.so");
    g_TclLibHandle = LOAD_LIB(tcllib.string() + "tcl91.so");
#endif
    if (!g_libtommathHandle || !g_Zlib1Handle || !g_TclLibHandle) {
		LOG_ERROR(PLID, "Can not load tcl91 library\n");
        return false;
    }

    // 依次加载所有API
#pragma region Load API
    LOAD_TCL_API(Tcl_PkgProvideEx);
    LOAD_TCL_API(Tcl_PkgRequireEx);
    LOAD_TCL_API(Tcl_Panic);
    LOAD_TCL_API(Tcl_Alloc);
    LOAD_TCL_API(Tcl_Free);
    LOAD_TCL_API(Tcl_Realloc);
    LOAD_TCL_API(Tcl_DbCkalloc);
    LOAD_TCL_API(Tcl_DbCkfree);
    LOAD_TCL_API(Tcl_DbCkrealloc);
    LOAD_TCL_API(Tcl_CreateFileHandler);
    LOAD_TCL_API(Tcl_DeleteFileHandler);
    LOAD_TCL_API(Tcl_SetTimer);
    LOAD_TCL_API(Tcl_Sleep);
    LOAD_TCL_API(Tcl_WaitForEvent);
    LOAD_TCL_API(Tcl_AppendAllObjTypes);
    LOAD_TCL_API(Tcl_AppendStringsToObj);
    LOAD_TCL_API(Tcl_AppendToObj);
    LOAD_TCL_API(Tcl_ConcatObj);
    LOAD_TCL_API(Tcl_ConvertToType);
    LOAD_TCL_API(Tcl_DbDecrRefCount);
    LOAD_TCL_API(Tcl_DbIncrRefCount);
    LOAD_TCL_API(Tcl_DbIsShared);
    LOAD_TCL_API(Tcl_DbNewByteArrayObj);
    LOAD_TCL_API(Tcl_DbNewDoubleObj);
    LOAD_TCL_API(Tcl_DbNewListObj);
    LOAD_TCL_API(Tcl_DbNewObj);
    LOAD_TCL_API(Tcl_DbNewStringObj);
    LOAD_TCL_API(Tcl_DuplicateObj);
    LOAD_TCL_API(TclFreeObj);
    LOAD_TCL_API(Tcl_GetDouble);
    LOAD_TCL_API(Tcl_GetDoubleFromObj);
    LOAD_TCL_API(Tcl_GetInt);
    LOAD_TCL_API(Tcl_GetIntFromObj);
    LOAD_TCL_API(Tcl_GetLongFromObj);
    LOAD_TCL_API(Tcl_GetObjType);
    LOAD_TCL_API(TclGetStringFromObj);
    LOAD_TCL_API(Tcl_InvalidateStringRep);
    LOAD_TCL_API(Tcl_ListObjAppendList);
    LOAD_TCL_API(Tcl_ListObjAppendElement);
    LOAD_TCL_API(TclListObjGetElements);
    LOAD_TCL_API(Tcl_ListObjIndex);
    LOAD_TCL_API(TclListObjLength);
    LOAD_TCL_API(Tcl_ListObjReplace);
    LOAD_TCL_API(Tcl_NewByteArrayObj);
    LOAD_TCL_API(Tcl_NewDoubleObj);
    LOAD_TCL_API(Tcl_NewListObj);
    LOAD_TCL_API(Tcl_NewObj);
    LOAD_TCL_API(Tcl_NewStringObj);
    LOAD_TCL_API(Tcl_SetByteArrayLength);
    LOAD_TCL_API(Tcl_SetByteArrayObj);
    LOAD_TCL_API(Tcl_SetDoubleObj);
    LOAD_TCL_API(Tcl_SetListObj);
    LOAD_TCL_API(Tcl_SetObjLength);
    LOAD_TCL_API(Tcl_SetStringObj);
    LOAD_TCL_API(Tcl_AllowExceptions);
    LOAD_TCL_API(Tcl_AppendElement);
    LOAD_TCL_API(Tcl_AppendResult);
    LOAD_TCL_API(Tcl_AsyncCreate);
    LOAD_TCL_API(Tcl_AsyncDelete);
    LOAD_TCL_API(Tcl_AsyncInvoke);
    LOAD_TCL_API(Tcl_AsyncMark);
    LOAD_TCL_API(Tcl_AsyncReady);
    LOAD_TCL_API(Tcl_BadChannelOption);
    LOAD_TCL_API(Tcl_CallWhenDeleted);
    LOAD_TCL_API(Tcl_CancelIdleCall);
    LOAD_TCL_API(Tcl_CommandComplete);
    LOAD_TCL_API(Tcl_Concat);
    LOAD_TCL_API(Tcl_ConvertElement);
    LOAD_TCL_API(Tcl_ConvertCountedElement);
    LOAD_TCL_API(Tcl_CreateAlias);
    LOAD_TCL_API(Tcl_CreateAliasObj);
    LOAD_TCL_API(Tcl_CreateChannel);
    LOAD_TCL_API(Tcl_CreateChannelHandler);
    LOAD_TCL_API(Tcl_CreateCloseHandler);
    LOAD_TCL_API(Tcl_CreateCommand);
    LOAD_TCL_API(Tcl_CreateEventSource);
    LOAD_TCL_API(Tcl_CreateExitHandler);
    LOAD_TCL_API(Tcl_CreateInterp);
    LOAD_TCL_API(Tcl_CreateObjCommand);
    LOAD_TCL_API(Tcl_CreateChild);
    LOAD_TCL_API(Tcl_CreateTimerHandler);
    LOAD_TCL_API(Tcl_CreateTrace);
    LOAD_TCL_API(Tcl_DeleteAssocData);
    LOAD_TCL_API(Tcl_DeleteChannelHandler);
    LOAD_TCL_API(Tcl_DeleteCloseHandler);
    LOAD_TCL_API(Tcl_DeleteCommand);
    LOAD_TCL_API(Tcl_DeleteCommandFromToken);
    LOAD_TCL_API(Tcl_DeleteEvents);
    LOAD_TCL_API(Tcl_DeleteEventSource);
    LOAD_TCL_API(Tcl_DeleteExitHandler);
    LOAD_TCL_API(Tcl_DeleteHashEntry);
    LOAD_TCL_API(Tcl_DeleteHashTable);
    LOAD_TCL_API(Tcl_DeleteInterp);
    LOAD_TCL_API(Tcl_DetachPids);
    LOAD_TCL_API(Tcl_DeleteTimerHandler);
    LOAD_TCL_API(Tcl_DeleteTrace);
    LOAD_TCL_API(Tcl_DontCallWhenDeleted);
    LOAD_TCL_API(Tcl_DoOneEvent);
    LOAD_TCL_API(Tcl_DoWhenIdle);
    LOAD_TCL_API(Tcl_DStringAppend);
    LOAD_TCL_API(Tcl_DStringAppendElement);
    LOAD_TCL_API(Tcl_DStringEndSublist);
    LOAD_TCL_API(Tcl_DStringFree);
    LOAD_TCL_API(Tcl_DStringGetResult);
    LOAD_TCL_API(Tcl_DStringInit);
    LOAD_TCL_API(Tcl_DStringResult);
    LOAD_TCL_API(Tcl_DStringSetLength);
    LOAD_TCL_API(Tcl_DStringStartSublist);
    LOAD_TCL_API(Tcl_Eof);
    LOAD_TCL_API(Tcl_ErrnoId);
    LOAD_TCL_API(Tcl_ErrnoMsg);
    LOAD_TCL_API(Tcl_EvalFile);
    LOAD_TCL_API(Tcl_EventuallyFree);
    LOAD_TCL_API(Tcl_Exit);
    LOAD_TCL_API(Tcl_ExposeCommand);
    LOAD_TCL_API(Tcl_ExprBoolean);
    LOAD_TCL_API(Tcl_ExprBooleanObj);
    LOAD_TCL_API(Tcl_ExprDouble);
    LOAD_TCL_API(Tcl_ExprDoubleObj);
    LOAD_TCL_API(Tcl_ExprLong);
    LOAD_TCL_API(Tcl_ExprLongObj);
    LOAD_TCL_API(Tcl_ExprObj);
    LOAD_TCL_API(Tcl_ExprString);
    LOAD_TCL_API(Tcl_Finalize);
    LOAD_TCL_API(Tcl_FirstHashEntry);
    LOAD_TCL_API(Tcl_Flush);
    LOAD_TCL_API(TclGetAliasObj);
    LOAD_TCL_API(Tcl_GetAssocData);
    LOAD_TCL_API(Tcl_GetChannel);
    LOAD_TCL_API(Tcl_GetChannelBufferSize);
    LOAD_TCL_API(Tcl_GetChannelHandle);
    LOAD_TCL_API(Tcl_GetChannelInstanceData);
    LOAD_TCL_API(Tcl_GetChannelMode);
    LOAD_TCL_API(Tcl_GetChannelName);
    LOAD_TCL_API(Tcl_GetChannelOption);
    LOAD_TCL_API(Tcl_GetChannelType);
    LOAD_TCL_API(Tcl_GetCommandInfo);
    LOAD_TCL_API(Tcl_GetCommandName);
    LOAD_TCL_API(Tcl_GetErrno);
    LOAD_TCL_API(Tcl_GetHostName);
    LOAD_TCL_API(Tcl_GetInterpPath);
    LOAD_TCL_API(Tcl_GetParent);
    LOAD_TCL_API(Tcl_GetNameOfExecutable);
    LOAD_TCL_API(Tcl_GetObjResult);
    LOAD_TCL_API(Tcl_GetOpenFile);
    LOAD_TCL_API(Tcl_GetPathType);
    LOAD_TCL_API(Tcl_Gets);
    LOAD_TCL_API(Tcl_GetsObj);
    LOAD_TCL_API(Tcl_GetServiceMode);
    LOAD_TCL_API(Tcl_GetChild);
    LOAD_TCL_API(Tcl_GetStdChannel);
    LOAD_TCL_API(Tcl_GetVar2);
    LOAD_TCL_API(Tcl_HideCommand);
    LOAD_TCL_API(Tcl_Init);
    LOAD_TCL_API(Tcl_InitHashTable);
    LOAD_TCL_API(Tcl_InputBlocked);
    LOAD_TCL_API(Tcl_InputBuffered);
    LOAD_TCL_API(Tcl_InterpDeleted);
    LOAD_TCL_API(Tcl_IsSafe);
    LOAD_TCL_API(Tcl_JoinPath);
    LOAD_TCL_API(Tcl_LinkVar);
    LOAD_TCL_API(Tcl_MakeFileChannel);
    LOAD_TCL_API(Tcl_MakeTcpClientChannel);
    LOAD_TCL_API(Tcl_Merge);
    LOAD_TCL_API(Tcl_NextHashEntry);
    LOAD_TCL_API(Tcl_NotifyChannel);
    LOAD_TCL_API(Tcl_ObjGetVar2);
    LOAD_TCL_API(Tcl_ObjSetVar2);
    LOAD_TCL_API(Tcl_OpenCommandChannel);
    LOAD_TCL_API(Tcl_OpenFileChannel);
    LOAD_TCL_API(Tcl_OpenTcpClient);
    LOAD_TCL_API(Tcl_OpenTcpServer);
    LOAD_TCL_API(Tcl_Preserve);
    LOAD_TCL_API(Tcl_PrintDouble);
    LOAD_TCL_API(Tcl_PutEnv);
    LOAD_TCL_API(Tcl_PosixError);
    LOAD_TCL_API(Tcl_QueueEvent);
    LOAD_TCL_API(Tcl_Read);
    LOAD_TCL_API(Tcl_ReapDetachedProcs);
    LOAD_TCL_API(Tcl_RecordAndEval);
    LOAD_TCL_API(Tcl_RecordAndEvalObj);
    LOAD_TCL_API(Tcl_RegisterChannel);
    LOAD_TCL_API(Tcl_RegisterObjType);
    LOAD_TCL_API(Tcl_RegExpCompile);
    LOAD_TCL_API(Tcl_RegExpExec);
    LOAD_TCL_API(Tcl_RegExpMatch);
    LOAD_TCL_API(Tcl_RegExpRange);
    LOAD_TCL_API(Tcl_Release);
    LOAD_TCL_API(Tcl_ResetResult);
    LOAD_TCL_API(Tcl_ScanElement);
    LOAD_TCL_API(Tcl_ScanCountedElement);
    LOAD_TCL_API(Tcl_ServiceAll);
    LOAD_TCL_API(Tcl_ServiceEvent);
    LOAD_TCL_API(Tcl_SetAssocData);
    LOAD_TCL_API(Tcl_SetChannelBufferSize);
    LOAD_TCL_API(Tcl_SetChannelOption);
    LOAD_TCL_API(Tcl_SetCommandInfo);
    LOAD_TCL_API(Tcl_SetErrno);
    LOAD_TCL_API(Tcl_SetErrorCode);
    LOAD_TCL_API(Tcl_SetMaxBlockTime);
    LOAD_TCL_API(Tcl_SetRecursionLimit);
    LOAD_TCL_API(Tcl_SetServiceMode);
    LOAD_TCL_API(Tcl_SetObjErrorCode);
    LOAD_TCL_API(Tcl_SetObjResult);
    LOAD_TCL_API(Tcl_SetStdChannel);
    LOAD_TCL_API(Tcl_SetVar2);
    LOAD_TCL_API(Tcl_SignalId);
    LOAD_TCL_API(Tcl_SignalMsg);
    LOAD_TCL_API(Tcl_SourceRCFile);
    LOAD_TCL_API(TclSplitList);
    LOAD_TCL_API(TclSplitPath);
    LOAD_TCL_API(Tcl_TraceVar2);
    LOAD_TCL_API(Tcl_TranslateFileName);
    LOAD_TCL_API(Tcl_Ungets);
    LOAD_TCL_API(Tcl_UnlinkVar);
    LOAD_TCL_API(Tcl_UnregisterChannel);
    LOAD_TCL_API(Tcl_UnsetVar2);
    LOAD_TCL_API(Tcl_UntraceVar2);
    LOAD_TCL_API(Tcl_UpdateLinkedVar);
    LOAD_TCL_API(Tcl_UpVar2);
    LOAD_TCL_API(Tcl_VarEval);
    LOAD_TCL_API(Tcl_VarTraceInfo2);
    LOAD_TCL_API(Tcl_Write);
    LOAD_TCL_API(Tcl_WrongNumArgs);
    LOAD_TCL_API(Tcl_DumpActiveMemory);
    LOAD_TCL_API(Tcl_ValidateAllMemory);
    LOAD_TCL_API(Tcl_HashStats);
    LOAD_TCL_API(Tcl_ParseVar);
    LOAD_TCL_API(Tcl_PkgPresentEx);
    LOAD_TCL_API(Tcl_WaitPid);
    LOAD_TCL_API(Tcl_GetVersion);
    LOAD_TCL_API(Tcl_InitMemory);
    LOAD_TCL_API(Tcl_StackChannel);
    LOAD_TCL_API(Tcl_UnstackChannel);
    LOAD_TCL_API(Tcl_GetStackedChannel);
    LOAD_TCL_API(Tcl_SetMainLoop);
    LOAD_TCL_API(Tcl_GetAliasObj);
    LOAD_TCL_API(Tcl_AppendObjToObj);
    LOAD_TCL_API(Tcl_CreateEncoding);
    LOAD_TCL_API(Tcl_CreateThreadExitHandler);
    LOAD_TCL_API(Tcl_DeleteThreadExitHandler);
    LOAD_TCL_API(Tcl_EvalEx);
    LOAD_TCL_API(Tcl_EvalObjv);
    LOAD_TCL_API(Tcl_EvalObjEx);
    LOAD_TCL_API(Tcl_ExitThread);
    LOAD_TCL_API(Tcl_ExternalToUtf);
    LOAD_TCL_API(Tcl_ExternalToUtfDString);
    LOAD_TCL_API(Tcl_FinalizeThread);
    LOAD_TCL_API(Tcl_FinalizeNotifier);
    LOAD_TCL_API(Tcl_FreeEncoding);
    LOAD_TCL_API(Tcl_GetCurrentThread);
    LOAD_TCL_API(Tcl_GetEncoding);
    LOAD_TCL_API(Tcl_GetEncodingName);
    LOAD_TCL_API(Tcl_GetEncodingNames);
    LOAD_TCL_API(Tcl_GetIndexFromObjStruct);
    LOAD_TCL_API(Tcl_GetThreadData);
    LOAD_TCL_API(Tcl_GetVar2Ex);
    LOAD_TCL_API(Tcl_InitNotifier);
    LOAD_TCL_API(Tcl_MutexLock);
    LOAD_TCL_API(Tcl_MutexUnlock);
    LOAD_TCL_API(Tcl_ConditionNotify);
    LOAD_TCL_API(Tcl_ConditionWait);
    LOAD_TCL_API(TclNumUtfChars);
    LOAD_TCL_API(Tcl_ReadChars);
    LOAD_TCL_API(Tcl_SetSystemEncoding);
    LOAD_TCL_API(Tcl_SetVar2Ex);
    LOAD_TCL_API(Tcl_ThreadAlert);
    LOAD_TCL_API(Tcl_ThreadQueueEvent);
    LOAD_TCL_API(Tcl_UniCharAtIndex);
    LOAD_TCL_API(Tcl_UniCharToLower);
    LOAD_TCL_API(Tcl_UniCharToTitle);
    LOAD_TCL_API(Tcl_UniCharToUpper);
    LOAD_TCL_API(Tcl_UniCharToUtf);
    LOAD_TCL_API(TclUtfAtIndex);
    LOAD_TCL_API(TclUtfCharComplete);
    LOAD_TCL_API(Tcl_UtfBackslash);
    LOAD_TCL_API(Tcl_UtfFindFirst);
    LOAD_TCL_API(Tcl_UtfFindLast);
    LOAD_TCL_API(TclUtfNext);
    LOAD_TCL_API(TclUtfPrev);
    LOAD_TCL_API(Tcl_UtfToExternal);
    LOAD_TCL_API(Tcl_UtfToExternalDString);
    LOAD_TCL_API(Tcl_UtfToLower);
    LOAD_TCL_API(Tcl_UtfToTitle);
    LOAD_TCL_API(Tcl_UtfToChar16);
    LOAD_TCL_API(Tcl_UtfToUpper);
    LOAD_TCL_API(Tcl_WriteChars);
    LOAD_TCL_API(Tcl_WriteObj);
    LOAD_TCL_API(Tcl_AlertNotifier);
    LOAD_TCL_API(Tcl_ServiceModeHook);
    LOAD_TCL_API(Tcl_UniCharIsAlnum);
    LOAD_TCL_API(Tcl_UniCharIsAlpha);
    LOAD_TCL_API(Tcl_UniCharIsDigit);
    LOAD_TCL_API(Tcl_UniCharIsLower);
    LOAD_TCL_API(Tcl_UniCharIsSpace);
    LOAD_TCL_API(Tcl_UniCharIsUpper);
    LOAD_TCL_API(Tcl_UniCharIsWordChar);
    LOAD_TCL_API(Tcl_Char16Len);
    LOAD_TCL_API(Tcl_Char16ToUtfDString);
    LOAD_TCL_API(Tcl_UtfToChar16DString);
    LOAD_TCL_API(Tcl_GetRegExpFromObj);
    LOAD_TCL_API(Tcl_FreeParse);
    LOAD_TCL_API(Tcl_LogCommandInfo);
    LOAD_TCL_API(Tcl_ParseBraces);
    LOAD_TCL_API(Tcl_ParseCommand);
    LOAD_TCL_API(Tcl_ParseExpr);
    LOAD_TCL_API(Tcl_ParseQuotedString);
    LOAD_TCL_API(Tcl_ParseVarName);
    LOAD_TCL_API(Tcl_GetCwd);
    LOAD_TCL_API(Tcl_Chdir);
    LOAD_TCL_API(Tcl_Access);
    LOAD_TCL_API(Tcl_Stat);
    LOAD_TCL_API(TclUtfNcmp);
    LOAD_TCL_API(TclUtfNcasecmp);
    LOAD_TCL_API(Tcl_StringCaseMatch);
    LOAD_TCL_API(Tcl_UniCharIsControl);
    LOAD_TCL_API(Tcl_UniCharIsGraph);
    LOAD_TCL_API(Tcl_UniCharIsPrint);
    LOAD_TCL_API(Tcl_UniCharIsPunct);
    LOAD_TCL_API(Tcl_RegExpExecObj);
    LOAD_TCL_API(Tcl_RegExpGetInfo);
    LOAD_TCL_API(Tcl_NewUnicodeObj);
    LOAD_TCL_API(Tcl_SetUnicodeObj);
    LOAD_TCL_API(TclGetCharLength);
    LOAD_TCL_API(TclGetUniChar);
    LOAD_TCL_API(TclGetRange);
    LOAD_TCL_API(Tcl_AppendUnicodeToObj);
    LOAD_TCL_API(Tcl_RegExpMatchObj);
    LOAD_TCL_API(Tcl_SetNotifier);
    LOAD_TCL_API(Tcl_GetAllocMutex);
    LOAD_TCL_API(Tcl_GetChannelNames);
    LOAD_TCL_API(Tcl_GetChannelNamesEx);
    LOAD_TCL_API(Tcl_ProcObjCmd);
    LOAD_TCL_API(Tcl_ConditionFinalize);
    LOAD_TCL_API(Tcl_MutexFinalize);
    LOAD_TCL_API(Tcl_CreateThread);
    LOAD_TCL_API(Tcl_ReadRaw);
    LOAD_TCL_API(Tcl_WriteRaw);
    LOAD_TCL_API(Tcl_GetTopChannel);
    LOAD_TCL_API(Tcl_ChannelBuffered);
    LOAD_TCL_API(Tcl_ChannelName);
    LOAD_TCL_API(Tcl_ChannelVersion);
    LOAD_TCL_API(Tcl_ChannelBlockModeProc);
    LOAD_TCL_API(Tcl_ChannelClose2Proc);
    LOAD_TCL_API(Tcl_ChannelInputProc);
    LOAD_TCL_API(Tcl_ChannelOutputProc);
    LOAD_TCL_API(Tcl_ChannelSetOptionProc);
    LOAD_TCL_API(Tcl_ChannelGetOptionProc);
    LOAD_TCL_API(Tcl_ChannelWatchProc);
    LOAD_TCL_API(Tcl_ChannelGetHandleProc);
    LOAD_TCL_API(Tcl_ChannelFlushProc);
    LOAD_TCL_API(Tcl_ChannelHandlerProc);
    LOAD_TCL_API(Tcl_JoinThread);
    LOAD_TCL_API(Tcl_IsChannelShared);
    LOAD_TCL_API(Tcl_IsChannelRegistered);
    LOAD_TCL_API(Tcl_CutChannel);
    LOAD_TCL_API(Tcl_SpliceChannel);
    LOAD_TCL_API(Tcl_ClearChannelHandlers);
    LOAD_TCL_API(Tcl_IsChannelExisting);
    LOAD_TCL_API(Tcl_DbCreateHashEntry);
    LOAD_TCL_API(Tcl_CreateHashEntry);
    LOAD_TCL_API(Tcl_InitCustomHashTable);
    LOAD_TCL_API(Tcl_InitObjHashTable);
    LOAD_TCL_API(Tcl_CommandTraceInfo);
    LOAD_TCL_API(Tcl_TraceCommand);
    LOAD_TCL_API(Tcl_UntraceCommand);
    LOAD_TCL_API(Tcl_AttemptAlloc);
    LOAD_TCL_API(Tcl_AttemptDbCkalloc);
    LOAD_TCL_API(Tcl_AttemptRealloc);
    LOAD_TCL_API(Tcl_AttemptDbCkrealloc);
    LOAD_TCL_API(Tcl_AttemptSetObjLength);
    LOAD_TCL_API(Tcl_GetChannelThread);
    LOAD_TCL_API(TclGetUnicodeFromObj);
    LOAD_TCL_API(Tcl_SubstObj);
    LOAD_TCL_API(Tcl_DetachChannel);
    LOAD_TCL_API(Tcl_IsStandardChannel);
    LOAD_TCL_API(Tcl_FSCopyFile);
    LOAD_TCL_API(Tcl_FSCopyDirectory);
    LOAD_TCL_API(Tcl_FSCreateDirectory);
    LOAD_TCL_API(Tcl_FSDeleteFile);
    LOAD_TCL_API(Tcl_FSLoadFile);
    LOAD_TCL_API(Tcl_FSMatchInDirectory);
    LOAD_TCL_API(Tcl_FSLink);
    LOAD_TCL_API(Tcl_FSRemoveDirectory);
    LOAD_TCL_API(Tcl_FSRenameFile);
    LOAD_TCL_API(Tcl_FSLstat);
    LOAD_TCL_API(Tcl_FSUtime);
    LOAD_TCL_API(Tcl_FSFileAttrsGet);
    LOAD_TCL_API(Tcl_FSFileAttrsSet);
    LOAD_TCL_API(Tcl_FSFileAttrStrings);
    LOAD_TCL_API(Tcl_FSStat);
    LOAD_TCL_API(Tcl_FSAccess);
    LOAD_TCL_API(Tcl_FSOpenFileChannel);
    LOAD_TCL_API(Tcl_FSGetCwd);
    LOAD_TCL_API(Tcl_FSChdir);
    LOAD_TCL_API(Tcl_FSConvertToPathType);
    LOAD_TCL_API(Tcl_FSJoinPath);
    LOAD_TCL_API(TclFSSplitPath);
    LOAD_TCL_API(Tcl_FSEqualPaths);
    LOAD_TCL_API(Tcl_FSGetNormalizedPath);
    LOAD_TCL_API(Tcl_FSJoinToPath);
    LOAD_TCL_API(Tcl_FSGetInternalRep);
    LOAD_TCL_API(Tcl_FSGetTranslatedPath);
    LOAD_TCL_API(Tcl_FSEvalFile);
    LOAD_TCL_API(Tcl_FSNewNativePath);
    LOAD_TCL_API(Tcl_FSGetNativePath);
    LOAD_TCL_API(Tcl_FSFileSystemInfo);
    LOAD_TCL_API(Tcl_FSPathSeparator);
    LOAD_TCL_API(Tcl_FSListVolumes);
    LOAD_TCL_API(Tcl_FSRegister);
    LOAD_TCL_API(Tcl_FSUnregister);
    LOAD_TCL_API(Tcl_FSData);
    LOAD_TCL_API(Tcl_FSGetTranslatedStringPath);
    LOAD_TCL_API(Tcl_FSGetFileSystemForPath);
    LOAD_TCL_API(Tcl_FSGetPathType);
    LOAD_TCL_API(Tcl_OutputBuffered);
    LOAD_TCL_API(Tcl_FSMountsChanged);
    LOAD_TCL_API(Tcl_EvalTokensStandard);
    LOAD_TCL_API(Tcl_GetTime);
    LOAD_TCL_API(Tcl_CreateObjTrace);
    LOAD_TCL_API(Tcl_GetCommandInfoFromToken);
    LOAD_TCL_API(Tcl_SetCommandInfoFromToken);
    LOAD_TCL_API(Tcl_DbNewWideIntObj);
    LOAD_TCL_API(Tcl_GetWideIntFromObj);
    LOAD_TCL_API(Tcl_NewWideIntObj);
    LOAD_TCL_API(Tcl_SetWideIntObj);
    LOAD_TCL_API(Tcl_AllocStatBuf);
    LOAD_TCL_API(Tcl_Seek);
    LOAD_TCL_API(Tcl_Tell);
    LOAD_TCL_API(Tcl_ChannelWideSeekProc);
    LOAD_TCL_API(Tcl_DictObjPut);
    LOAD_TCL_API(Tcl_DictObjGet);
    LOAD_TCL_API(Tcl_DictObjRemove);
    LOAD_TCL_API(TclDictObjSize);
    LOAD_TCL_API(Tcl_DictObjFirst);
    LOAD_TCL_API(Tcl_DictObjNext);
    LOAD_TCL_API(Tcl_DictObjDone);
    LOAD_TCL_API(Tcl_DictObjPutKeyList);
    LOAD_TCL_API(Tcl_DictObjRemoveKeyList);
    LOAD_TCL_API(Tcl_NewDictObj);
    LOAD_TCL_API(Tcl_DbNewDictObj);
    LOAD_TCL_API(Tcl_RegisterConfig);
    LOAD_TCL_API(Tcl_CreateNamespace);
    LOAD_TCL_API(Tcl_DeleteNamespace);
    LOAD_TCL_API(Tcl_AppendExportList);
    LOAD_TCL_API(Tcl_Export);
    LOAD_TCL_API(Tcl_Import);
    LOAD_TCL_API(Tcl_ForgetImport);
    LOAD_TCL_API(Tcl_GetCurrentNamespace);
    LOAD_TCL_API(Tcl_GetGlobalNamespace);
    LOAD_TCL_API(Tcl_FindNamespace);
    LOAD_TCL_API(Tcl_FindCommand);
    LOAD_TCL_API(Tcl_GetCommandFromObj);
    LOAD_TCL_API(Tcl_GetCommandFullName);
    LOAD_TCL_API(Tcl_FSEvalFileEx);
    LOAD_TCL_API(Tcl_LimitAddHandler);
    LOAD_TCL_API(Tcl_LimitRemoveHandler);
    LOAD_TCL_API(Tcl_LimitReady);
    LOAD_TCL_API(Tcl_LimitCheck);
    LOAD_TCL_API(Tcl_LimitExceeded);
    LOAD_TCL_API(Tcl_LimitSetCommands);
    LOAD_TCL_API(Tcl_LimitSetTime);
    LOAD_TCL_API(Tcl_LimitSetGranularity);
    LOAD_TCL_API(Tcl_LimitTypeEnabled);
    LOAD_TCL_API(Tcl_LimitTypeExceeded);
    LOAD_TCL_API(Tcl_LimitTypeSet);
    LOAD_TCL_API(Tcl_LimitTypeReset);
    LOAD_TCL_API(Tcl_LimitGetCommands);
    LOAD_TCL_API(Tcl_LimitGetTime);
    LOAD_TCL_API(Tcl_LimitGetGranularity);
    LOAD_TCL_API(Tcl_SaveInterpState);
    LOAD_TCL_API(Tcl_RestoreInterpState);
    LOAD_TCL_API(Tcl_DiscardInterpState);
    LOAD_TCL_API(Tcl_SetReturnOptions);
    LOAD_TCL_API(Tcl_GetReturnOptions);
    LOAD_TCL_API(Tcl_IsEnsemble);
    LOAD_TCL_API(Tcl_CreateEnsemble);
    LOAD_TCL_API(Tcl_FindEnsemble);
    LOAD_TCL_API(Tcl_SetEnsembleSubcommandList);
    LOAD_TCL_API(Tcl_SetEnsembleMappingDict);
    LOAD_TCL_API(Tcl_SetEnsembleUnknownHandler);
    LOAD_TCL_API(Tcl_SetEnsembleFlags);
    LOAD_TCL_API(Tcl_GetEnsembleSubcommandList);
    LOAD_TCL_API(Tcl_GetEnsembleMappingDict);
    LOAD_TCL_API(Tcl_GetEnsembleUnknownHandler);
    LOAD_TCL_API(Tcl_GetEnsembleFlags);
    LOAD_TCL_API(Tcl_GetEnsembleNamespace);
    LOAD_TCL_API(Tcl_SetTimeProc);
    LOAD_TCL_API(Tcl_QueryTimeProc);
    LOAD_TCL_API(Tcl_ChannelThreadActionProc);
    LOAD_TCL_API(Tcl_NewBignumObj);
    LOAD_TCL_API(Tcl_DbNewBignumObj);
    LOAD_TCL_API(Tcl_SetBignumObj);
    LOAD_TCL_API(Tcl_GetBignumFromObj);
    LOAD_TCL_API(Tcl_TakeBignumFromObj);
    LOAD_TCL_API(Tcl_TruncateChannel);
    LOAD_TCL_API(Tcl_ChannelTruncateProc);
    LOAD_TCL_API(Tcl_SetChannelErrorInterp);
    LOAD_TCL_API(Tcl_GetChannelErrorInterp);
    LOAD_TCL_API(Tcl_SetChannelError);
    LOAD_TCL_API(Tcl_GetChannelError);
    LOAD_TCL_API(Tcl_InitBignumFromDouble);
    LOAD_TCL_API(Tcl_GetNamespaceUnknownHandler);
    LOAD_TCL_API(Tcl_SetNamespaceUnknownHandler);
    LOAD_TCL_API(Tcl_GetEncodingFromObj);
    LOAD_TCL_API(Tcl_GetEncodingSearchPath);
    LOAD_TCL_API(Tcl_SetEncodingSearchPath);
    LOAD_TCL_API(Tcl_GetEncodingNameFromEnvironment);
    LOAD_TCL_API(Tcl_PkgRequireProc);
    LOAD_TCL_API(Tcl_AppendObjToErrorInfo);
    LOAD_TCL_API(Tcl_AppendLimitedToObj);
    LOAD_TCL_API(Tcl_Format);
    LOAD_TCL_API(Tcl_AppendFormatToObj);
    LOAD_TCL_API(Tcl_ObjPrintf);
    LOAD_TCL_API(Tcl_AppendPrintfToObj);
    LOAD_TCL_API(Tcl_CancelEval);
    LOAD_TCL_API(Tcl_Canceled);
    LOAD_TCL_API(Tcl_CreatePipe);
    LOAD_TCL_API(Tcl_NRCreateCommand);
    LOAD_TCL_API(Tcl_NREvalObj);
    LOAD_TCL_API(Tcl_NREvalObjv);
    LOAD_TCL_API(Tcl_NRCmdSwap);
    LOAD_TCL_API(Tcl_NRAddCallback);
    LOAD_TCL_API(Tcl_NRCallObjProc);
    LOAD_TCL_API(Tcl_GetFSDeviceFromStat);
    LOAD_TCL_API(Tcl_GetFSInodeFromStat);
    LOAD_TCL_API(Tcl_GetModeFromStat);
    LOAD_TCL_API(Tcl_GetLinkCountFromStat);
    LOAD_TCL_API(Tcl_GetUserIdFromStat);
    LOAD_TCL_API(Tcl_GetGroupIdFromStat);
    LOAD_TCL_API(Tcl_GetDeviceTypeFromStat);
    LOAD_TCL_API(Tcl_GetAccessTimeFromStat);
    LOAD_TCL_API(Tcl_GetModificationTimeFromStat);
    LOAD_TCL_API(Tcl_GetChangeTimeFromStat);
    LOAD_TCL_API(Tcl_GetSizeFromStat);
    LOAD_TCL_API(Tcl_GetBlocksFromStat);
    LOAD_TCL_API(Tcl_GetBlockSizeFromStat);
    LOAD_TCL_API(Tcl_SetEnsembleParameterList);
    LOAD_TCL_API(Tcl_GetEnsembleParameterList);
    LOAD_TCL_API(TclParseArgsObjv);
    LOAD_TCL_API(Tcl_GetErrorLine);
    LOAD_TCL_API(Tcl_SetErrorLine);
    LOAD_TCL_API(Tcl_TransferResult);
    LOAD_TCL_API(Tcl_InterpActive);
    LOAD_TCL_API(Tcl_BackgroundException);
    LOAD_TCL_API(Tcl_ZlibDeflate);
    LOAD_TCL_API(Tcl_ZlibInflate);
    LOAD_TCL_API(Tcl_ZlibCRC32);
    LOAD_TCL_API(Tcl_ZlibAdler32);
    LOAD_TCL_API(Tcl_ZlibStreamInit);
    LOAD_TCL_API(Tcl_ZlibStreamGetCommandName);
    LOAD_TCL_API(Tcl_ZlibStreamEof);
    LOAD_TCL_API(Tcl_ZlibStreamChecksum);
    LOAD_TCL_API(Tcl_ZlibStreamPut);
    LOAD_TCL_API(Tcl_ZlibStreamGet);
    LOAD_TCL_API(Tcl_ZlibStreamClose);
    LOAD_TCL_API(Tcl_ZlibStreamReset);
    LOAD_TCL_API(Tcl_SetStartupScript);
    LOAD_TCL_API(Tcl_GetStartupScript);
    LOAD_TCL_API(Tcl_CloseEx);
    LOAD_TCL_API(Tcl_NRExprObj);
    LOAD_TCL_API(Tcl_NRSubstObj);
    LOAD_TCL_API(Tcl_LoadFile);
    LOAD_TCL_API(Tcl_FindSymbol);
    LOAD_TCL_API(Tcl_FSUnloadFile);
    LOAD_TCL_API(Tcl_ZlibStreamSetCompressionDictionary);
    LOAD_TCL_API(Tcl_OpenTcpServerEx);
    LOAD_TCL_API(TclZipfs_Mount);
    LOAD_TCL_API(TclZipfs_Unmount);
    LOAD_TCL_API(TclZipfs_TclLibrary);
    LOAD_TCL_API(TclZipfs_MountBuffer);
    LOAD_TCL_API(Tcl_FreeInternalRep);
    LOAD_TCL_API(Tcl_InitStringRep);
    LOAD_TCL_API(Tcl_FetchInternalRep);
    LOAD_TCL_API(Tcl_StoreInternalRep);
    LOAD_TCL_API(Tcl_HasStringRep);
    LOAD_TCL_API(Tcl_IncrRefCount);
    LOAD_TCL_API(Tcl_DecrRefCount);
    LOAD_TCL_API(Tcl_IsShared);
    LOAD_TCL_API(Tcl_LinkArray);
    LOAD_TCL_API(Tcl_GetIntForIndex);
    LOAD_TCL_API(Tcl_UtfToUniChar);
    LOAD_TCL_API(Tcl_UniCharToUtfDString);
    LOAD_TCL_API(Tcl_UtfToUniCharDString);
    LOAD_TCL_API(TclGetBytesFromObj);
    LOAD_TCL_API(Tcl_GetBytesFromObj);
    LOAD_TCL_API(Tcl_GetStringFromObj);
    LOAD_TCL_API(Tcl_GetUnicodeFromObj);
    LOAD_TCL_API(Tcl_GetSizeIntFromObj);
    LOAD_TCL_API(Tcl_UtfCharComplete);
    LOAD_TCL_API(Tcl_UtfNext);
    LOAD_TCL_API(Tcl_UtfPrev);
    LOAD_TCL_API(Tcl_FSTildeExpand);
    LOAD_TCL_API(Tcl_ExternalToUtfDStringEx);
    LOAD_TCL_API(Tcl_UtfToExternalDStringEx);
    LOAD_TCL_API(Tcl_AsyncMarkFromSignal);
    LOAD_TCL_API(Tcl_ListObjGetElements);
    LOAD_TCL_API(Tcl_ListObjLength);
    LOAD_TCL_API(Tcl_DictObjSize);
    LOAD_TCL_API(Tcl_SplitList);
    LOAD_TCL_API(Tcl_SplitPath);
    LOAD_TCL_API(Tcl_FSSplitPath);
    LOAD_TCL_API(Tcl_ParseArgsObjv);
    LOAD_TCL_API(Tcl_UniCharLen);
    LOAD_TCL_API(Tcl_NumUtfChars);
    LOAD_TCL_API(Tcl_GetCharLength);
    LOAD_TCL_API(Tcl_UtfAtIndex);
    LOAD_TCL_API(Tcl_GetRange);
    LOAD_TCL_API(Tcl_GetUniChar);
    LOAD_TCL_API(Tcl_GetBool);
    LOAD_TCL_API(Tcl_GetBoolFromObj);
    LOAD_TCL_API(Tcl_CreateObjCommand2);
    LOAD_TCL_API(Tcl_CreateObjTrace2);
    LOAD_TCL_API(Tcl_NRCreateCommand2);
    LOAD_TCL_API(Tcl_NRCallObjProc2);
    LOAD_TCL_API(Tcl_GetNumberFromObj);
    LOAD_TCL_API(Tcl_GetNumber);
    LOAD_TCL_API(Tcl_RemoveChannelMode);
    LOAD_TCL_API(Tcl_GetEncodingNulLength);
    LOAD_TCL_API(Tcl_GetWideUIntFromObj);
    LOAD_TCL_API(Tcl_DStringToObj);
    LOAD_TCL_API(Tcl_UtfNcmp);
    LOAD_TCL_API(Tcl_UtfNcasecmp);
    LOAD_TCL_API(Tcl_NewWideUIntObj);
    LOAD_TCL_API(Tcl_SetWideUIntObj);
    LOAD_TCL_API(Tcl_IsEmpty);
    LOAD_TCL_API(Tcl_GetEncodingNameForUser);
    LOAD_TCL_API(Tcl_ListObjReverse);
    LOAD_TCL_API(Tcl_ListObjRepeat);
    LOAD_TCL_API(Tcl_ListObjRange);
    LOAD_TCL_API(TclUnusedStubEntry);
#pragma endregion
    return true;
}

void UnloadTCLLibrary() {
    if (g_TclLibHandle) {
        FREE_LIB(g_TclLibHandle);
        g_TclLibHandle = nullptr;
    }
    if (g_Zlib1Handle) {
        FREE_LIB(g_Zlib1Handle);
        g_Zlib1Handle = nullptr;
	}
    if (g_libtommathHandle) {
        FREE_LIB(g_libtommathHandle);
        g_libtommathHandle = nullptr;
    }
}