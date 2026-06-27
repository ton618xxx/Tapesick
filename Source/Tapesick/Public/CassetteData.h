// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CassetteData.generated.h"

class UTexture2D;

/**
 * Описывает содержимое одной VHS-кассеты: что грузить и как показать в меню.
 * Это данные (контент), отдельно от статуса прохождения (он в CassetteProgressSubsystem).
 */
UCLASS(BlueprintType)
class TAPESICK_API UCassetteData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// Название кассеты для меню 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cassette")
	FText DisplayName;

	// Уровень-кошмар, который грузит эта кассета. Мягкая ссылка — не грузит карту в память заранее.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cassette")
	TSoftObjectPtr<UWorld> LevelToLoad;

	// Обложка кассеты для меню.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cassette")
	TObjectPtr<UTexture2D> CoverImage;

	// Описание / текст интро.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cassette", meta = (MultiLine = true))
	FText Description;
};