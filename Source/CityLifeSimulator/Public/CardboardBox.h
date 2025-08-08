// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupableObject.h"
#include "CardboardBox.generated.h"

/**
 * 
 */
UCLASS()
class CITYLIFESIMULATOR_API ACardboardBox : public APickupableObject
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(AActor* Interactor) override;
	virtual void Pickup (AActor* Picker) override;
	void OpenBox(AActor* Interactor);

	UFUNCTION(BlueprintImplementableEvent, Category="CardboardBox")
    void OpenCloseBox();
public:
	ACardboardBox();
	void MoveObject(FVector NewTargetLocation, float Duration);

private:
	void Throw();
	bool bIsLerping = false;
	float LerpElapsedTime = 0.0f;
	float LerpDuration = 1.0f;
	FVector StartLocation;
	FVector TargetLocation;
};
