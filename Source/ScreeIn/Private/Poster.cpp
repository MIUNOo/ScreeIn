// Fill out your copyright notice in the Description page of Project Settings.


#include "Poster.h"

#include "IImageWrapper.h"
#include "IImageWrapperModule.h"


// Sets default values
APoster::APoster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

UTexture2D* APoster::LoadTextureFromPath(const FString& FilePath)
{
	// Load the image from file
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		return nullptr;
	}

	// Create image wrapper (PNG format)
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	// Decompress the image data
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		return nullptr;
	}

	// Convert the decompressed image data to RGBA
	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData))
	{
		return nullptr;
	}

	// Create a texture and initialize it with the decompressed image data
	UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
	if (!Texture)
	{
		return nullptr;
	}

	// Fill the texture data
	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
	void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(Data, RawData.GetData(), RawData.Num());
	Mip.BulkData.Unlock();

	// Update the texture settings
	Texture->UpdateResource();

	return Texture;
}

void APoster::ChangeMaterialFromPath(const FString& FilePath)
{
	// Load the texture from the path
	UTexture2D* Texture = LoadTextureFromPath(FilePath);

	if (DefaultTexture)
	{
		Texture = DefaultTexture;
	}

	if (Texture)
	{
		// Create a dynamic material instance
		UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialTemplate, this);
		if (DynamicMaterial)
		{
			// DynamicMaterial->SetForceMipLevelsToBeResident(true, true, 10);
			// Set the texture parameter in the material
			DynamicMaterial->SetTextureParameterValue("PNGTexture", Texture);

			// Apply the material to the actor's static mesh component
			UPrimitiveComponent* component = Cast<UPrimitiveComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (component)
			{
				component->SetMaterial(0, DynamicMaterial);
				UE_LOG(LogTemp, Warning, TEXT("Actor does have a valid static mesh component to apply material."));

			}
			else
			{
				// If the actor doesn't have a static mesh component, print a warning
				UE_LOG(LogTemp, Warning, TEXT("Actor does not have a valid static mesh component to apply material."));
			}
		}
		else
		{
			// If failed to create dynamic material instance, print a warning
			UE_LOG(LogTemp, Warning, TEXT("Failed to create dynamic material instance."));
		}
	}
	else
	{
		// If failed to load texture, print a warning
		UE_LOG(LogTemp, Warning, TEXT("Failed to load texture from path: %s"), *Path.FilePath);
	}
}

// Called when the game starts or when spawned
void APoster::BeginPlay()
{
	Super::BeginPlay();
	
}

void APoster::OnConstruction(const FTransform& Transform)
{
	ChangeMaterialFromPath(Path.FilePath);
}

// Called every frame
void APoster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

