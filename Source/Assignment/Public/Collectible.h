// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectible.generated.h"


class USphereComponent; class UPrimitiveComponent; class UStaticMeshComponent; class USoundBase; class AActor;

UCLASS()
class ASSIGNMENT_API ACollectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ECollisionResponse originalResponse;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	USphereComponent* sphereComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USoundBase* collectSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int value;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);

	static int totalValue;
};
