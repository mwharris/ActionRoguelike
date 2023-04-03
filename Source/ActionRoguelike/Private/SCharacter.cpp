#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "SProjectile.h"
#include "Kismet/GameplayStatics.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Comp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Comp");
	CameraComp->SetupAttachment(SpringArmComp);

	// ActorComponents
	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("Interaction Component");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");

	// Ignore the controller's yaw rotation
	bUseControllerRotationYaw = false;
	// Always orient our player in the direction we're moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get a reference to the local player through the Player Controller
		if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->Player))
		{
			// Get a reference to the Enhanced Input Local Player subsystem through the Local Player
			if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Set our Input Mapping Context in the Enhanced Input Local Player subsystem
				if (BaseMappingContext != nullptr)
				{
					InputSystem->ClearAllMappings();
					InputSystem->AddMappingContext(BaseMappingContext, 0);
				}
			}
		}
	}
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Bind Enhanced Input actions
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MovementAction != nullptr) {
			Input->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASCharacter::EnhancedMove);
		} else {
			UE_LOG(LogTemp, Error, TEXT("MovementAction not set!"));
		}
		
		if (LookAction != nullptr) {
			Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::EnhancedLook);
		} else {
			UE_LOG(LogTemp, Error, TEXT("LookAction not set!"));
		}
		
		if (PrimaryAttackAction != nullptr) {
			Input->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &ASCharacter::PlayPrimaryAttackAnim);
		} else {
			UE_LOG(LogTemp, Error, TEXT("PrimaryAttackAction not set!"));
		}

		if (UltimateAttackAction != nullptr) {
			Input->BindAction(UltimateAttackAction, ETriggerEvent::Started, this, &ASCharacter::PlayUltimateAttackAnim);
		} else {
			UE_LOG(LogTemp, Error, TEXT("UltimateAttackAction not set!"));
		}

		if (TeleportAction != nullptr) {
			Input->BindAction(TeleportAction, ETriggerEvent::Started, this, &ASCharacter::PlayTeleportAttackAnim);
		} else {
			UE_LOG(LogTemp, Error, TEXT("TeleportAction not set!"));
		}
		
		if (JumpAction != nullptr) {
			Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ASCharacter::Jump);
			Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASCharacter::StopJumping);
		} else {
			UE_LOG(LogTemp, Error, TEXT("JumpAction not set!"));
		}

		if (InteractAction != nullptr) {
			Input->BindAction(InteractAction, ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);
		} else {
			UE_LOG(LogTemp, Error, TEXT("InteractAction not set!"));
		}
	}
} 

// Value is an Axis2D where index 0 is X-Axis and index 1 is Y-Axis
void ASCharacter::EnhancedMove(const FInputActionValue& Value)
{
	if (Value.GetMagnitude() == 0.0f)
	{
		return;
	}

	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.f;
	ControlRotation.Roll = 0.f;

	// Move Left/Right
	AddMovementInput(FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y), Value[0] * 1000.f);
	// Move Forward/Back
	AddMovementInput(ControlRotation.Vector(), Value[1] * 1000.f);
}

void ASCharacter::EnhancedLook(const FInputActionValue& Value)
{
	AddControllerYawInput(Value[0]);
	AddControllerPitchInput(Value[1] * -1);
}

void ASCharacter::PlayPrimaryAttackAnim(const FInputActionValue& Value)
{
	PlayAnimMontage(PrimaryAttackAnim);
}

void ASCharacter::FirePrimaryAttack()
{
	SpawnProjectile(PrimaryProjectileClass);
}

void ASCharacter::PlayUltimateAttackAnim(const FInputActionValue& Value)
{
	PlayAnimMontage(UltimateAttackAnim);	
}

void ASCharacter::FireUltimateAttack()
{
	SpawnProjectile(UltimateProjectileClass);
}

void ASCharacter::PlayTeleportAttackAnim(const FInputActionValue& Value)
{
	PlayAnimMontage(TeleportAttackAnim);
}

void ASCharacter::FireTeleportProjectile()
{
	SpawnProjectile(TeleportProjectileClass);
}

void ASCharacter::SpawnProjectile(TSubclassOf<ASProjectile> ProjectileToSpawn)
{
	if (ProjectileToSpawn == nullptr)
	{
		return;
	}
	
	// Determine rotation using the camera's location and view rotation
	FRotator SpawnRotation = GetControlRotation();
	FHitResult HitResult;
	if (PerformAttackTrace(HitResult))
	{
		SpawnRotation = (HitResult.ImpactPoint - GetMesh()->GetSocketLocation("Muzzle_01")).Rotation();
	}
	
	const FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	// Overrides the default collision detection when spawning this actor
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn the projectile
	GetWorld()->SpawnActor<AActor>(ProjectileToSpawn, SpawnTransform, SpawnParameters);

	// Show a muzzle flash particle
	UGameplayStatics::SpawnEmitterAttached(PrimaryMuzzleFlash, GetMesh(), "Muzzle_01");
}

bool ASCharacter::PerformAttackTrace(FHitResult& OutHitResult) const
{
	const FVector Start = CameraComp->GetComponentLocation();
	const FVector End = Start + (CameraComp->GetForwardVector() * 5000.f);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	return GetWorld()->SweepSingleByObjectType(OutHitResult, Start, End, FQuat::Identity, ObjectQueryParams, Shape, Params);
}

void ASCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if (Delta < 0.f)
	{
		// Play camera shake
		UGameplayStatics::PlayWorldCameraShake(GetWorld(), HitCameraShake, GetActorLocation(), 0.f, 500.f);
		// Trigger a hit flash on the character mesh 
		GetMesh()->SetScalarParameterValueOnMaterials(FName("TimeToHit"), GetWorld()->GetTimeSeconds());
		// Handle death
		if (NewHealth <= 0.f && Delta < 0.f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
}