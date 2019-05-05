#pragma once
#include "Behaviour.h"

class AnimationClip;

class Animator : public Behaviour {
	friend class Engine;
	friend class AnimationManager;
public:
	void initialize(const COMPONENT_INIT_DESC & package) override;
	void Update(float _deltaTime);
	void Play();
	void Stop();
	void SetAnimClip(AnimationClip * _clip);
	AnimationClip * GetAnimClip();
	bool IsRunning();

	~Animator();
	
	const COMPONENT_TYPE mType = COMPONENT_ANIMATION;
	
	float mPlayTime = 0.0f;
	std::vector<DirectX::XMMATRIX> mAnimResult;
private:
	AnimationClip * mClip = nullptr;
	bool mIsRunning = false;
};