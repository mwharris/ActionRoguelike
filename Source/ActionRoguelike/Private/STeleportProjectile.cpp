#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASTeleportProjectile::ASTeleportProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	// Start the explode timer
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ASTeleportProjectile::TriggerExplosion, 1.0f, false, ExplodeDelayTimeSeconds);
}

void ASTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Immediately trigger the explosion if we hit something
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	TriggerExplosion();
}

void ASTeleportProjectile::TriggerExplosion()
{
	// Halt movement
	MovementComp->StopMovementImmediately();
	// Explode
	ParticleSystemComp->Deactivate();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, GetActorLocation(), GetActorRotation());
	// Start the teleport timer
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ASTeleportProjectile::TriggerTeleport, 1.0f, false, TeleportDelayTimeSeconds);
}

void ASTeleportProjectile::TriggerTeleport()
{
	// Teleport the character to our hit location
	GetInstigator()->SetActorLocation(GetActorLocation());
	// Destroy ourselves
	Destroy();
}
