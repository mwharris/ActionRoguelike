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
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
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
