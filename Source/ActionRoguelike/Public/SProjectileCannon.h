#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileCannon.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASProjectileCannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectileCannon();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnProjectile();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Cannon", meta=(AllowPrivateAccess=true))
	TSubclassOf<class ASProjectile> ProjectileClass;

	FTimerHandle SpawnTimerHandle;

};
