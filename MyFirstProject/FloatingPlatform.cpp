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

	//设置默认没有返回
	bInterping = false;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	//设置基于世界的开始位置
	StartPoint = GetActorLocation();
	//结束位置是基于当前物体的相对坐标
	//结束位置的世界坐标= 结束位置+开始位置的世界坐标
	EndPoint += StartPoint;

	//设置计时器
	GetWorldTimerManager().SetTimer(InterpTimer,this,&AFloatingPlatform::ToggleInterping, InterpTime);

	//获取起始距离之间的大小
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//往返漂浮动作
	if (bInterping)
	{
		//1. 获取当前位置
		FVector CurrentLocation = GetActorLocation();
		//2. 获取下一秒的位置
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		//3. 设置物体位置
		SetActorLocation(Interp);

		//获取移动距离
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
/// 设置状态
/// </summary>
void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

/// <summary>
/// 交换位置
/// </summary>
/// <param name="VecOne">位置1</param>
/// <param name="VecTwo">位置2</param>
void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}