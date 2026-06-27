// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CassetteProgressSubsystem.generated.h"

/** Статус одной VHS-кассеты в прогрессе игрока. */
class UCassetteData;
UENUM(BlueprintType)
enum class ECassetteStatus : uint8
{
	Locked      UMETA(DisplayName = "Locked"),
	Unlocked    UMETA(DisplayName = "Unlocked"),
	Completed   UMETA(DisplayName = "Completed")
};

/**
 * Хранит прогресс кассет и состояние пролога.
 * Живёт всё время игры, переживает смену уровней. Источник правды для меню, уровней и сохранений.
 */
UCLASS()
class TAPESICK_API UCassetteProgressSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Движок вызывает при создании подсистемы — задаём стартовое состояние.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Вернуть статус кассеты по индексу (0 — первая). Для меню.
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	ECassetteStatus GetCassetteStatus(int32 Index) const;

	// Вернуть данные кассеты (название, уровень, обложку) по индексу. Для меню.
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	UCassetteData* GetCassetteData(int32 Index) const;

	// Пометить кассету пройденной и открыть следующую. Для конца уровня. Идемпотентна.
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	void CompleteCassette(int32 Index);

	// Пройден ли пролог (развилка запуска: пролог или сразу хаб).
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	bool IsIntroCompleted() const;

	// Отметить пролог пройденным (вызывается на первом автосейве / пробуждении).
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	void SetIntroCompleted();

	// Сколько всего кассет.
	UFUNCTION(BlueprintCallable, Category = "Cassette Progress")
	int32 GetNumCassettes() const;

protected:
	// Сколько всего кассет в игре. Пока константа, позже свяжем с Data Assets.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cassette Progress")
	int32 NumCassettes = 3;

	// Статус каждой кассеты. Индекс = номер кассеты. Размер = NumCassettes.
	UPROPERTY(BlueprintReadOnly, Category = "Cassette Progress")
	TArray<ECassetteStatus> CassetteStatuses;
	UPROPERTY()
	TArray<TObjectPtr<UCassetteData>> LoadedCassettes;

	// Пройден ли вступительный пролог.
	UPROPERTY(BlueprintReadOnly, Category = "Cassette Progress")
	bool bIntroCompleted = false;
};