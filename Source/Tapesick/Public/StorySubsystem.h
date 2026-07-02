#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StorySubsystem.generated.h"

// Этапы истории. Каждый определяет состояние мира: двери, погоду, доступность кассет.
UENUM(BlueprintType)
enum class EStoryStage : uint8
{
	Intro                UMETA(DisplayName = "Intro"),
	FatherDialogue       UMETA(DisplayName = "FatherDialogue"),
	FreeRoamBeforeSleep  UMETA(DisplayName = "FreeRoamBeforeSleep"),
	Hub                  UMETA(DisplayName = "Hub"),
	InCassette           UMETA(DisplayName = "InCassette"),
	Finale               UMETA(DisplayName = "Finale")
};

// Делегат: «этап сменился». Передаёт новый этап подписчикам (двери, погода, стена).
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageChanged, EStoryStage, NewStage);

UCLASS(BlueprintType)
class TAPESICK_API UStorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Сменить этап и оповестить подписчиков. Главная точка входа.
	UFUNCTION(BlueprintCallable, Category = "Story")
	void SetStage(EStoryStage NewStage);

	// Узнать текущий этап (разовый запрос — например, дверь при BeginPlay).
	UFUNCTION(BlueprintCallable, Category = "Story")
	EStoryStage GetCurrentStage() const;

	// На это событие подписываются те, кто должен реагировать на смену этапа.
	UPROPERTY(BlueprintAssignable, Category = "Story")
	FOnStageChanged OnStageChanged;

protected:
	// Текущий этап. Стартуем с Intro. Источник правды.
	UPROPERTY(BlueprintReadOnly, Category = "Story")
	EStoryStage CurrentStage = EStoryStage::Intro;
};