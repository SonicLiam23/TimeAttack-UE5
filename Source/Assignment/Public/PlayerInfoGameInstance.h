// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerInfo.h"
#include "UserEnum.h"
#include "PlayerInfoGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UPlayerInfoGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    UPlayerInfoGameInstance();

    // the persistent EUserPlayStateEnum
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EUserPlayStateEnum currentEUserPlayStateEnum;
};
