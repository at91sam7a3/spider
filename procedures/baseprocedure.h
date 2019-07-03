#ifndef BASEPROCEDURE_H
#define BASEPROCEDURE_H



class BaseProcedure
{
public:
    BaseProcedure();
    virtual ~BaseProcedure();
    virtual void execute() = 0;
};

#endif // BASEPROCEDURE_H
