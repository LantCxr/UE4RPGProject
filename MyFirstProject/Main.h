// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"
//运动状态
UENUM(BlueprintType)
enum class EMovementStatus:uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(Display = "DefaultMAX")
};

//体力状态
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
	/// 攻击对象坐标
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	FVector CombatTargetLocation;

	/// <summary>
	/// 玩家控制器
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	/// <summary>
	/// 是否拥有对战目标
	/// true：有
	/// false：没有（默认）
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool HasCombatTarget) { bHasCombatTarget = HasCombatTarget; }

	/// <summary>
	/// 受攻击显示的特效
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	/// <summary>
	/// 被击中音效
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class USoundCue* HitSound;

	/// <summary>
	/// 世界掉落物拾取位置
	/// </summary>
	TArray<FVector> PickupLocations;

	/// <summary>
	/// 任意时间内的体力状态
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }

	/// <summary>
	/// 每秒消耗的体力数
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float StaminaDrainRate;

	/// <summary>
	/// 体力下限
	/// 当体力小于下限时，将不再奔跑
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinSprintStamina;

	/// <summary>
	/// 任意时间内的运动状态
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Enums")
	EMovementStatus MovementStatus;

	/// <summary>
	/// 走路速度
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Running")
	float RunningSpeed;

	/// <summary>
	/// 奔跑速度
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	/// <summary>
	/// 判断shift是否按下
	/// false：未按下
	/// true：按下
	/// </summary>
	bool bShiftKeyDown;

	/** Camera boom positioning the camera behind the player  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** 调节摄像机转向功能的基础转向速率 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/// <summary>
	/// 最大生命值
	/// </summary>
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player Stats")
	float MaxHealth;

	/// <summary>
	/// 生命值
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float Health;

	/// <summary>
	/// 最大耐力值
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Player Stats")
	float MaxStamina;

	/// <summary> 
	/// 耐力值
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	float Stamina;

	/// <summary>
	/// 硬币数量
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Player Stats")
	int32 Coins;

	/// <summary>
	/// 灵魂数量
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Souls;

	/// <summary>
	/// 蒙太奇
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	/// <summary>
	/// 赋值大小，用于转向
	/// </summary>
	float InterpSpeed;

	/// <summary>
	/// 是否已经赋值
	/// </summary>
	bool bInterpToEnemy;

	/// <summary>
	/// 攻击对象
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	class AEnemy* CombatTraget;

	FORCEINLINE void SetCombatTraget(AEnemy* Traget) { CombatTraget = Traget; }

	/// <summary>
	/// 是否前后移动
	/// true：移动
	/// false：未移动
	/// </summary>
	bool bMovingForward;

	/// <summary>
    /// 是否左右移动
	/// true：移动
	/// false：未移动
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

	/* 前后移动 */
	void MoveForward(float Value);

	/* 左右移动 */
	void MoveRight(float Value);

	/* 左右转动摄像机 */
	void Turn(float Value);

	/* 上下转动摄像机 */
	void LookUp(float Value);

	/// <summary>
	/// 鼠标左键是否按下
	/// </summary>
	bool bLMBDown;

	/// <summary>
	/// 鼠标左键按下
	/// </summary>
	void LMBDown();

	/// <summary>
	/// 鼠标左键抬起
	/// </summary>
	void LMBUp();

	/// <summary>
	/// ESC键是否按下
	/// </summary>
	bool bESCDown;

	/// <summary>
	/// ESC键按下
	/// </summary>
	void ESCDown();

	/// <summary>
	/// ESC键抬起
	/// </summary>
	void ESCUp();

	/** 设置转向速率
	* @param Rate  标准速率，1.0 表示 100% 的速率
	*/
	void TurnAtRate(float Rate);

	/** 设置摄像机上下看的速率
	* @param Rate  标准速率，1.0 表示 100% 的速率
	*/
	void LookUpAtRate(float Rate);

	/// <summary>
	/// 设置移动状态和速度
	/// </summary>
	/// <param name="Status"></param>
	void SetMovementStatus(EMovementStatus Status);

	/// <summary>
	/// 获取摄像臂
	/// </summary>
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/// <summary>
	/// 获取摄像机
	/// </summary>
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/// <summary>
	/// 角色血量更新
	/// </summary>
	/// <param name="Amount">血量</param>
	void DecrementHealth(float Amount);

	/// <summary>
	/// 角色金币更新
	/// </summary>
	/// <param name="Amount">金币数量</param>
	UFUNCTION(BlueprintCallable)
	void IncrementCoins(int32 Amount);

	/// <summary>
	/// 角色生命值更新
	/// </summary>
	/// <param name="Amount">生命值</param>
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(float Amount);

	/// <summary>
	/// 角色死亡
	/// </summary>
	void Die();

	/// <summary>
	/// 开始冲刺
	/// </summary>
	void ShiftKeyDown();

	/// <summary>
	/// 停止冲刺
	/// </summary>
	void ShiftKeyUp();

	/// <summary>
	/// 显示拾取物的位置信息
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	/// <summary>
	/// 武器
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Items)
	class AWeapon* EquippedWeapon;

	void SetEquippedWeapon(AWeapon* WeaponToSet);
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	/// <summary>
	/// 待激活的重叠项目
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetActiveOverlappingItem(AItem* ItemToSet) { ActiveOverlappingItem = ItemToSet; }

	/// <summary>
	/// 攻击
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻击结束
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/// <summary>
	/// 播放攻击音效
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	/// <summary>
	/// 赋值给敌人，使其转向
	/// </summary>
	void SetInterpToEnemy(bool Interp);

	/// <summary>
	/// 插值到对象
	/// </summary>
	/// <param name="Target">对象位置</param>
	/// <returns></returns>
	FRotator GetLookAtRotationYaw(FVector Target);

	/// <summary>
	/// 伤害函数，继承自父类
	/// </summary>
	/// <param name="DamageAmount"></param>
	/// <param name="DamageEvent"></param>
	/// <param name="EventInstigator"></param>
	/// <param name="DamageCauser"></param>
	/// <returns></returns>
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
	
	/// <summary>
	/// 跳跃
	/// </summary>
	virtual void Jump() override;

	/// <summary>
	/// 死亡后处理
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/// <summary>
	/// 更新战斗目标
	/// </summary>
	void UpdateCombatTarget();

	/// <summary>
	/// 更新灵魂数量
	/// </summary>
	/// <param name="soul"></param>
	UFUNCTION(BlueprintCallable)
	void UpdateSouls(int32 Amount);
	
	/// <summary>
	/// 切换关卡
	/// </summary>
	/// <param name="LevelName"></param>
	void SwitchLevel(FName LevelName);

	/// <summary>
	/// 保存游戏
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	/// <summary>
	/// 读取游戏
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);

	/// <summary>
	/// 播放材质效果
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void PlayMaterialEffect(FLinearColor LinearColor);

	/// <summary>
	/// 能否移动判断
	/// </summary>
	bool CanMove(float Value);

	void LoadGameNoSwitch();
};
