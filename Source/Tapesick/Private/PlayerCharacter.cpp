// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Создаём компонент камеры
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

	// Прикрепляем камеру к капсуле персонажа
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());

	// Поднимаем камеру на высоту глаз (пока временно, подростковую высоту настроим позже)
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));

	// Разрешаем камере поворачиваться вместе со взглядом игрока (мышь)
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Получаем PlayerController, управляющий этим персонажем
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Получаем подсистему Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// Добавляем наш набор привязок (если он назначен в Blueprint)
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Превращаем базовый InputComponent в Enhanced-версию
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Движение: пока клавиша зажата (Triggered) — вызываем Move
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}

		// Обзор: пока двигается мышь — вызываем Look
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}

		// Спринт: нажал (Started) — StartSprint, отпустил (Completed) — StopSprint
		if (SprintAction)
		{
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
			EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		}

		// Присед: нажал — StartCrouch, отпустил — StopCrouch
		if (CrouchAction)
		{
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::StartCrouch);
			EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
		}
	}
}

// === Движение (WASD) ===
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// Достаём вектор направления из ввода (X = вбок, Y = вперёд/назад)
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Вперёд/назад: ось Y вектора
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);

		// Вправо/влево: ось X вектора
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

// === Обзор мышью ===
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Поворот влево/вправо (по горизонтали)
		AddControllerYawInput(LookVector.X);

		// Наклон вверх/вниз (по вертикали)
		AddControllerPitchInput(LookVector.Y);
	}
}

// === Спринт ===
void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// === Приседание ===
void APlayerCharacter::StartCrouch()
{
	Crouch();
}

void APlayerCharacter::StopCrouch()
{
	UnCrouch();
}