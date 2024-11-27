#include "world/player_test.h"

#include "engine_debug/logger.h"

void Player::FixedUpdate()
{
	GameTransform->Position += {0, -0.2f, 0};
	Logger::Debug("{}", GameTransform->Position);
}
