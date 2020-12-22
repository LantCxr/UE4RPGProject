// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f); 
	InterpSpeed = 4.0f;

	//����Ĭ��û�з���
	bInterping = false;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//���û�������Ŀ�ʼλ��
	StartPoint = GetActorLocation();
	//����λ���ǻ��ڵ�ǰ������������
	//����λ�õ���������= ����λ��+��ʼλ�õ���������
	EndPoint += StartPoint;

	//���ü�ʱ��
	GetWorldTimerManager().SetTimer(InterpTimer,this,&AFloatingPlatform::ToggleInterping, InterpTime);

	//��ȡ��ʼ����֮��Ĵ�С
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����Ư������
	if (bInterping)
	{
		//1. ��ȡ��ǰλ��
		FVector CurrentLocation = GetActorLocation();
		//2. ��ȡ��һ���λ��
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//3. ��������λ��
		SetActorLocation(Interp);

		//��ȡ�ƶ�����
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();
		if ((Distance - DistanceTraveled) <= 1.f) 
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
			SwapVectors(StartPoint, EndPoint);
		}
	}
}

/// <summary>
/// ����״̬
/// </summary>
void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

/// <summary>
/// ����λ��
/// </summary>
/// <param name="VecOne">λ��1</param>
/// <param name="VecTwo">λ��2</param>
void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}