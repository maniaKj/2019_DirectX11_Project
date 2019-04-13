#pragma once
#include "Component.h"
#include "..\Graphics\Model.h"

class Transform : public Component {
public:
	const XMVECTOR & GetPositionVector() const;
	const XMFLOAT3 & GetPositionFloat3() const;
	const XMVECTOR & GetRotationVector() const;
	const XMFLOAT3 & GetRotationFloat3() const;

	void SetPosition(const XMVECTOR & pos);
	void SetPosition(const XMFLOAT3 & pos);
	void SetPosition(float x, float y, float z);

	void translate(const XMVECTOR & pos);
	void translate(float x, float y, float z);

	void SetRotation(const XMVECTOR & rot);
	void SetRotation(const XMFLOAT3 & rot);
	void SetRotation(float x, float y, float z);

	void rotate(const XMVECTOR & rot);
	void rotate(const XMFLOAT3 & rot);
	void rotate(float x, float y, float z);
	void SetScale(float xScale, float yScale, float zScale = 1.0f);

	void SetLookAtPos(XMFLOAT3 lookAtPos);

	const XMVECTOR & GetForwardVector(bool omitY = false);
	const XMVECTOR & GetRightVector(bool omitY = false);
	const XMVECTOR & GetLeftVector(bool omitY = false);
	const XMVECTOR & GetBackwardVector(bool omitY = false);
	const XMVECTOR & GetUpwardVector(bool omitY = false);

	const DirectX::XMMATRIX & GetWorldMatrix();

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
	XMVECTOR vec_upward;

	void UpdateMatrix();
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
protected:
	
	void UpdateDirectionVectors();

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR posVector;
	XMVECTOR rotVector;

	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;

	XMVECTOR vec_forward_noY;
	XMVECTOR vec_left_noY;
	XMVECTOR vec_right_noY;
	XMVECTOR vec_backward_noY;
	XMVECTOR vec_upward_noY;

	
};