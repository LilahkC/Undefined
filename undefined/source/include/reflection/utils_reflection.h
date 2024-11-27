#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <type_traits>
#include <refl.hpp>
#include <json/json.h>
#include <toolbox/Quaternion.h>
#include <toolbox/Vector3.h>
#include <toolbox/calc.h>

#include "engine_debug/logger.h"
#include "reflection/attributes.h"
#include "wrapper/reflection.h"
#include "resources/resource_manager.h"
#include "resources/texture.h"
#include "audio/sound_source.h"
#include "utils/utils.h"

namespace Reflection
{
	/// <summary>
	/// Display the variables we added to the reflection struct 
	/// </summary>
	/// <typeparam name="T"> base type </typeparam>
	/// <typeparam name="MemberT"> type of the variable we wish to reflect </typeparam>
	/// <typeparam name="DescriptorT"> metadata from the element we want to reflect </typeparam>
	/// <param name="obj"> object we want to display </param>
	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayObj(MemberT* obj);

	/// <summary>
	/// Value we wish to write into Json string
	/// </summary>
	/// <typeparam name="MemberT"> type of the value we want to write </typeparam>
	/// <param name="val"> value we write </param>
	template<typename MemberT>
	Json::Value WriteValue(MemberT* val);

	Json::Value WriteValueWithHash(void* val, size_t hash);

	/// <summary>
	/// Object we wish to write into Json string
	/// </summary>
	/// <typeparam name="T"> type of the object we want to write </typeparam>
	/// <param name="obj"> object we write </param>
	template<typename T>
	Json::Value WriteObj(T* obj);

	/// <summary>
	/// Value we wish to read from Json Value
	/// </summary>
	/// <param name="jsonValue"> object we write </param>
	template<typename MemberT>
	MemberT ReadValue(Json::Value jsonValue);

	void* ReadValueWithName(Json::Value jsonObj, std::string name);

		/// <summary>
	/// Object we wish to read from Json Value
	/// </summary>
	/// <param name="jsonObj"> object we write </param>
	template<typename T>
	T* ReadObj(Json::Value jsonObj);

	/// <summary>
	/// Object we wish to reflect
	/// </summary>
	/// <typeparam name="T"> type of the object we want to reflect </typeparam>
	/// <param name="obj"> object we reflect </param>
	template<typename T>
	void ReflectionObj(T* obj);

	/// <summary>
	/// Get a class that inherits from Component with it's hash code
	/// </summary>
	/// <param name="obj"> object we want to display </param>
	/// <param name="hash"> hash code of the class </param>
	void DisplayWithHash(void* obj, size_t hash);
	
	/// <summary>
	/// Display bases types in inspector
	/// </summary>
	/// <typeparam name="T"> base class </typeparam>
	/// <typeparam name="MemberT"> type of the variable </typeparam>
	/// <typeparam name="DescriptorT"> metadata from the element we want to reflect </typeparam>
	/// <param name="obj"> object we want to display </param>
	/// <param name="name"> name of the object we want to display </param>
	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayBaseTypes(MemberT* obj, std::string mName);

	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayStandardTypes(MemberT* obj, std::string mName);

	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayToolboxTypes(MemberT* obj, std::string mName);

	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayOurTypes(MemberT* obj, std::string name);

	template<typename T, typename MemberT, typename DescriptorT>
	void DisplayOurTypesSharedPtr(std::shared_ptr<MemberT>* obj, std::string mName);

	template<typename T, typename MemberT, typename DescriptorT>
	void Attributes(std::string& mName);
}

