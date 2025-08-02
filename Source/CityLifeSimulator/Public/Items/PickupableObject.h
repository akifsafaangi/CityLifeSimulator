// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interactable.h"
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include "Characters/FPS_Character.h"
#include "PickupableObject.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API APickupableObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableObject();

	void Interact_Implementation(AActor* Interactor) override;
	void InteractHighlight_Implementation() override;
	void InteractEnd_Implementation() override;
	virtual void Pickup(AActor* Picker);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AActor* CurrentInteractor = nullptr;
	bool bIsPickedUp;

	float PickupCameraOffset; // Offset for the camera when picking up
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetCurrentInteractor() const { return CurrentInteractor; }
	void SetCurrentInteractor(AActor* Interactor) { CurrentInteractor = Interactor; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;
};
