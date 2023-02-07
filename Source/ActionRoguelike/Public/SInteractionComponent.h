#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PrimaryInteract() const;

protected:
	virtual void BeginPlay() override;

private:
	bool PerformLineTrace(FHitResult& OutHitResult, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams) const;
	bool PerformSphereTrace(TArray<FHitResult>& OutHitResult, const FVector& Start, const FVector& End, const FCollisionObjectQueryParams& ObjectQueryParams) const;
		
};