template<typename MemberT>
Json::Value Reflection::WriteValue(MemberT* val)
{
	Json::Value root;

	if constexpr (std::_Is_any_of_v<MemberT, bool, std::string> || std::is_integral_v<MemberT> || std::is_floating_point_v<MemberT>)
	{
		root = *val;
	}
	else if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4, Vector3, Vector2>)
	{
		root["x"] = val->x;
		root["y"] = val->y;
		if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4, Vector3>)
		{
			root["z"] = val->z;
		}
		if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4>)
		{
			root["w"] = val->w;
		}
	}
	else if constexpr (Reflection::is_vector_v<MemberT>)
	{
		using ListT = typename MemberT::value_type;

		//If it's a std::vector we reflect every element inside it
		for (int i = 0; i < val->size(); i++)
		{
			root[i] = WriteValue<ListT>(&(*val)[i]);
		}
	}
	else if constexpr (is_shared_ptr_v<MemberT> && std::_Is_any_of_v<MemberT, std::shared_ptr<Model>, std::shared_ptr<Texture>>)
	{
		root = val->get()->Name;
	}
	else if constexpr (std::is_pointer_v<MemberT> && std::is_abstract_v<std::remove_pointer_t<MemberT>>)
	{
		root = Reflection::WriteValueWithHash(*val, typeid(**val).hash_code());
	}
	else if constexpr (Reflection::IsReflectable<MemberT>())
	{
		root = Reflection::WriteObj<MemberT>(val);
	}

	return root;
}

template<typename T>
Json::Value Reflection::WriteObj(T* obj)
{
	constexpr Reflection::TypeDescriptor<T> descriptor = Reflection::Reflect<T>();

	Json::Value root;

	std::string type = typeid(T).name();
	type.erase(0, 6);

	root["Type"] = type;

	//Lamba that loops for each element we reflect from a class
	Reflection::ForEach(descriptor.members, [&]<typename DescriptorT>(const DescriptorT)
	{
		//We make sure that what we are trying to reflect isn't a function or a variable that has the attribute HideInInspector
		if constexpr (!Reflection::IsFunction<DescriptorT>())
		{
			//MemberT is the type of the element we reflect
			using MemberT = typename DescriptorT::value_type;

			const char* name = DescriptorT::name.c_str();
			MemberT value = DescriptorT::get(obj);

			root["Values"][name] = WriteValue<MemberT>(&value);
		}
	});

	return root;
}

template<typename MemberT>
MemberT Reflection::ReadValue(Json::Value jsonValue)
{
	if constexpr (std::_Is_any_of_v<MemberT, bool, std::string> || std::is_integral_v<MemberT> || std::is_floating_point_v<MemberT>)
	{
		return jsonValue.as<MemberT>();
	}
	else if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4, Vector3, Vector2>)
	{
		MemberT value;
		value.x = jsonValue.get("x", 0.0f).asFloat();
		value.y = jsonValue.get("y", 0.0f).asFloat();
		if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4, Vector3>)
		{
			value.z = jsonValue.get("z", 0.0f).asFloat();
		}
		if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4>)
		{
			value.w = jsonValue.get("w", 0.0f).asFloat();
		}
		return value;
	}
	else if constexpr (Reflection::is_vector_v<MemberT>)
	{
		using ListT = typename MemberT::value_type;

		MemberT valueList;
		valueList.resize(jsonValue.size());

		//If it's a std::vector we reflect every element inside it
		for (int i = 0; i < (int)jsonValue.size(); i++)
		{
			valueList[i] = ReadValue<ListT>(jsonValue[i]);
		}
		return valueList;
	}
	else if constexpr (is_shared_ptr_v<MemberT> && std::is_same_v<MemberT, std::shared_ptr<Model>>)
	{
		return ResourceManager::Get<Model>(jsonValue.asString());
	}
	else if constexpr (std::is_pointer_v<MemberT> && std::is_abstract_v<std::remove_pointer_t<MemberT>>)
	{
		std::string subJsonType = jsonValue.get("Type", std::string()).asString();
		return static_cast<MemberT>(ReadValueWithName(jsonValue, subJsonType));
	}
	else if constexpr (Reflection::IsReflectable<MemberT>())
	{
		std::string subJsonType = jsonValue.get("Type", std::string()).asString();
		if (subJsonType != ((std::string)(typeid(MemberT).name())).erase(0,6))
		{
			return MemberT();
		}

		return *ReadObj<MemberT>(jsonValue);
	}
}

