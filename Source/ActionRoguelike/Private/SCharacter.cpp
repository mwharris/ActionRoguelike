#include "SCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Comp");
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Comp");
	CameraComp->SetupAttachment(SpringArmComp);
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
		Input->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASCharacter::EnhancedMove);
	}
} 

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::EnhancedMove(const FInputActionValue& Value)
{
	// Value is an Axis2D where index 0 is X-Axis and index 1 is Y-Axis
	if (Value.GetMagnitude() != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value[0] * 1000.f);
		AddMovementInput(GetActorForwardVector(), Value[1] * 1000.f);
	}
}
