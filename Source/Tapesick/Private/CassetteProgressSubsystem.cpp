// Fill out your copyright notice in the Description page of Project Settings.

#include "CassetteProgressSubsystem.h"
#include "CassetteData.h"
#include "Engine/AssetManager.h"

void UCassetteProgressSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadedCassettes.Empty();

	// Спрашиваем у Asset Manager все ассеты типа CassetteData.
	if (UAssetManager* Manager = UAssetManager::GetIfInitialized())
	{
		const FPrimaryAssetType CassetteType(TEXT("CassetteData"));

		TArray<FPrimaryAssetId> CassetteIds;
		Manager->GetPrimaryAssetIdList(CassetteType, CassetteIds);

		// Сортируем по имени, чтобы порядок был стабильным (линейная разблокировка).
		CassetteIds.Sort([](const FPrimaryAssetId& A, const FPrimaryAssetId& B)
			{
				return A.PrimaryAssetName.LexicalLess(B.PrimaryAssetName);
			});

		// Загружаем каждую кассету синхронно (данные лёгкие) и складываем в массив.
		for (const FPrimaryAssetId& Id : CassetteIds)
		{
			if (UCassetteData* Data = Cast<UCassetteData>(Manager->GetPrimaryAssetObject(Id)))
			{
				LoadedCassettes.Add(Data);
			}
			else
			{
				// Объект ещё не загружен в память — грузим синхронно.
				const FSoftObjectPath Path = Manager->GetPrimaryAssetPath(Id);
				if (UCassetteData* LoadedData = Cast<UCassetteData>(Path.TryLoad()))
				{
					LoadedCassettes.Add(LoadedData);
				}
			}
		}
	}

	// Число кассет = сколько реально нашли.
	const int32 Count = LoadedCassettes.Num();

	// Стартовое состояние: все Locked, кроме первой (Unlocked).
	CassetteStatuses.Init(ECassetteStatus::Locked, Count);
	if (Count > 0)
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

UCassetteData* UCassetteProgressSubsystem::GetCassetteData(int32 Index) const
{
	if (LoadedCassettes.IsValidIndex(Index))
	{
		return LoadedCassettes[Index];
	}
	return nullptr;
}

int32 UCassetteProgressSubsystem::GetNumCassettes() const
{
	return LoadedCassettes.Num();
}