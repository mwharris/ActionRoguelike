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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class USphereComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class UProjectileMovementComponent* MovementComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UParticleSystemComponent* ParticleSystemComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UParticleSystem* HitParticleSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile Properties")
	float ProjectileLifetimeSeconds = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile Properties")
	float DamageDelta = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile Properties")
	bool DestroyOnOverlap = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Audio")
	USoundBase* ImpactSound;
	
	void DestroyProjectile();
	UFUNCTION(BlueprintCallable)
	void DestroyProjectile(bool PlayHitSound);
	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

private:
	FTimerHandle DestroyTimerHandle;

};
