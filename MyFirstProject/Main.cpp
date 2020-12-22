// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Sound/SoundCue.h"
#include "kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"
#include "Materials/MaterialInstanceDynamic.h"
// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ʵ�������ɱ���������������ײ��������ҿ���
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//�ѵ��ɱ�����󶨵������
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;//���õ��ɱ۳���
	CameraBoom->bUsePawnControlRotation = true;//��ת���ڿ�����

	//ʵ�������
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//�õ��ɱۿ����������ת
	FollowCamera->bUsePawnControlRotation = false;

	//�����ת����������
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//���ý��ҵ���ײ���С
	GetCapsuleComponent()->SetCapsuleSize(48.f, 105.f);

	//��������תʱ����ɫ���������ת
	//��Ӱ�����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//���ý�ɫ��������ķ����ƶ�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//��ת���ٶ�
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 840.f, 0.0f);
	//��Ծ�ĸ߶�
	GetCharacterMovement()->JumpZVelocity = 650.f;
	//���ÿ�����Ծ���Ƶķ���
	GetCharacterMovement()->AirControl = 0.2f;


	MaxHealth = 100.f;
	Health = 80.f;
	MaxStamina = 150.f;
	Stamina = 120.f;
    Coins = 0;
	RunningSpeed = 650;
	SprintingSpeed = 1300;
	bShiftKeyDown = false;
	bLMBDown = false;
	//��ʼ��ö��ֵ
	StaminaStatus = EStaminaStatus::ESS_Normal;
	MovementStatus = EMovementStatus::EMS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	bAttacking = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bHasCombatTarget = false;
	bMovingForward = false;
	bMovingRight = false;
	Souls = 0;
	bESCDown = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	FString Map = GetWorld()->GetMapName();
	Map.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	//if (Map != "SunTemple") {
	//	LoadGameNoSwitch();
	//	if (MainPlayerController) {
	//		MainPlayerController->GameModeOnly();
	//	}
	//}
	LoadGameNoSwitch();
	if (MainPlayerController) {
		MainPlayerController->GameModeOnly();
	}
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }
	
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	//����+�����߼�
	switch (StaminaStatus)
	{
		case EStaminaStatus::ESS_Normal:
			if (bShiftKeyDown) 
			{
				Stamina -= DeltaStamina;
				if (Stamina <= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				}
				if (bMovingForward || bMovingRight) {
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else {
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
			else 
			{
				if ( (Stamina + DeltaStamina) < MaxStamina) 
				{
					Stamina += DeltaStamina;
				}
				else 
				{
					Stamina = MaxStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			break;
		case EStaminaStatus::ESS_BelowMinimum:
			if (bShiftKeyDown) 
			{
				Stamina -= DeltaStamina;
				if (Stamina <= 0.f) 
				{
					Stamina = 0.f;
					SetMovementStatus(EMovementStatus::EMS_Normal);
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				}
				else 
				{
					if (bMovingForward || bMovingRight) {
						SetMovementStatus(EMovementStatus::EMS_Sprinting);
					}
					else {
						SetMovementStatus(EMovementStatus::EMS_Normal);
					}
				}
			}
			else 
			{
				Stamina += DeltaStamina;
				if (Stamina >= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Normal);
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			break;
		case EStaminaStatus::ESS_Exhausted:
			if (bShiftKeyDown)
			{
				Stamina = 0.f;
			}
			else
			{
				Stamina += DeltaStamina;
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		case EStaminaStatus::ESS_ExhaustedRecovering:
			Stamina += DeltaStamina;
			if (Stamina > MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
			break;
		default:
			break;
	}
	
	//ʹ����ʼ�ն��Ź���
	if (bInterpToEnemy && CombatTraget) {
		//������ת
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTraget->GetActorLocation());
		//������ת���ɣ�ʹ��������Ӳ
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTraget) {
		CombatTargetLocation = CombatTraget->GetActorLocation();
		if (MainPlayerController) {
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

/// <summary>
/// �����������
/// </summary>
/// <param name="Amount"></param>
void AMain::UpdateSouls(int32 Amount) {
	Souls += Amount;
}

/// <summary>
/// ��ȡս��Ŀ���λ��
/// </summary>
/// <param name="Target"></param>
/// <returns></returns>
FRotator AMain::GetLookAtRotationYaw(FVector Target) {
	//��ȡ����Ŀ��֮���ƫ����
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f,LookAtRotation.Yaw,0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ESC", IE_Pressed, this, &AMain::ESCDown);
	PlayerInputComponent->BindAction("ESC", IE_Released, this, &AMain::ESCUp);

	PlayerInputComponent->BindAction("Sprinting", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprinting", IE_Released, this, &AMain::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpAtRate);

	PlayerInputComponent->BindAxis("Turn", this, &AMain::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMain::LookUp);
	
}

/* ǰ���ƶ� */
void AMain::MoveForward(float Value)
{
	bMovingForward = false;
	if (CanMove(Value))
	{
		//ȷ����ǰ��ҵķ���
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		bMovingForward = true;
	}
}

/* �����ƶ� */
void AMain::MoveRight(float Value)
{
	bMovingRight = false;
	if (CanMove(Value))
	{
		//ȷ����ǰ��ҵķ���
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		bMovingRight = true;
	}
}

/// <summary>
/// ������ת�����
/// </summary>
/// <param name="Value"></param>
void AMain::Turn(float Value) {
	if (CanMove(Value)) {
		AddControllerYawInput(Value);
	}
}

/// <summary>
/// ������ת�����
/// </summary>
/// <param name="Value"></param>
void AMain::LookUp(float Value) {
	if (CanMove(Value)) {
		AddControllerPitchInput(Value);
	}
}

void AMain::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpAtRate(float Rate)
{
	//1.ʹ���Զ��巽����������ֱۿ����� -15 �� -80 �����䷶Χ��
	//FRotator NewSpringArmRotation = CameraBoom->GetComponentRotation();
	//NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += Rate, -80.f, -15.f);
	//CameraBoom->SetWorldRotation(NewSpringArmRotation);

	//3
	if (Controller && Rate)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMain::DecrementHealth(float Amount)
{
	Health += Amount;
	if (Health <= 0.f)
	{
		Die();
	}
}

void AMain::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

/// <summary>
/// ��ɫ����ֵ����
/// </summary>
/// <param name="Amount">����ֵ</param>
void AMain::IncrementHealth(float Amount) {
	Health += Amount;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
}

void AMain::Die()
{
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage){
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void AMain::SetMovementStatus(EMovementStatus Status) 
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Normal) 
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
}

/// <summary>
/// ��ʼ���
/// </summary>
void AMain::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

/// <summary>
/// ֹͣ���
/// </summary>
void AMain::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

/// <summary>
/// ��ʾʰȡ���λ����Ϣ
/// </summary>
void AMain::ShowPickupLocations()
{
	for(FVector Location:PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, 0.25f);
	}
}

/// <summary>
/// ����������
/// </summary>
void AMain::LMBDown()
{
	bLMBDown = true;

	if (MovementStatus == EMovementStatus::EMS_Dead) return; 
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if(EquippedWeapon) //���û�д�������Ŀ����������״̬��,���Խ��빥��������̫��
	{
		Attack();
	}
}

/// <summary>
/// ������̧��
/// </summary>
void AMain::LMBUp() 
{
	bLMBDown = false;
}

/// <summary>
/// ESC������
/// </summary>
void AMain::ESCDown()
{
	bESCDown = true;

	if (MainPlayerController) {
		MainPlayerController->TogglePauseMenu();
	}
}

/// <summary>
/// ESC��̧��
/// </summary>
void AMain::ESCUp()
{
	bESCDown = false;
}

void AMain::SetEquippedWeapon(AWeapon* WeaponToSet) 
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->Destroy();
	}
	EquippedWeapon = WeaponToSet; 
}

/// <summary>
/// ����
/// </summary>
void AMain::Attack() 
{
	//���������ڹ������״̬����У���ֹ��Ҷ�ε�� 
	if (!bAttacking && MovementStatus != EMovementStatus::EMS_Dead) 
	{
		bAttacking = true;
		SetInterpToEnemy(true); 
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 Section = FMath::RandRange(0,1);
			switch (Section)
			{
				case 0:
					AnimInstance->Montage_Play(CombatMontage, 2.2f);
					AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
					break;
				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.8f);
					AnimInstance->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
					break;
				default:
					;
			}
		} 
	}
}

/// <summary>
/// ��������
/// </summary>
void AMain::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	/// <summary>
	/// ��������ʱ������������Ƿ񻹴��ڰ���״̬������ǣ���������
	/// </summary>
	if (bLMBDown) 
	{
		Attack();
	}
}

/// <summary>
/// ���Ź�����Ч
/// </summary>
void AMain::PlaySwingSound() 
{
	if (EquippedWeapon && EquippedWeapon->SwingSound) 
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

/// <summary>
/// �����˸�ֵ
/// </summary>
/// <param name="Interp"></param>
void AMain::SetInterpToEnemy(bool Interp) {
	bInterpToEnemy = Interp;
}

/// <summary>
/// �̳��Ը�����˺�����
/// </summary>
/// <param name="DamageAmount"></param>
/// <param name="DamageEvent"></param>
/// <param name="EventInstigator"></param>
/// <param name="DamageCauser"></param>
/// <returns></returns>
float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {

	Health -= DamageAmount;
	if (Health <= 0.f)
	{
		Die();
		if (DamageCauser) {
			AEnemy* Enemy = Cast<AEnemy>(DamageCauser);
			if (Enemy) {
				Enemy->bHasValidTarget = false;
			}
		}
	}

	return DamageAmount;

}

void AMain::DeathEnd() {
	//ֹͣ���ö���
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AMain::Jump() {
	
	if (MainPlayerController) if (MainPlayerController->bPauseMenuVisible) return;
	if (MovementStatus == EMovementStatus::EMS_Dead) return; 
	Super::Jump();

}

/// <summary>
/// ����ս��Ŀ��
/// </summary>
void AMain::UpdateCombatTarget() {

	//��ȡ�����������ص��Ķ���
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, EnemyFilter);
	if (OverlappingActors.Num() == 0) {
		if (MainPlayerController) {
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	//��������ĵ��˺���̾���
	AEnemy* ClosestEnemy = Cast<AEnemy>(OverlappingActors[0]);
	if (ClosestEnemy) {
		FVector Location = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - Location).Size();
		for (auto Actor : OverlappingActors) {
			AEnemy* Enemy = Cast<AEnemy>(Actor);
			if (Enemy) {
				float DistanceToActor = (Enemy->GetActorLocation() - Location).Size();
				if (DistanceToActor < MinDistance) {
					MinDistance = DistanceToActor;
					ClosestEnemy = Enemy;
				}
			}
		}
		if (MainPlayerController) {
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTraget(ClosestEnemy);
		bHasCombatTarget = true;
	}
}

/// <summary>
/// �л��ؿ�
/// </summary>
/// <param name="LevelName"></param>
void AMain::SwitchLevel(FName LevelName) {
	UWorld* World = GetWorld();
	if (World) {
		FString CurrentLevel = World->GetMapName();
		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName) {
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}
}

/// <summary>
/// ������Ϸ
/// </summary>
void AMain::SaveGame() {
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Coins = Coins;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
	
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	SaveGameInstance->CharacterStats.LevelName = MapName;

	if (EquippedWeapon) {
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->Name;
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance,SaveGameInstance->PlayerName,SaveGameInstance->UserIndex);
}

/// <summary>
/// ��ȡ��Ϸ
/// </summary>
void AMain::LoadGame(bool SetPosition) {
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
	if (LoadGameInstance) {
		if (LoadGameInstance->CharacterStats.Health) {
			Health = LoadGameInstance->CharacterStats.Health;
		}
		if (LoadGameInstance->CharacterStats.MaxHealth) {
			MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
		}
		if (LoadGameInstance->CharacterStats.Stamina) {
			Stamina = LoadGameInstance->CharacterStats.Stamina;
		}
		if (LoadGameInstance->CharacterStats.MaxStamina) {
			MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
		}
		if (LoadGameInstance->CharacterStats.Coins) {
			Coins = LoadGameInstance->CharacterStats.Coins;
		}

		if (WeaponStorage) {
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons) {
				FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

				if (Weapons->WeaponMap.Contains(WeaponName)) {
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}
			}
		}
 	}

	if (SetPosition && LoadGameInstance) {
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
	}

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims = false;
	GetMesh()->bNoSkeletonUpdate = false;
	//UE_LOG(LogTemp, Warning, TEXT("LoadGame,MapName: %s"), *LoadGameInstance->CharacterStats.LevelName);
	if (LoadGameInstance->CharacterStats.LevelName != TEXT("")) {
		FName LevelName(*LoadGameInstance->CharacterStats.LevelName);
		SwitchLevel(LevelName);
	}
}

/// <summary>
/// ���Ų���Ч��
/// </summary>
void AMain::PlayMaterialEffect(FLinearColor LinearColor){
	//�ڲ�����������������ֵ
	if (UMaterialInterface* Mat = GetMesh()->GetMaterial(0))
	{
		if (!Mat->IsA(UMaterialInstanceDynamic::StaticClass()))
		{
			if (UMaterialInstanceDynamic* MatInstance = UMaterialInstanceDynamic::Create(Mat, this))
			{
				//GetMesh()->SetMaterial(0, MatInstance);
				//Mat = MatInstance;MatInstance
				UE_LOG(LogTemp, Warning, TEXT("MatInstance"));
				MatInstance->SetVectorParameterValue(FName("EffectColor"), LinearColor);

				//�ڲ��������ñ�������ֵ
				MatInstance->SetScalarParameterValue(FName("StartTime"), GetWorld()->GetDeltaSeconds());
			}
		}

		//if (UMaterialInstanceDynamic* DynaMat = Cast<UMaterialInstanceDynamic>(Mat))
		//{
		//	DynaMat->SetVectorParameterValue(FName("EffectColor"), LinearColor);
		//
		//	//�ڲ��������ñ�������ֵ
		//	DynaMat->SetScalarParameterValue(FName("StartTime"), GetWorld()->GetDeltaSeconds());
		//}
	}

}

/// <summary>
/// �ܷ��ƶ��ж�
/// </summary>
bool AMain::CanMove(float Value) {
	if (MainPlayerController) {
		return (Value != 0.0f) && 
			!bAttacking && 
			MovementStatus != EMovementStatus::EMS_Dead &&
			!MainPlayerController->bPauseMenuVisible;
	}
	return false;
}

void AMain::LoadGameNoSwitch() {
	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));
	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));
	if (LoadGameInstance) {
		if (LoadGameInstance->CharacterStats.Health) {
			Health = LoadGameInstance->CharacterStats.Health;
		}
		if (LoadGameInstance->CharacterStats.MaxHealth) {
			MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
		}
		if (LoadGameInstance->CharacterStats.Stamina) {
			Stamina = LoadGameInstance->CharacterStats.Stamina;
		}
		if (LoadGameInstance->CharacterStats.MaxStamina) {
			MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
		}
		if (LoadGameInstance->CharacterStats.Coins) {
			Coins = LoadGameInstance->CharacterStats.Coins;
		}

		if (WeaponStorage) {
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons) {
				FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;

				if (Weapons->WeaponMap.Contains(WeaponName)) {
					AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}
			}
		}

		SetMovementStatus(EMovementStatus::EMS_Normal);
		GetMesh()->bPauseAnims = false;
		GetMesh()->bNoSkeletonUpdate = false;
	}
}