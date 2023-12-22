#include "BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity* drone) {
  this->drone = drone;
  this->battery = 100.0;
  this->drone->setColor("green");
  this->nearestStation = nullptr;
  this->charging = false;
  this->toStation = nullptr;
  this->standByAtStation = true;
  this->goingToStation = false;
  this->atStation = false;
}

BatteryDecorator::~BatteryDecorator() {
  // delete dynamically allocated variables
  if (drone) delete drone;
}

void BatteryDecorator::linkModel(SimulationModel* model) {
  drone->linkModel(model);
  this->model = model;
}

int BatteryDecorator::getId() const { return drone->getId(); }

Vector3 BatteryDecorator::getPosition() const { return drone->getPosition(); }

Vector3 BatteryDecorator::getDirection() const { return drone->getDirection(); }

const JsonObject& BatteryDecorator::getDetails() const {
  return drone->getDetails();
}

std::string BatteryDecorator::getColor() const { return drone->getColor(); }

std::string BatteryDecorator::getName() const { return drone->getName(); }

double BatteryDecorator::getSpeed() const { return drone->getSpeed(); }

void BatteryDecorator::setPosition(Vector3 pos_) { drone->setPosition(pos_); }

void BatteryDecorator::setDirection(Vector3 dir_) { drone->setDirection(dir_); }

void BatteryDecorator::setColor(std::string col_) { drone->setColor(col_); }

void BatteryDecorator::rotate(double angle) { drone->rotate(angle); }

void BatteryDecorator::update(double dt) {
  Drone* drone = dynamic_cast<Drone*>(this->drone);

  if (battery > 0.0) {  // if drone battery if greater than 0
    if (drone && !goingToStation &&
        !charging) {  // if drone is not going to a station and is charging
      decrementBattery(dt);
      drone->update(dt);
    }
    if (drone && !charging &&
        !drone->isAvailable()) {  // if drone is not charging and is doing a
                                  // delivery
      atStation = false;
      if (battery >
          70.0) {  // change color to green if batteru is greater than 70.0
        drone->setColor("green");
      } else if (battery >
                 35.0) {  // change color to yellow if battery is less than 70.0
        drone->setColor("yellow");
      } else {  // change color to red and go to nearest station to recharge if
                // battery less than 35.0
        drone->setColor("red");
        goingToStation = true;
        if (model && model->allStations.size() == 8) {
          std::vector<IEntity*> stations = model->allStations;
          if (toStation == nullptr) {
            getNearestStation(stations);
            std::cout << "Drone battery low, heading to nearest station"
                      << std::endl;
          }
          if (toStation) {
            toStation->move(drone, dt);
            Package* package = drone->getPackage();
            if (package && drone->isPickedUp()) {
              package->setPosition(this->getPosition());
              package->setDirection(this->getDirection());
            }

            if (toStation->isCompleted()) {
              std::cout << "Drone docked at station" << std::endl;
              delete toStation;
              toStation = nullptr;
              atStation = true;
              charging = true;
            }
          }
        }
      }
    } else if (drone &&
               drone->isAvailable()) {  // have drone standby at station if it
                                        // is not doing a delivery
      if (model && model->allStations.size() == 8) {
        std::vector<IEntity*> stations = model->allStations;
        if (toStation == nullptr && !atStation) {
          goingToStation = true;
          getNearestStation(stations);
          std::cout << "Drone free, heading to nearest station" << std::endl;
        }
        if (toStation) {
          toStation->move(drone, dt);
          if (toStation->isCompleted()) {
            std::cout << "Drone standing by at station" << std::endl;
            delete toStation;
            toStation = nullptr;
            atStation = true;
            goingToStation = false;
          }
        }
      }

      if (atStation) {
        chargeBattery(20 * dt);
      }
      if (battery > 70.0) {
        drone->setColor("green");
      } else if (battery > 35.0) {
        drone->setColor("yellow");
      }
    } else {  // drone is at a station and needs charging
      chargeBattery(20 * dt);
      if (battery == 100.0) {
        std::cout << "Drone fully charged" << std::endl;
        charging = false;
        goingToStation = false;
        atStation = false;
      }
    }
  }
}

void BatteryDecorator::decrementBattery(double amount) {
  battery -= amount;
  if (battery < 0.0) {
    battery = 0.0;
  }
}

void BatteryDecorator::chargeBattery(double amount) {
  battery += amount;
  if (battery > 100.0) {
    battery = 100.0;
  }
}

void BatteryDecorator::getNearestStation(std::vector<IEntity*> stations) {
  double nearest = -1.0;
  for (IEntity* station : stations) {
    std::string type = (station->getDetails())["type"];
    if (type.compare("recharge station") == 0) {
      double current =
          std::abs(station->getPosition().dist(this->getPosition()));
      if (nearest > current || nearest == -1.0) {
        nearest = current;
        nearestStation = station;
      }
    }
  }
  toStation =
      new BeelineStrategy(this->getPosition(), nearestStation->getPosition());
}
