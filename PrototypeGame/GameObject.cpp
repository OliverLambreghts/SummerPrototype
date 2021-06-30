#include "pch.h"
#include "GameObject.h"
#include <algorithm>
#include "Component.h"
#include "BaseRenderComponent.h"

void GameObject::Update(float elapsedSec)
{
	std::for_each(m_pComponents.begin(), m_pComponents.end(), [elapsedSec, this](auto e)
		{
			e->Update(elapsedSec, *this);
		});
}

void GameObject::Render() const
{
	for (auto& comp : m_pComponents)
	{
		if (std::dynamic_pointer_cast<BaseRenderComponent>(comp))
			std::dynamic_pointer_cast<BaseRenderComponent>(comp)->Render();
	}
}

void GameObject::AddComponent(std::shared_ptr<Component> comp)
{
	m_pComponents.push_back(comp);
}