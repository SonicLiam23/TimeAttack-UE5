// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UAIPerceptionComponent; class UAISenseConfig_Sight; class APawn;

UCLASS()
class ASSIGNMENT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	FVector requiredDistance;

	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy")
	UAIPerceptionComponent* aiPercComp;
	UPROPERTY(VisibleDefaultsOnly, Category = "Enemy")
	UAISenseConfig_Sight* sightConfig;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector baseLoc;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	FVector currVelocity;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float speed;
	UPROPERTY(EditAnywhere, Category = "Enemy")
	float radius;

	float distSqrd;
	bool backToBase;

	UFUNCTION()
	void OnSensed(const TArray<AActor*>& testActors);

	AActor* target;

	UCapsuleComponent* collider = nullptr;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
	FTimerHandle damageTick;

	UFUNCTION()
	void OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void DamagePlayer();
};
