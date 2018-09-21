// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "Carla.h"
#include "Carla/Sensor/SensorFactory.h"

#include "Carla/Actor/ActorBlueprintFunctionLibrary.h"
#include "Carla/Sensor/SceneCaptureSensor.h"

TArray<FActorDefinition> ADeprecatedSensorFactory::GetDefinitions()
{
  FActorDefinition Cameras;
  bool Success = false;
  UActorBlueprintFunctionLibrary::MakeCameraDefinition(
      {TEXT("camera"), ASceneCaptureSensor::StaticClass()},
      Success,
      Cameras);
  check(Success);
  return {Cameras};
}

FActorSpawnResult ADeprecatedSensorFactory::SpawnActor(
    const FTransform &Transform,
    const FActorDescription &Description)
{
  FActorSpawnParameters Params;
  auto *World = GetWorld();
  if (World == nullptr)
  {
    return {};
  }
  auto *Sensor = World->SpawnActorDeferred<ASceneCaptureSensor>(
      Description.Class,
      Transform,
      this,
      nullptr,
      ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
  if (Sensor != nullptr)
  {
    using ABFL = UActorBlueprintFunctionLibrary;
    Sensor->SetImageSize(
        ABFL::RetrieveActorAttributeToInt("image_size_x", Description.Variations, 800),
        ABFL::RetrieveActorAttributeToInt("image_size_y", Description.Variations, 600));
    Sensor->SetFOVAngle(
        ABFL::RetrieveActorAttributeToFloat("fov", Description.Variations, 90.0f));
    /// @todo No post-process effects available.
    // Sensor->SetPostProcessEffect(
    //     PostProcessEffect::FromString(
    //         ABFL::RetrieveActorAttributeToString("post_processing", Description.Variations, "SceneFinal")));
  }
  UGameplayStatics::FinishSpawningActor(Sensor, Transform);
  return FActorSpawnResult{Sensor};
}
