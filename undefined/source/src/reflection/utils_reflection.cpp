#include "reflection/utils_reflection.h"
#include "reflection/runtime_classes.h"
#include "world/object.h"

Json::Value Reflection::WriteValueWithHash(void* val, size_t hash)
{
	return RuntimeClasses::WriteValue(val, hash);
}

void Reflection::DisplayWithHash(void* obj, size_t hash)
{
	RuntimeClasses::Display(obj, hash);
}

void* Reflection::ReadValueWithName(Json::Value jsonObj, std::string name)
{
	return RuntimeClasses::ReadObj(jsonObj, name);
}