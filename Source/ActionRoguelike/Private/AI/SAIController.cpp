#include "AI/SAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		UBlackboardComponent* BC = GetBlackboardComponent();
		BC->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
		BC->SetValueAsObject("TargetActor", PlayerPawn);
	}
}
