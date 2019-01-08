#include "servomanager.h"
#include <iostream>
#include <map>
#include <functional>
#include <thread>
#include <chrono>
#include <wiringPi.h>

static lx16driver driver("/dev/serial0", true);
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
    wiringPiSetup ();
    pinMode (7,OUTPUT);
    std::cout <<"Servos power on"<<std::endl;
    turnOnServoPower(true);
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

void ServoManager::setAngleF(int idx, double angle)
{
    if (idx>8) angle = 180.0 - angle;
    driver.ServoMoveTimeWrite(idx,degree2raw(static_cast<int>(angle)),100);
}

void ServoManager::turnOnServoPower(bool on)
{
        digitalWrite (7,on);
}

Command::ResponceFromServo ServoManager::getAngle(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    int rawValue = driver.ServoPostionRead(command.servoid());
    ret.set_result(raw2degree(rawValue));
    return ret;
}

Command::ResponceFromServo ServoManager::setAngle(Command::CommandToServo &command)
{
    Command::ResponceFromServo ret;
    ret.set_result(0);
    driver.ServoMoveTimeWrite(command.servoid(),degree2raw(command.param1()),100);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout<<"Reading angle? "<<driver.ServoPostionRead(command.servoid())<<std::endl;
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
