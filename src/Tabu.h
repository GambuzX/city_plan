#pragma once

typedef enum {
    NONE = -1,
    BUILD = 1,
    REPLACE = 2,
    REMOVE = 3
} OperationType;

class Tabu {
    private:
        int projectID = -1;
        int row = -1;
        int col = -1;
        OperationType operation = NONE;

        bool restrictsProject = false;
        bool restrictsPosition = false;
        bool restrictsOperation = false;

    public:
        Tabu();
        void setPositionRestriction(int row, int col);
        void setProjectRestriction(int projectID);
        void setOperationRestriction(OperationType operation);

        bool restrictedPosition(int row, int col);
        bool restrictedProject(int projectID);
        bool restrictedOperation(OperationType operation);

        bool restrictedAction(int row, int col, int projectID, OperationType OperationType);
};