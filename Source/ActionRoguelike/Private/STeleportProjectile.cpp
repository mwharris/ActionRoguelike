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
	GetWorldTimerManager().SetTimer(ExplodeTimerHandle, this, &ASTeleportProjectile::TriggerExplosion, ExplodeDelayTimeSeconds);
}

void ASTeleportProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Immediately trigger the explosion if we hit something
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	TriggerExplosion();
}

void ASTeleportProjectile::TriggerExplosion()
{
	GetWorldTimerManager().ClearTimer(ExplodeTimerHandle);
	// Halt movement
	MovementComp->StopMovementImmediately();
	// Disable collisions
	SetActorEnableCollision(false);
	// Explode
	ParticleSystemComp->Deactivate();
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, GetActorLocation(), GetActorRotation());
	// Start the teleport timer
	GetWorldTimerManager().SetTimer(TeleportTimerHandle, this, &ASTeleportProjectile::TriggerTeleport, TeleportDelayTimeSeconds);
}

void ASTeleportProjectile::TriggerTeleport()
{
	GetWorldTimerManager().ClearTimer(TeleportTimerHandle);
	// Teleport the character to our hit location
	GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation());
	// Destroy ourselves
	Destroy();
}
