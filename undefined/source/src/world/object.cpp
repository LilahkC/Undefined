#include "world/object.h"

#include "world/scene_manager.h"

#include <random>

static std::random_device RANDOM_DEVICE;
static std::mt19937_64 ENGINE(RANDOM_DEVICE());
static std::uniform_int_distribution<uint64_t> UNIFORM_DISTRIBUTION;

Object::Object()
	: Name("Default"), mUUID(UNIFORM_DISTRIBUTION(ENGINE))
{
}

Object::Object(const std::string& mName)
	: Name(mName), mUUID(UNIFORM_DISTRIBUTION(ENGINE))
{
}

Object::~Object()
{
	for (Component* comp : Components)
	{
		delete comp;
	}
}

void Object::Enable()
{
	for (Component* comp : Components)
	{
		comp->OnEnable();
	}
	mIsEnable = true;
}

void Object::Disable()
{
	for (Component* comp : Components)
	{
		comp->OnDisable();
	}
	mIsEnable = false;
}

const bool Object::IsEnable() const
{
	return mIsEnable;
}

Component* Object::AddComponent(Component* comp)
{
	if (!comp)
	{
		Logger::Error("AddComponent(Component* comp) comp is nullptr");
		return nullptr;
	}

	comp->GameObject = this;
	comp->GameTransform = GameTransform;

	Components.push_back(comp);
	Logger::Info("Component {} added in object {}", typeid(*comp).name(), Name);

	return comp;
}

void Object::RemoveComponent(Component* comp)
{
	if (!comp)
	{
		Logger::Error("RemoveComponent(Component* comp) comp is nullptr");
		return;
	}

	int index = 0;
	for (Component* findComp : Components)
	{
		if (findComp == comp)
		{
			Components.erase(Components.begin() + index);
			Logger::Info("Component {} removed in object {}", typeid(*comp).name(), Name);
			delete comp;
			Components.shrink_to_fit();
			return;
		}
		index++;
	}
	Logger::Warning("Component to remove not found in object {}", Name);
}

const Object* Object::GetParent() const
{
	if (mParent == mRoot)
	{
		return nullptr;
	}
	return mParent;
}

void Object::SetParent(Object* parent)
{
	if ((mParent && mParent == parent) || this == parent)
	{
		return;
	}

	if (mParent)
	{
		mParent->DetachChild(this);
	}

	if (parent)
	{
		Object* current = parent;
		while (current->mParent != mRoot && current->mParent != nullptr)
		{
			if (current->mParent == this)
			{
				SetParent(nullptr);
				return;
			}

			current = current->mParent;
		}

		mParent = parent;
		mParent->mChildren.emplace_back(this);
		mParent->mChildrenUUIDs.emplace_back(mUUID);
		mTransform.mParentTransform = &parent->mTransform;
	}
	else
	{
		mParent = mRoot;
		mParent->mChildren.emplace_back(this);
		mParent->mChildrenUUIDs.emplace_back(mUUID);
		mTransform.mParentTransform = nullptr;
	}
}

const std::vector<Object*> Object::GetChildren() const
{
	return mChildren;
}

void Object::DetachChildren()
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		(*it)->mParent = mRoot;
	}
	mChildren.clear();
	mChildrenUUIDs.clear();
}

const Object* Object::GetChild(unsigned int index) const
{
	if (index > mChildren.size())
	{
		return nullptr;
	}
	auto it = mChildren.begin();
	std::advance(it, index);
	return *(it);
}

const Object* Object::GetChild(std::string mName) const
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		if ((*it)->Name == mName)
		{
			return (*it);
		}
	}
	return nullptr;
}

void Object::DetachChild(unsigned int index)
{
	if (index < mChildren.size())
	{
		return;
	}
	mChildren[index]->mParent = mRoot;
	mChildren[index]->mTransform.mParentTransform = nullptr;
	mChildren.erase(mChildren.begin() + index);
	mChildrenUUIDs.erase(mChildrenUUIDs.begin() + index);
}

void Object::DetachChild(std::string mName)
{
	unsigned int index = 0;
	for (Object* child : mChildren)
	{
		if (child->Name == mName)
		{
			child->mParent = mRoot;
			child->mTransform.mParentTransform = nullptr;
			mChildren.erase(mChildren.begin() + index);
			mChildrenUUIDs.erase(mChildrenUUIDs.begin() + index);
		}
		index++;
	}
}

void Object::DetachChild(Object* child)
{
	if (!child)
	{
		Logger::Warning("DetachChild(Object* child) argument invalid, child does not exist");
		return;
	}
	unsigned int index = 0;
	for (Object* currChild : mChildren)
	{
		if (currChild->mUUID == child->mUUID)
		{
			currChild->mParent = mRoot;
			currChild->mTransform.mParentTransform = nullptr;
			mChildren.erase(mChildren.begin() + index);
			mChildrenUUIDs.erase(mChildrenUUIDs.begin() + index);
			break;
		}
		index++;
	}
}

void Object::AtachChild(Object* child, unsigned int index)
{
	if (!child)
	{
		Logger::Warning("AtachChild(Object* child, unsigned int index) argument invalid, child does not exist");
		return;
	}
	child->SetParent(this);
	unsigned int findIndex = 0; // find object findIndex
	for (Object* find : child->mParent->mChildren)
	{
		if (find == child)
		{
			break;
		}
		findIndex++;
	}
	mChildren.erase(mChildren.begin() + findIndex, mChildren.begin() + findIndex + 1);
	mChildrenUUIDs.erase(mChildrenUUIDs.begin() + findIndex, mChildrenUUIDs.begin() + findIndex + 1);
	if (index > findIndex)
	{
		index--;
	}
	mChildren.insert(mChildren.begin() + index, child);
	mChildrenUUIDs.insert(mChildrenUUIDs.begin() + index, child->mUUID);
}

void Object::ResetPointerLink()
{
	for (Object* obj : SceneManager::ActualScene->Objects)
	{
		if (obj->Parent)
		{
			continue;
		}

		for (uint64_t childUUID : mChildrenUUIDs)
		{
			if (obj->mUUID == childUUID)
			{
				obj->SetParent(this);
				break;
			}
		}
	}
	
	for (size_t i = 0; i < Components.size(); i++)
	{
		Components[i]->mObject = this;
		Components[i]->mTransform = &mTransform;
	}
}

void Object::ChangeEnableStatus()
{
	if (mIsEnable)
	{
		Enable();
	}
	else
	{
		Disable();
	}
}
