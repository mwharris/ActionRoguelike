#include "SExplosiveBarrel.h"

#include "SAttributeComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
	MeshComp->SetSimulatePhysics(true);
	SetRootComponent(MeshComp);
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("Radial Force Component");
	RadialForceComp->SetAutoActivate(false);
	RadialForceComp->Radius = 750.f;
	RadialForceComp->ImpulseStrength = 2500.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->SetupAttachment(MeshComp);
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
}

void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// Damage the player if they walked into us
	if (OtherActor != nullptr)
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AttributeComp != nullptr)
		{
			AttributeComp->ApplyHealthChange(DamageDelta);
		}
	}
	RadialForceComp->FireImpulse();
}

