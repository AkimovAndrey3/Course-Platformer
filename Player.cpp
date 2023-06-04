#include "Player.h"

Player::Player()
{
	velocity = normalVelocity;

	itemCollisionMask.SetBit((uint16)CollisionBits::Player);
	itemCollisionMask.SetBit((uint16)CollisionBits::Item);

	WorldContactListener::AddHandler(itemCollisionMask, this);

	spikesCollisionMask.SetBit((uint16)CollisionBits::Player);
	spikesCollisionMask.SetBit((uint16)CollisionBits::Spikes);

	WorldContactListener::AddHandler(spikesCollisionMask, this);

	enemyCollisionMask.SetBit((uint16)CollisionBits::Player);
	enemyCollisionMask.SetBit((uint16)CollisionBits::Enemy);

	WorldContactListener::AddHandler(enemyCollisionMask, this);

	finishCollisionMask.SetBit((uint16)CollisionBits::Player);
	finishCollisionMask.SetBit((uint16)CollisionBits::FinishSensor);

	WorldContactListener::AddHandler(finishCollisionMask, this);

	texture.loadFromFile(PLAYER_TEXTURE_FILE_PATH);
	setTexture(texture);
	setTextureRect(sf::IntRect(0, 0, 32, 32));
	setOrigin(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);
	setScale(sf::Vector2f(2.f, 2.f));
}

Player::~Player()
{
	WorldContactListener::DeleteHandler(itemCollisionMask, this);
	WorldContactListener::DeleteHandler(spikesCollisionMask, this);
	WorldContactListener::DeleteHandler(enemyCollisionMask, this);
	WorldContactListener::DeleteHandler(finishCollisionMask, this);
}

void Player::OnCollisionEnter(b2Contact* contact)
{
	uint16 collisionMask = contact->GetFixtureA()->GetFilterData().categoryBits | contact->GetFixtureB()->GetFilterData().categoryBits;
	if (collisionMask == itemCollisionMask.GetMask())
	{
		WorldContactListener::AddBodyRemovable(contact->GetFixtureA());
		levelManager->DeleteObject(contact->GetFixtureA()->GetUserData().pointer);
		playerUi->CollectSweet();
	}
	if (collisionMask == spikesCollisionMask.GetMask() || collisionMask == enemyCollisionMask.GetMask())
	{
		if (!GODMODE)
		{
			playerUi->TakeDamage();
			if (playerUi->IsDied())
			{
				sceneManager->SwitchScene(SceneTypes::FinishScene);
			}
			isDamaged = true;
			levelManager->ReloadLevel();
		}
	}
	if (collisionMask == finishCollisionMask.GetMask())
	{
		if (Statistics::GetCollectedSweets() >= GAME_DIFFICULTY_SWEETS[(short)ISceneManager::GetDifficulty()])
		{
			Statistics::FinishGame();
			sceneManager->SwitchScene(SceneTypes::FinishScene);
		}
	}
}

void Player::Update()
{
	currentAnimState = AnimStates::Idle;
	auto input = Input::GetInputAxes();

	//Horizontal movement
	if (Input::IsKeyPressed(Input::Key::Horizontal))
	{
		currentAnimState = AnimStates::Move;
		CheckDirectionSwap(input);

		auto vel = playerBody->GetLinearVelocity();
		float deltaVel = velocity - abs(vel.x);
		vel = b2Vec2(input.x * Time::FixedDeltaTime() * deltaVel, vel.y);
		playerBody->SetLinearVelocity(vel);
	}

	//Sprint
	if (Input::IsKeyDown(Input::Key::LShift))
	{
		velocity = sprintVelocity;
	}
	if (Input::IsKeyUp(Input::Key::LShift))
	{
		velocity = normalVelocity;
	}


	// Jump
	if (Input::IsKeyDown(Input::Key::Space))
	{
		playerBody->SetGravityScale(preJumpGravityScale);

		auto vel = playerBody->GetLinearVelocity();
		float deltaVel = jumpSpeed - abs(vel.y);
		vel += b2Vec2(0, -deltaVel);
		playerBody->SetLinearVelocity(vel);
	}
	if (Input::IsKeyPressed(Input::Key::Space))
	{
		jumpTime += Time::DeltaTime();

		if (jumpTime > jumpMaxTime)
		{
			playerBody->SetGravityScale(normalGravityScale);
		}
	}
	if (Input::IsKeyUp(Input::Key::Space))
	{

		playerBody->SetGravityScale(normalGravityScale);
		jumpTime = 0.f;
	}


	b2Vec2 pos = playerBody->GetPosition();

	if (isDamaged)
	{
		playerBody->SetTransform(spawnPositon, 0.f);
		isDamaged = false;

	}

	setPosition(pos.x, pos.y);
	UpdateAnimation();
}

void Player::CheckDirectionSwap(sf::Vector2f& direction)
{
	if (direction.x != (float)lastFaceDirection && direction.x != 0.f)
	{
		if (direction.x < 0)
		{
			faceDirection = FaceDirection::Left;
		}
		else
		{
			faceDirection = FaceDirection::Right;
		}
		lastFaceDirection = faceDirection;
		levelManager->SwitchFaceDirection(faceDirection);
	}
}

void Player::UpdateAnimation()
{
	++currentAnimFrame;
	if (currentAnimFrame >= ANIM_FRAMES[(short)currentAnimState])
	{
		currentAnimFrame = 0;
	}
	currentFrameTime += Time::DeltaTime();
	if (currentFrameTime >= ANIM_FRAME_TIMES[(short)currentAnimState])
	{
		if (faceDirection == FaceDirection::Right)
		{
			setTextureRect(sf::IntRect(FRAME_WIDTH * currentAnimFrame, (short)currentAnimState * currentAnimFrame, FRAME_WIDTH, FRAME_HEIGHT));
		}
		else
		{
			setTextureRect(sf::IntRect(FRAME_WIDTH * currentAnimFrame + FRAME_WIDTH, (short)currentAnimState * currentAnimFrame, -FRAME_WIDTH, FRAME_HEIGHT));
		}
		currentFrameTime = 0.f;
	}
}

void Player::Draw(sf::RenderTarget& window)
{
	window.draw(*this);
}

void Player::CreateBody()
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(spawnPositon.x, spawnPositon.y);

	bodyDef.fixedRotation = true;
	playerBody = world->CreateBody(&bodyDef);

	{
		// collider
		b2PolygonShape shape;
		shape.SetAsBox(FRAME_WIDTH / 2, FRAME_HEIGHT / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 1.f;
		fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::Player));

		playerBody->CreateFixture(&fixtureDef);
	}

	{
		// switcher sensor
		b2PolygonShape shape;
		shape.SetAsBox(levelSwitcherSensorWidth, FRAME_HEIGHT / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::PlayerLevelSwitcher));

		playerBody->CreateFixture(&fixtureDef);
	}
}