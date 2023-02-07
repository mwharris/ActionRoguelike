#include "SInteractionComponent.h"

#include "SGameplayInterface.h"

USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}




void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USInteractionComponent::PrimaryInteract() const
{
	FHitResult HitResult;
	TArray<FHitResult> HitResults;
	     
	// Get the owning pawn's eye location and rotation
	FVector EyeLocation;
	FRotator EyeRotation;
	APawn* Owner = Cast<APawn>(GetOwner());
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	// For now, line trace for World Dynamic actors
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// Determine the end location of the trace
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);

	// Perform the trace
	bool bHitResult = PerformSphereTrace(HitResults, EyeLocation, End, ObjectQueryParams);

	// Make sure we hit an Actor that implements ISGameplayInterface
	FColor LineColor = FColor::Red;
	for (FHitResult Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				LineColor = FColor::Green;
				// Call the actor's interact method through the ISGameplayInterface
				ISGameplayInterface::Execute_Interact(HitActor, Owner);
				break;
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32.f, LineColor, false, 5.f);
	}
}

bool USInteractionComponent::PerformLineTrace(FHitResult& OutHitResult, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams) const
{
	return GetWorld()->LineTraceSingleByObjectType(OutHitResult, Start, End, ObjectQueryParams);
}

bool USInteractionComponent::PerformSphereTrace(TArray<FHitResult>& OutHitResult, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams) const
{
	FCollisionShape Sphere;
	Sphere.SetSphere(30.f);
	return GetWorld()->SweepMultiByObjectType(OutHitResult, Start, End, FQuat::Identity, ObjectQueryParams, Sphere);
}
