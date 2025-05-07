// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowOrb.generated.h"



class USphereComponent; class UPrimitiveComponent; class UStaticMeshComponent; class USoundBase; class AActor;

UCLASS()
class ASSIGNMENT_API AThrowOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowOrb();

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
	int healthPenalty;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
};