template <typename T>
T* Reflection::ReadObj(Json::Value jsonObj)
{
	T* obj = new T();
	constexpr Reflection::TypeDescriptor<T> descriptor = Reflection::Reflect<T>();

	Json::Value jsonValues = jsonObj.get("Values", Json::Value());

	//Lamba that loops for each element we reflect from a class
	Reflection::ForEach(descriptor.members, [&]<typename DescriptorT>(const DescriptorT)
	{
		//We make sure that what we are trying to reflect isn't a function or a variable that has the attribute HideInInspector
		if constexpr (!Reflection::IsFunction<DescriptorT>())
		{
			//MemberT is the type of the element we reflect
			using MemberT = typename DescriptorT::value_type;

			const char* name = DescriptorT::name.c_str();

			DescriptorT::get(obj) = ReadValue<MemberT>(jsonValues.get(name, Json::Value()));
		}
	});

	return obj;
}

template<typename T>
void Reflection::ReflectionObj(T* obj)
{
	if constexpr (Reflection::IsReflectable<T>())
	{
		constexpr Reflection::TypeDescriptor<T> descriptor = Reflection::Reflect<T>();
		//Lamba that loops for each element we reflect from a class
		Reflection::ForEach(descriptor.members, [&]<typename DescriptorT>(const DescriptorT)
		{
			//We make sure that what we are trying to reflect isn't a function or a variable that has the attribute HideInInspector
			if constexpr (!Reflection::IsFunction<DescriptorT>() &&
				!HasAttribute<DescriptorT, HideInInspector>())
			{
				//MemberT is the type of the element we reflect
				using MemberT = typename DescriptorT::value_type;

				//If there's a change we put notify to true
				if constexpr (HasAttribute<DescriptorT, NotifyChange<T>>() || HasAttribute<DescriptorT, Callback<T>>())
				{
  					const MemberT oldValue = DescriptorT::get(obj);
					DisplayObj<T, MemberT, DescriptorT>(&DescriptorT::get(obj));

					if constexpr (HasAttribute<DescriptorT, NotifyChange<T>>())
					{
						if (oldValue != DescriptorT::get(obj))
						{
							constexpr NotifyChange<T> notify = GetAttribute<DescriptorT, NotifyChange<T>>();
							obj->*notify.ptr = true;
						}
					}
					else if constexpr (HasAttribute<DescriptorT, Callback<T>>())
					{
						if (oldValue != DescriptorT::get(obj))
						{
							constexpr Callback<T> notify = GetAttribute<DescriptorT, Callback<T>>();
							(obj->*notify.func)();
						}
					}
				}

				else
				{
					DisplayObj<T, MemberT, DescriptorT>(&DescriptorT::get(obj));
				}
			}
		});
	}
	else
	{
		Logger::Debug("{}", typeid(T).name());
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayBaseTypes(MemberT* obj, std::string mName)
{
	if constexpr (std::is_same_v<float, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_Float, obj, .1f);
	}
	else if constexpr (std::is_same_v<double, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_Double, obj, .1f);
	}
	else if constexpr (std::is_same_v<int8_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_S8, obj, .1f);
	}
	else if constexpr (std::is_same_v<int16_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_S16, obj, .1f);
	}
	else if constexpr (std::is_same_v<int, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_S32, obj, .1f);
	}
	else if constexpr (std::is_same_v<int64_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_S64, obj, .1f);
	}
	else if constexpr (std::is_same_v<uint8_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_U8, obj, .1f);
	}
	else if constexpr (std::is_same_v<uint16_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_U16, obj, .1f);
	}
	else if constexpr (std::is_same_v<unsigned int, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_U32, obj, .1f);
	}
	else if constexpr (std::is_same_v<uint64_t, MemberT>)
	{
		ImGui::DragScalar(mName.c_str(), ImGuiDataType_U64, obj, .1f);
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayStandardTypes(MemberT* obj, std::string mName)
{
	//We check if MemberT (type of the variable we are reflecting) is the same as one of the standard lib
	if constexpr (std::is_same_v<std::string, MemberT>)
	{
		ImGui::InputText(mName.c_str(), obj);
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayToolboxTypes(MemberT* obj, std::string mName)
{
	//We check if MemberT (type of the variable we are reflecting) is the same as one of the math toolbox
	if constexpr (std::is_same_v<Vector4, MemberT>)
	{
		//If the field has the attribute ToDeg we use a sliderAngle which convert radiants to degrees else we use DragFloat and send radiants directly
		if constexpr (HasAttribute<DescriptorT, ToDeg>())
		{
			ImGui::SliderAngle4(mName.c_str(), &obj->x);
		}
		else
		{
			ImGui::DragFloat4(mName.c_str(), &obj->x, .1f);
		}
	}
	else if constexpr (std::is_same_v<Vector3, MemberT>)
	{
		if constexpr (HasAttribute<DescriptorT, ToDeg>())
		{
			ImGui::SliderAngle3(mName.c_str(), &obj->x);
		}
		else
		{
			ImGui::DragFloat3(mName.c_str(), &obj->x, .1f);
		}
	}
	else if constexpr (std::is_same_v<Vector2, MemberT>)
	{
		if constexpr (HasAttribute<DescriptorT, ToDeg>())
		{
			ImGui::SliderAngle2(mName.c_str(), &obj->x);
		}
		else
		{
			ImGui::DragFloat2(mName.c_str(), &obj->x, .1f);
		}
	}
	else if constexpr (std::is_same_v<Quaternion, MemberT>)
	{
		Vector3 euler = obj->ToEuler();
		ImGui::SliderAngle3(mName.c_str(), &euler.x, .1f);
		Quaternion quat = Quaternion::GetRotationQuaternion(euler);
		obj->x = quat.x;
		obj->y = quat.y;
		obj->z = quat.z;
		obj->w = quat.w;
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayOurTypes(MemberT* obj, std::string name)
{
	if constexpr (std::is_same_v<SoundSource, MemberT>)
	{
		std::unordered_map<std::string, std::shared_ptr<MemberT>> resource = ResourceManager::GetType<MemberT>();

		if (ImGui::Button(name.c_str()))
		{
			ImGui::OpenPopup("audio_popup");
		}

		if (ImGui::BeginPopup("audio_popup"))
		{
			for (auto kv : resource)
			{
				if (ImGui::Selectable(kv.first.c_str()))
				{
					*obj = kv.second;
				}
			}
		}
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayOurTypesSharedPtr(std::shared_ptr<MemberT>* obj, std::string name)
{
	//We check if MemberT (type of the variable we are reflecting) is the same as one of our ResourceTypes
	if constexpr (std::is_same_v<Texture, MemberT>)
	{
		std::unordered_map<std::string, std::shared_ptr<MemberT>> resource = ResourceManager::GetType<MemberT>();
		if (ImGui::Button(name.c_str()))
		{
			ImGui::OpenPopup("resource_popup");
		}

		if (ImGui::BeginPopup("resource_popup"))
		{
			for (auto kv : resource)
			{
				if (ImGui::Selectable(kv.first.c_str()))
				{
					*obj = kv.second;
				}
			}
			ImGui::EndPopup();
		}
	}

	if constexpr (std::is_same_v<Model, MemberT>)
	{
		std::unordered_map<std::string, std::shared_ptr<MemberT>> resource = ResourceManager::GetType<MemberT>();

		if (ImGui::Button(name.c_str()))
		{
			ImGui::OpenPopup("model_popup");
		}

		if (ImGui::BeginPopup("model_popup"))
		{
			for (auto kv : resource)
			{
				if (ImGui::Selectable(kv.first.c_str()))
				{
					*obj = kv.second;
				}
			}
			ImGui::EndPopup();
		}
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::Attributes(std::string& mName)
{
	//If the Descriptor has a certain attribute
	if constexpr (HasAttribute<DescriptorT, DontDisplayName>())
	{
		mName = "##" + mName;
	}

	if constexpr (HasAttribute<DescriptorT, SameLine>())
	{
		ImGui::SameLine();
	}

	if constexpr (HasAttribute<DescriptorT, Spacing>())
	{
		ImGui::Dummy(GetAttribute<DescriptorT, Spacing>().size);
	}
}

template<typename T, typename MemberT, typename DescriptorT>
void Reflection::DisplayObj(MemberT* obj)
{
	ImGui::PushID(obj);

	std::string name = DescriptorT::name.c_str();

	Attributes<T, MemberT, DescriptorT>(name);

	//We check if MemberT (type of the variable we are reflecting) is the same as one of the bases types
	if constexpr (std::is_same_v<bool, MemberT>)
	{
		ImGui::Checkbox(name.c_str(), obj);
	}
	else if constexpr (std::is_integral_v<MemberT> || std::is_floating_point_v<MemberT>)
	{
		DisplayBaseTypes<T, MemberT, DescriptorT>(obj, name);
	}
	else if constexpr (std::_Is_any_of_v<MemberT, std::string>)
	{
		DisplayStandardTypes<T, MemberT, DescriptorT>(obj, name);
	}
	else if constexpr (std::_Is_any_of_v<MemberT, Quaternion, Vector4, Vector3, Vector2>)
	{
		DisplayToolboxTypes<T, MemberT, DescriptorT>(obj, name);
	}
	else if constexpr (std::_Is_any_of_v<MemberT, SoundSource>)
	{
		DisplayOurTypes<T, MemberT, DescriptorT>(obj, name);
	}
	else if constexpr (Reflection::is_vector_v<MemberT>)
	{
		using ListT = typename MemberT::value_type;

		//If it's a std::vector we reflect every element inside it
		for (size_t i = 0; i < obj->size(); i++)
		{
			Reflection::DisplayObj<T, ListT, DescriptorT>(&(*obj)[i]);
		}
	}

	else if constexpr (Reflection::is_shared_ptr_v<MemberT>)
	{
		using TypeT = typename MemberT::element_type;

		if constexpr (std::_Is_any_of_v <MemberT, std::shared_ptr<Texture>>)
		{
			DisplayOurTypesSharedPtr<T, TypeT, DescriptorT>(obj, name);
		}

		else if constexpr (std::_Is_any_of_v <MemberT, std::shared_ptr<Model>>)
		{
			if (obj->get() != nullptr)
			{
				DisplayObj<MemberT, TypeT, DescriptorT>(obj->get());
			}
			DisplayOurTypesSharedPtr<T, TypeT, DescriptorT>(obj, name);
		}

		else
		{
			//If it's a shared_ptr we reflect every element inside it
			DisplayObj<MemberT, TypeT, DescriptorT>(obj->get());
		}
	}

	else if constexpr (Reflection::is_pair_v<MemberT>)
	{
		using FirstT = typename MemberT::first_type;
		using SecondT = typename MemberT::second_type;

		if constexpr (Reflection::is_shared_ptr_v<FirstT>)
		{
			using FirstTT = typename FirstT::element_type;
			ReflectionObj<FirstTT>(obj->first.get());
		}
		else
		{
			ReflectionObj<FirstT>(obj->first);
		}

		if constexpr (Reflection::is_shared_ptr_v<SecondT>)
		{
			using SecondTT = typename SecondT::element_type;
			ReflectionObj<SecondTT>(obj->second.get());
		}
		else
		{
			ReflectionObj<SecondT>(obj->second);
		}
	}

	else if constexpr (std::is_pointer_v<MemberT> && std::is_abstract_v<std::remove_pointer_t<MemberT>>)
	{
		// Remove "class" from typeid().name()
		if (ImGui::CollapsingHeader(((std::string)typeid(**obj).name()).erase(0, 6).c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap))
		{
			ImGui::SameLine(ImGui::GetWindowWidth() - 13*2.82f, 0.0f);
			if (ImGui::ImageButton(Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/trash_can.png")->GetID()), ImVec2(13, 13), ImVec2(0.1f, 0.1f), ImVec2(0.9f, 0.9f)))
			{
				Component* comp = static_cast<Component*>(*obj);
				comp->GameObject->RemoveComponent(comp);
			}

			else
			{
				Reflection::DisplayWithHash(*obj, typeid(**obj).hash_code());
			}
		}
		else
		{
			ImGui::SameLine(ImGui::GetWindowWidth() - 13 * 2.82f, 0.0f);
			if (ImGui::ImageButton(Utils::IntToPointer<ImTextureID>(ResourceManager::Get<Texture>("imgui/trash_can.png")->GetID()), ImVec2(13, 13), ImVec2(0.15f, 0.15f), ImVec2(0.85f, 0.85f),-1, ImVec4(0,0,0,0), ImVec4(1, 0, 0, 1)))
			{
				Component* comp = static_cast<Component*>(*obj);
				comp->GameObject->RemoveComponent(comp);
			}
		}
		
	}
	//Recursivity if there's a reflectable type
	if constexpr (Reflection::IsReflectable<MemberT>())
	{
		ReflectionObj<MemberT>(obj);
	}

	ImGui::PopID();
}
