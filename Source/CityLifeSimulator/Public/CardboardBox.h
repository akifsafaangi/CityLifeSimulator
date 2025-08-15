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
	ACardboardBox();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(AActor* Interactor) override;
	virtual void Pickup (AActor* Picker) override;
	void OpenBox(AActor* Interactor);

	UFUNCTION(BlueprintImplementableEvent, Category="CardboardBox")
    void OpenCloseBox();
public:
	void MoveObject(class UShelfSlotItemComponent* TargetSlot, float Duration, bool bDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardboardBoxItems")
	TArray<class UShelfSlotItemComponent*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	UStaticMesh* ItemMesh;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DefaultItemMaterial;

	UFUNCTION(BlueprintCallable, Category = "CardboardBox")
	void Init(UStaticMesh* NewItemMesh, UMaterialInterface* NewDefaultItemMaterial, int size = 4);
private:
	void Throw();
};
