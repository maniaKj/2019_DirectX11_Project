#include "Animator.h"
#include "../AnimationClip.h"

void Animator::initialize(const COMPONENT_INIT_DESC & package)
{
	gameObject = package.gameObj;
	Time = package.time;
}

void Animator::Update(float _deltaTime)
{
	mClip->GetResultInTime(mPlayTime, &mAnimResult);
	//gameObject->renderer

	mPlayTime += _deltaTime / 12;
}

void Animator::Play()
{
	mIsRunning = true;
}

void Animator::Stop()
{
	mIsRunning = false;
}

bool Animator::IsRunning()
{
	return mIsRunning;
}
