#pragma once
#include "Component.h"
#include "GameObject_v2.h"
#include "../Timer.h"

class Behaviour : public Component {
public:

protected:
	template<class T>
	T* AddComponent();
	template<class T>
	T* GetComponent();
	
};

template<class T>
inline T * Behaviour::AddComponent()
{
	return gameObject->AddComponent<T>();
}

template<class T>
inline T * Behaviour::GetComponent()
{
	return gameObject->GetComponent<T>();
}