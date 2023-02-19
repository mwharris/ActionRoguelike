#include "STargetDummy.h"
#include "SAttributeComponent.h"

ASTargetDummy::ASTargetDummy()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");
	SetRootComponent(MeshComp);
	
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		MeshComp->SetScalarParameterValueOnMaterials(FName("TimeToHit"), GetWorld()->GetTimeSeconds());
	}
}