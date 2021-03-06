#include "Sophia.h"
#include "../Core/Coordinator.h"
#include "../Component/PositionComponent.h"
#include "../Component/SpeedComponent.h"
#include "../Component/AnimationComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/TransformationComponent.h"
#include "../Component/DirectionComponent.h"
#include "../HelperHeader/UtilHeader.h"
#include "../System/AnimationSystem.h"
#include "../System/SpriteSystem.h"
#include "../System/MovementSystem.h"
#include "../HelperHeader/Debug.h"

Sophia::Sophia(std::shared_ptr<Coordinator> coordinator) {
    this->coordinator = coordinator;

    entityID = coordinator->CreateEntity();
    Position Pos,temp;
    Pos.x = 64;  
    Pos.y = 64;

    coordinator->AddComponent<Position>(entityID, Pos, ComponentType::Position);

    bodyID = coordinator->CreateEntity();
    coordinator->AddComponent<Position>(bodyID, Pos, ComponentType::Position);
    Animation bodyAni;
    bodyAni.textureID = BODY_SOPHIA;
    bodyAni.delayValue = 1000;
	bodyAni.isFinished = false;
    State state;
    state.endFrame= 0;
    state.startFrame = 0;
    state.isLoopable = true;
    bodyAni.stateDictionary.emplace(BODY_CAR_IDLE_LEFT, state);

    state.endFrame = 2;
    state.startFrame = 2;
    state.isLoopable = true;
    bodyAni.stateDictionary.emplace(BODY_CAR_IDLE_RIGHT, state);

    state.startFrame = 0;
    state.endFrame = 2;
    state.isLoopable = false;
    bodyAni.stateDictionary.emplace(CAR_CHANGE_DIRECTION, state);
    bodyAni.currentState = BODY_CAR_IDLE_LEFT;
	bodyAni.currentFrame = 0;
    bodyAni.defaultState = BODY_CAR_IDLE_LEFT;
    coordinator->AddComponent<Animation>(bodyID, bodyAni, ComponentType::Animation);
 
    Transformation transform;
    D3DXMatrixTransformation2D(
        &transform.transformMatrix,
        NULL,
        NULL,
        NULL,
        //rotation for aim up sophia
       // &D3DXVECTOR2(90,58),
        NULL,
        0, //radiant
        &D3DXVECTOR2(5, 0)
    );
    coordinator->AddComponent<Transformation>(bodyID, transform, ComponentType::Transformation);
    coordinator->GetSystem<AnimationSystem>(SystemType::Animation)->AddEntity(bodyID);

    axelID = coordinator->CreateEntity();
    temp.x = Pos.x + 4; //Entity Pos.x + 4
    temp.y = Pos.y + 8; //Entity Pos.y + 8
    Sprite sprite;
    sprite.spriteID = 0;
    sprite.textureID = AXEL_SOPHIA;
    coordinator->AddComponent<Position>(axelID, temp, ComponentType::Position);
    coordinator->AddComponent<Sprite>(axelID, sprite, ComponentType::Sprite);

    coordinator->GetSystem<SpriteSystem>(SystemType::Sprite)->AddEntity(axelID);

    wheel1ID = coordinator->CreateEntity();
    Sprite wheelSprite;
    wheelSprite.textureID = WHEEL_SOPHIA;
    wheelSprite.spriteID = 1;
    coordinator->AddComponent<Position>(wheel1ID, Pos, ComponentType::Position);
    coordinator->AddComponent<Sprite>(wheel1ID, wheelSprite, ComponentType::Sprite);

    Transformation wheel1Transform;
    D3DXMatrixTransformation2D(
        &wheel1Transform.transformMatrix,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        &D3DXVECTOR2(-3, 9)
    );
    coordinator->AddComponent<Transformation>(wheel1ID, wheel1Transform, ComponentType::Transformation);

    coordinator->GetSystem<SpriteSystem>(SystemType::Sprite)->AddEntity(wheel1ID);

    wheel2ID = coordinator->CreateEntity();
    Sprite wheelSprite2;
    wheelSprite2.textureID = WHEEL_SOPHIA;
    wheelSprite2.spriteID = 2;
    coordinator->AddComponent<Position>(wheel2ID, Pos, ComponentType::Position);
    coordinator->AddComponent<Sprite>(wheel2ID, wheelSprite2, ComponentType::Sprite);

    Transformation wheel2Transform;
    D3DXMatrixTransformation2D(
        &wheel2Transform.transformMatrix,
        NULL,
        NULL,
        NULL,
        NULL,
        0,
        &D3DXVECTOR2(12, 9)
    );
    coordinator->AddComponent<Transformation>(wheel2ID, wheel2Transform, ComponentType::Transformation);

    coordinator->GetSystem<SpriteSystem>(SystemType::Sprite)->AddEntity(wheel2ID);

    barrelID = coordinator->CreateEntity();
    temp.x = Pos.x - 3; //Entity Pos.x - 3
    temp.y = Pos.y; //Entity Pos.y
    coordinator->AddComponent<Position>(barrelID, temp, ComponentType::Position);
    Animation barrelAni;
    barrelAni.textureID = BARREL_SOPHIA;
    bodyAni.delayValue = 1000;
    bodyAni.isFinished = false;
    State barrelState;
    barrelState.endFrame = 0;
    barrelState.startFrame = 0;
    barrelState.isLoopable = true;
    barrelAni.stateDictionary.emplace(BARREL_IDLE_RIGHT, barrelState);

    barrelState.endFrame = 3;
    barrelState.startFrame = 3;
    barrelState.isLoopable = true;
    barrelAni.stateDictionary.emplace(BARREL_IDLE_LEFT, barrelState);

    barrelState.endFrame = 2;
    barrelState.startFrame = 2;
    barrelState.isLoopable = true;
    barrelAni.stateDictionary.emplace(BARREL_IDLE_TOP, barrelState);

    barrelState.endFrame = 2;
    barrelState.startFrame = 0;
    barrelState.isLoopable = false;
    barrelAni.stateDictionary.emplace(BARREL_LOOK_UP_RIGHT, barrelState);

    barrelState.endFrame = 5;
    barrelState.startFrame = 3;
    barrelState.isLoopable = false;
    barrelAni.stateDictionary.emplace(BARREL_LOOK_UP_LEFT, barrelState);
    barrelAni.currentState = BARREL_IDLE_LEFT;
    barrelAni.currentFrame = 3;
    barrelAni.defaultState = BARREL_IDLE_LEFT;
    coordinator->AddComponent<Animation>(barrelID, barrelAni, ComponentType::Animation);
    coordinator->GetSystem<AnimationSystem>(SystemType::Animation)->AddEntity(barrelID);

    Direction dir;
    coordinator->AddComponent<Direction>(entityID, dir, ComponentType::Direction);

    Velocity velocity;
    velocity.vx = 0;
    velocity.vy = 0;

    coordinator->AddComponent<Velocity>(entityID, velocity, ComponentType::Speed);

    coordinator->GetSystem<MovementSystem>(SystemType::Movement)->AddEntity(entityID);

    currentState = Sophia::Idle_Left;
}

