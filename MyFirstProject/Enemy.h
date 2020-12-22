// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle		UMETA(DeplayName = "EMS_Idle"),
	EMS_MoveToTaget UMETA(DeplayName = "MoveToTaget"),
	EMS_Attacking	UMETA(DeplayName = "Attacking"),
	EMS_Dead		UMETA(DeplayName = "Dead"),

	EMS_MAX			UMETA(DeplayName = "DefaultMAX")
};

/**
*敌人
*/
UCLASS()
class MYFIRSTPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

public:

	/// <summary>
	/// 敌人是否有效
	/// true：已死亡
	/// false：存在
	/// </summary>
	bool bHasValidTarget;

	/// <summary>
	/// 移动状态
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Movement") 
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status;}
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	/// <summary>
	/// 用于追踪敌人的追踪范围
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	/// <summary>
	/// 用于攻击敌人的攻击范围
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	/// <summary>
	/// 用于怪物自动寻路等...
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	/// <summary>
	/// 对战目标
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AMain* CombatTraget;

	/// <summary>
	/// 生命值
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health;

	/// <summary>
	/// 最大生命值
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxHealth;

	/// <summary>
	/// 伤害
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Damage;

	/// <summary>
	/// 受攻击显示的粒子
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* HitParticles;

	/// <summary>
	/// 攻击状态
	/// true：开始
	/// false：结束
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	/// <summary>
	/// 被击中音效
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	/// <summary>
	/// 攻击音效
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USoundCue* SwingSound;

	/// <summary>
	/// 攻击碰撞
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UBoxComponent* CombatCollision;

	/// <summary>
	/// 骨骼网络
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Combat")
	class UAnimMontage* CombatMontage;

	/// <summary>
	/// 攻击间隔时间
	/// </summary>
	FTimerHandle AttackTimer;

	/// <summary>
	/// 最低攻击间隔
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMinTime;

	/// <summary>
	/// 最高攻击间隔
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMaxTime;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	/// <summary>
	/// 死亡计时器
	/// </summary>
	FTimerHandle DeathTimer;

	/// <summary>
	/// 死亡倒计时
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float DeathDelay;

	/// <summary>
	/// 赋值大小，用于转向
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float InterpSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// 碰撞开始事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 碰撞开始事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 追踪敌人
	/// </summary>
	/// <param name="Target"></param>
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);


	/// <summary>
	/// 碰撞开始事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	/// <param name="bFromSweep"></param>
	/// <param name="SweepResult"></param>
	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// <summary>
	/// 碰撞结束事件，使用virtual定义，方便子类继承
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// 攻击开始动画 标注
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ActiveCollision();

	/// <summary>
	/// 攻击结束动画 标注
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	/// <summary>
	/// 是否攻击
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bAttacking;

	/// <summary>
	/// 攻击事件
	/// 负责整个攻击功能
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Attack();

	/// <summary>
	/// 攻击结束事件
	/// 用于标记攻击结束
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

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
	/// 死亡
	/// </summary>
	void Die(AActor* Causer);

	/// <summary>
	/// 死亡结束
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/// <summary>
	/// 判断角色是否死亡
	/// </summary>
	bool Alive();

	/// <summary>
	/// 删除角色
	/// </summary>
	void Disappear();

	/// <summary>
	/// 获取战斗目标的位置
	/// </summary>
	/// <param name="Target"></param>
	/// <returns></returns>
	FRotator GetLookAtRotationYaw(FVector Target);
};
