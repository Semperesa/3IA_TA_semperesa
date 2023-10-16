
#include <stdio.h>

#include "agent.h"

Agent::Agent(int id) {
  printf("Creating new agent with id %d...\n", id);
  id_ = id;
}

int Agent::get_id() {
  return id_;
}