#include "Animator.h"
#include "../AnimationClip.h"

void Animator::initialize(const COMPONENT_INIT_DESC & package)
{
	gameObject = package.mGameObj;
	Time = package.mTime;

	gameObject->renderer.animator = this;
	gameObject->renderer.drawSkinnedMesh = false;

}

void Animator::Update(float _deltaTime)
{
	if (mClip == nullptr) return;

	mClip->GetResultInTime(mPlayTime, &mAnimResult);
	//gameObject->renderer

	mPlayTime += _deltaTime / 10;
}

void Animator::Play()
{
	mIsRunning = true;
	if (mClip != nullptr) gameObject->renderer.drawSkinnedMesh = true;
	else gameObject->renderer.drawSkinnedMesh = false;
}

void Animator::Stop()
{
	mIsRunning = false;
	gameObject->renderer.drawSkinnedMesh = false;
}

void Animator::SetAnimClip(AnimationClip * _clip)
{
	mClip = _clip;
	if (mClip == nullptr) Stop();
	else Play();
		
}

AnimationClip * Animator::GetAnimClip()
{
	return mClip;
}

bool Animator::IsRunning()
{
	return mIsRunning;
}

Animator::~Animator()
{
	gameObject->renderer.animator = nullptr;
	gameObject->renderer.drawSkinnedMesh = false;
}
