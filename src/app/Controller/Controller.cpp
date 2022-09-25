#include "Controller.h"

Controller::Controller(MembersManageService *membersManageService)
{
    monitor = new Monitor();
    //membersManageService = new MembersManageService();
    this->membersManageService = membersManageService;

}

Controller::~Controller()
{

}

void Controller::updateEvent(DeviceData data)
{
    if(data.devName == "CardReader")
    {
        int cardNumber[5];
        for(int i = 0;i<5;i++)
        {
            cardNumber[i] = data.devData[i];
        }
        membersManageService->checkCardNumber(cardNumber);
        // std::cout << data.devName << " : ";
        // for (const auto data : data.devData)
        // {
        //     std::cout << std::hex << data << " ";
        // }
        // std::cout << std::endl;
    }

    if(data.devName == "modeButton")
    {
        membersManageService->updateStateEvent(data.devName);
    }
}