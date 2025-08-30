#include <format>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <sstream>

#include "tcl_dynamic.h"
#undef DLLEXPORT
#undef DLLIMPORT
#undef CRTIMPORT

#include "CEccoScriptSystem.h"

#include <extdll.h>
#include "meta_utility.h"

static Tcl_Interp* s_pTclinterp = nullptr;
static char s_szLibpath[MAX_PATH] = { 0 };
static char s_szPkgpath[MAX_PATH] = { 0 };

class CScriptCmd {
public:
	IEccoScriptSystem* interface;
	const char* name;
	const char* description;
	std::vector<IEccoScriptSystem::ScriptArgType> required_args;
	std::string args_str;
	std::string symbol;
	IEccoScriptSystem::fnFunc callback;
	void* user_args;
};

static std::map<const char*, CScriptCmd*> s_mapCommands{};

std::vector<std::string> GetTCLCommandsDoc() {
	std::vector<std::string> docs;
	std::string buffer = std::format("|{:<24} | {:<36} | {:<48} ", "Command", "Args", "Description"); 
	docs.push_back(buffer);
	for (const auto& [name, cmd] : s_mapCommands) {
		buffer = std::format("|{:<24} | {:<36} | {:<48}", name, cmd->symbol, cmd->description);
		docs.push_back(buffer);
	}
	return docs;
}

CEccoScriptSystem::CEccoScriptSystem(){
	if (s_pTclinterp)
		Tcl_DeleteInterp(s_pTclinterp);
	s_pTclinterp = Tcl_CreateInterp();

	auto tclPath = std::filesystem::path(EccoMetaUtility::GetGameDir());
	tclPath.append("addons/ecco/tcl");
	strncpy(s_szLibpath, (tclPath.string() + "/lib").c_str(), MAX_PATH);
	strncpy(s_szPkgpath, (tclPath.string() + "/pkg").c_str(), MAX_PATH);

	Tcl_SetVar(s_pTclinterp, "tcl_library", s_szLibpath, TCL_GLOBAL_ONLY);
	Tcl_SetVar(s_pTclinterp, "tcl_pkgPath", s_szPkgpath, TCL_GLOBAL_ONLY);
	Tcl_SetVar(s_pTclinterp, "tcl_interactive", "0", TCL_GLOBAL_ONLY);
	if (Tcl_Init(s_pTclinterp) == TCL_ERROR) {
		LOG_ERROR(PLID, "Tcl init failed!\n%s", Tcl_GetStringResult(s_pTclinterp));
		return;
	}
#define MAX_AUTO_PATH (MAX_PATH * 2 + 6)
	char path[MAX_AUTO_PATH];
	snprintf(path, MAX_AUTO_PATH, "{%s}\n{%s}", s_szLibpath, s_szPkgpath);
	Tcl_SetVar(s_pTclinterp, "auto_path", path, TCL_GLOBAL_ONLY);
	if (Tcl_SetSystemEncoding(s_pTclinterp, "utf-8") == TCL_ERROR) {
		LOG_ERROR(PLID, "Tcl set utf-8 encoding failed!\n%s", Tcl_GetStringResult(s_pTclinterp));
	}
}

