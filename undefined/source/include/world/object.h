#pragma once

#include <vector>

#include "utils/flag.h"

#include "world/component.h"
#include "engine_debug/logger.h"
#include "reflection/attributes.h"

template<class Comp>
concept ComponentType = std::is_base_of<Component, Comp>::value;

/// <summary>
/// Class Object
/// </summary>
class Object
{
public:
	Object();
	/// <summary>
	/// Constructor of Object
	/// </summary>
	/// <param name="name">: Name of the Object</param>
	Object(const std::string& mName);
	/// <summary>
	/// Destructor of Object
	/// </summary>
	~Object();

	/// <summary>
	/// Enable an Object
	/// </summary>
	void Enable();
	/// <summary>
	/// Disable an Object
	/// </summary>
	void Disable();
	/// <summary>
	/// Check if the Object is enable
	/// </summary>
	/// <returns>Return either true if it is enable or false</returns>
	const bool IsEnable() const;

	/// <summary>
	/// Add a component to the Object
	/// </summary>
	/// <typeparam name="Comp">: Component type</typeparam>
	/// <param name="...args">: Variadic parameter for all the components</param>
	/// <returns></returns>
	template <ComponentType Comp, typename... Args>
	Comp* AddComponent(Args... args)
	{
		Comp* comp = new Comp(args...);

		comp->GameObject = this;
		comp->GameTransform = GameTransform;

		Components.push_back(comp);
		Logger::Info("Component {} added in object {}", typeid(Comp).name(), Name);
		
		return comp;
	}
	Component* AddComponent(Component* comp);
	void RemoveComponent(Component* comp);

	/// <summary>
	/// Get a component
	/// </summary>
	/// <typeparam name="Comp">Type of the component you want to get</typeparam>
	/// <returns>Return a pointer to the component</returns>
	template <ComponentType Comp>
	Comp* GetComponent()
	{
		for (Component* findComp : Components)
		{
			if (typeid(*findComp) == typeid(Comp))
			{
				return (Comp*)findComp;
			}
		}

		return nullptr;
	}

	__declspec(property(get = GetTransform, put = SetTransform)) Transform* GameTransform;
	UNDEFINED_ENGINE Transform* GetTransform() { return &mTransform; };

	__declspec(property(get = GetParent, put = SetParent)) Object* Parent; // TODO: See if necessary and useful
	UNDEFINED_ENGINE const Object* GetParent() const;
	UNDEFINED_ENGINE void SetParent(Object* parent);

	/// <summary>
	/// Get all the Children on this Object 
	/// </summary>
	/// <returns>Return a list of pointer to the objects</returns>
	const std::vector<Object*> GetChildren() const;
	/// <summary>
	/// Detach this object to all of his children
	/// </summary>
	void DetachChildren();
	/// <summary>
	/// Get a child by his index
	/// </summary>
	/// <param name="index">: Index of the child</param>
	/// <returns>Return a pointer to the child</returns>
	const Object* GetChild(unsigned int index) const;
	/// <summary>
	/// Get a child by his name
	/// </summary>
	/// <param name="name">: Name of the child</param>
	/// <returns>Return a pointer to the child</returns>
	const Object* GetChild(std::string mName) const;
	/// <summary>
	/// Detach a child by his index
	/// </summary>
	/// <param name="index">: Index of the child</param>
	void DetachChild(unsigned int index);
	/// <summary>
	/// Detach a child by his name
	/// </summary>
	/// <param name="name">: Name of the child</param>
	void DetachChild(std::string mName);
	/// <summary>
	/// Detach a child by a pointer to the child
	/// </summary>
	/// <param name="child">: Pointer to the child</param>
	void DetachChild(Object* child);
	/// <summary>
	/// Atach a child Object at this index
	/// </summary>
	/// <param name="child">: Pointer to the child</param>
	void AtachChild(Object* child, unsigned int index);


	/// <summary>
	/// Name of the Object
	/// </summary>
	std::string Name = "empty";

	/// <summary>
	/// List of the Object's components
	/// </summary>
	std::vector<Component*> Components;

private:
	void SetTransform(Transform newTransform) { mTransform = newTransform; };
	void SetTransform(Transform* newTransform) { mTransform = *newTransform; };

	void ResetPointerLink();

	/// <summary>
	/// Transform of the Object
	/// </summary>
	Transform mTransform;
	/// <summary>
	/// Poibter to the parent of the Object
	/// </summary>
	Object* mParent;
	/// <summary>
	/// List of the children of the Object
	/// </summary>
	std::vector<Object*> mChildren;

	/// <summary>
	/// Universally Unique Identifier for the Object
	/// </summary>
	uint64_t mUUID;

	std::vector<uint64_t> mChildrenUUIDs;

	/// <summary>
	/// Boolean to know if the Object is enable
	/// </summary>
	bool mIsEnable = true;

	friend struct refl_impl::metadata::type_info__ <Object>;

private:
	void ChangeEnableStatus();

	friend class Application;
	friend class Scene;
	friend class SceneManager;
	friend class SceneGraph;

	static inline Object* mRoot;
};

REFL_AUTO(type(Object),
	field(mUUID, HideInInspector()),
	field(mIsEnable, DontDisplayName(), Callback(&Object::ChangeEnableStatus)),
	field(Name, SameLine()),
	field(mChildrenUUIDs, HideInInspector()),
	field(mTransform),
	field(Components, Spacing(ImVec2(0, 30)))
)