// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TAPESICK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FirstPersonCamera;

	// --- Enhanced Input: ссылки на ассеты (выставляются в Blueprint) ---

	// Набор привязок клавиш (наш IMC_Player)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	// Действие движения (WASD)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	// Действие обзора (мышь)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	// Действие спринта (Shift)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	// Действие приседания (Ctrl/C)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;

	// --- Параметры движения (тюнингуются в Blueprint) ---

	// Обычная скорость ходьбы (медленный темп в духе Visage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkSpeed = 165.0f;

	// Ускоренная скорость (по Shift)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float SprintSpeed = 300.0f;

	// Скорость в приседе (самая медленная, крадущаяся)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float CrouchSpeed = 90.0f;

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// --- Функции обработки ввода ---

	// Движение (вызывается, пока зажаты WASD)
	void Move(const FInputActionValue& Value);

	// Обзор мышью
	void Look(const FInputActionValue& Value);

	// Спринт: начать (зажал Shift) и закончить (отпустил)
	void StartSprint();
	void StopSprint();

	// Приседание: начать и закончить
	void StartCrouch();
	void StopCrouch();

};
