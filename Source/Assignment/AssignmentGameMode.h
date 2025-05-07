// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssignmentGameMode.generated.h"

UCLASS(minimalapi)
class AAssignmentGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAssignmentGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerUIClass;

	void PostBeginPlay();
};



