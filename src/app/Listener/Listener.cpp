#include "Listener.h"

Listener::Listener(Controller *controller)
// : rfid(new CardReader(new SPI(10,3000000))), modeButton(new ManageButton(27, "modeButton"))
{
    rfid = new CardReader(new SPI(10, 3000000));
    modeButton = new ManageButton(27, "modeButton");
    this->controller = controller;
}

Listener::~Listener()
{

}



void Listener::checkEvent()
{
    if(checkRfid())
    {
        controller->updateEvent(rfid->get_card_number());
    }
    
    if(modeButton->checkButton())
    {
        controller->updateEvent(modeButton->getButtonData());
    }
}


bool Listener::checkRfid()
{
    static unsigned int prevRfidTime = 0;
    if(millis() - prevRfidTime >= 1000)
    {
        prevRfidTime = millis();
    }
    else
    {
        return false;
    }

    if(rfid->isCard())
    {
        return true;
    }
    return false;
}