#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ASAICharacter::ASAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensing Component");
 	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Bind a function to PawnSensingComp so we can perform an action when we sense a Pawn
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
	// Bind to On Health Changed
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	// Get our AI Controller
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController != nullptr)
	{
 		// Access our Blackboard Component
 		UBlackboardComponent* BC = AIController->GetBlackboardComponent();
 		// Set our TargetActor to the Pawn we just sensed
 		BC->SetValueAsObject("TargetActor", Pawn);
		// BC->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
 		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, false);
	}
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy HP: %f"), NewHealth);
}