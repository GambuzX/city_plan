#include "Tabu.h"


void Tabu::setPositionRestriction(int row, int col){
    this->row = row;
    this->col = col;
    this->restrictsPosition = true;
}

void Tabu::setProjectRestriction(int projectID){
    this->projectID = projectID;
    this->restrictsProject = true;
}

void Tabu::setOperationRestriction(OperationType operation){
    this->operation = operation;
    this->restrictsOperation = true;
}

bool Tabu::restrictedPosition(int row, int col){
    if(!restrictsPosition) return false;

    if(this->row != -1 && this->row == row) return true;
    if(this->col != -1 && this->col == col) return true;

    return false;
}

bool Tabu::restrictedProject(int projectID){
    if(!restrictsProject) return false;

    if(this->projectID == projectID) return true;

    return false;
}

bool Tabu::restrictedOperation(OperationType operation){
    if(!restrictsOperation) return false;

    if(this->operation == operation) return true;

    return false;
}

bool Tabu::restrictedAction(int row, int col, int projectID, OperationType OperationType){
    return restrictedOperation(operation) || restrictedPosition(row,col) || restrictedProject(projectID);
}
