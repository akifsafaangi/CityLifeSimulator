// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPS_Character.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API AFPS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* Camera;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void TurnCamera(float value);
	void LookUp(float value);

	//Place Objects
	void Interact();
	void LineTrace();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable Object")
	AActor* HitObject;

	UPROPERTY()
	class APickupableObject* HeldActor = nullptr;

	UPROPERTY()
	class APlacableObject* PlacingActor = nullptr;
};