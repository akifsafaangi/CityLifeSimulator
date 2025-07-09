// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/PickupableObject.h"
#include "ContainerBox.generated.h"

UCLASS()
class CITYLIFESIMULATOR_API AContainerBox : public APickupableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContainerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Interact_Implementation(AActor* Interactor) override;
private:
	void Throw();
	void OpenBox();

	UPROPERTY(EditDefaultsOnly, Category = "Box")
	TSubclassOf<class APlacableObject> ContainedObjectClass;

	UPROPERTY()
	APlacableObject* ContainedObject;
};
