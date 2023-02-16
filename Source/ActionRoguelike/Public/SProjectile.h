#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class UParticleSystemComponent* ParticleSystemComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class UParticleSystem* HitParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Lifetime")
	float ProjectileLifetimeSeconds = 0.f;
	
	UFUNCTION(BlueprintCallable)
	void DestroyProjectile();
	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;

private:
	FTimerHandle DestroyTimerHandle;

};
