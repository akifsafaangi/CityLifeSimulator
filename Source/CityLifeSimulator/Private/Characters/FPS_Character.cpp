// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FPS_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Items/PickupableObject.h"
#include "Items/PlacableObject.h"

// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	//LineTrace();
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPS_Character::Interact);


	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::TurnCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::LookUp);
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Start: ") + start.ToString());
	}
	FVector end = (Camera->GetForwardVector() * 500.0f) + start;
	FHitResult Hit;

	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECollisionChannel::ECC_Visibility, params);

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.0f, 0, 0.5f);

	if (bHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
			FString::Printf(TEXT("Hit: %s"), *Hit.GetActor()->GetName()));
	}
}

UCameraComponent* AFPS_Character::GetCamera()
{
	return Camera;
}

void AFPS_Character::Interact() {
	if (!HeldActor && !PlacingActor) {
		if (HitObject && HitObject->GetClass()->ImplementsInterface(UInteractable::StaticClass())) {
			IInteractable::Execute_Interact(HitObject, this);

			if (APickupableObject* Pick = Cast<APickupableObject>(HitObject)) {
				if (APlacableObject* Place = Cast<APlacableObject>(HitObject)) {
					PlacingActor = Place;
				}
				else {
					HeldActor = Pick;
				}
			}
		}
	}
}