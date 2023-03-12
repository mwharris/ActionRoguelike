#include "SProjectile.h"

#include "SAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASProjectile::ASProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>("Particle System Component");
	ParticleSystemComp->SetupAttachment(SphereComp);

	AudioComp = CreateDefaultSubobject<UAudioComponent>("Audio Component");
	AudioComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement Component");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.f;
}

void ASProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASProjectile::OnComponentOverlap);
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectile::OnComponentHit);
}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	// Start playing looping sound
	if (IsValid(AudioComp->Sound))
	{
		AudioComp->Play();
	}
	// If Lifetime is 0 then allow the projectile to manage it's own lifetime
	if (ProjectileLifetimeSeconds > 0.f)
	{
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASProjectile::DestroyProjectile, 1.0f, false, ProjectileLifetimeSeconds);
	}
}

void ASProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	DestroyProjectile(true);
}

void ASProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Ignore us hitting the actor that spawned us
	if (GetInstigator() == OtherActor)
	{
		return;
	}
	// Damage a hit actor if valid
	if (OtherActor != nullptr && DamageDelta != 0.f)
	{
		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComponent != nullptr)
		{
			AttributeComponent->ApplyHealthChange(DamageDelta);
		}
	}
	// We might not want to destroy a projectile on overlap (EX: black hole projectile)
	if (DestroyOnOverlap)
	{
		DestroyProjectile(true);
	}
}

void ASProjectile::DestroyProjectile()
{
	DestroyProjectile(false);
}

void ASProjectile::DestroyProjectile(bool PlayHitSound)
{
	AudioComp->Stop();
	// Play hit particle FX
	if (HitParticleSystem != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, GetActorLocation(), GetActorRotation());
	}
	// Play an impact sound if one is set
	if (PlayHitSound && ImpactSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation(), GetActorRotation());
	}
	// Destroy ourselves
	Destroy();
}