void CEccoScriptSystem::CreateCommand(const char* name, const char* symbol, const char* description, fnFunc callback, void* user_args) {
	CScriptCmd* cmd = new CScriptCmd();
	cmd->interface = this;
	cmd->name = name;
	cmd->description = description;
	cmd->symbol = symbol;
	cmd->callback = callback;
	cmd->user_args = user_args;
	std::vector<std::string> split{};
	std::stringstream ss(symbol);
	std::string item;
	while (std::getline(ss, item, ',')) {
		size_t start = item.find_first_not_of(" \t");
		size_t end = item.find_last_not_of(" \t");
		if (start != std::string::npos && end != std::string::npos) {
			std::string s = item.substr(start, end - start + 1);
			size_t space = s.find(' ');
			if (space != std::string::npos)
				split.push_back(s.substr(0, space));
			else
				split.push_back(s);
			cmd->args_str += *split.rbegin() + ", ";
		}
	}
	if (!cmd->args_str.empty())
		cmd->args_str = cmd->args_str.substr(0, cmd->args_str.size() - 2);
	for(auto&s : split) {
		if(!strcmp(s.c_str(), "int"))
			cmd->required_args.push_back(IEccoScriptSystem::ScriptArgType::Int);
		else if (!strcmp(s.c_str(), "string"))
			cmd->required_args.push_back(IEccoScriptSystem::ScriptArgType::String);
		else if (!strcmp(s.c_str(), "double"))
			cmd->required_args.push_back(IEccoScriptSystem::ScriptArgType::Double);
		else if(!strcmp(s.c_str(), "float"))
			cmd->required_args.push_back(IEccoScriptSystem::ScriptArgType::Float);
		else if (!strcmp(s.c_str(), "bool"))
			cmd->required_args.push_back(IEccoScriptSystem::ScriptArgType::Boolean);
		else {
			LOG_ERROR(PLID, "Unknown script command argument type: %s in command %s", s.c_str(), name);
			delete cmd;
			return;
		}
	}

	Tcl_CreateCommand(s_pTclinterp, name, [](ClientData clientData, Tcl_Interp* interp, int argc, const char* argv[]) {
		CScriptCmd* cmd = static_cast<CScriptCmd*>(clientData);
		if (argc - 1 != (int)cmd->required_args.size()) {
			Tcl_SetErrorCode(s_pTclinterp, "arguments count not match", nullptr);
			Tcl_AddErrorInfo(s_pTclinterp, "Require argument: ");
			Tcl_AddErrorInfo(s_pTclinterp, cmd->args_str.c_str());
			return TCL_ERROR;
		}
		std::vector<IEccoScriptSystem::ScriptContent*> args{};
		for (int i = 1; i < argc; i++) {
			const char* arg = argv[i];
			if (arg == nullptr) {
				Tcl_SetErrorCode(s_pTclinterp, "Argument is NULL", nullptr);
				Tcl_AddErrorInfo(s_pTclinterp, "Require argument: ");
				Tcl_AddErrorInfo(s_pTclinterp, cmd->args_str.c_str());
				return TCL_ERROR;
			}
			IEccoScriptSystem::ScriptArgType type = cmd->required_args[i - 1];
			switch (type) {
			case IEccoScriptSystem::ScriptArgType::Int: {
				char* endptr = nullptr;
				long val = strtol(arg, &endptr, 10);
				if (*endptr != '\0') {
					Tcl_SetErrorCode(s_pTclinterp, "Argument not match the symbol", nullptr);
					Tcl_AddErrorInfo(s_pTclinterp, "Require argument: ");
					Tcl_AddErrorInfo(s_pTclinterp, cmd->args_str.c_str());
					return TCL_ERROR;
				}
				auto item = new IEccoScriptSystem::ScriptContent();
				item->type = IEccoScriptSystem::ScriptArgType::Int;
				item->intValue = static_cast<int>(val);
				args.push_back(item);
				break;
			}
			case IEccoScriptSystem::ScriptArgType::Double: {
				char* endptr = nullptr;
				double val = strtod(arg, &endptr);
				if (*endptr != '\0') {
					Tcl_SetErrorCode(s_pTclinterp, "Argument not match the symbol", nullptr);
					Tcl_AddErrorInfo(s_pTclinterp, "Require argument: ");
					Tcl_AddErrorInfo(s_pTclinterp, cmd->args_str.c_str());
					return TCL_ERROR;
				}
				auto item = new IEccoScriptSystem::ScriptContent();
				item->type = IEccoScriptSystem::ScriptArgType::Double;
				item->doubleValue = val;
				args.push_back(item);
				break;
			}
			case IEccoScriptSystem::ScriptArgType::Float: {
				char* endptr = nullptr;
				float val = strtof(arg, &endptr);
				if (*endptr != '\0') {
					Tcl_SetErrorCode(s_pTclinterp, "Argument not match the symbol", nullptr);
					Tcl_AddErrorInfo(s_pTclinterp, "Require argument: ");
					Tcl_AddErrorInfo(s_pTclinterp, cmd->args_str.c_str());
					return TCL_ERROR;
				}
				auto item = new IEccoScriptSystem::ScriptContent();
				item->type = IEccoScriptSystem::ScriptArgType::Float;
				item->floatValue = val;
				args.push_back(item);
				break;
			}
			case IEccoScriptSystem::ScriptArgType::Boolean: {
				std::string lowerArg = arg;
				std::transform(lowerArg.begin(), lowerArg.end(), lowerArg.begin(), ::tolower);
				bool val = (lowerArg == "1" || lowerArg == "true" || lowerArg == "yes");
				auto item = new IEccoScriptSystem::ScriptContent();
				item->type = IEccoScriptSystem::ScriptArgType::Boolean;
				item->boolValue = val;
				args.push_back(item);
				break;
			}
			default:
			case IEccoScriptSystem::ScriptArgType::String: {
				auto item = new IEccoScriptSystem::ScriptContent();
				item->type = IEccoScriptSystem::ScriptArgType::String;
				item->strValue = arg;
				args.push_back(item);
				break;
			}
			}
		}
		IEccoScriptSystem::ScriptContent* const* argv2 = args.data();
		auto ret = cmd->callback(cmd->interface, args.size(), argv2, cmd->user_args);
		for (auto arg : args) {
			delete arg;
		}
		return (int)ret;
	}, cmd, nullptr);
	s_mapCommands.emplace(name, cmd);
}
void CEccoScriptSystem::RemoveCommand(const char* name){
	auto it = s_mapCommands.find(name);
	if (it != s_mapCommands.end()) {
		Tcl_DeleteCommand(s_pTclinterp, name);
		delete it->second;
		s_mapCommands.erase(it);
	}
}
CEccoScriptSystem::Result CEccoScriptSystem::Eval(const char* content){
	auto result = Tcl_Eval(s_pTclinterp, content);
	if (result == TCL_ERROR) {
		const char* errorInfo = Tcl_GetVar(s_pTclinterp, "errorInfo", TCL_GLOBAL_ONLY);
		const char* errorCode = Tcl_GetVar(s_pTclinterp, "errorCode", TCL_GLOBAL_ONLY);
		LOG_DEVELOPER(PLID, "Error when eval script, code: %s\n%s\n", errorCode, errorInfo);
	}
	return (Result)result;
}
void CEccoScriptSystem::ThrowError(const char* code, const char* msg){
	Tcl_SetErrorCode(s_pTclinterp, code, nullptr);
	Tcl_AddErrorInfo(s_pTclinterp, msg);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewObject(){
	return Tcl_NewObj();
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewIntObject(int value){
	return Tcl_NewIntObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewStringObject(const char* value, size_t len){
	return Tcl_NewStringObj(value, len);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewDoubleObject(double value){
	return Tcl_NewDoubleObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewDictObject(){
	return Tcl_NewDictObj();
}
void CEccoScriptSystem::SetDictValue(Object* dict, const char* key, Object* value){
	Tcl_DictObjPut(s_pTclinterp, static_cast<Tcl_Obj*>(dict), Tcl_NewStringObj(key, -1), static_cast<Tcl_Obj*>(value));
}
IEccoScriptSystem::Object* CEccoScriptSystem::GetDictValue(Object* dict, const char* key){
	Tcl_Obj* value = nullptr;
	int result = Tcl_DictObjGet(s_pTclinterp, static_cast<Tcl_Obj*>(dict), Tcl_NewStringObj(key, -1), &value);
	if (result != TCL_OK || value == nullptr) {
		return nullptr;
	}
	return value;
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewBooleanObject(bool value){
	return Tcl_NewBooleanObj(value);
}
IEccoScriptSystem::Object* CEccoScriptSystem::NewListObject(size_t size, Object** list) {
	return Tcl_NewListObj(static_cast<int>(size), reinterpret_cast<Tcl_Obj**>(list));
}
IEccoScriptSystem::Object* CEccoScriptSystem::GetListObject(Object* list, size_t index){
	Tcl_Obj* elem = nullptr;
	int result = Tcl_ListObjIndex(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Size>(index), &elem);
	if (result != TCL_OK || elem == nullptr) {
		return nullptr;
	}
	return elem;
}

void CEccoScriptSystem::AppendListObject(Object* list, Object* obj){
	Tcl_ListObjAppendElement(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Obj*>(obj));
}

void CEccoScriptSystem::SetListObject(Object* list, size_t index, Object* obj){
	Tcl_Size listLen = 0;
	Tcl_ListObjLength(s_pTclinterp, static_cast<Tcl_Obj*>(list), &listLen);
	if (index >= static_cast<size_t>(listLen)) {
		for (Tcl_Size i = listLen; i <= static_cast<Tcl_Size>(index); ++i) {
			Tcl_ListObjAppendElement(s_pTclinterp, static_cast<Tcl_Obj*>(list), Tcl_NewObj());
		}
	}
	Tcl_Obj* objs[1] = { static_cast<Tcl_Obj*>(obj) };
	Tcl_ListObjReplace(s_pTclinterp, static_cast<Tcl_Obj*>(list), static_cast<Tcl_Size>(index), 1, 1, objs);
}

size_t CEccoScriptSystem::GetListSize(Object* list){
	Tcl_Size size = 0;
	Tcl_ListObjLength(s_pTclinterp, static_cast<Tcl_Obj*>(list), &size);
	return size;
}

void CEccoScriptSystem::SetObjectResult(Object* object){
	return Tcl_SetObjResult(s_pTclinterp, static_cast<Tcl_Obj*>(object));
}
void CEccoScriptSystem::SetValue(const char* name, Object* value){
	Tcl_SetVar2Ex(s_pTclinterp, name, nullptr, static_cast<Tcl_Obj*>(value), TCL_GLOBAL_ONLY);
}
void CEccoScriptSystem::UnsetValue(const char* name){
	Tcl_UnsetVar2(s_pTclinterp, name, nullptr, TCL_GLOBAL_ONLY);
}
IEccoScriptSystem::Object* CEccoScriptSystem::GetValue(const char* name){
	return Tcl_GetVar2Ex(s_pTclinterp, name, nullptr, TCL_GLOBAL_ONLY);
}
int CEccoScriptSystem::GetValueInt(Object* value){
	if (value == nullptr)
		return 0;
	int intValue = 0;
	Tcl_GetIntFromObj(s_pTclinterp, static_cast<Tcl_Obj*>(value), &intValue);
	return intValue;
}
const char* CEccoScriptSystem::GetValueString(Object* value, size_t* len){
	if (value != nullptr)
		return Tcl_GetStringFromObj(static_cast<Tcl_Obj*>(value), reinterpret_cast<int*>(len));
	return nullptr;
}
double CEccoScriptSystem::GetValueDouble(Object* value){
	if (value != nullptr) {
		double doubleValue = 0.0;
		Tcl_GetDoubleFromObj(s_pTclinterp, static_cast<Tcl_Obj*>(value), &doubleValue);
		return doubleValue;
	}
	return 0.0;
}
float CEccoScriptSystem::GetValueFloat(Object* value){
	if (value != nullptr) {
		double doubleValue = 0.0;
		Tcl_GetDoubleFromObj(s_pTclinterp, static_cast<Tcl_Obj*>(value), &doubleValue);
		return static_cast<float>(doubleValue);
	}
	return 0.0f;
}
bool CEccoScriptSystem::GetValueBoolean(Object* value){
	if (value != nullptr) {
		int boolValue = 0;
		Tcl_GetBooleanFromObj(s_pTclinterp, static_cast<Tcl_Obj*>(value), &boolValue);
		return boolValue != 0;
	}
	return false;
}
void* CEccoScriptSystem::GetScriptEngine() const{
	return s_pTclinterp;
}

void CEccoScriptSystem::ResetEnviroment(){
	Tcl_ResetResult(s_pTclinterp);
}