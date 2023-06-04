#include "Enemy.h"

EnemyId Enemy::idCounter = 0;

Enemy::Enemy(b2World* world, sf::Vector2f spawnPosition) : world(world), enemyId(idCounter++)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(spawnPosition.x, spawnPosition.y);

	body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(TILE_WIDTH / 2, TILE_HEIGHT / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 1.f;
	fixtureDef.shape = &shape;
	fixtureDef.userData.pointer = enemyId;
	fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::Enemy));

	body->CreateFixture(&fixtureDef);
	body->SetFixedRotation(true);

	texture.loadFromFile(ENEMY_TEXTURE_FILE_PATH);
	setTexture(texture);
	setTextureRect(sf::IntRect(0, 0, TILE_WIDTH, TILE_HEIGHT));
	setOrigin(TILE_WIDTH / 2, TILE_HEIGHT / 2);

	sensorCollisionMask.SetBit((uint16)CollisionBits::Enemy);
	sensorCollisionMask.SetBit((uint16)CollisionBits::EnemySensor);

	WorldContactListener::AddHandler(sensorCollisionMask, this);
}

Enemy::~Enemy()
{
	WorldContactListener::DeleteHandler(sensorCollisionMask, this);
}

void Enemy::Update()
{
	body->SetLinearVelocity(velocity);
	auto position = body->GetPosition();
	setPosition(position.x, position.y + 2);

	UpdateAnimation();
}

void Enemy::UpdateAnimation()
{
	++currentAnimFrame;
	if (currentAnimFrame >= ENEMY_ANIM_FRAMES)
	{
		currentAnimFrame = 0;
	}
	currentFrameTime += Time::DeltaTime();
	if (currentFrameTime >= ENEMY_ANIM_FRAME_TIMES)
	{
		if (faceDirection == FaceDirection::Right)
		{
			setTextureRect(sf::IntRect(FRAME_WIDTH * currentAnimFrame, currentAnimFrame, FRAME_WIDTH, FRAME_HEIGHT));
		}
		else
		{
			setTextureRect(sf::IntRect(FRAME_WIDTH * currentAnimFrame + FRAME_WIDTH, currentAnimFrame, -FRAME_WIDTH, FRAME_HEIGHT));
		}
		currentFrameTime = 0.f;
	}
}

void Enemy::Draw(sf::RenderWindow& window)
{
	window.draw(*this);
}

void Enemy::OnCollisionEnter(b2Contact* contact)
{
	uint16 collisionMask = contact->GetFixtureA()->GetFilterData().categoryBits | contact->GetFixtureB()->GetFilterData().categoryBits;
	if (collisionMask == sensorCollisionMask.GetMask())
	{
		if (contact->GetFixtureA()->GetUserData().pointer == enemyId || contact->GetFixtureB()->GetUserData().pointer == enemyId)
		{
			velocity.x *= -1;
			if (faceDirection == FaceDirection::Right)
			{
				faceDirection = FaceDirection::Left;
			}
			else
			{
				faceDirection = FaceDirection::Right;
			}
		}
	}
}
