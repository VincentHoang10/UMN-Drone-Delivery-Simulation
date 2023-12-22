#ifndef RECHARGE_STATION_H
#define RECHARGE_STATION_H

#include <vector>

#include "IEntity.h"
#include "math/vector3.h"
#include "util/json.h"

class RechargeStation : public IEntity {
 public:
  /**
   * @brief Constructor
   * 
   * @param obj JSON object containing the recharge station's information
   */
  RechargeStation(JsonObject& obj);

  /**
   * @brief Destructor
   */
  ~RechargeStation() {}

  /**
   * @brief Updates the Recharge Station
   * 
   * @param dt difference in time since last update
  */
  void update(double dt);
};

#endif
