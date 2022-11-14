#include "Agent.h"

bool queueEntry::operator<(const queueEntry& rhs) const {
  return this->weight < rhs.weight;
}

bool queueEntry::operator==(const queueEntry& rhs) const {
  return this->position == rhs.position;
}
