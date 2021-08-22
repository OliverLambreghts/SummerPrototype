#pragma once
#include <memory>

#include "Component.h"

class PlayerParticleEffectsComponent final : public Component
{
public:
	PlayerParticleEffectsComponent() = default;
	virtual ~PlayerParticleEffectsComponent() override = default;
	PlayerParticleEffectsComponent(const PlayerParticleEffectsComponent& other) = delete;
	PlayerParticleEffectsComponent(PlayerParticleEffectsComponent&& other) = delete;
	void operator=(const PlayerParticleEffectsComponent& rhs) = delete;
	void operator=(const PlayerParticleEffectsComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject & obj) override;

	void SetParticleEffect(const std::string& filePath, int rows, int cols, float frt, int rowToRender);
private:
	void CheckParticleEffect() const;
	void UpdateParticleEffect(GameObject& obj);
	
	std::shared_ptr<GameObject> m_pParticleEffect = nullptr;
	bool m_HasBeenSet = false;
};
