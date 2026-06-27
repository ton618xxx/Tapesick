// Fill out your copyright notice in the Description page of Project Settings.

#include "CassetteProgressSubsystem.h"

void UCassetteProgressSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Стартовое состояние новой игры: все кассеты Locked, кроме первой (Unlocked).
	CassetteStatuses.Init(ECassetteStatus::Locked, NumCassettes);
	if (NumCassettes > 0)
	{
		CassetteStatuses[0] = ECassetteStatus::Unlocked;
	}

	bIntroCompleted = false;
}

ECassetteStatus UCassetteProgressSubsystem::GetCassetteStatus(int32 Index) const
{
	// Защита от выхода за границы массива.
	if (CassetteStatuses.IsValidIndex(Index))
	{
		return CassetteStatuses[Index];
	}
	return ECassetteStatus::Locked;
}

void UCassetteProgressSubsystem::CompleteCassette(int32 Index)
{
	if (!CassetteStatuses.IsValidIndex(Index))
	{
		return;
	}

	// Помечаем пройденной.
	CassetteStatuses[Index] = ECassetteStatus::Completed;

	// Открываем следующую, если она есть и ещё заблокирована.
	const int32 NextIndex = Index + 1;
	if (CassetteStatuses.IsValidIndex(NextIndex) &&
		CassetteStatuses[NextIndex] == ECassetteStatus::Locked)
	{
		CassetteStatuses[NextIndex] = ECassetteStatus::Unlocked;
	}
}

bool UCassetteProgressSubsystem::IsIntroCompleted() const
{
	return bIntroCompleted;
}

void UCassetteProgressSubsystem::SetIntroCompleted()
{
	bIntroCompleted = true;
}

int32 UCassetteProgressSubsystem::GetNumCassettes() const
{
	return NumCassettes;
}