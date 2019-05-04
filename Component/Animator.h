#pragma once
#include "Behaviour.h"

class AnimationClip;

class Animator : public Behaviour {
	friend class AnimationManager;
public:
	void initialize(const COMPONENT_INIT_DESC & package) override;
	void Update(float _deltaTime);
	void Play();
	void Stop();
	bool IsRunning();

	AnimationClip * mClip;
	float mPlayTime = 0.0f;
	std::vector<DirectX::XMMATRIX> mAnimResult;
private:
	
	bool mIsRunning = true;
};