#include "GoLeftExteriorCommand.h"
#include "../HelperHeader/PlayerType.h"
//For testing purpose
#include "../Debug.h"
#include "Core/InputContext.h"
#include "../Core/Coordinator.h"
#include"../Component/SpeedComponent.h"
#include "../Component/PositionComponent.h"
#include "../Component/AnimationComponent.h"
#include "../System/AnimationSystem.h"
#include "../UtilHeader.h"

/*void GoLeftExteriorCommand::execute(PlayerType * EntityID) 
{
    DebugOut(L"Go Left Exterior Command\n");
    InputContext* context = InputContext::GetInstance();
    DebugOut(L"%d\n", context->coordinator);
    //context->coordinator->GetComponent<Animation>(EntityID, ComponentType::Animation);

    Position pos = context->coordinator->GetComponent<Position>(EntityID, ComponentType::Position);;

    pos.x = pos.x + 10;
   // DebugOut(L"%d, %d\n", pos.x, &pos.y);
   /* Velocity vel;
    vel.vx = 10;

    pos.x = pos.x + vel.vx;
    context->coordinator->AddComponent<Position>(EntityID, pos, ComponentType::Position);
    context->coordinator->GetSystem<AnimationSystem>(SystemType::Animation)->AddEntity(EntityID);*/


void GoLeftExteriorCommand::execute(PlayerType * EntityID) {
    DebugOut(L"Go Left Exterior Command\n\n");
    InputContext* context = InputContext::GetInstance();
    
    switch (EntityID->currentPlayerType) {
    case PlayerType::JASON:
    {
        Position& pos = context->coordinator->GetComponent<Position>(EntityID->jason->GetID(), ComponentType::Position);
        pos.x += 10;

        Animation& animation = context->coordinator->GetComponent<Animation>(EntityID->jason->GetID(), ComponentType::Animation);
        /*animation.currentState = JASON_GO_LEFT;
        animation.currentFrame = 0;
        animation.defaultState = JASON_IDLE_LEFT;*/

        animation.textureID = JASON_WALK;
        animation.delayValue = 100;
        animation.isFinished = false;
        State jasonState;
        jasonState.endFrame = 3;
        jasonState.startFrame = 0;
        jasonState.isLoopable = true;
        animation.stateDictionary.emplace(JASON_GO_LEFT, jasonState);

        animation.currentState = JASON_GO_LEFT;
        animation.currentFrame = 0;
        animation.defaultState = JASON_GO_LEFT;

        break;
    }
    case PlayerType::SOPHIA:
        EntityID->sophia->Test();
        break;
    }

}