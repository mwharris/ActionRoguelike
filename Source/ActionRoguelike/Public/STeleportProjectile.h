#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "STeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:	
	ASTeleportProjectile();

protected:
	virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void BeginPlay() override;
	void TriggerExplosion();
	void TriggerTeleport();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Teleport", meta=(AllowPrivateAccess=true))
	float ExplodeDelayTimeSeconds = 0.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Teleport", meta=(AllowPrivateAccess=true))
	float TeleportDelayTimeSeconds = 0.2f;
	
	FTimerHandle ExplodeTimerHandle;
	FTimerHandle TeleportTimerHandle;
	
};
