#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_

#include "BeelineStrategy.h"
#include "IEntity.h"
#include "Package.h"
#include "SimulationModel.h"

class BatteryDecorator : public IEntity {
 public:
  /**
   * @brief Construct a new BatteryDecorator object
   * @param drone The drone to be wrapped in the BatteryDecorator
   */
  BatteryDecorator(IEntity* drone);

  /**
   * @brief Destructor
   */
  ~BatteryDecorator();

  /**
   * @brief Links this entity to a simulation model,
   *  giving it access to the model's public variables
   *  and functions.
   * @param[in] model The simulation model to link.
   */
  void linkModel(SimulationModel* model);

  /**
   * @brief Gets the ID of the entity.
   * @return The ID of the entity.
   */
  int getId() const;

  /**
   * @brief Gets the position of the entity.
   * @return The position of the entity.
   */
  Vector3 getPosition() const;

  /**
   * @brief Gets the direction of the entity.
   * @return The direction of the entity.
   */
  Vector3 getDirection() const;

  /**
   * @brief Gets the details of the entity.
   * @return The details of the entity.
   */
  const JsonObject& getDetails() const;

  /**
   * @brief Gets the color of the entity
   * @return The color of the entity
   */
  std::string getColor() const;

  /**
   * @brief Gets the name of the entity
   * @return The name of the entity
   */
  std::string getName() const;

  /**
   * @brief Gets the speed of the entity.
   * @return The speed of the entity.
   */
  double getSpeed() const;

  /**
   * @brief Sets the position of the entity.
   * @param pos_ The desired position of the entity.
   */
  void setPosition(Vector3 pos_);

  /**
   *@brief Set the direction of the entity.
   *@param dir_ The new direction of the entity.
   */
  void setDirection(Vector3 dir_);

  /**
   * @brief Sets the color of the entity
   * @param col_ The new color of the entity
   */
  void setColor(std::string col_);

  /**
   * @brief Rotate the entity around y axis.
   * @param angle The angle to rotate the entity by.
   */
  void rotate(double angle);

  /**
   * @brief Updates the entity's position in the physical system.
   * @param dt The time step of the update.
   */
  void update(double dt);

  /**
   * @brief Decrements the drone's battery.
   * @param amount The amount to decrement the battery by.
   */
  void decrementBattery(double amount);

  /**
   * @brief Charges the drone's battery.
   * @param amount The amount to charge the battery by.
   */
  void chargeBattery(double amount);

  /**
   * @brief Gets the nearest recharge station.
   */
  void getNearestStation(std::vector<IEntity*> stations);

 private:
  IEntity* drone;
  double battery;
  IEntity* nearestStation;
  bool charging;
  IStrategy* toStation;
  bool standByAtStation;
  bool goingToStation;
  bool atStation;
};

#endif
