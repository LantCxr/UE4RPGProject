// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h" 
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpawnVolume.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600);//����Ѱ·��ײ�뾶

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75);//���ù�����ײ�뾶

	// ���ù������
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetMesh(),FName("EnemySocket"));
	
	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;
	bOverlappingCombatSphere = false;
	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;
	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
	DeathDelay = 3.f;
	bHasValidTarget = false;
	InterpSpeed = 15.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	//����ײ�¼�
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//���ú��������������ײ
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ʹ����ʼ�ն��Ź�����
	if (EnemyMovementStatus == EEnemyMovementStatus::EMS_Attacking && CombatTraget) {
		//������ת
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTraget->GetActorLocation());
		//������ת���ɣ�ʹ��������Ӳ
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

		SetActorRotation(InterpRotation);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/// <summary>
/// ׷�ٵ�����ײ��ʼ�¼�
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//������Ϊ���ҵ�ǰ��ɫ���ƶ����ö������	
	if(OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

/// <summary>
/// ׷�ٵ�����ײ�����¼�
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			//����Ѫ��
			bHasValidTarget = false;
			if (Main->CombatTraget == this) {
				Main->SetCombatTraget(nullptr);
			}
			Main->SetHasCombatTarget(false);
			Main->UpdateCombatTarget();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

/// <summary>
/// ����������ײ��ʼ�¼�
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
/// <param name="bFromSweep"></param>
/// <param name="SweepResult"></param>
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && Alive())
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if(Main)
		{
			bHasValidTarget = true;
			Main->SetCombatTraget(this);
			//����ս������
			Main->UpdateCombatTarget();
			CombatTraget = Main;
			bOverlappingCombatSphere = true;
			FLinearColor LinearColor = FLinearColor(0.f, 0.f, 0.f, 1.f);
			Main->PlayMaterialEffect(LinearColor);
			//���ù������������С�����֮�����ȡһ����ֵ��Ϊ�������
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		}
	}
}

/// <summary>
/// ����������ײ�����¼�
/// </summary>
/// <param name="OverlappedComponent"></param>
/// <param name="OtherActor"></param>
/// <param name="OtherComp"></param>
/// <param name="OtherBodyIndex"></param>
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			bHasValidTarget = false;
			bOverlappingCombatSphere = false;
			MoveToTarget(Main);
			CombatTraget = nullptr;

			if (Main->MainPlayerController) {
				USkeletalMeshComponent* MainMesh = Cast <USkeletalMeshComponent>(OtherComp);
				if (MainMesh) Main->MainPlayerController->RemoveEnemyHealthBar();
			}

			if (Main->CombatTraget == this) {
				Main->SetCombatTraget(nullptr);
				Main->bHasCombatTarget = false;
				Main->UpdateCombatTarget();
			}
			
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}


void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main)
		{
			if (Main->HitParticles)
			{
				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SocketLocation, FRotator(0.f), false);
				}
			}
			if (Main->HitSound) {
				UGameplayStatics::PlaySound2D(this, Main->HitSound);
			}
			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
			}
		}
	}
}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


/// <summary>
/// ׷�ٵ���
/// </summary>
/// <param name="Target">����</param>
void AEnemy::MoveToTarget(class AMain* Target) 
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTaget);
	if(AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.0);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

/// <summary>
/// ������ʼ��̫��
/// </summary>
void AEnemy::ActiveCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

/// <summary>
/// ����������̫��
/// </summary>
void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/// <summary>
/// ������ʼ
/// </summary>
void AEnemy::Attack() {
	if (Alive() && bHasValidTarget) {
		if (AIController) {
			AIController->StopMovement();//ֹͣ�ƶ�
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);//�����ƶ�״̬Ϊ����״̬
		}

		//�Ѿ���ʼ�Ĺ�����ȥ��ϣ�������δ��ʼʱ�����й�������
		if (!bAttacking && CombatMontage) {
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage) {
				AnimInstance->Montage_Play(CombatMontage, 1.35f);//����montage�����Լ������ٶ�
				AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);//��ת����Ҫ���ŵ�Ƭ��,�Լ���̫��ʵ��
			}
		}
	}
}

/// <summary>
/// ���������¼�
/// </summary>
void AEnemy::AttackEnd() {
	bAttacking = false;
	if (bOverlappingCombatSphere) {//�����������ʱ��Ȼ�ص��ڹ�����Χ�����������
		//���ù������������С�����֮�����ȡһ����ֵ��Ϊ�������
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	}
}

/// <summary>
/// �˺��������̳��Ը���
/// </summary>
/// <param name="DamageAmount"></param>
/// <param name="DamageEvent"></param>
/// <param name="EventInstigator"></param>
/// <param name="DamageCauser"></param>
/// <returns></returns>
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	//����ֵ����ʱ��ֱ�Ӳ�����������
	Health -= DamageAmount;
	if (Health <= 0.f) {
		Die(DamageCauser);
	}
	return DamageAmount;
}

/// <summary>
/// ����
/// </summary>
void AEnemy::Die(AActor* Causer) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage) {

		AnimInstance->Montage_Play(CombatMontage, 0.8f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);

	//�����е���ײ�������Ϊ����ײ
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	AMain* Main = Cast<AMain>(Causer);
	if (Main) {
		Main->UpdateCombatTarget();
	}
}

/// <summary>
/// ��������
/// </summary>
void AEnemy::DeathEnd() {
	//ֹͣ���ö���
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
	//����ս��Ʒ
	UE_LOG(LogTemp, Warning, TEXT("Drop booty"));
}

/// <summary>
/// �жϽ�ɫ�Ƿ�����
/// true:���
/// false:����
/// </summary>
/// <returns></returns>
bool AEnemy::Alive() {
	return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

/// <summary>
/// ɾ������
/// </summary>
void AEnemy::Disappear() {
	Destroy();
}

/// <summary>
/// ��ȡս��Ŀ���λ��
/// </summary>
/// <param name="Target"></param>
/// <returns></returns>
FRotator AEnemy::GetLookAtRotationYaw(FVector Target) {
	//��ȡ����Ŀ��֮���ƫ����
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}