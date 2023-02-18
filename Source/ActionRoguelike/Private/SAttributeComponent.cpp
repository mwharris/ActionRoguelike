#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	OnHealthChanged.Broadcast(GetOwner(), this, Health, Delta);
	return true;
}