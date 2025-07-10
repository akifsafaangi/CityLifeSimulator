// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/Interactable.h"
#include <PhysicsEngine/PhysicsHandleComponent.h>
#include "PickupableObject.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API APickupableObject : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupableObject();

	void Interact_Implementation(AActor* Interactor) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Pickup(AActor* Picker);
	bool bIsPickedUp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetCurrentInteractor() const { return CurrentInteractor; }
	void SetCurrentInteractor(AActor* Interactor) { CurrentInteractor = Interactor; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;
private:
	AActor* CurrentInteractor = nullptr;
};
