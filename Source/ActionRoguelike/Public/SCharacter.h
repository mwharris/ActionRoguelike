#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "SAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void FirePrimaryAttack();
	UFUNCTION(BlueprintCallable)
	void FireUltimateAttack();
	UFUNCTION(BlueprintCallable)
	void FireTeleportProjectile();
	
	ASCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class UCameraComponent* CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class USInteractionComponent* InteractionComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	class USAttributeComponent* AttributeComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* MovementAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* PrimaryAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* UltimateAttackAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* TeleportAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Actions")
	class UInputAction* InteractAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controls|Input Mappings")
	class UInputMappingContext* BaseMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Projectiles")
	TSubclassOf<class ASProjectile> PrimaryProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Projectiles")
	TSubclassOf<class ASProjectile> UltimateProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Projectiles")
	TSubclassOf<class ASProjectile> TeleportProjectileClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Animation")
	UAnimMontage* PrimaryAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Animation")
	UAnimMontage* UltimateAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|Animation")
	UAnimMontage* TeleportAttackAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|FX")
	UParticleSystem* PrimaryMuzzleFlash;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Attacks|FX")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, class USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	void EnhancedMove(const FInputActionValue& Value);
	void EnhancedLook(const FInputActionValue& Value);
	void PlayPrimaryAttackAnim(const FInputActionValue& Value);
	void PrimaryInteract(const FInputActionValue& Value);
	void PlayUltimateAttackAnim(const FInputActionValue& Value);
	void PlayTeleportAttackAnim(const FInputActionValue& Value);

private:
	bool PerformAttackTrace(FHitResult& OutHitResult) const;
	void SpawnProjectile(TSubclassOf<class ASProjectile> ProjectileToSpawn);
	
};
