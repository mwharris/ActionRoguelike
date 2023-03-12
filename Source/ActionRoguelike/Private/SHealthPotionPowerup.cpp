#include "SHealthPotionPowerup.h"
#include "SAttributeComponent.h"

void ASHealthPotionPowerup::Interact_Implementation(APawn* InstigatorPawn)
{
	// Restore health to the pawn that interacted with us
	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComponent != nullptr && AttributeComponent->Health < AttributeComponent->MaxHealth)
	{
		AttributeComponent->ApplyHealthChange(HealthDelta);
		// Call Super() to deactivate us for a time
		Super::Interact_Implementation(InstigatorPawn);
	}
}