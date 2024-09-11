#include "graphics.h"
#include "graphics/game_object.h"

namespace WBEngine
{
	GameObject::GameObject(Model* pModel)
	{
		m_pModel = pModel;

		m_mWorld = DirectX::XMMatrixIdentity();
	}

	GameObject::~GameObject()
	{
		m_pModel = nullptr;
	}

	bool GameObject::Render(Graphics* pGraphics, DirectX::XMMATRIX mView, DirectX::XMMATRIX mProjection)
	{
		m_pModel->Render(pGraphics->GetDirect3D()->GetDeviceContext());

		pGraphics->GetColorShader()->Render(pGraphics->GetDirect3D()->GetDeviceContext(), m_pModel->GetIndexCount(),
			m_mWorld, mView, mProjection);

		return true;
	}
}