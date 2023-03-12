#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupBase.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerupBase : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	ASPowerupBase();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual bool IsInteractable_Implementation() const override;
	// virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Powerup Properties")
	float InactiveTimeSeconds = 10.f;
	
	virtual void BeginPlay() override;

private:
	FTimerHandle InactiveTimerHandle;
	void Activate() const;
	void Deactivate() const;
	
};
