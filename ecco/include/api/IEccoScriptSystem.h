#pragma once
#include <cstddef>
class IEccoScriptSystem {
public:	
	using Object = void;
	enum class ScriptArgType {
		Unknown = 0,
		Int,
		String,
		Double,
		Float,
		Boolean
	};
	using ScriptContent = struct script_content_s {
		ScriptArgType type = ScriptArgType::Unknown;
		union {
			int intValue;
			const char* strValue;
			double doubleValue;
			float floatValue;
			bool boolValue;
		};
		operator int() const { return type == ScriptArgType::Int ? intValue : 0; }
		operator const char* () const { return type == ScriptArgType::String ? strValue : nullptr; }
		operator double() const { return type == ScriptArgType::Double ? doubleValue : 0.0; }
		operator float() const { return type == ScriptArgType::Float ? floatValue : 0.0f; }
		operator bool() const { return type == ScriptArgType::Boolean ? boolValue : false; }
	};
	enum class Result {
		OK = 0,
		Error,
		Return,
		Break,
		Continue
	};
	typedef Result(*fnFunc)(IEccoScriptSystem* interp, int argc, IEccoScriptSystem::ScriptContent* const* argv, void* user_args);
	
	/// <summary>
	/// Create a command in the script engine
	/// </summary>
	/// <param name="name">command name</param>
	/// <param name="callback">callback</param>
	virtual void CreateCommand(const char* name, const char* symbol, const char* description, fnFunc callback, void* user_args = nullptr) = 0;
	/// <summary>
	/// Remove a command from the script engine
	/// </summary>
	/// <param name="name">command name</param>
	virtual void RemoveCommand(const char* name) = 0;
	/// <summary>
	/// Eval a script content
	/// </summary>
	/// <param name="content">content</param>
	virtual Result Eval(const char* content) = 0;
	/// <summary>
	/// Thorw a script error with code and message
	/// </summary>
	/// <param name="code">error code</param>
	/// <param name="msg">error message</param>
	virtual void ThrowError(const char* code, const char* msg) = 0;
	/// <summary>
	/// Create a new object
	/// </summary>
	/// <returns>object</returns>
	virtual Object* NewObject() = 0;
	/// <summary>
	/// Create a new object with int value
	/// </summary>
	/// <param name="value">value</param>
	/// <returns>object</returns>
	virtual Object* NewIntObject(int value) = 0;
	/// <summary>
	/// Create a new object with string value
	/// </summary>
	/// <param name="value">value</param>
	/// <param name="len">length, -1 mean engine handle it</param>
	/// <returns>object</returns>
	virtual Object* NewStringObject(const char* value, size_t len) = 0;
	/// <summary>
	/// Create a new object with double value
	/// </summary>
	/// <param name="value">value</param>
	/// <returns>object</returns>
	virtual Object* NewDoubleObject(double value) = 0;
	/// <summary>
	/// Create a new object with dictionary value
	/// </summary>
	/// <returns>object</returns>
	virtual Object* NewDictObject() = 0;
	/// <summary>
	/// Set the dictionary object key-value pair
	/// </summary>
	/// <param name="dict">dictionary object</param>
	/// <param name="key">key name</param>
	/// <param name="value">value</param>
	virtual void SetDictValue(Object* dict, const char* key, Object* value) = 0;
	/// <summary>
	/// Get a value from dictionary object by key
	/// </summary>
	/// <param name="dict">dict</param>
	/// <param name="key">key</param>
	/// <returns></returns>
	virtual Object* GetDictValue(Object* dict, const char* key) = 0;
	/// <summary>
	/// Create a new object with boolean value
	/// </summary>
	/// <param name="value">value</param>
	/// <returns>object</returns>
	virtual Object* NewBooleanObject(bool value) = 0;

	/// <summary>
	/// Create a new list object with size
	/// </summary>
	/// <param name="size">size</param>
	/// <returns>object</returns>
	virtual Object* NewListObject(size_t size, Object** list) = 0;
	/// <summary>
	/// Get a list object item by index
	/// </summary>
	/// <param name="list">list</param>
	/// <param name="index">index</param>
	/// <returns>object</returns>
	virtual Object* GetListObject(Object* list, size_t index) = 0;
	/// <summary>
	/// Append a list object item
	/// </summary>
	/// <param name="list">list</param>
	/// <param name="obj">object</param>
	virtual void AppendListObject(Object* list, Object* obj) = 0;
	/// <summary>
	/// Set a list object item by index
	/// </summary>
	/// <param name="list">list</param>
	/// <param name="index">index</param>
	/// <param name="obj">object</param>
	virtual void SetListObject(Object* list, size_t index, Object* obj) = 0;
	/// <summary>
	/// Get the size of a list object
	/// </summary>
	/// <param name="list">list</param>
	/// <returns>size</returns>
	virtual size_t GetListSize(Object* list) = 0;

	/// <summary>
	/// Set the result of the script
	/// </summary>
	/// <param name="object">result</param>
	virtual void SetObjectResult(Object* object) = 0;

	/// <summary>
	/// Set a global variable value
	/// </summary>
	/// <param name="name">name</param>
	/// <param name="value">value</param>
	virtual void SetValue(const char* name, Object* value) = 0;
	/// <summary>
	/// Unset a global variable
	/// </summary>
	/// <param name="name">name</param>
	virtual void UnsetValue(const char* name) = 0;

	/// <summary>
	/// Reset whole enviroment
	/// </summary>
	virtual void ResetEnviroment() = 0;

	/// <summary>
	/// Get the script engine pointer
	/// </summary>
	/// <returns>script engine</returns>
	virtual void* GetScriptEngine() const = 0;
};