#pragma once

#include "utils/utils.h"
#include <unordered_map>
#include <functional>
#include "reflection/utils_reflection.h"
#include <vector>

struct RuntimeClass
{
	std::string className;

	std::function<void(void*)> display;
	std::function<Json::Value(void*)> write;
	std::function<void*(Json::Value)> read;
	std::function<void*()> create;
};

class RuntimeClasses
{
	STATIC_CLASS(RuntimeClasses)

public:
	template <typename T>
	static void AddClass();

	static void AddAllClasses();

	static const RuntimeClass* GetHashedClass(size_t hash);

	static void Display(void* obj, size_t hash);
	static Json::Value WriteValue(void* val, size_t hash);
	static void* ReadObj(Json::Value jsonObj, std::string name);
	static void* CreateClass(std::string name);

	static inline std::vector<std::string> names;
	
private:
	static inline std::unordered_map<size_t, RuntimeClass> mHashClasses;
};

template <typename T>
void RuntimeClasses::AddClass()
{
	if constexpr (Reflection::IsReflectable<T>())
	{
		const std::string className = refl::reflect<T>().name.c_str();
		RuntimeClass info = 
		{
			.className = className,
			.display = [](void* obj) -> void { Reflection::ReflectionObj<T>(static_cast<T*>(obj)); },
			.write = [](void* obj) -> Json::Value { return Reflection::WriteValue<T>(static_cast<T*>(obj)); },
			.read = [](Json::Value jsonObj) -> void* { return Reflection::ReadObj<T>(jsonObj); },
			.create = []() -> void* { return new T; }
		};

		if constexpr (!std::is_default_constructible_v<T>)
		{
			info.create = []() -> void* { return nullptr; };
		}

		names.push_back(className);

		mHashClasses.emplace(typeid(T).hash_code(), info);
	}
	else
	{
		Logger::Debug("{}", typeid(T).name());
	}
}
