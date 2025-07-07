// Fill out your copyright notice in the Description page of Project Settings.


#include "FPS_Character.h"
#include "Camera/CameraComponent.h"

// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
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

}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	//Camera Rotation
	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::LookUp);
}

void AFPS_Character::MoveForward(float value) {
	AddMovementInput(GetActorForwardVector(), value);
}

void AFPS_Character::MoveRight(float value) {
	AddMovementInput(GetActorRightVector(), value);
}

void AFPS_Character::Turn(float value) {
	AddControllerYawInput(value);
}

void AFPS_Character::LookUp(float value) {
	AddControllerPitchInput(value);
}
