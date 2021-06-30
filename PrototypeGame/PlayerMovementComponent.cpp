#include "pch.h"
#include "PlayerMovementComponent.h"

#include <iostream>

PlayerMovementComponent::PlayerMovementComponent()
	: m_DirectionX{ DirectionX::none },
	m_Position{},
	m_Speed{ 200 },
	m_DirectionY{ DirectionY::none },
	m_State{ State::idle }
{

}

void PlayerMovementComponent::Update(float elapsedSec, GameObject& /*obj*/)
{
	if (m_State == State::idle)
		return;

	HandleXMovement(elapsedSec);
	HandleYMovement(elapsedSec);
}

void PlayerMovementComponent::HandleXMovement(float elapsedSec)
{
	switch (m_DirectionX)
	{
	case DirectionX::left:
		m_Position.x -= m_Speed * elapsedSec;
		break;
	case DirectionX::right:
		m_Position.x += m_Speed * elapsedSec;
		break;
	}
}

void PlayerMovementComponent::HandleYMovement(float elapsedSec)
{
	switch (m_DirectionY)
	{
	case DirectionY::down:
		m_Position.y -= m_Speed * elapsedSec;
		break;
	case DirectionY::up:
		m_Position.y += m_Speed * elapsedSec;
		break;
	}
}

void PlayerMovementComponent::Move(DirectionX* directionX, DirectionY* directionY)
{
	m_State = State::moving;
	if (directionX)
		m_DirectionX = *directionX;
	if (directionY)
		m_DirectionY = *directionY;

	if (m_DirectionX == DirectionX::none && m_DirectionY == DirectionY::none)
		m_State = State::idle;
}

void PlayerMovementComponent::SetSpeed(float speed)
{
	m_Speed = speed;
}

void PlayerMovementComponent::SetState(State state)
{
	m_State = state;
}

PlayerMovementComponent::State PlayerMovementComponent::GetState() const
{
	return m_State;
}

void PlayerMovementComponent::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

const Point2f& PlayerMovementComponent::GetPosition() const
{
	return m_Position;
}
