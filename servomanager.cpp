#include "servomanager.h"
#include <iostream>
#include <map>
#include <functional>

static lx16driver driver("/dev/ttyAMA0", true);
//360=757

static int raw2degree(int in)
{
    float out=static_cast<float>(in)/757*180;
    return static_cast<int>(out);
}

//TODO set correct formula!
static int degree2raw(int in)
{
    float out=static_cast<float>(in)/180*757;
    return static_cast<int>(out);
}

ServoManager::ServoManager()
{

    if (driver.isOperational()==false)
    {
        std::cout<<"Something wrong with connection, do you have rights? is port correct? is device online?"<<std::endl;
    }
}

Command::ResponceFromServo ServoManager::processServoCommand(Command::CommandToServo& command)
{
    if (driver.isOperational()==false)
    {
        std::cout<<"Something wrong with connection, do you have rights? is port correct? is device online?"<<std::endl;
        throw("com port not operational");
    }
    if(command.has_name())
        std::cout<<"Got command name: "<<command.name()<<std::endl;

    std::string commandName = command.name();
    static std::map<std::string,std::function<Command::ResponceFromServo(Command::CommandToServo&)>> commandDispatcher=
    {
    {"GetAngle",&ServoManager::getAngle},
    {"SetAngle",&ServoManager::setAngle},
    {"GetVoltage",&ServoManager::getVoltage},
    {"GetTemperature",&ServoManager::getTemperature}

};
    return commandDispatcher[commandName](command);
}

void ServoManager::setAngleF(int idx, float angle)
{
    driver.ServoMoveTimeWrite(idx,degree2raw(angle),100);
}

Command::ResponceFromServo ServoManager::getAngle(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    int rawValue = driver.ServoAdjustAngleGet(command.servoid());
    ret.set_result(raw2degree(rawValue));
    return ret;
}

Command::ResponceFromServo ServoManager::setAngle(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    ret.set_result(0);
    driver.ServoMoveTimeWrite(command.servoid(),degree2raw(command.param1()),100);
    std::cout<<"Reading angle? "<<driver.ServoAdjustAngleGet(command.servoid())<<std::endl;
    return ret;
}

Command::ResponceFromServo ServoManager::getVoltage(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    ret.set_result(0);
    int voltage = driver.ServoVoltageRead(command.servoid());
    ret.set_result(voltage);
    return ret;
}

Command::ResponceFromServo ServoManager::getTemperature(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    ret.set_result(0);
    int temperature = 20; //TODO fix stub
    ret.set_result(temperature);
    return ret;
}