int Sophia::GetID() {
    return entityID;
}

void Sophia::Test()
{
    DebugOut(L"This is Sophia\n");
}

void Sophia::PartPosUpdate()
{
    Position& entityPos = coordinator->GetComponent<Position>(entityID, ComponentType::Position);

    Position& wheel1Pos = coordinator->GetComponent<Position>(wheel1ID, ComponentType::Position);
    Position& wheel2Pos = coordinator->GetComponent<Position>(wheel2ID, ComponentType::Position);
    Position& bodyPos = coordinator->GetComponent<Position>(bodyID, ComponentType::Position);
    Position& barrelPos = coordinator->GetComponent<Position>(barrelID, ComponentType::Position);
    Position& axelPos = coordinator->GetComponent<Position>(axelID, ComponentType::Position);

    wheel1Pos = entityPos;
    wheel2Pos = entityPos;

    bodyPos = entityPos;

    barrelPos.x = entityPos.x - 3;
    barrelPos.y = entityPos.y;

    axelPos.x = entityPos.x + 4;
    axelPos.y = entityPos.y + 8;

}

void Sophia::SwitchState(int aniID)
{
    DebugOut(L"aniID %d\n   ", aniID);
    if (aniID == (int)currentState) return; 
    Animation& animationNeedToSwap = coordinator->GetComponent<Animation>(bodyID, ComponentType::Animation);

    switch (aniID)
    {
    case (int)Sophia::Idle_Left:
    {
        
        Animation sophia_Ani_Idle_Left;
        sophia_Ani_Idle_Left.textureID = BODY_SOPHIA;
        sophia_Ani_Idle_Left.delayValue = 100;
        sophia_Ani_Idle_Left.isFinished = false;

        State sophiaState;
        sophiaState.endFrame = 0;
        sophiaState.startFrame = 0;
        sophiaState.isLoopable = true;
        sophia_Ani_Idle_Left.stateDictionary.emplace(BODY_CAR_IDLE_LEFT, sophiaState);

        sophia_Ani_Idle_Left.currentState = BODY_CAR_IDLE_LEFT;
        sophia_Ani_Idle_Left.currentFrame = 0;
        sophia_Ani_Idle_Left.defaultState = BODY_CAR_IDLE_LEFT;

        animationNeedToSwap = sophia_Ani_Idle_Left;
        currentState = Sophia::Idle_Left;
    }
    break;
    case (int)Sophia::Idle_Right:
    {
        DebugOut(L"Idling Right\n");
        Animation sophia_Ani_Idle_Right;
        sophia_Ani_Idle_Right.textureID = BODY_SOPHIA;
        sophia_Ani_Idle_Right.delayValue = 100;
        sophia_Ani_Idle_Right.isFinished = false;

        State sophiaState;
        sophiaState.endFrame = 2;
        sophiaState.startFrame = 2;
        sophiaState.isLoopable = true;
        sophia_Ani_Idle_Right.stateDictionary.emplace(BODY_CAR_IDLE_RIGHT, sophiaState);

        sophia_Ani_Idle_Right.currentState = BODY_CAR_IDLE_RIGHT;
        sophia_Ani_Idle_Right.currentFrame = 2;
        sophia_Ani_Idle_Right.defaultState = BODY_CAR_IDLE_RIGHT;

        animationNeedToSwap = sophia_Ani_Idle_Right;
        currentState = Sophia::Idle_Right;
    }
    break;
    case (int)Sophia::Go_Left:
    {
        Sprite& sprite1wheel = coordinator->GetComponent<Sprite>(wheel1ID, ComponentType::Sprite);
        Sprite& sprite2wheel = coordinator->GetComponent<Sprite>(wheel2ID, ComponentType::Sprite);
        sprite1wheel.textureID = WHEEL_SOPHIA;
        sprite2wheel.textureID = WHEEL_SOPHIA;

       
        switch (sprite1wheel.spriteID)
        {
        case 0:
            sprite1wheel.spriteID = 3;
            break;
        case 1:
            sprite1wheel.spriteID = 0;
            break;
        case 2:
            sprite1wheel.spriteID = 1;
            break;
        case 3:
            sprite1wheel.spriteID = 2;
            break;
        default:
            break;
        }

        switch (sprite2wheel.spriteID)
        {
        case 0:
            sprite2wheel.spriteID = 3;
            break;
        case 1:
            sprite2wheel.spriteID = 0;
            break;
        case 2:
            sprite2wheel.spriteID = 1;
            break;
        case 3:
            sprite2wheel.spriteID = 2;
            break;
        default:
            break;
        }
        
        currentState = Sophia::Bait;
        //For testing only
        DebugOut(L"[INFO] Swap to GO_LEFT success\n");
        break;
    }
    case (int)Sophia::Go_Right:
    {
        Sprite& sprite1wheel = coordinator->GetComponent<Sprite>(wheel1ID, ComponentType::Sprite);
        Sprite& sprite2wheel = coordinator->GetComponent<Sprite>(wheel2ID, ComponentType::Sprite);
        sprite1wheel.textureID = WHEEL_SOPHIA;
        sprite2wheel.textureID = WHEEL_SOPHIA;

        switch (sprite1wheel.spriteID)
        {
        case 0:
            sprite1wheel.spriteID = 1;
            break;
        case 1:
            sprite1wheel.spriteID = 2;
            break;
        case 2:
            sprite1wheel.spriteID = 3;
            break;
        case 3:
            sprite1wheel.spriteID = 0;
            break;
        default:
            break;
        }

        switch (sprite2wheel.spriteID)
        {
        case 0:
            sprite2wheel.spriteID = 1;
            break;
        case 1:
            sprite2wheel.spriteID = 2;
            break;
        case 2:
            sprite2wheel.spriteID = 3;
            break;
        case 3:
            sprite2wheel.spriteID = 0;
            break;
        default:
            break;
        }

        currentState = Sophia::Bait;
        //For testing only
        DebugOut(L"[INFO] Swap to GO_RIGHT success\n");
        break;
    }
    case (int)Sophia::Right_To_Left:
    {
        Sprite& sprite1wheel = coordinator->GetComponent<Sprite>(wheel1ID, ComponentType::Sprite);
        Sprite& sprite2wheel = coordinator->GetComponent<Sprite>(wheel2ID, ComponentType::Sprite);
        sprite1wheel.textureID = WHEEL_SOPHIA;
        sprite2wheel.textureID = WHEEL_SOPHIA;

        switch (sprite1wheel.spriteID)
        {
        case 0:
            sprite1wheel.spriteID = 3;
            break;
        case 1:
            sprite1wheel.spriteID = 0;
            break;
        case 2:
            sprite1wheel.spriteID = 1;
            break;
        case 3:
            sprite1wheel.spriteID = 2;
            break;
        default:
            break;
        }

        switch (sprite2wheel.spriteID)
        {
        case 0:
            sprite2wheel.spriteID = 3;
            break;
        case 1:
            sprite2wheel.spriteID = 0;
            break;
        case 2:
            sprite2wheel.spriteID = 1;
            break;
        case 3:
            sprite2wheel.spriteID = 2;
            break;
        default:
            break;
        }

        currentState = Sophia::Bait;
        //For testing only
        DebugOut(L"[INFO] Swap to GO_RIGHT success\n");
        break;
    }
    case (int)Sophia::Left_To_Right:
    {
        Sprite& sprite1wheel = coordinator->GetComponent<Sprite>(wheel1ID, ComponentType::Sprite);
        Sprite& sprite2wheel = coordinator->GetComponent<Sprite>(wheel2ID, ComponentType::Sprite);
        sprite1wheel.textureID = WHEEL_SOPHIA;
        sprite2wheel.textureID = WHEEL_SOPHIA;

        switch (sprite1wheel.spriteID)
        {
        case 0:
            sprite1wheel.spriteID = 3;
            break;
        case 1:
            sprite1wheel.spriteID = 0;
            break;
        case 2:
            sprite1wheel.spriteID = 1;
            break;
        case 3:
            sprite1wheel.spriteID = 2;
            break;
        default:
            break;
        }

        switch (sprite2wheel.spriteID)
        {
        case 0:
            sprite2wheel.spriteID = 3;
            break;
        case 1:
            sprite2wheel.spriteID = 0;
            break;
        case 2:
            sprite2wheel.spriteID = 1;
            break;
        case 3:
            sprite2wheel.spriteID = 2;
            break;
        default:
            break;
        }

        currentState = Sophia::Bait;
        //For testing only
        DebugOut(L"[INFO] Swap to GO_RIGHT success\n");
        break;
    }
    default:
        DebugOut(L"[ERROR] %d is incompatible with Sophia animation's type\n");
        break;
    }
}

