// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EUserPlayStateEnum : uint8
{
    PLAY        UMETA(DisplayName = "Play"),
    MENU        UMETA(DisplayName = "Menu"),
    WIN         UMETA(DisplayName = "Win"),
    LOSE        UMETA(DisplayName = "Lose")
};

#include "UserEnum.generated.h"