#ifndef _CAMERA_H
#define _CAMERA_H

#include <DirectXMath.h>

namespace WBEngine
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetPosition(float fX, float fY, float fZ);
		void SetRotation(float fX, float fY, float fZ);

		void Render();
		DirectX::XMMATRIX GetViewMatrix();

	private:
		float m_fPositionX;
		float m_fPositionY;
		float m_fPositionZ;

		float m_fRotationX;
		float m_fRotationY;
		float m_fRotationZ;

		DirectX::XMMATRIX m_mView;
	};
}

#endif