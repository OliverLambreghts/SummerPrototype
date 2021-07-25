#include "pch.h"
#include "ConsumableComponent.h"

std::shared_ptr<GameObject> ConsumableComponent::Clone()
{
    return std::shared_ptr<GameObject>();
}

void ConsumableComponent::Update(float /*elapsedSec*/, GameObject& /*obj*/)
{
}

void ConsumableComponent::OnUse(std::shared_ptr<GameObject> /*player*/, std::shared_ptr<GameObject> /*enemy*/)
{
}

void ConsumableComponent::PrintStats()
{
}
