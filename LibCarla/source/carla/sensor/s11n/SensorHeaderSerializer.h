// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "carla/Buffer.h"
#include "carla/rpc/Transform.h"

namespace carla {
namespace sensor {
namespace s11n {

  class SensorHeaderSerializer {
  public:

#pragma pack(push, 1)
    struct Header {
      uint64_t sensor_type;
      uint64_t frame_number;
      rpc::Transform sensor_transform;
    };
#pragma pack(pop)

    constexpr static auto header_offset = sizeof(Header);

    static Buffer Serialize(uint64_t index, uint64_t frame, rpc::Transform transform);

    static const Header &Deserialize(const Buffer &message) {
      return *reinterpret_cast<const Header *>(message.data());
    }
  };

} // namespace s11n
} // namespace sensor
} // namespace carla
