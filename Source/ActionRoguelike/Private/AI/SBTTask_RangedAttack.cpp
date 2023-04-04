#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		const ACharacter* AICharacter = Cast<ACharacter>(AIController->GetPawn());
		if (AICharacter == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		// Determine spawn location and fire direction from AI to Target
		const FVector MuzzleLocation = AICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		const FRotator ProjectileRotation = (TargetActor->GetActorLocation() - MuzzleLocation).Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = AIController->GetPawn();

		// Spawn the projectile given the above location and rotation
		const AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, ProjectileRotation, Params);

		// Succeed only if the projectile was spawned properly
		return Projectile != nullptr ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}
