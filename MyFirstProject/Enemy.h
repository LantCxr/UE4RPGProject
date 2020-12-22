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
*����
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
	/// �����Ƿ���Ч
	/// true��������
	/// false������
	/// </summary>
	bool bHasValidTarget;

	/// <summary>
	/// �ƶ�״̬
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Movement") 
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status) { EnemyMovementStatus = Status;}
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; }

	/// <summary>
	/// ����׷�ٵ��˵�׷�ٷ�Χ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	/// <summary>
	/// ���ڹ������˵Ĺ�����Χ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	/// <summary>
	/// ���ڹ����Զ�Ѱ·��...
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	/// <summary>
	/// ��սĿ��
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AMain* CombatTraget;

	/// <summary>
	/// ����ֵ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health;

	/// <summary>
	/// �������ֵ
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxHealth;

	/// <summary>
	/// �˺�
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Damage;

	/// <summary>
	/// �ܹ�����ʾ������
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UParticleSystem* HitParticles;

	/// <summary>
	/// ����״̬
	/// true����ʼ
	/// false������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	/// <summary>
	/// ��������Ч
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USoundCue* HitSound;

	/// <summary>
	/// ������Ч
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	USoundCue* SwingSound;

	/// <summary>
	/// ������ײ
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UBoxComponent* CombatCollision;

	/// <summary>
	/// ��������
	/// </summary>
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Combat")
	class UAnimMontage* CombatMontage;

	/// <summary>
	/// �������ʱ��
	/// </summary>
	FTimerHandle AttackTimer;

	/// <summary>
	/// ��͹������
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMinTime;

	/// <summary>
	/// ��߹������
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float AttackMaxTime;

	/// <summary>
	/// 
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	/// <summary>
	/// ������ʱ��
	/// </summary>
	FTimerHandle DeathTimer;

	/// <summary>
	/// ��������ʱ
	/// </summary>
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Combat")
	float DeathDelay;

	/// <summary>
	/// ��ֵ��С������ת��
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
	/// ��ײ��ʼ�¼���ʹ��virtual���壬��������̳�
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
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// ��ײ��ʼ�¼���ʹ��virtual���壬��������̳�
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
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// ׷�ٵ���
	/// </summary>
	/// <param name="Target"></param>
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);


	/// <summary>
	/// ��ײ��ʼ�¼���ʹ��virtual���壬��������̳�
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
	/// ��ײ�����¼���ʹ��virtual���壬��������̳�
	/// </summary>
	/// <param name="OverlappedComponent"></param>
	/// <param name="OtherActor"></param>
	/// <param name="OtherComp"></param>
	/// <param name="OtherBodyIndex"></param>
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// ������ʼ���� ��ע
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ActiveCollision();

	/// <summary>
	/// ������������ ��ע
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	/// <summary>
	/// �Ƿ񹥻�
	/// </summary>
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bAttacking;

	/// <summary>
	/// �����¼�
	/// ����������������
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Attack();

	/// <summary>
	/// ���������¼�
	/// ���ڱ�ǹ�������
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

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
	/// ����
	/// </summary>
	void Die(AActor* Causer);

	/// <summary>
	/// ��������
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	/// <summary>
	/// �жϽ�ɫ�Ƿ�����
	/// </summary>
	bool Alive();

	/// <summary>
	/// ɾ����ɫ
	/// </summary>
	void Disappear();

	/// <summary>
	/// ��ȡս��Ŀ���λ��
	/// </summary>
	/// <param name="Target"></param>
	/// <returns></returns>
	FRotator GetLookAtRotationYaw(FVector Target);
};
