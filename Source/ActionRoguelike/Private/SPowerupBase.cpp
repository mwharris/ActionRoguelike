#include "SPowerupBase.h"

ASPowerupBase::ASPowerupBase()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	SetRootComponent(MeshComp);
}

void ASPowerupBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupBase::Interact_Implementation(APawn* InstigatorPawn)
{
	// ISGameplayInterface::Interact_Implementation(InstigatorPawn);
	// Can only interact if we are interactable
	if (IsInteractable_Implementation())
	{
		Deactivate();
		GetWorldTimerManager().SetTimer(InactiveTimerHandle, this, &ASPowerupBase::Activate, 1.0f, false, InactiveTimeSeconds);
	}
}

void ASPowerupBase::Activate() const
{
	MeshComp->SetVisibility(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASPowerupBase::Deactivate() const
{
	MeshComp->SetVisibility(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ASPowerupBase::IsInteractable_Implementation() const
{
	return MeshComp->IsVisible();
}