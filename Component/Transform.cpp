#include "Transform.h"
#include <DirectXMath.h>

using DirectX::operator+=;

DirectX::XMFLOAT3 QuatenionToEuler(float x, float y, float z, float w) {
	float roll;
	float pitch;
	float yaw;

	// roll (x-axis rotation)
	double sinr_cosp = +2.0 * (w * x + y * z);
	double cosr_cosp = +1.0 - 2.0 * (x * x + y * y);
	roll = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (w * y - y * x);
	if (fabs(sinp) >= 1)
		pitch = copysign(3.14f / 2, sinp); // use 90 degrees if out of range
	else
		pitch = asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = +2.0 * (w * y + x * y);
	double cosy_cosp = +1.0 - 2.0 * (y * y + y * y);
	yaw = atan2(siny_cosp, cosy_cosp);

	return DirectX::XMFLOAT3(roll, pitch, yaw);
}


void Transform::UpdateMatrix()
{
	TRANSFORM_UPDATED = true;

	this->worldMatrix = 
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
		* DirectX::XMMatrixRotationQuaternion(quaternion)
		* DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	//this->UpdateDirectionVectors();
}

void Transform::initialize(const COMPONENT_INIT_DESC & desc)
{
	gameObject = desc.mGameObj;
	Time = desc.mTime;
}

const DirectX::XMVECTOR Transform::GetPositionVector() const
{
	return DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f);
}

const DirectX::XMFLOAT3 & Transform::GetPositionFloat3() const
{
	return this->position;
}

const DirectX::XMVECTOR Transform::GetRotationVector() const
{
	return DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, 0.0f);
}

const DirectX::XMFLOAT3 & Transform::GetRotationFloat3() const
{
	return this->rotation;
}

const DirectX::XMVECTOR & Transform::GetQuaternion() const
{
	return quaternion;
}

const DirectX::XMFLOAT3 & Transform::GetScaleFloat3() const
{
	return this->scale;
}

void Transform::SetPosition(const DirectX::XMVECTOR & pos)
{
	XMStoreFloat3(&this->position, pos);
	this->UpdateMatrix();
}

void Transform::SetPosition(const DirectX::XMFLOAT3 & pos)
{

	this->position = pos;
	this->UpdateMatrix();
}

void Transform::SetPosition(float x, float y, float z)
{
	this->position = DirectX::XMFLOAT3(x, y, z);
	this->UpdateMatrix();
}

void Transform::translate(const DirectX::XMVECTOR & _pos)
{
	DirectX::XMFLOAT3 translation;
	DirectX::XMStoreFloat3(&translation, _pos);
	position.x += translation.x;
	position.y += translation.y;
	position.z += translation.z;
	this->UpdateMatrix();
}

void Transform::translate(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMVECTOR & rot)
{
	DirectX::XMStoreFloat3(&rotation, rot);
	quaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(rot);
	this->UpdateMatrix();
}

void Transform::SetRotation(const DirectX::XMFLOAT3 & rot)
{
	rotation = rot;
	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	this->UpdateMatrix();
}

void Transform::SetRotation(float x, float y, float z)
{
	rotation = DirectX::XMFLOAT3(x, y, z);
	quaternion = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
	this->UpdateMatrix();
}

void Transform::SetRotation(DirectX::XMVECTOR & _quat)
{
	this->quaternion = _quat;
	//quaternion -> euler 변경할 적합한 방법이 안 떠오름
	DirectX::XMFLOAT4 quatFloat;
	DirectX::XMStoreFloat4(&quatFloat, quaternion);
	rotation = QuatenionToEuler(quatFloat.x, quatFloat.y, quatFloat.z, quatFloat.w);
	this->UpdateMatrix();
}

void Transform::rotate(const DirectX::XMVECTOR & rot)
{
	DirectX::XMVECTOR rotating = DirectX::XMQuaternionRotationRollPitchYawFromVector(rot);
	quaternion = DirectX::XMQuaternionMultiply(rotating, quaternion);

	DirectX::XMFLOAT4 quatFloat;
	DirectX::XMStoreFloat4(&quatFloat, quaternion);
	rotation = QuatenionToEuler(quatFloat.x, quatFloat.y, quatFloat.z, quatFloat.w);
	this->UpdateMatrix();
}

void Transform::rotate(const DirectX::XMFLOAT3 & rot)
{
	DirectX::XMVECTOR rotating = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);
	quaternion = DirectX::XMQuaternionMultiply(rotating, quaternion);

	DirectX::XMFLOAT4 quatFloat;
	DirectX::XMStoreFloat4(&quatFloat, quaternion);
	rotation = QuatenionToEuler(quatFloat.x, quatFloat.y, quatFloat.z, quatFloat.w);
	this->UpdateMatrix();
}

void Transform::rotate(float x, float y, float z)
{
	DirectX::XMVECTOR rotating = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
	quaternion = DirectX::XMQuaternionMultiply(rotating, quaternion);

	DirectX::XMFLOAT4 quatFloat;
	DirectX::XMStoreFloat4(&quatFloat, quaternion);
	rotation = QuatenionToEuler(quatFloat.x, quatFloat.y, quatFloat.z, quatFloat.w);
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
	if (lookAtPos.x == position.x && lookAtPos.y == position.y && lookAtPos.z == position.z)
		return;

	lookAtPos.x = position.x - lookAtPos.x;
	lookAtPos.y = position.y - lookAtPos.y;
	lookAtPos.z = position.z - lookAtPos.z;

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
	//안써요
	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, 0.0f);
	this->vec_forward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	//this->vec_backward = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_left = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	//this->vec_right = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->vec_upward = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);

	DirectX::XMMATRIX vecRotationMatrixNoY = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rotation.y, 0.0f);
	this->vec_forward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_backward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrixNoY);
	this->vec_left_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrixNoY);
	this->vec_right_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrixNoY);
	this->vec_upward_noY = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrixNoY);
}

const DirectX::XMVECTOR & Transform::GetForwardVector(bool omitY)
{
	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
	vec_forward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	if (omitY) return vec_forward_noY;
	return vec_forward;
}

const DirectX::XMVECTOR & Transform::GetRightVector(bool omitY)
{
	//미구현
	if (omitY) return vec_right_noY;
	return vec_right;
}

const DirectX::XMVECTOR & Transform::GetLeftVector(bool omitY)
{
	//미구현
	if (omitY) return vec_left_noY;
	return vec_left;
}

const DirectX::XMVECTOR & Transform::GetBackwardVector(bool omitY)
{
	//미구현
	if (omitY) return vec_backward_noY;
	return vec_backward;
}

const DirectX::XMVECTOR & Transform::GetUpwardVector(bool omitY)
{
	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
	vec_upward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	if (omitY) return vec_upward_noY;
	return vec_upward;
}

const DirectX::XMMATRIX & Transform::GetWorldMatrix()
{
	//행렬값 이상하게 나옴 assert 걸어두자
	
	return worldMatrix;
}
