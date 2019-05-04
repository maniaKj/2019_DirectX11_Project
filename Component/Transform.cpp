#include "Transform.h"
#include <DirectXMath.h>

using DirectX::operator+=;

void Transform::UpdateMatrix()
{
	this->worldMatrix = 
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
		* DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z)
		* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	this->UpdateDirectionVectors();
}

void Transform::initialize(const COMPONENT_INIT_DESC & package)
{
	gameObject = package.gameObj;
	Time = package.time;
}

const DirectX::XMVECTOR & Transform::GetPositionVector() const
{
	return this->posVector;
}

const DirectX::XMFLOAT3 & Transform::GetPositionFloat3() const
{
	return this->pos;
}

const DirectX::XMVECTOR & Transform::GetRotationVector() const
{
	return this->rotVector;
}

const DirectX::XMFLOAT3 & Transform::GetRotationFloat3() const
{
	return this->rot;
}

const DirectX::XMFLOAT3 & Transform::GetScaleFloat3() const
{
	return this->scale;
}

void Transform::SetPosition(const DirectX::XMVECTOR & pos)
{
	XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateMatrix();
}

void Transform::SetPosition(const DirectX::XMFLOAT3 & pos)
{

	this->pos = pos;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
	this->pos = DirectX::XMFLOAT3(x, y, z);
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateMatrix();
}

void Transform::translate(const DirectX::XMVECTOR & pos)
{
	this->posVector += pos;
	DirectX::XMStoreFloat3(&this->pos, this->posVector);
	this->UpdateMatrix();
}

void Transform::translate(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMVECTOR & rot)
{
	this->rotVector = rot;
	DirectX::XMStoreFloat3(&this->rot, rot);
	this->UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMFLOAT3 & rot)
{
	this->rot = rot;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void Transform::SetRotation(float x, float y, float z)
{
	this->rot = DirectX::XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void Transform::rotate(const DirectX::XMVECTOR & rot)
{
	this->rotVector += rot;
	DirectX::XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateMatrix();
}

void Transform::rotate(const DirectX::XMFLOAT3 & rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void Transform::rotate(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = XMLoadFloat3(&this->rot);
	this->UpdateMatrix();
}

void Transform::SetScale(float xScale, float yScale, float zScale)
{
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;
	UpdateMatrix();
}

void Transform::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
	//lookatpos 와 cam pos가 동일한지 확인. 이 둘을 같을 수가 없음.
	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
		return;

	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	//pitch 각도 구하기
	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f) {
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f) {
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0) yaw += DirectX::XM_PI;

	this->SetRotation(pitch, yaw, 0.0f);
}

void Transform::UpdateDirectionVectors() {
	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);
	this->vec_forward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_backward = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_right = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->vec_upward = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);

	DirectX::XMMATRIX vecRotationMatrixNoY = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_backward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_left_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
	this->vec_right_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);
	this->vec_upward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrixNoY);
}

const DirectX::XMVECTOR & Transform::GetForwardVector(bool omitY)
{
	if (omitY) return vec_forward_noY;
	return vec_forward;
}

const DirectX::XMVECTOR & Transform::GetRightVector(bool omitY)
{
	if (omitY) return vec_right_noY;
	return vec_right;
}

const DirectX::XMVECTOR & Transform::GetLeftVector(bool omitY)
{
	if (omitY) return vec_left_noY;
	return vec_left;
}

const DirectX::XMVECTOR & Transform::GetBackwardVector(bool omitY)
{
	if (omitY) return vec_backward_noY;
	return vec_backward;
}

const DirectX::XMVECTOR & Transform::GetUpwardVector(bool omitY)
{
	if (omitY) return vec_upward_noY;
	return vec_upward;
}

const DirectX::XMMATRIX & Transform::GetWorldMatrix()
{
	//행렬값 이상하게 나옴 assert 걸어두자
	
	return worldMatrix;
}
