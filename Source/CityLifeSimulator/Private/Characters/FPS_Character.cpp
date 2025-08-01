// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FPS_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/PickupableObject.h"
#include "Items/PlacableObject.h"
#include "Items/ContainerBox.h"

// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsHolding = false;
	bIsInPlacementMode = false;
	HoldingTime = 0.0f;
	bLongPressTriggered	= false;
	bCountHolding = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = true;

	SpringArm->TargetArmLength = 0;
}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCountHolding && !bLongPressTriggered)
	{	
		if (HoldingTime >= 2.0f && HitObject) {
			if (APlacableObject* Place = Cast<APlacableObject>(HitObject)) {
				bLongPressTriggered = true;
				IInteractable::Execute_Interact(HitObject, this);
				HitObject->StaticMesh->SetRenderCustomDepth(false);
				bIsHolding = true;
				PlacingActor = Place;
				bIsInPlacementMode = true;
				HoldingTime = 0.0f;
			}
		} else {
			HoldingTime += DeltaTime;
			MyDynMat->SetScalarParameterValue("Percent", HoldingTime);
		}
	}
	UpdatePlacement();
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPS_Character::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AFPS_Character::InteractRelease);
	PlayerInputComponent->BindAction("OpenObject", IE_Pressed, this, &AFPS_Character::OpenBox);


	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::TurnCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::LookUp);
	PlayerInputComponent->BindAxis("TurnObject", this, &AFPS_Character::RotatePlacementObject);
}

void AFPS_Character::MoveForward(float value) {
	AddMovementInput(GetActorForwardVector(), value);
}

void AFPS_Character::MoveRight(float value) {
	AddMovementInput(GetActorRightVector(), value);
}

void AFPS_Character::TurnCamera(float value) {
	AddControllerYawInput(value);
}

void AFPS_Character::LookUp(float value) {
	AddControllerPitchInput(value);
}

void AFPS_Character::LineTrace()
{
	if (!Camera) return;

	FVector start = Camera->GetComponentLocation();

	FVector end = (Camera->GetForwardVector() * 500.0f) + start;
	FHitResult Hit;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECollisionChannel::ECC_Visibility, params);

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f, 0, 0.5f);
}

UCameraComponent* AFPS_Character::GetCamera()
{
	return Camera;
}

void AFPS_Character::Interact() {
	if (!HeldActor && !PlacingActor) {
		if (HitObject) {
			if (Cast<APlacableObject>(HitObject)) {
				HoldingTime = 0.0f;
				bCountHolding = true;
				bLongPressTriggered = false;
			}
			else {
				IInteractable::Execute_Interact(HitObject, this);
				HitObject->StaticMesh->SetRenderCustomDepth(false);
				bIsHolding = true;
				HeldActor = Cast<APickupableObject>(HitObject);
			}
		}
	}
	else {
		if (HeldActor) {
			IInteractable::Execute_Interact(HeldActor, this);
			bIsHolding = false;
			HeldActor = nullptr;
			HitObject = nullptr;
		} else if (PlacingActor) {
			PlaceObject();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("No Held or Placing Actor Found"));
		}
	}
}

void AFPS_Character::InteractRelease()
{
	bCountHolding = false;
	HoldingTime = 0.0f;
	bLongPressTriggered = false;
}

void AFPS_Character::PlaceObject()
{
    if (PlacingActor && PlacingActor->bIsInPlacementMode)
    {
        PlacingActor->Place();
        PlacingActor = nullptr;
		bIsInPlacementMode = false;
		bIsHolding = false;
		HitObject = nullptr;
    }
}


void AFPS_Character::UpdatePlacement()
{
	if (PlacingActor && PlacingActor->bIsInPlacementMode)
    {
        FVector Start = Camera->GetComponentLocation();
        FVector End = Start + (Camera->GetForwardVector() * 1000.f);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(PlacingActor);
        if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
        {
            PlacingActor->UpdatePlacement(Hit.Location, Hit.Normal);
        }
    }
}

void AFPS_Character::RotatePlacementObject(float Value)
{
    if (PlacingActor && PlacingActor->bIsInPlacementMode && FMath::Abs(Value) > KINDA_SMALL_NUMBER)
    {
        PlacingActor->RotateDuringPlacement(Value);
    }
}

void AFPS_Character::OpenBox()
{
	if (HitObject)
	{
		if (AContainerBox* Box = Cast<AContainerBox>(HitObject))
		{
			Box->OpenBox(this);
		}
	}
}