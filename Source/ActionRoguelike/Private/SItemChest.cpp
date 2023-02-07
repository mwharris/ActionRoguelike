#include "SItemChest.h"

ASItemChest::ASItemChest()
{
	PrimaryActorTick.bCanEverTick = false;

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("Chest Mesh");
	SetRootComponent(ChestMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("Lid Mesh");
	LidMesh->SetupAttachment(ChestMesh);
}

void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0.f, 0.f));
}