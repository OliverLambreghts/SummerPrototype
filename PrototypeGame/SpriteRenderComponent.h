#pragma once
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include "BaseRenderComponent.h"
#include "Sprite.h"
class SpriteRenderComponent final : public BaseRenderComponent
{
public:
	enum class Direction
	{
		right,
		down,
		left,
		up
	};
	
	SpriteRenderComponent(const std::string& fileName, int nrCols, int nrRows, float frameSec, int rowToRender);
	virtual ~SpriteRenderComponent() override = default;
	SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
	SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
	void operator=(const SpriteRenderComponent& rhs) = delete;
	void operator=(const SpriteRenderComponent&& rhs) = delete;

	virtual void Update(float elapsedSec, GameObject& obj) override;
	virtual void Render() const override;

	void ChangeRenderDirection(Direction direction);
	void Idle();
	void Move();
	Sprite& GetSprite() const;
	Direction GetDirection() const;
	void ToggleMirror();
	bool IsMirrored() const;
private:
	std::vector<Direction> m_ActiveDirections;
	std::shared_ptr<Sprite> m_pSprite;
	Point2f m_RenderPos;
	int m_RowToRender;
	bool m_IsIdle;
	bool m_IsMirrored;
};
