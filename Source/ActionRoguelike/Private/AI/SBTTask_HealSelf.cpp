#include "AI/SBTTask_HealSelf.h"

#include "AIController.h"
#include "AI/SAICharacter.h"

EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController))
	{
		return EBTNodeResult::Failed;
	}
	
	ASAICharacter* AICharacter = Cast<ASAICharacter>(AIController->GetPawn());
	if (!ensure(AICharacter))
	{
		return EBTNodeResult::Failed;
	}
	
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AICharacter->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (!ensure(AttributeComp))
	{
		return EBTNodeResult::Failed;
	}

	// Heal back up to full HP
	AttributeComp->ApplyHealthChange(AttributeComp->MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %f"), AttributeComp->Health);
	
	return EBTNodeResult::Succeeded;
}
