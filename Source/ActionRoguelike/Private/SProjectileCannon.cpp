#include "SProjectileCannon.h"
#include "SProjectile.h"

ASProjectileCannon::ASProjectileCannon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASProjectileCannon::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASProjectileCannon::SpawnProjectile, 2.0f, true);
}

void ASProjectileCannon::SpawnProjectile()
{
	const FTransform SpawnTransform = FTransform(GetActorRotation(), GetActorLocation());
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	// Overrides the default collision detection when spawning this actor
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
}