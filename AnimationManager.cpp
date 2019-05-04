#include "AnimationManager.h"
#include "Component/Animator.h"

void AnimationManager::Update()
{
	typedef std::vector<Animator*>::iterator ITERATOR;

	ITERATOR begin = mAnimatorBuffer->begin();
	ITERATOR end = mAnimatorBuffer->end();

	for (ITERATOR it = begin; it != end; it++) {
		bool gameObjectValid = (*it)->gameObject->enabled;
		bool componentValid = (*it)->enabled;
		bool IsRunning = (*it)->mIsRunning;
		if (gameObjectValid && componentValid && IsRunning == false) continue;

		(*it)->Update(Time->GetDeltaTime());
	}
}