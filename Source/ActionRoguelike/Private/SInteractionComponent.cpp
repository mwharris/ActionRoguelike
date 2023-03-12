#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "Camera/CameraComponent.h"

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
	FVector Start = FVector::ZeroVector;
	FVector End = FVector::ZeroVector;
	APawn* Owner = Cast<APawn>(GetOwner());

	// Determine Start and End location based on if we can find a camera component on the Owner
	UCameraComponent* CameraComp = Cast<UCameraComponent>(GetOwner()->GetComponentByClass(UCameraComponent::StaticClass()));
	if (CameraComp != nullptr)
	{
		Start = CameraComp->GetComponentLocation();
		End = Start + (CameraComp->GetForwardVector() * 1000.f);
	}
	else
	{
		// Get the owning pawn's eye location and rotation
		FVector EyeLocation;
		FRotator EyeRotation;
		Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		// Determine the start/end location of the trace based on eye location
		Start = EyeLocation;
		End = EyeLocation + (EyeRotation.Vector() * 1000.f);
	}

	// For now, line trace for World Dynamic actors
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	// Perform the trace
	bool bHitResult = PerformSphereTrace(HitResults, Start, End, ObjectQueryParams);

	// Make sure we hit an Actor that implements ISGameplayInterface
	FColor LineColor = FColor::Red;
	for (FHitResult Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				if (ISGameplayInterface::Execute_IsInteractable(HitActor))
				{
					LineColor = FColor::Green;
					// Call the actor's interact method through the ISGameplayInterface
					ISGameplayInterface::Execute_Interact(HitActor, Owner);
					break;
				}
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
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 5.0f);
	return GetWorld()->SweepMultiByObjectType(OutHitResult, Start, End, FQuat::Identity, ObjectQueryParams, Sphere);
}