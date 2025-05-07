// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReleaseBlock.generated.h"

class UBoxComponent; class UPrimitiveComponent; class UStaticMeshComponent; class USoundBase; class AActor;

UCLASS()
class ASSIGNMENT_API AReleaseBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReleaseBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool isActivated = false;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UMaterialInterface* activatedMaterial;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float boxSize = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mesh = nullptr;

};
