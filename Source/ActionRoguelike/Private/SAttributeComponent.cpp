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
	// Update Health making sure to stay between 0 and MaxHealth
	Health = FMath::Clamp(Health += Delta, 0.f, MaxHealth);
	// Broadcast to subscribers that our health changed
	OnHealthChanged.Broadcast(GetOwner(), this, Health, Delta);
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}
