// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"
//�˶�״̬
UENUM(BlueprintType)
enum class EMovementStatus:uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(Display = "DefaultMAX")
};

//����״̬
UENUM(BlueprintType)
enum class EStaminaStatus :uint8 
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),
	
	ESS_MAX UMETA(Display = "DefaultMAX")
};

UCLASS()
class MYFIRSTPROJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();

	UPROPERTY(EditDefaultsOnly,Category = "SavedData")
	TSubclassOf<class AItemStorage> WeaponStorage;

	/// <summary>
	/// ������������
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	FVector CombatTargetLocation;

	/// <summary>
	/// ��ҿ�����
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	/// <summary>
	/// �Ƿ�ӵ�ж�սĿ��
	/// true����
	/// false��û�У�Ĭ�ϣ�
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasCombatTarget) { bHasCombatTarget = HasCombatTarget; }

	/// <summary>
	/// �ܹ�����ʾ����Ч
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	/// <summary>
	/// ��������Ч
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	/// <summary>
	/// ���������ʰȡλ��
	/// </summary>
	TArray<FVector> PickupLocations;

	/// <summary>
	/// ����ʱ���ڵ�����״̬
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	/// <summary>
	/// ÿ�����ĵ�������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	/// <summary>
	/// ��������
	/// ������С������ʱ�������ٱ���
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	/// <summary>
	/// ����ʱ���ڵ��˶�״̬
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Enums")
	EMovementStatus MovementStatus;

	/// <summary>
	/// ��·�ٶ�
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Running")
	float RunningSpeed;

	/// <summary>
	/// �����ٶ�
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	/// <summary>
	/// �ж�shift�Ƿ���
	/// false��δ����
	/// true������
	/// </summary>
	bool bShiftKeyDown;

	/** Camera boom positioning the camera behind the player  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** ���������ת���ܵĻ���ת������ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/// <summary>
	/// �������ֵ
	/// </summary>
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player Stats")
	float MaxHealth;

	/// <summary>
	/// ����ֵ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float Health;

	/// <summary>
	/// �������ֵ
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Player Stats")
	float MaxStamina;

	/// <summary> 
	/// ����ֵ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float Stamina;

	/// <summary>
	/// Ӳ������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	int32 Coins;

	/// <summary>
	/// �������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Souls;

	/// <summary>
	/// ��̫��
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	/// <summary>
	/// ��ֵ��С������ת��
	/// </summary>
	float InterpSpeed;

	/// <summary>
	/// �Ƿ��Ѿ���ֵ
	/// </summary>
	bool bInterpToEnemy;

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	class AEnemy* CombatTraget;

	FORCEINLINE void SetCombatTraget(AEnemy* Traget) { CombatTraget = Traget; }

	/// <summary>
	/// �Ƿ�ǰ���ƶ�
	/// true���ƶ�
	/// false��δ�ƶ�
	/// </summary>
	bool bMovingForward;

	/// <summary>
    /// �Ƿ������ƶ�
	/// true���ƶ�
	/// false��δ�ƶ�
	/// </summary>
	bool bMovingRight;

	// <summary>
	/// CombatObjectType
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;

protected:
	//Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* ǰ���ƶ� */
	void MoveForward(float Value);

	/* �����ƶ� */
	void MoveRight(float Value);

	/* ����ת������� */
	void Turn(float Value);

	/* ����ת������� */
	void LookUp(float Value);

	/// <summary>
	/// �������Ƿ���
	/// </summary>
	bool bLMBDown;

	/// <summary>
	/// ����������
	/// </summary>
	void LMBDown();

	/// <summary>
	/// ������̧��
	/// </summary>
	void LMBUp();

	/// <summary>
	/// ESC���Ƿ���
	/// </summary>
	bool bESCDown;

	/// <summary>
	/// ESC������
	/// </summary>
	void ESCDown();

	/// <summary>
	/// ESC��̧��
	/// </summary>
	void ESCUp();

	/** ����ת������
	* @param Rate  ��׼���ʣ�1.0 ��ʾ 100% ������
	*/
	void TurnAtRate(float Rate);

	/** ������������¿�������
	* @param Rate  ��׼���ʣ�1.0 ��ʾ 100% ������
	*/
	void LookUpAtRate(float Rate);

	/// <summary>
	/// �����ƶ�״̬���ٶ�
	/// </summary>
	/// <param name="Status"></param>
	void SetMovementStatus(EMovementStatus Status);

	/// <summary>
	/// ��ȡ�����
	/// </summary>
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// <summary>
	/// ��ȡ�����
	/// </summary>
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// ��ɫѪ������
	/// </summary>
	/// <param name="Amount">Ѫ��</param>
	void DecrementHealth(float Amount);

	/// <summary>
	/// ��ɫ��Ҹ���
	/// </summary>
	/// <param name="Amount">�������</param>
	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);

	/// <summary>
	/// ��ɫ����ֵ����
	/// </summary>
	/// <param name="Amount">����ֵ</param>
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);

	/// <summary>
	/// ��ɫ����
	/// </summary>
	void Die();

	/// <summary>
	/// ��ʼ���
	/// </summary>
	void ShiftKeyDown();

	/// <summary>
	/// ֹͣ���
	/// </summary>
	void ShiftKeyUp();

	/// <summary>
	/// ��ʾʰȡ���λ����Ϣ
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	/// <summary>
	/// ����
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	class AWeapon* EquippedWeapon;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	/// <summary>
	/// ��������ص���Ŀ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }

	/// <summary>
	/// ����
	/// </summary>
	void Attack();

	/// <summary>
	/// ��������
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/// <summary>
	/// ���Ź�����Ч
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	/// <summary>
	/// ��ֵ�����ˣ�ʹ��ת��
	/// </summary>
	void SetInterpToEnemy(bool Interp);

	/// <summary>
	/// ��ֵ������
	/// </summary>
	/// <param name="Target">����λ��</param>
	/// <returns></returns>
	FRotator GetLookAtRotationYaw(FVector Target);

	/// <summary>
	/// �˺��������̳��Ը���
	/// </summary>
	/// <param name="DamageAmount"></param>
	/// <param name="DamageEvent"></param>
	/// <param name="EventInstigator"></param>
	/// <param name="DamageCauser"></param>
	/// <returns></returns>
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
	/// <summary>
	/// ��Ծ
	/// </summary>
	virtual void Jump() override;

	/// <summary>
	/// ��������
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/// <summary>
	/// ����ս��Ŀ��
	/// </summary>
	void UpdateCombatTarget();

	/// <summary>
	/// �����������
	/// </summary>
	/// <param name="soul"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateSouls(int32 Amount);
	
	/// <summary>
	/// �л��ؿ�
	/// </summary>
	/// <param name="LevelName"></param>
	void SwitchLevel(FName LevelName);

	/// <summary>
	/// ������Ϸ
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	/// <summary>
	/// ��ȡ��Ϸ
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	/// <summary>
	/// ���Ų���Ч��
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlayMaterialEffect(FLinearColor LinearColor);

	/// <summary>
	/// �ܷ��ƶ��ж�
	/// </summary>
	bool CanMove(float Value);

	void LoadGameNoSwitch();
};
