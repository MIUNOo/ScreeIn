// Fill out your copyright notice in the Description page of Project Settings.


#include "RubicsCube.h"

#include "BlockPlane.h"


// Sets default values
ARubicsCube::ARubicsCube()
{
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ARubicsCube::BeginPlay()
{
	for (UChildActorComponent* CubePlane : CubePlanes)
	{
		if (CubePlane)
		{
			CubePlane->DestroyComponent();
		}
	}

	CubePlanes.Empty();
	Blocks.Empty();
	
	FVector TotalSize = FVector(
		Size.X * BlockSpacing,
		Size.Y * BlockSpacing,
		Size.Z * BlockSpacing
	);

	int BlockIndex = 0; // 静态变量用于索引编号
	for (int32 i = 0; i < PlaneDirections.Num(); ++i)
    {
        FVector Normal = PlaneDirections[i];
        FRotator Rotation = PlaneRotations[i];

        int32 CountA = 0; // 第一轴上的平面数
        int32 CountB = 0; // 第二轴上的平面数
        FVector AxisA = FVector::ZeroVector; // 面内第一个方向
        FVector AxisB = FVector::ZeroVector; // 面内第二个方向
        FVector FaceCenterOffset = FVector::ZeroVector; // 面中心相对于魔方中心的偏移

        // 根据当前面的法向确定所属轴和网格的行列数
        if (FMath::Abs(Normal.X) > 0.5f)
        {
            // X 面：网格在 Y-Z 平面上
            CountA = Size.Y;
            CountB = Size.Z;
            AxisA = FVector(0, 1, 0);
            AxisB = FVector(0, 0, 1);
            // 面中心位于 X 轴正负半个魔方宽度处
            FaceCenterOffset = Normal * (Size.X * BlockSpacing * 0.5f);
        }
        else if (FMath::Abs(Normal.Y) > 0.5f)
        {
            // Y 面：网格在 X-Z 平面上
            CountA = Size.X;
            CountB = Size.Z;
            AxisA = FVector(1, 0, 0);
            AxisB = FVector(0, 0, 1);
            FaceCenterOffset = Normal * (Size.Y * BlockSpacing * 0.5f);
        }
        else if (FMath::Abs(Normal.Z) > 0.5f)
        {
            // Z 面：网格在 X-Y 平面上
            CountA = Size.X;
            CountB = Size.Y;
            AxisA = FVector(1, 0, 0);
            AxisB = FVector(0, 1, 0);
            FaceCenterOffset = Normal * (Size.Z * BlockSpacing * 0.5f);
        }



		// static int32 BlockIndex = 0; // 静态变量用于索引编号
        // 在当前面上按照网格排列生成各个平面
        for (int32 a = 0; a < CountA; a++)
        {
            for (int32 b = 0; b < CountB; b++)
            {
                // 计算网格内的局部偏移，令整个网格在面中心对齐
                float OffsetA = (a - (CountA - 1) / 2.0f) * BlockSpacing;
                float OffsetB = (b - (CountB - 1) / 2.0f) * BlockSpacing;
                FVector LocalOffset = AxisA * OffsetA + AxisB * OffsetB;

                // 最终平面的位置：魔方中心 + 面中心偏移 + 网格内偏移
                FVector PlaneLocation = FaceCenterOffset + LocalOffset;
            	

            	UChildActorComponent* ChildComp = Cast<UChildActorComponent>(AddComponentByClass(UChildActorComponent::StaticClass(), false, FTransform::Identity, false));
            	if (ChildComp)
            	{
            		// ChildComp->SetFlags(RF_Transient);
            		
           		// // 生成唯一名称
            	// 	FString UniqueChildCompName = FString::Printf(TEXT("ChildComp_%d"), BlockIndex);
            	// 	FString UniqueBlockName = FString::Printf(TEXT("Block_%d"), BlockIndex);
		           //
            	// 	BlockIndex++; 
            		
            		ChildComp->RegisterComponent(); // 注册组件使其生效
            		AddInstanceComponent(ChildComp); // 添加到 Actor中
            		ChildComp->SetRelativeLocation(PlaneLocation);
            		ChildComp->SetRelativeRotation(Rotation);
            		ChildComp->SetChildActorClass(BlockClass);
    
            		// 确保子Actor被正确创建
            		ChildComp->CreateChildActor();

            		// ChildComp->Rename(*UniqueChildCompName);
    
            		ABlockPlane* Block = Cast<ABlockPlane>(ChildComp->GetChildActor());
            		if (Block)
            		{
            			Blocks.Add(Block);
        
            			// 赋值颜色
            			Block->CubeColor = BlocksProxy[BlockIndex].CurrentCubeColor;
            			Block->bCanInteract = BlocksProxy[BlockIndex].bCanInteract;
            			Block->ParentRubicsCube = this;
            			Block->ApplyCubeColor();
            			// Block->Text->SetText(FText::FromString(UniqueBlockName)); // 设置显示的名称
            			// Block->SetActorLabel(*UniqueBlockName); // 设置 Hierarchy 视图的名称
        
            			// // 初始化 block 数据
            			// FBlockPlaneData Data;
            			// Data.BlockRef = Block;
            			// Data.CubeColor = Block->CubeColor;
            			// BlocksProxy.Add(Data);

            			// Block->Rename(*UniqueBlockName);
            		}

            		CubePlanes.Add(ChildComp);
            		ChildComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

            		BlockIndex++;
            	}

                UE_LOG(LogTemp, Log, TEXT("Create Plane at %s"), *PlaneLocation.ToString());
            }
        }
    }

	
}

// Called every frame
void ARubicsCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ARubicsCube::CheckAllBlocksCorect()
{
	for (int i = 0; i < Blocks.Num(); ++i)
	{
		if (Blocks[i]->CubeColor != BlocksProxy[i].CorrectCubeColor)
		{
			return false;
		}
	}

	for (auto& Element : Blocks)
	{
		Element->bCanInteract = false;
	}

	OnPuzzleSolved.Broadcast(); // 触发事件
	
	return true;
}

void ARubicsCube::OnConstruction(const FTransform& Transform)
{
	// Super::OnConstruction(Transform);

	if (HasAnyFlags(RF_Transient) || GetWorld()->IsGameWorld()) 
	{
		return; // 运行时避免编辑器重复创建
	}
	
	GenerateRubicsCube();
	
	
	
}

void ARubicsCube::UnregisterAllComponents(bool bForReregister)
{
	
}

// void ARubicsCube::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
// 	for (auto& block : Blocks)
// 	{
// 		block->ApplyCubeColor();
// 	}
// }


void ARubicsCube::GenerateRubicsCube()
{
	// if (bHasGenerated) return; // 如果已经生成过，则直接返回
	// bHasGenerated = true;      // 设置标记，确保只执行一次

	// if (CubePlanes.Num() > 0) return; // 避免重复创建

	// 根据 Size 计算所需的平面数量
	int32 RequiredPlanes = 2 * ((Size.Y * Size.Z) + (Size.X * Size.Z) + (Size.X * Size.Y));

	
	// 如果已经生成的平面数量达到要求，则无需重新生成
	if (CubePlanes.Num() == RequiredPlanes && Blocks.Num() == RequiredPlanes)
	{
		return;
	}

	
	
	
	// 清除现有的方块
	for (UChildActorComponent* CubePlane : CubePlanes)
	{
		if (CubePlane)
		{
			CubePlane->DestroyComponent();
		}
	}
	
	CubePlanes.Empty();
	Blocks.Empty();
	BlocksProxy.Empty();

	// 计算魔方的总尺寸（包括间距）
	FVector TotalSize = FVector(
		Size.X * BlockSpacing,
		Size.Y * BlockSpacing,
		Size.Z * BlockSpacing
	);

	
	int32 BlockIndex = 0; // 静态变量用于索引编号
	for (int32 i = 0; i < PlaneDirections.Num(); ++i)
    {
        FVector Normal = PlaneDirections[i];
        FRotator Rotation = PlaneRotations[i];

        int32 CountA = 0; // 第一轴上的平面数
        int32 CountB = 0; // 第二轴上的平面数
        FVector AxisA = FVector::ZeroVector; // 面内第一个方向
        FVector AxisB = FVector::ZeroVector; // 面内第二个方向
        FVector FaceCenterOffset = FVector::ZeroVector; // 面中心相对于魔方中心的偏移

        // 根据当前面的法向确定所属轴和网格的行列数
        if (FMath::Abs(Normal.X) > 0.5f)
        {
            // X 面：网格在 Y-Z 平面上
            CountA = Size.Y;
            CountB = Size.Z;
            AxisA = FVector(0, 1, 0);
            AxisB = FVector(0, 0, 1);
            // 面中心位于 X 轴正负半个魔方宽度处
            FaceCenterOffset = Normal * (Size.X * BlockSpacing * 0.5f);
        }
        else if (FMath::Abs(Normal.Y) > 0.5f)
        {
            // Y 面：网格在 X-Z 平面上
            CountA = Size.X;
            CountB = Size.Z;
            AxisA = FVector(1, 0, 0);
            AxisB = FVector(0, 0, 1);
            FaceCenterOffset = Normal * (Size.Y * BlockSpacing * 0.5f);
        }
        else if (FMath::Abs(Normal.Z) > 0.5f)
        {
            // Z 面：网格在 X-Y 平面上
            CountA = Size.X;
            CountB = Size.Y;
            AxisA = FVector(1, 0, 0);
            AxisB = FVector(0, 1, 0);
            FaceCenterOffset = Normal * (Size.Z * BlockSpacing * 0.5f);
        }



        // 在当前面上按照网格排列生成各个平面
        for (int32 a = 0; a < CountA; a++)
        {
            for (int32 b = 0; b < CountB; b++)
            {
                // 计算网格内的局部偏移，令整个网格在面中心对齐
                float OffsetA = (a - (CountA - 1) / 2.0f) * BlockSpacing;
                float OffsetB = (b - (CountB - 1) / 2.0f) * BlockSpacing;
                FVector LocalOffset = AxisA * OffsetA + AxisB * OffsetB;

                // 最终平面的位置：魔方中心 + 面中心偏移 + 网格内偏移
                FVector PlaneLocation = FaceCenterOffset + LocalOffset;


             //    // 创建子Actor组件（代表一个平面）
             //    UChildActorComponent* ChildComp = NewObject<UChildActorComponent>(this);
             //    ChildComp->RegisterComponent(); // 注册组件使其生效
            	// AddInstanceComponent(ChildComp); // 添加到 Actor 中
             //    ChildComp->SetRelativeLocation(PlaneLocation);
             //    ChildComp->SetRelativeRotation(Rotation);
             //    ChildComp->SetChildActorClass(BlockClass);
            	// ABlockPlane* Block = Cast<ABlockPlane>(ChildComp->GetChildActor());
            	// Blocks.Add(Block);
             //
             //
            	//
             //    // if (DefaultMaterial)
             //    // {
             //    // 	Block->SetSharedMaterial(DefaultMaterial);
             //    // 	Block->ApplyCubeColor();
             //    // }
            	//
            	//
             //    ChildComp->CreateChildActor();
            	// ChildComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
             //
             //    CubePlanes.Add(ChildComp);
             //
            	// //test color
            	// Block->CubeColor = ECubeColor::Yellow;
            	// Block->ApplyCubeColor();
             //
            	// // initialize block data
            	// FBlockPlaneData Data;
            	// Data.BlockRef = Block;
            	// Data.CubeColor = Block->CubeColor;
            	// BlocksProxy.Add(Data);

            	UChildActorComponent* ChildComp = Cast<UChildActorComponent>(AddComponentByClass(UChildActorComponent::StaticClass(), false, FTransform::Identity, false));
            	if (ChildComp)
            	{
            		
            		// ChildComp->SetFlags(RF_Transient);
            		ChildComp->SetFlags(RF_Transient);
            		
            		// 生成唯一名称
            		// FString UniqueChildCompName = FString::Printf(TEXT("ChildComp_%d"), BlockIndex);
            		FString UniqueBlockName = FString::Printf(TEXT("Block_%d"), BlockIndex);

            		BlockIndex++;
            		
            		ChildComp->RegisterComponent(); // 注册组件使其生效
            		AddInstanceComponent(ChildComp); // 添加到 Actor中
            		ChildComp->SetRelativeLocation(PlaneLocation);
            		ChildComp->SetRelativeRotation(Rotation);
            		ChildComp->SetChildActorClass(BlockClass);
            		ChildComp->SetChildActorNameIsExact(true);
    
            		// 确保子Actor被正确创建
            		ChildComp->CreateChildActor();

     //        		// 确保 BlockClass 有效
     //        		if (!BlockClass)
     //        		{
     //        			UE_LOG(LogTemp, Error, TEXT("BlockClass is nullptr!"));
     //        			return;
     //        		}
		   //
     //        		ChildComp->SetChildActorClass(BlockClass);
		   //
     //        		// **延迟创建，避免 DestroyChildActor 被调用**
     //        		FTimerHandle TimerHandle;
     //        		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [ChildComp]()
					// {
					// 	if (ChildComp)
					// 	{
					// 		ChildComp->CreateChildActor();
					// 	}
					// }, 0.1f, false);


            		// ChildComp->Rename(*UniqueChildCompName);
    
            		ABlockPlane* Block = Cast<ABlockPlane>(ChildComp->GetChildActor());
            		if (Block)
            		{
            			Blocks.Add(Block);
        
            			// 赋值颜色
            			Block->CubeColor = ECubeColor::Grey;
            			Block->ApplyCubeColor();
            			Block->Text->SetText(FText::FromString(UniqueBlockName)); // 设置显示的名称
            			// Block->SetActorLabel(*UniqueBlockName); // 设置 Hierarchy 视图的名称
        
            			// 初始化 block 数据
            			FBlockPlaneData Data;
            			// Data.BlockRef = Block;
            			Data.CurrentCubeColor = Block->CubeColor;
            			Data.CorrectCubeColor = Block->CubeColor;
            			Data.bCanInteract = Block->bCanInteract;
            			BlocksProxy.Add(Data);

            			// Block->Rename(*UniqueBlockName);
            		}

            		CubePlanes.Add(ChildComp);
            		ChildComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            	}

                UE_LOG(LogTemp, Log, TEXT("Create Plane at %s"), *PlaneLocation.ToString());
            }
        }
    }


	UE_LOG(LogTemp, Log, TEXT("Finished Generate Rubics Cube with index: %s"), *Blocks[0].GetName());
	
	//有可能是blockplane里的onconstruction一直在覆盖 可能可以通过强链接将父子联系起来
	//另一种思路是不用newobject 而是用defaultsubobject 在onconstruction里初始化 然后对参数调整 单独处理生成
	//证实了不用newobject效果是一样的 链接还是会丢失 现在就是找是在哪里丢失的
	//根据destroy的日志来看，似乎是在register的时候发生了什么，导致了丢失

}




void ARubicsCube::SetMaterialForAllBlocks(UMaterialInterface* NewMaterial)
{
	for (UChildActorComponent* ChildPlane : CubePlanes)
	{
		if (ChildPlane)
		{
			ABlockPlane* Block = Cast<ABlockPlane>(ChildPlane->GetChildActor());
			if (Block) 
			{
				Block->SetSharedMaterial(NewMaterial);
				Block->ApplyCubeColor();
				// Block->ApplyCubeColor();
			}
		}
	}
}