#include "StorySubsystem.h"

void UStorySubsystem::SetStage(EStoryStage NewStage)
{
	// Если этап не изменился — ничего не делаем (не дёргаем подписчиков зря).
	if (CurrentStage == NewStage)
	{
		return;
	}

	CurrentStage = NewStage;

	// Оповещаем всех подписчиков: двери, погоду, стену.
	OnStageChanged.Broadcast(CurrentStage);
}

EStoryStage UStorySubsystem::GetCurrentStage() const
{
	return CurrentStage;
}