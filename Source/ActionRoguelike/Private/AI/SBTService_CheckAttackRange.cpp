#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (ensure(BC))
	{
		if (const AActor* TargetActor = Cast<AActor>(BC->GetValueAsObject("TargetActor")))
		{
			if (const AAIController* AIController = OwnerComp.GetAIOwner())
			{
				const APawn* AIPawn = AIController->GetPawn();
				if (ensure(AIPawn))
				{
					// Get distance between our pawn and the target
					const FVector DistanceVector = TargetActor->GetActorLocation() - AIPawn->GetActorLocation();
					const float Distance = DistanceVector.Length();

					// Check distance and LoS
					const bool WithinAttackRange = Distance <= 2000.f;
					const bool LineOfSight = WithinAttackRange ? AIController->LineOfSightTo(TargetActor) : false;

					// Update Blackboard variables accordingly
					BC->SetValueAsBool(AttackRangeKey.SelectedKeyName, (WithinAttackRange && LineOfSight));
				}
			}
		}
	}
}
