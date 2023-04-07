#include "AI/SBTService_CheckLowHealth.h"

#include "AIController.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!ensure(BlackboardComp))
	{
		return;
	}
	
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return;
	}

	const ASAICharacter* AICharacter = Cast<ASAICharacter>(AIController->GetPawn());
	if (AICharacter == nullptr)
	{
		return;
	}

	UActorComponent* ActorComponent = AICharacter->GetComponentByClass(USAttributeComponent::StaticClass());
	if (ActorComponent == nullptr)
	{
		return;
	}
	
	const USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(ActorComponent);
	if (AttributeComp == nullptr)
	{
		return;
	}

	// Determine if our Health is under the Low Health Threshold percentage
	const float HealthPercent = 100.f * (AttributeComp->Health / AttributeComp->MaxHealth);
	const bool LowHealth = HealthPercent <= LowHealthThresholdPercent;

	// Update Blackboard key accordingly
	BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, LowHealth);
}
