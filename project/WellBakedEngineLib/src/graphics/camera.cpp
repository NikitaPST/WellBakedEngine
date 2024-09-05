#include "graphics/camera.h"

namespace WBEngine
{
	Camera::Camera()
	{
		m_fPositionX = 0.0f;
		m_fPositionY = 0.0f;
		m_fPositionZ = 0.0f;

		m_fRotationX = 0.0f;
		m_fRotationY = 0.0f;
		m_fRotationZ = 0.0f;

		m_mView = DirectX::XMMatrixIdentity();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPosition(float fX, float fY, float fZ)
	{
		m_fPositionX = fX;
		m_fPositionY = fY;
		m_fPositionZ = fZ;
	}

	void Camera::SetRotation(float fX, float fY, float fZ)
	{
		m_fRotationX = fX;
		m_fRotationY = fY;
		m_fRotationZ = fZ;
	}

	void Camera::Render()
	{
		DirectX::XMVECTOR vUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR vPosition = DirectX::XMVectorSet(m_fPositionX, m_fPositionY, m_fPositionZ, 0.0f);
		DirectX::XMVECTOR vLookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		float fPitch = DirectX::XMConvertToRadians(m_fRotationX);
		float fYaw = DirectX::XMConvertToRadians(m_fRotationY);
		float fRoll = DirectX::XMConvertToRadians(m_fRotationZ);
		DirectX::XMMATRIX mRotation = DirectX::XMMatrixRotationRollPitchYaw(fPitch, fYaw, fRoll);

		vLookAt = DirectX::XMVector3TransformCoord(vLookAt, mRotation);
		vUp = DirectX::XMVector3TransformCoord(vUp, mRotation);
		vLookAt = DirectX::XMVectorAdd(vPosition, vLookAt);

		m_mView = DirectX::XMMatrixLookAtLH(vPosition, vLookAt, vUp);
	}

	DirectX::XMMATRIX Camera::GetViewMatrix()
	{
		return m_mView;
	}
}