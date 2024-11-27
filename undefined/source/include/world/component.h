#pragma once

#include <refl.hpp>

#include "utils/flag.h"
#include "world/transform.h"

class Object;

/// <summary>
/// Component Class which will be inherited by all our class that is a component
/// </summary>
class Component
{
public:
	/// <summary>
	/// Virtual Destructor of Component which could be override by our components
	/// </summary>
	virtual ~Component() = 0;

	/// <summary>
	/// Enable a component
	/// </summary>
	virtual void Enable();
	/// <summary>
	/// Disable a component
	/// </summary>
	virtual void Disable();

	/// <summary>
	/// Check if the component is enable
	/// </summary>
	/// <returns>Return either true if he is enable or false</returns>
	const bool IsEnable() const;
	
	/// <summary>
	/// Function triggered one time at the launch of the game
	/// </summary>
	virtual void Start();

	/// <summary>
	/// Update fix at 60 fps call after FixedUpdate
	/// </summary>
	virtual void PreFixedUpdate();

	/// <summary>
	/// Update fix at 60fps (mainly use for physics)
	/// </summary>
	virtual void FixedUpdate();

	/// <summary>
	/// Update fix at 60 fps call before FixedUpdate
	/// </summary>
	virtual void PostFixedUpdate();

	/// <summary>
	/// Update function 
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Update function call after the Update()
	/// </summary>
	virtual void LateUpdate();

	/// <summary>
	/// Draw function
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// Post Draw 
	/// </summary>
	virtual void PostDraw();

	/// <summary>
	/// Function triggered one time when the component is enabled
	/// </summary>
	virtual void OnEnable();

	/// <summary>
	/// Function triggered one time when the component is disabled
	/// </summary>
	virtual void OnDisable();

	__declspec(property(get = GetObject, put = SetObject)) Object* GameObject;
	Object* GetObject() const { return mObject; };

	__declspec(property(get = GetTransform, put = SetTransform)) Transform* GameTransform;
	Transform* GetTransform() const { return mTransform; };

private:
	void SetObject(Object* newObject) { mObject = newObject; };
	void SetTransform(Transform* newTransform) { mTransform = newTransform; };

	/// <summary>
	/// Boolean to know if the component is enable
	/// </summary>
	bool mIsEnable = true;
	
	/// <summary>
	/// Pointer to the object that contains the component
	/// </summary>
	Object* mObject;

	/// <summary>
	/// Pointer to the transform of the object that contains the component
	/// </summary>
	Transform* mTransform;

private:
	/// <summary>
	/// Object is a friend class from component
	/// </summary>
	friend class Object;

	friend struct refl_impl::metadata::type_info__ <Component>;
};

REFL_AUTO(type(Component),
	field(mIsEnable)
);