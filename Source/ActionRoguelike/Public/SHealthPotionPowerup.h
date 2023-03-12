#pragma once

#include "CoreMinimal.h"
#include "SPowerupBase.h"
#include "SHealthPotionPowerup.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASHealthPotionPowerup : public ASPowerupBase
{
	GENERATED_BODY()

public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Powerup Properties")
	float HealthDelta = 20.f;
	
};
